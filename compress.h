#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Compresses the data in the provided buffer in-place.
 *
 * @param[in,out] data_ptr  Pointer to the buffer containing the data to compress.
 *                          Assume each byte in the input is a number between 0x00 to 0x7F.
 *                          Assume it is common for the input to have the same value repeated.
 *                          After compression, this buffer will contain the compressed data.
 * @param[in] data_size     The number of bytes in the buffer to compress.
 * 
 * @return The size data compressed in the buffer.
 *
 */
size_t byte_compress(uint8_t *data_ptr, size_t data_size);
