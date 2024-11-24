import ctypes
from decompress import byte_decompress
from test_data import TEST_CASES

LIBRARY_PATH = "./bin/libcompress.so"  # Path to the shared library
lib = ctypes.CDLL(LIBRARY_PATH)  # Load the shared library

# Define the function argument and return types
lib.byte_compress.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t]
lib.byte_compress.restype = ctypes.c_size_t

def run_compress(input_data):
    if not input_data:
        return []
    
    input_array = (ctypes.c_uint8 * len(input_data))(*input_data)
    buffer = (ctypes.c_uint8 * len(input_data))()
    ctypes.memmove(buffer, input_array, len(input_data))
    
    compressed_size = lib.byte_compress(buffer, len(input_data))  # Call the compress function
    
    if compressed_size == 0:
        raise RuntimeError("Compression failed")
    
    compressed_data = [buffer[i] for i in range(compressed_size)]  # Extract compressed data from buffer
    return compressed_data

def run_tests():
    passed = 0
    failed = 0

    for i, (description, input_data, should_pass) in enumerate(TEST_CASES, start=1):
        try:
            compressed_data = run_compress(input_data)
            decompressed_data = byte_decompress(compressed_data)

            if not should_pass:
                print(f"FAILED: Test {i} ({description})\nExpected failure but test passed")
                failed += 1
            else:
                assert decompressed_data == input_data, (
                    f"Test {i} ({description}) failed:\n"
                    f"Input: {input_data}\n"
                    f"Compressed: {compressed_data}\n"
                    f"Decompressed: {decompressed_data}\n"
                    f"Expected: {input_data}"
                )
                print(f"PASSED: Test {i} ({description})")
                passed += 1

        except Exception as e:
            if not should_pass:
                print(f"PASSED: Test {i} ({description} - Expected failure)")
                passed += 1
            else:
                print(f"FAILED: Test {i} ({description})\n{e}")
                failed += 1

    print("\nSummary:")
    print(f"Passed: {passed}")
    print(f"Failed: {failed}")

if __name__ == "__main__":
    run_tests()
