#!/usr/bin/env python
#
# Python-bindings file_entry type test script
#
# Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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
import random
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
    if range_size is None:
      stat_object = os.stat(path)
      range_size = stat_object.st_size

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


class FileEntryTypeTests(unittest.TestCase):
  """Tests the file_entry type."""

  def test_get_size(self):
    """Tests the get_size function and size property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        size = fsntfs_file_entry.get_size()
        self.assertIsNotNone(size)

        self.assertIsNotNone(fsntfs_file_entry.size)

      finally:
        fsntfs_volume.close()

  def test_get_number_of_extents(self):
    """Tests the get_number_of_extents function and number_of_extents property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        number_of_extents = fsntfs_file_entry.get_number_of_extents()
        self.assertIsNotNone(number_of_extents)

        self.assertIsNotNone(fsntfs_file_entry.number_of_extents)

      finally:
        fsntfs_volume.close()

  def test_get_extent(self):
    """Tests the get_extent function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        if not fsntfs_file_entry.number_of_extents:
          raise unittest.SkipTest('missing extents')

        extent = fsntfs_file_entry.get_extent(0)
        self.assertIsNotNone(extent)

      finally:
        fsntfs_volume.close()

  def test_is_empty(self):
    """Tests the is_empty function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        is_empty = fsntfs_file_entry.is_empty()
        self.assertFalse(is_empty)

      finally:
        fsntfs_volume.close()

  def test_is_allocated(self):
    """Tests the is_allocated function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        is_allocated = fsntfs_file_entry.is_allocated()
        self.assertTrue(is_allocated)

      finally:
        fsntfs_volume.close()

  def test_has_directory_entries_index(self):
    """Tests the has_directory_entries_index function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        has_directory_entries_index = fsntfs_file_entry.has_directory_entries_index()
        self.assertFalse(has_directory_entries_index)

      finally:
        fsntfs_volume.close()

  def test_has_default_data_stream(self):
    """Tests the has_default_data_stream function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        has_default_data_stream = fsntfs_file_entry.has_default_data_stream()
        self.assertTrue(has_default_data_stream)

      finally:
        fsntfs_volume.close()

  def test_get_file_reference(self):
    """Tests the get_file_reference function and file_reference property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        file_reference = fsntfs_file_entry.get_file_reference()
        self.assertIsNotNone(file_reference)

        self.assertIsNotNone(fsntfs_file_entry.file_reference)

      finally:
        fsntfs_volume.close()

  def test_get_base_record_file_reference(self):
    """Tests the get_base_record_file_reference function and base_record_file_reference property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        base_record_file_reference = fsntfs_file_entry.get_base_record_file_reference()
        self.assertIsNotNone(base_record_file_reference)

        self.assertIsNotNone(fsntfs_file_entry.base_record_file_reference)

      finally:
        fsntfs_volume.close()

  # TODO: add tests for parent_file_reference
  # TODO: add tests for get_parent_file_reference_by_attribute_index

  def test_get_journal_sequence_number(self):
    """Tests the get_journal_sequence_number function and journal_sequence_number property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        journal_sequence_number = fsntfs_file_entry.get_journal_sequence_number()
        self.assertIsNotNone(journal_sequence_number)

        self.assertIsNotNone(fsntfs_file_entry.journal_sequence_number)

      finally:
        fsntfs_volume.close()

  def test_get_creation_time(self):
    """Tests the get_creation_time function and creation_time property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        creation_time = fsntfs_file_entry.get_creation_time()
        self.assertIsNotNone(creation_time)

        self.assertIsNotNone(fsntfs_file_entry.creation_time)

      finally:
        fsntfs_volume.close()

  def test_get_creation_time_as_integer(self):
    """Tests the get_creation_time_as_integer function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        creation_time = fsntfs_file_entry.get_creation_time_as_integer()
        self.assertIsNotNone(creation_time)

      finally:
        fsntfs_volume.close()

  def test_get_modification_time(self):
    """Tests the get_modification_time function and modification_time property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        modification_time = fsntfs_file_entry.get_modification_time()
        self.assertIsNotNone(modification_time)

        self.assertIsNotNone(fsntfs_file_entry.modification_time)

      finally:
        fsntfs_volume.close()

  def test_get_modification_time_as_integer(self):
    """Tests the get_modification_time_as_integer function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        modification_time = fsntfs_file_entry.get_modification_time_as_integer()
        self.assertIsNotNone(modification_time)

      finally:
        fsntfs_volume.close()

  def test_get_access_time(self):
    """Tests the get_access_time function and access_time property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        access_time = fsntfs_file_entry.get_access_time()
        self.assertIsNotNone(access_time)

        self.assertIsNotNone(fsntfs_file_entry.access_time)

      finally:
        fsntfs_volume.close()

  def test_get_access_time_as_integer(self):
    """Tests the get_access_time_as_integer function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        access_time = fsntfs_file_entry.get_access_time_as_integer()
        self.assertIsNotNone(access_time)

      finally:
        fsntfs_volume.close()

  def test_get_entry_modification_time(self):
    """Tests the get_entry_modification_time function and entry_modification_time property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        entry_modification_time = fsntfs_file_entry.get_entry_modification_time()
        self.assertIsNotNone(entry_modification_time)

        self.assertIsNotNone(fsntfs_file_entry.entry_modification_time)

      finally:
        fsntfs_volume.close()

  def test_get_entry_modification_time_as_integer(self):
    """Tests the get_entry_modification_time_as_integer function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        entry_modification_time = fsntfs_file_entry.get_entry_modification_time_as_integer()
        self.assertIsNotNone(entry_modification_time)

      finally:
        fsntfs_volume.close()

  def test_get_name(self):
    """Tests the get_name function and name property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        name = fsntfs_file_entry.get_name()
        self.assertIsNotNone(name)

        self.assertIsNotNone(fsntfs_file_entry.name)

      finally:
        fsntfs_volume.close()

  def test_get_name_attribute_index(self):
    """Tests the get_name_attribute_index function and name_attribute_index property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        name_attribute_index = fsntfs_file_entry.get_name_attribute_index()
        self.assertIsNotNone(name_attribute_index)

        self.assertIsNotNone(fsntfs_file_entry.name_attribute_index)

      finally:
        fsntfs_volume.close()

  def test_get_file_attribute_flags(self):
    """Tests the get_file_attribute_flags function and file_attribute_flags property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        file_attribute_flags = fsntfs_file_entry.get_file_attribute_flags()
        self.assertIsNotNone(file_attribute_flags)

        self.assertIsNotNone(fsntfs_file_entry.file_attribute_flags)

      finally:
        fsntfs_volume.close()

  # TODO: add tests for path_hint

  def test_get_symbolic_link_target(self):
    """Tests the get_symbolic_link_target function and symbolic_link_target property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        symbolic_link_target = fsntfs_file_entry.get_symbolic_link_target()
        self.assertIsNone(symbolic_link_target)

        self.assertIsNone(fsntfs_file_entry.symbolic_link_target)

      finally:
        fsntfs_volume.close()

  def test_get_security_descriptor_data(self):
    """Tests the get_security_descriptor_data function and security_descriptor_data property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_root_directory()
        self.assertIsNotNone(fsntfs_file_entry)

        security_descriptor_data = fsntfs_file_entry.get_security_descriptor_data()
        self.assertIsNotNone(security_descriptor_data)

        self.assertIsNotNone(fsntfs_file_entry.security_descriptor_data)

      finally:
        fsntfs_volume.close()

  def test_get_number_of_attributes(self):
    """Tests the get_number_of_attributes function and number_of_attributes property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        number_of_attributes = fsntfs_file_entry.get_number_of_attributes()
        self.assertIsNotNone(number_of_attributes)

        self.assertIsNotNone(fsntfs_file_entry.number_of_attributes)

      finally:
        fsntfs_volume.close()

  def test_get_attribute(self):
    """Tests the get_attribute function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        if not fsntfs_file_entry.number_of_attributes:
          raise unittest.SkipTest('missing attributes')

        attribute = fsntfs_file_entry.get_attribute(0)
        self.assertIsNotNone(attribute)

      finally:
        fsntfs_volume.close()

  def test_get_number_of_alternate_data_streams(self):
    """Tests the get_number_of_alternate_data_streams function and number_of_alternate_data_streams property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        number_of_alternate_data_streams = fsntfs_file_entry.get_number_of_alternate_data_streams()
        self.assertIsNotNone(number_of_alternate_data_streams)

        self.assertIsNotNone(fsntfs_file_entry.number_of_alternate_data_streams)

      finally:
        fsntfs_volume.close()

  # TODO: add tests for get_alternate_data_stream
  # TODO: add tests for has_alternate_data_stream_by_name
  # TODO: add tests for get_alternate_data_stream_by_name

  def test_get_number_of_sub_file_entries(self):
    """Tests the get_number_of_sub_file_entries function and number_of_sub_file_entries property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest('missing source')

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsntfs_volume = pyfsntfs.volume()
      fsntfs_volume.open_file_object(file_object)

      if not fsntfs_volume.number_of_file_entries:
        raise unittest.SkipTest('missing file entries')

      try:
        fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path('\\$MFT')
        self.assertIsNotNone(fsntfs_file_entry)

        number_of_sub_file_entries = fsntfs_file_entry.get_number_of_sub_file_entries()
        self.assertIsNotNone(number_of_sub_file_entries)

        self.assertIsNotNone(fsntfs_file_entry.number_of_sub_file_entries)

      finally:
        fsntfs_volume.close()

  # TODO: add tests for get_sub_file_entry


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
