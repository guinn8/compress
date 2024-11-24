#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief Compresses the data in the provided buffer in-place.
 *
 * @param[in,out] data_ptr  Pointer to the buffer containing the data to compress.
 *                          Assume each byte in the input is a number between 0x00 to 0x7F.
 *                          Assume it is common for the input to have the same value repeated.
 *                          After compression, this buffer will contain the compressed data.
 * @param[in] data_size     The number of bytes in the buffer to compress.
 * 
 * @return The size data compressed in the buffer, 0 on fail.
 *
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
