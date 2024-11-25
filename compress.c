/**
 * @file compress.c
 * @author Gavin Guinn (gavinguinn1@gmail.com)
 * @brief Implementation of a constrained RLE algorithm.
 * @date 2024-11-24
 * 
 * @link https://github.com/guinn8/compress
 * 
 * This compression algorithm is suitable for data where repeated literals are common.
 * This encoding relies on the input data being in the range `[0x00, 0x7F]`.
 * This algorithm compresses the input buffer in-place, no additional memory is allocated.
 * This design ensures the compressed data never exceeds exceeds the length of the input data.
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief Compresses data in-place using a customized Run-Length Encoding (RLE) algorithm.
 *
 * This function modifies the buffer in-place to store the compressed data, adhering to the following format:
 * 
 * - **Single Literals:** A single literal value is directly encoded as-is in the compressed output.
 *   For example, an input of `[0x10, 0x20, 0x30]` will be compressed to `[0x10, 0x20, 0x30]`.
 * 
 * - **Runs of Repeated Literals:** Sequences of the same literal are encoded as a 2-byte pair:
 *   - **First Byte:** The literal value with the most significant bit (MSB) set (`literal | 0x80`).
 *   - **Second Byte:** A multiplier indicating the length of the run (2–255).
 *   For example, an input of `[0x0A, 0x0A, 0x0A]` will be compressed to `[0x8A, 0x03]`.
 * 
 * @param[in,out] data_ptr
 *     Pointer to the input buffer containing the data to be compressed.
 *     - Each byte in the input buffer must represent a value in the range `[0x00, 0x7F]`.
 *     - After compression, this buffer will contain the compressed output.
 *
 * @param[in] data_size The number of bytes in the input buffer to be processed for compression.
 *
 * @return The size of the compressed data in bytes, or 0 on error.
 * 
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
            data_ptr[compression_end++] = test_literal | 0x80; // MSB set to indicate a run
            data_ptr[compression_end++] = (uint8_t)run_len; // Run length (2–255)
        }
    }

    return compression_end;
}
