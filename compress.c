#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "compress.h"


/**
 * @brief Compresses the data in the provided buffer in-place.
 *
 * @param[in,out] data_ptr  Pointer to the buffer containing the data to compress.
 *                          Assume each byte in the input is a number between 0x00 to 0x7F.
 *                          Assume it is common for the input to have the same value repeated.
 *                          After compression, this buffer will contain the compressed data.
 * @param[in] data_size     The number of bytes in the buffer to compress.
 * 
 *  so a big problem we could run into is our RLE over running the in-place buffer like many encoding schemes this is an edge case
 *
 *  so we can get around that by being clever, singleton numbers will be encoded as a normal 7 bit int. 
 *  so in the works case where we dont have any runs at all we are not overrunning our buffer
 *  but if we have a run we flip bit 8, this will signal that the following byte is a length multiplier
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
        size_t run_start = run_end;
        do {
            test_literal = data_ptr[run_end];
            assert(test_literal >=0 && test_literal < 128);
            printf("literal: %d run_len: %ld run_start: %ld, run_end:%ld\n\r", test_literal, run_end - run_start + 1, run_start, run_end);
        } while(++run_end < data_size &&
                test_literal == data_ptr[run_end]);

        printf("run_end %ld data_size %ld\n", run_end, data_size);
        if(run_end - 1 == run_start) {
            data_ptr[compression_end] = test_literal;
            compression_end++;



        } else {
            // so at this point I have the bounds of a run captured
            // so we need to collapse these bounds into a tuple where
            // - we need to set the high bit of the run_start
            // - we need to set run_start + 1 to the multiplier
            // but this complecates matter because we need to compress in place
            // so we need to track an index of the end of compressed data
            // we do have a strict gaurentee that end of compressed data < run_start


        }
    }

    return compression_end;
}

/**
 * [D]
 * 
 * 
 * 
 */

/**
 * @brief Decompresses the data from a source buffer into a destination buffer.
 *
 * @param[in] src_ptr Pointer to the buffer containing the compressed data.
 * @param[in] compressed_size Number of compressed bytes.
 * @param[out] dest_ptr Pointer to the buffer where the decompressed data will be written.
 * @param[in] dest_capacity Size of the destination, large enough to accommodate the decompression.
 *
 * @return The size of the decompressed data, or 0 on fail.
 *
 */
size_t byte_decompress(const uint8_t *src_ptr, size_t compressed_size, uint8_t *dest_ptr, size_t dest_capacity){
    return 0;
}

