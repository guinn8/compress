#!/usr/bin/env python3

import os
import sys
import ctypes
from decompress import byte_decompress

# Load the shared library
lib = ctypes.CDLL('./libcompress.so')

# Define the byte_compress function signature
lib.byte_compress.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t]
lib.byte_compress.restype = ctypes.c_size_t

def run_tests():
    # Define paths
    test_data_dir = "scripts/test_data"

    # Verify required files exist
    if not os.path.isdir(test_data_dir):
        print(f"Error: Test data directory '{test_data_dir}' not found.", file=sys.stderr)
        sys.exit(1)

    # Find all test files
    test_files = [f for f in os.listdir(test_data_dir) if f.endswith(".csv")]
    if not test_files:
        print(f"No test files found in '{test_data_dir}'.")
        sys.exit(1)

    all_passed = True
    for test_file in test_files:
        test_file_path = os.path.join(test_data_dir, test_file)
        print(f"Running test: {test_file}...")

        try:
            # Read original test file
            with open(test_file_path, "r") as f:
                original_data = list(map(int, f.read().strip().split(",")))

            input_data = (ctypes.c_uint8 * len(original_data))(*original_data)
            input_size = len(original_data)

            # Call the C compression function
            compressed_size = lib.byte_compress(input_data, input_size)
            compressed_data = list(input_data[:compressed_size])

            decompressed_data = byte_decompress(compressed_data)

            if decompressed_data == original_data:
                print(f"Test {test_file} passed.")
            else:
                all_passed = False
                print(f"Test {test_file} failed:")
                print("Original Data:", original_data)
                print("Compressed Data:", compressed_data)
                print("Decompressed Data:", decompressed_data)

        except Exception as e:
            all_passed = False
            print(f"Error running test {test_file}: {e}")

    if all_passed:
        print("All tests passed!")
    else:
        print("Some tests failed. Please review the output above.")

if __name__ == "__main__":
    run_tests()
