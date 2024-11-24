import subprocess
from decompress import byte_decompress

# Path to the `compress` binary
COMPRESS_BINARY = "./bin/compress"

TEST_CASES = [
    ("No repeats", [10, 20, 30, 40, 50]),
    ("All same", [10, 10, 10, 10]),
    ("Mix repeats", [10, 20, 20, 20, 30, 40, 40, 50]),
    ("One value", [70] * 7),
    ("Alternating", [10, 20, 10, 20, 10, 20]),
    ("Max run", [1] * 255),
    ("Single value", [10]),
    ("Large w/ repeats", [10, 10, 20, 20, 30] * 200),
    ("Empty", []),
    ("Unique values", list(range(1, 101))),
    ("Low edge", [0, 1, 0, 1]),
    ("High edge", [127, 126, 127, 126]),
    ("One no repeat", [42]),
    ("Random reps", [10, 10, 20, 20, 30, 30, 40, 40, 50, 50]),
    ("Large unique", list(range(1, 501))),
    ("Run 127", [42] * 127),
    ("Run 128", [42] * 128),
    ("Multi runs", [10] * 10 + [20] * 20 + [30] * 30),
    ("Runs & unique", [10, 10, 20, 30, 30, 30, 40, 50]),
]

def run_compress(input_data):
    """
    Runs the `compress` binary with the input data and returns the compressed output.
    """
    process = subprocess.Popen(
        [COMPRESS_BINARY],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    input_str = ",".join(map(str, input_data)) + "\n"
    stdout, stderr = process.communicate(input=input_str.encode())

    if process.returncode != 0:
        raise RuntimeError(f"Compression failed: {stderr.decode().strip()}")
    
    returned_string = stdout.decode().strip()

    return list(map(int, returned_string.split(",")))

def run_tests():
    """
    Runs all test cases and validates the results.
    """
    passed = 0
    failed = 0

    for i, (description, input_data) in enumerate(TEST_CASES, start=1):
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
            print(f"PASSED: Test {i} ({description})")
            passed += 1
        except Exception as e:
            print(f"FAILED: Test {i} ({description})\n{e}")
            failed += 1

    print("\nSummary:")
    print(f"Passed: {passed}")
    print(f"Failed: {failed}")

if __name__ == "__main__":
    run_tests()
