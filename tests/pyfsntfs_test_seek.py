#!/usr/bin/env python
#
# Python-bindings seek testing program
#
# Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

from __future__ import print_function
import argparse
import os
import sys

import pyfsntfs


def get_whence_string(whence):
  """Retrieves a human readable string representation of the whence."""
  if whence == os.SEEK_CUR:
    whence_string = "SEEK_CUR"
  elif whence == os.SEEK_END:
    whence_string = "SEEK_END"
  elif whence == os.SEEK_SET:
    whence_string = "SEEK_SET"
  else:
    whence_string = "UNKNOWN"
  return whence_string


def pyfsntfs_test_seek_offset(
    fsntfs_file_entry, input_offset, input_whence, expected_offset):

  print("Testing seek of offset: {0:d} and whence: {1:s}\t".format(
      input_offset, get_whence_string(input_whence)), end="")

  error_string = ""
  result = True
  try:
    fsntfs_file_entry.seek(input_offset, input_whence)

    result_offset = fsntfs_file_entry.get_offset()
    if expected_offset != result_offset:
      result = False

  except Exception as exception:
    error_string = str(exception)
    if expected_offset != -1:
      result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyfsntfs_test_seek(fsntfs_file_entry):
  file_size = fsntfs_file_entry.size

  # Test: SEEK_SET offset: 0
  # Expected result: 0
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, 0, os.SEEK_SET, 0):
    return False

  # Test: SEEK_SET offset: <file_size>
  # Expected result: <file_size>
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, file_size, os.SEEK_SET, file_size):
    return False

  # Test: SEEK_SET offset: <file_size / 5>
  # Expected result: <file_size / 5>
  seek_offset, _ = divmod(file_size, 5)
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, seek_offset, os.SEEK_SET, seek_offset):
    return False

  # Test: SEEK_SET offset: <file_size + 987>
  # Expected result: <file_size + 987>
  seek_offset = file_size + 987
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, seek_offset, os.SEEK_SET, seek_offset):
    return False

  # Test: SEEK_SET offset: -987
  # Expected result: -1
  seek_offset = -987
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, seek_offset, os.SEEK_SET, -1):
    return False

  # Test: SEEK_CUR offset: 0
  # Expected result: <file_size + 987>
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, 0, os.SEEK_CUR, file_size + 987):
    return False

  # Test: SEEK_CUR offset: <-1 * (file_size + 987)>
  # Expected result: 0
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, -1 * (file_size + 987), os.SEEK_CUR, 0):
    return False

  # Test: SEEK_CUR offset: <file_size / 3>
  # Expected result: <file_size / 3>
  seek_offset, _ = divmod(file_size, 3)
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, seek_offset, os.SEEK_CUR, seek_offset):
    return False

  if file_size == 0:
    # Test: SEEK_CUR offset: <-2 * (file_size / 3)>
    # Expected result: 0
    seek_offset, _ = divmod(file_size, 3)
    if not pyfsntfs_test_seek_offset(fsntfs_file_entry, -2 * seek_offset, os.SEEK_CUR, 0):
      return False

  else:
    # Test: SEEK_CUR offset: <-2 * (file_size / 3)>
    # Expected result: -1
    seek_offset, _ = divmod(file_size, 3)
    if not pyfsntfs_test_seek_offset(
        fsntfs_file_entry, -2 * seek_offset, os.SEEK_CUR, -1):
      return False

  # Test: SEEK_END offset: 0
  # Expected result: <file_size>
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, 0, os.SEEK_END, file_size):
    return False

  # Test: SEEK_END offset: <-1 * file_size>
  # Expected result: 0
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, -1 * file_size, os.SEEK_END, 0):
    return False

  # Test: SEEK_END offset: <-1 * (file_size / 4)>
  # Expected result: <file_size - (file_size / 4)>
  seek_offset, _ = divmod(file_size, 4)
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, -1 * seek_offset, os.SEEK_END, file_size - seek_offset):
    return False

  # Test: SEEK_END offset: 542
  # Expected result: <file_size + 542>
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, 542, os.SEEK_END, file_size + 542):
    return False

  # Test: SEEK_END offset: <-1 * (file_size + 542)>
  # Expected result: -1
  if not pyfsntfs_test_seek_offset(
      fsntfs_file_entry, -1 * (file_size + 542), os.SEEK_END, -1):
    return False

  # Test: UNKNOWN (88) offset: 0
  # Expected result: -1
  if not pyfsntfs_test_seek_offset(fsntfs_file_entry, 0, 88, -1):
    return False

  return True


def pyfsntfs_test_seek_file(filename):
  fsntfs_volume = pyfsntfs.volume()

  fsntfs_volume.open(filename, "r")

  fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path("\\$UpCase")
  result = pyfsntfs_test_seek(fsntfs_file_entry)
  fsntfs_volume.close()

  return result


def pyfsntfs_test_seek_file_object(filename):
  file_object = open(filename, "rb")
  fsntfs_volume = pyfsntfs.volume()

  fsntfs_volume.open_file_object(file_object, "r")

  fsntfs_file_entry = fsntfs_volume.get_file_entry_by_path("\\$UpCase")
  result = pyfsntfs_test_seek(fsntfs_file_entry)
  fsntfs_volume.close()

  return result


def main():
  args_parser = argparse.ArgumentParser(description=(
      "Tests seek."))

  args_parser.add_argument(
      "source", nargs="?", action="store", metavar="FILENAME",
      default=None, help="The source filename.")

  options = args_parser.parse_args()

  if not options.source:
    print("Source value is missing.")
    print("")
    args_parser.print_help()
    print("")
    return False

  if not pyfsntfs_test_seek_file(options.source):
    return False

  if not pyfsntfs_test_seek_file_object(options.source):
    return False

  return True


if __name__ == "__main__":
  if not main():
    sys.exit(1)
  else:
    sys.exit(0)

