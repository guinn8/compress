import sys

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

def main():
    try:
        input_data = sys.stdin.read().strip()
        compressed = list(map(int, input_data.split(",")))
    except ValueError:
        print("Error: Invalid input. Please provide integers separated by commas.", file=sys.stderr)
        sys.exit(1)

    try:
        decompressed = byte_decompress(compressed)
    except Exception as e:
        print(f"Error during decompression: {e}", file=sys.stderr)
        sys.exit(1)

    print(",".join(map(str, decompressed)))

if __name__ == "__main__":
    main()
