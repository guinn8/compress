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
 * @param[in,out] data_ptr Pointer to the data buffer to be compressed.
 * @param[in] data_size Size of the data buffer in bytes.
 * @return Size of the compressed data in bytes, or 0 on error.
 */
size_t byte_compress(uint8_t * const data_ptr, const size_t data_size){
    if(data_ptr == NULL) {
        return 0;
    }
    
    size_t run_end = 0, compression_end = 0;
    while(run_end < data_size){
        uint8_t test_literal, run_len;
        size_t run_start = run_end;
        do {
            test_literal = data_ptr[run_end];
            if(test_literal >= 0x80){
                return 0; // Value out of range
            }
            run_len = run_end - run_start + 1;
        } while(++run_end < data_size &&
                test_literal == data_ptr[run_end] &&
                run_len <= UINT8_MAX);

        if (run_len == 1) {
            data_ptr[compression_end++] = test_literal; // Literal in range [0x00, 0x7F]
        } else {
            data_ptr[compression_end++] = test_literal | 0x80; // It's a run! MSB set. 
            data_ptr[compression_end++] = (uint8_t)run_len; // Run length (2–255)
        }
    }

    return compression_end;
}
