import ctypes

LIBRARY_PATH = "./libcompress.so"  # Path to the shared library
lib = ctypes.CDLL(LIBRARY_PATH)  # Load the shared library

# Define the function argument and return types
lib.byte_compress.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t]
lib.byte_compress.restype = ctypes.c_size_t

TEST_CASES = [
    ("Given example", [0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56,
                       0x09, 0x09, 0x09], True),
    ("Single number", [42], True),
    ("Large run", [42] * 10000, True),
    ("Repeated runs", [10]*100 + [20]*100 + [10]*100 + [20]*100, True),
    ("Long alternating runs", [10]*100 + [20]*100 + [10]*100 + [20]*100, True),
    ("Large w/ repeats", [10, 10, 20, 20, 30] * 200, True),
    ("Multiple runs with max length", [10]*255 + [20]*255, True),
    ("Max value runs", [127] * 255, True),
    ("Min value runs", [0] * 255, True),
    ("Run length at boundary", [42]*254, True),
    ("Run length at maximum", [42]*255, True),
    ("Run length exceeding maximum", [42]*256, True),
    ("Runs exceeding UINT8_MAX", [10]*300, True),
    ("Runs of varying lengths", [10]*100 + [20]*200 + [30]*300, True),
    ("Increasing sequence", list(range(0, 128)), True),
    ("Alternating runs of max length", [10]*255 + [20]*255, True),
    ("Mix repeats", [10, 20, 20, 20, 30, 40, 40, 50], True),
    ("Alternating", [10, 20, 10, 20, 10, 20], True),
    ("Random reps", [10, 10, 20, 20, 30, 30, 40, 40, 50, 50], True),
    ("Unique values", list(range(1, 101)), True),
    ("Empty", [], True),
    ("Value out of range", [128], False),
    ("Value out of range", [256], False),
    ("Values just out of range", list(range(0, 129)), False),
    ("Max length runs with singles", [70]*255 + [80] + [70]*255, True),
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
    results = []

    for i, (description, input_data, should_pass) in enumerate(TEST_CASES, start=1):
        try:
            compressed_data = run_compress(input_data)
            decompressed_data = byte_decompress(compressed_data)
            assert decompressed_data == input_data, "Input and decompressed data do not match."
            compression_ratio = (
                f"{(1 - len(compressed_data) / len(input_data)) * 100:.2f}%" 
                if input_data else "N/A"
            )
            results.append((i, description, "PASSED", compression_ratio))
            passed += 1
        except Exception as e:
            if not should_pass:
                results.append((i, description, "PASSED (Expected Failure)", "N/A"))
                passed += 1
            else:
                results.append((i, description, "FAILED", "N/A"))
                failed += 1

    print(f"{'Test #':<7} {'Description':<40} {'Result':<25} {'Reduction %':<15}")
    print("-" * 90)
    for test_num, desc, result, comp in results:
        print(f"{test_num:<7} {desc:<40} {result:<25} {comp:<15}")
    print("-" * 90)
    print(f"Total Tests: {len(TEST_CASES):<5} | Passed: {passed:<5} | Failed: {failed:<5}")

if __name__ == "__main__":
    run_tests()
