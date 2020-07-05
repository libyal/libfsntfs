#!/usr/bin/env python
#
# Python-bindings volume type test script
#
# Copyright (C) 2010-2020, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pyfsntfs


class DataRangeFileObject(object):
  """File-like object that maps an in-file data range."""

  def __init__(self, path, range_offset, range_size):
    """Initializes a file-like object.

    Args:
      path (str): path of the file that contains the data range.
      range_offset (int): offset where the data range starts.
      range_size (int): size of the data range starts, or None to indicate
          the range should continue to the end of the parent file-like object.
    """
    super(DataRangeFileObject, self).__init__()
    self._current_offset = 0
    self._file_object = open(path, "rb")
    self._range_offset = range_offset
    self._range_size = range_size

  def __enter__(self):
    """Enters a with statement."""
    return self

  def __exit__(self, unused_type, unused_value, unused_traceback):
    """Exits a with statement."""
    return

  def close(self):
    """Closes the file-like object."""
    if self._file_object:
      self._file_object.close()
      self._file_object = None

  def get_offset(self):
    """Retrieves the current offset into the file-like object.

    Returns:
      int: current offset in the data range.
    """
    return self._current_offset

  def get_size(self):
    """Retrieves the size of the file-like object.

    Returns:
      int: size of the data range.
    """
    return self._range_size

  def read(self, size=None):
    """Reads a byte string from the file-like object at the current offset.

    The function will read a byte string of the specified size or
    all of the remaining data if no size was specified.

    Args:
      size (Optional[int]): number of bytes to read, where None is all
          remaining data.

    Returns:
      bytes: data read.

    Raises:
      IOError: if the read failed.
    """
    if (self._range_offset < 0 or
        (self._range_size is not None and self._range_size < 0)):
      raise IOError("Invalid data range.")

    if self._current_offset < 0:
      raise IOError(
          "Invalid current offset: {0:d} value less than zero.".format(
              self._current_offset))

    if (self._range_size is not None and
        self._current_offset >= self._range_size):
      return b""

    if size is None:
      size = self._range_size
    if self._range_size is not None and self._current_offset + size > self._range_size:
      size = self._range_size - self._current_offset

    self._file_object.seek(
        self._range_offset + self._current_offset, os.SEEK_SET)

    data = self._file_object.read(size)

    self._current_offset += len(data)

    return data

  def seek(self, offset, whence=os.SEEK_SET):
    """Seeks to an offset within the file-like object.

    Args:
      offset (int): offset to seek to.
      whence (Optional(int)): value that indicates whether offset is an absolute
          or relative position within the file.

    Raises:
      IOError: if the seek failed.
    """
    if self._current_offset < 0:
      raise IOError(
          "Invalid current offset: {0:d} value less than zero.".format(
              self._current_offset))

    if whence == os.SEEK_CUR:
      offset += self._current_offset
    elif whence == os.SEEK_END:
      offset += self._range_size
    elif whence != os.SEEK_SET:
      raise IOError("Unsupported whence.")
    if offset < 0:
      raise IOError("Invalid offset value less than zero.")

    self._current_offset = offset


class VolumeTypeTests(unittest.TestCase):
  """Tests the volume type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    fsntfs_volume = pyfsntfs.volume()

    fsntfs_volume.signal_abort()

  def test_open(self):
    """Tests the open function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    if unittest.offset:
      raise unittest.SkipTest("source defines offset")

    fsntfs_volume = pyfsntfs.volume()

    fsntfs_volume.open(unittest.source)

    with self.assertRaises(IOError):
      fsntfs_volume.open(unittest.source)

    fsntfs_volume.close()

    with self.assertRaises(TypeError):
      fsntfs_volume.open(None)

    with self.assertRaises(ValueError):
      fsntfs_volume.open(unittest.source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(unittest.source):
      raise unittest.SkipTest("source not a regular file")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume.open_file_object(file_object)

      with self.assertRaises(IOError):
        fsntfs_volume.open_file_object(file_object)

      fsntfs_volume.close()

      with self.assertRaises(TypeError):
        fsntfs_volume.open_file_object(None)

      with self.assertRaises(ValueError):
        fsntfs_volume.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with self.assertRaises(IOError):
      fsntfs_volume.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    if not unittest.source:
      return

    if unittest.offset:
      raise unittest.SkipTest("source defines offset")

    fsntfs_volume = pyfsntfs.volume()

    # Test open and close.
    fsntfs_volume.open(unittest.source)
    fsntfs_volume.close()

    # Test open and close a second time to validate clean up on close.
    fsntfs_volume.open(unittest.source)
    fsntfs_volume.close()

    if os.path.isfile(unittest.source):
      with open(unittest.source, "rb") as file_object:

        # Test open_file_object and close.
        fsntfs_volume.open_file_object(file_object)
        fsntfs_volume.close()

        # Test open_file_object and close a second time to validate clean up on close.
        fsntfs_volume.open_file_object(file_object)
        fsntfs_volume.close()

        # Test open_file_object and close and dereferencing file_object.
        fsntfs_volume.open_file_object(file_object)
        del file_object
        fsntfs_volume.close()

  def test_get_bytes_per_sector(self):
    """Tests the get_bytes_per_sector function and bytes_per_sector property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      bytes_per_sector = fsntfs_volume.get_bytes_per_sector()
      self.assertIsNotNone(bytes_per_sector)

      self.assertIsNotNone(fsntfs_volume.bytes_per_sector)

      fsntfs_volume.close()

  def test_get_cluster_block_size(self):
    """Tests the get_cluster_block_size function and cluster_block_size property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      cluster_block_size = fsntfs_volume.get_cluster_block_size()
      self.assertIsNotNone(cluster_block_size)

      self.assertIsNotNone(fsntfs_volume.cluster_block_size)

      fsntfs_volume.close()

  def test_get_mft_entry_size(self):
    """Tests the get_mft_entry_size function and mft_entry_size property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      mft_entry_size = fsntfs_volume.get_mft_entry_size()
      self.assertIsNotNone(mft_entry_size)

      self.assertIsNotNone(fsntfs_volume.mft_entry_size)

      fsntfs_volume.close()

  def test_get_index_entry_size(self):
    """Tests the get_index_entry_size function and index_entry_size property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      index_entry_size = fsntfs_volume.get_index_entry_size()
      self.assertIsNotNone(index_entry_size)

      self.assertIsNotNone(fsntfs_volume.index_entry_size)

      fsntfs_volume.close()

  def test_get_name(self):
    """Tests the get_name function and name property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      name = fsntfs_volume.get_name()
      self.assertIsNotNone(name)

      self.assertIsNotNone(fsntfs_volume.name)

      fsntfs_volume.close()

  def test_get_serial_number(self):
    """Tests the get_serial_number function and serial_number property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      serial_number = fsntfs_volume.get_serial_number()
      self.assertIsNotNone(serial_number)

      self.assertIsNotNone(fsntfs_volume.serial_number)

      fsntfs_volume.close()

  def test_get_number_of_file_entries(self):
    """Tests the get_number_of_file_entries function and number_of_file_entries property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      number_of_file_entries = fsntfs_volume.get_number_of_file_entries()
      self.assertIsNotNone(number_of_file_entries)

      self.assertIsNotNone(fsntfs_volume.number_of_file_entries)

      fsntfs_volume.close()

  def test_get_file_entry(self):
    """Tests the get_file_entry function."""
    if not unittest.source:
      raise unittest.SkipTest('missing source')

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      file_entry = fsntfs_volume.get_file_entry(0)
      self.assertIsNotNone(file_entry)

      fsntfs_volume.close()

  def test_get_file_entry_by_path(self):
    """Tests the get_file_entry_by_path function."""
    if not unittest.source:
      raise unittest.SkipTest('missing source')

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
      self.assertIsNotNone(file_entry)

      fsntfs_volume.close()

  def test_get_root_directory(self):
    """Tests the get_root_directory function and root_directory property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      root_directory = fsntfs_volume.get_root_directory()
      self.assertIsNotNone(root_directory)

      fsntfs_volume.close()

  def test_get_usn_change_journal(self):
    """Tests the get_usn_change_journal function and usn_change_journal property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    fsntfs_volume = pyfsntfs.volume()

    with DataRangeFileObject(
        unittest.source, unittest.offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      _ = fsntfs_volume.get_usn_change_journal()

      fsntfs_volume.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "-o", "--offset", dest="offset", action="store", default=None,
      type=int, help="offset of the source file.")

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "offset", options.offset)
  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
