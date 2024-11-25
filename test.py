import ctypes

LIBRARY_PATH = "./libcompress.so"  # Path to the shared library
lib = ctypes.CDLL(LIBRARY_PATH)  # Load the shared library

# Define the function argument and return types
lib.byte_compress.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t]
lib.byte_compress.restype = ctypes.c_size_t

TEST_CASES = [
    ("Given example", [0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09], True),
    ("Single number", [42], True),
    ("Small run", [10]*3, True),
    ("Large input", [42] * 10000, True),
    ("Repeated runs", [10]*100 + [20]*100 + [10]*100 + [20]*100, True),
    ("Long alternating runs", [10]*100 + [20]*100 + [10]*100 + [20]*100, True),
    ("Large w/ repeats", [10, 10, 20, 20, 30] * 200, True),
    ("Run length 300", [42] * 300, True),
    ("Run length 256", [42] * 256, True),
    ("Multiple runs with max length", [10]*255 + [20]*255, True),
    ("Max value runs", [127] * 255, True),
    ("Min value runs", [0] * 255, True),
    ("Run 128", [42] * 128, True),
    ("Run 127", [42] * 127, True),
    ("Increasing sequence", list(range(0, 128)), True),
    ("Decreasing sequence", list(range(127, -1, -1)), True),
    ("Alternating runs of max length", [10]*255 + [20]*255, True),
    ("Patterned input", [10]*3 + [20]*4 + [30]*5 + [40]*6, True),
    ("Mix repeats", [10, 20, 20, 20, 30, 40, 40, 50], True),
    ("Alternating", [10, 20, 10, 20, 10, 20], True),
    ("Runs & unique", [10, 10, 20, 30, 30, 30, 40, 50], True),
    ("Random reps", [10, 10, 20, 20, 30, 30, 40, 40, 50, 50], True),
    ("Unique values", list(range(1, 101)), True),
    ("Empty", [], True),
    ("Out of range", [128], False),
    ("Out of range", [256], False),
    ("Just out of range", list(range(0, 129)), False),
]

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

def byte_decompress(compressed_data):
    def decompress_generator(data):
        it = iter(data)
        for byte in it:
            if byte & 0x80:  # High bit set, it's a run
                literal = byte & 0x7F  # Extract the 7-bit literal
                run_length = next(it)  # Get the run length
                yield from [literal] * run_length
            else:  # Single literal
                yield byte

    return list(decompress_generator(compressed_data))


def run_tests():
    passed = 0
    failed = 0

    for i, (description, input_data, should_pass) in enumerate(TEST_CASES, start=1):
        try:
            compressed_data = run_compress(input_data)
            decompressed_data = byte_decompress(compressed_data)
            assert decompressed_data == input_data, (
                f"Test {i} ({description}) failed:\n"
                f"Input: {input_data}\n"
                f"Compressed: {compressed_data}\n"
                f"Decompressed: {decompressed_data}\n"
                f"Expected: {input_data}"
            )

            compression_ratio = 1 - len(compressed_data) / len(input_data) if input_data else 0
            print(f"PASSED: Test {i} - Compression percent: {compression_ratio:.2f} - ({description})")
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
