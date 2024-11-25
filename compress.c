/**
 * @file compress.c
 * @author Gavin Guinn (gavinguinn1@gmail.com)
 * @brief Implementation of a constrained RLE algorithm.
 * @date 2024-11-24
 * 
 * @link https://github.com/guinn8/compress
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

/**
 * @brief Compresses data in-place using a customized Run-Length Encoding (RLE) algorithm.
 *
 * Compresses the input buffer in-place, suitable for data with repeated literals.
 * The algorithm operates on data in the range [0x00, 0x7F]. It ensures the compressed 
 * data never exceeds never exceeds the length of the input data.
 *
 * Compression Format:
 * - Single Literals: Encoded as-is in the compressed output.
 * - Runs of Repeated Literals  Sequences of the same literal are encoded as a two-byte pair:
 *   - First Byte: Literal value with the MSB set.
 *   - Second Byte: Run length (2–255).
 *
 * @param[in,out] data Pointer to the data buffer to be compressed.
 * @param[in] data_len Size of the data buffer in bytes.
 * @return Size of the compressed data in bytes, or 0 on error.
 */
size_t byte_compress(uint8_t * const data, const size_t data_len){
    if(data == NULL) {
        return 0;
    }
    
    size_t read_index = 0, compress_index = 0;
    while(read_index < data_len){
        uint8_t test_literal, run_length;
        size_t start_index = read_index;
        do {
            test_literal = data[read_index];
            if(test_literal >= 0x80){
                return 0;
            }
            run_length = read_index - start_index + 1;
        } while(++read_index < data_len &&
                test_literal == data[read_index] &&
                run_length < UINT8_MAX);

        if (run_length == 1) {
            assert((test_literal & 0x80) == 0); // Demonstrate that the MSB is 0.
            data[compress_index++] = test_literal; // Store 7 bit literal.
        } else {
            data[compress_index++] = test_literal | 0x80; // Its a run! Set the MSB.
            data[compress_index++] = run_length; // Store the multiplier.
        }
    }

    return compress_index;
}
