/**
 * @file compress.c
 * @author Gavin Guinn (gavinguinn1@gmail.com)
 * @brief Implementation of a constrained RLE algorithm.
 * @date 2024-11-24
 * 
 * @link https://github.com/guinn8/compress
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief Compresses data in-place using a customized Run-Length Encoding (RLE) algorithm.
 *
 * This function processes the input buffer and compresses it based on the frequency of repeated literals.
 * It modifies the buffer in-place to store the compressed data, adhering to the following format:
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
 *     - The function assumes that repeated values are common in the input data.
 *     - After compression, the same buffer will contain the compressed output.
 *
 * @param[in] data_size
 *     The number of bytes in the input buffer to be processed for compression.
 *
 * @return 
 *     The size of the compressed data in bytes, or 0 if:
 *     - The input pointer `data_ptr` is `NULL`.
 *     - Any value in the input buffer exceeds the valid range `[0x00, 0x7F]`.
 * 
 * @note 
 * - The function modifies the input buffer directly, the algorithm design ensures we dont overrun our initial buffer.
 */
size_t byte_compress(uint8_t * const data_ptr, const size_t data_size){
    if(data_ptr == NULL) {
        return 0;
    }
    
    size_t run_end = 0, compression_end = 0;
    while(run_end < data_size){
        uint8_t test_literal;
        size_t run_start = run_end, run_len;
        do {
            test_literal = data_ptr[run_end];
            if(!(test_literal >= 0X00 && test_literal < 0x80)){
                return 0; // Value out of range
            }
            run_len = run_end - run_start + 1;
        } while(++run_end < data_size &&
                test_literal == data_ptr[run_end] &&
                run_len < UINT8_MAX);

        if (run_len == 1) {
            data_ptr[compression_end++] = test_literal & 0x7F; // Store 7 bit literal
        } else {
            data_ptr[compression_end++] = (test_literal & 0x7F) | 0x80; // High bit marks a run
            data_ptr[compression_end++] = (uint8_t)run_len; // Store the multiplier
        }
    }

    return compression_end;
}
