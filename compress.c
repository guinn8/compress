#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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
 * @return The size data compressed in the buffer, 0 on fail.
 *
 */
size_t byte_compress(uint8_t *data_ptr, size_t data_size){
    if(data_ptr == NULL) {
        return 0;
    }

    printf("first byte = %d\n\r", data_ptr[0]);

    return 0;
}

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

