/**
* @file defines.h
 * @brief Contains global constants and macros used throughout the Huffman project.
 */
#pragma once
#include <cstdint>

#define BLOCK 4096                           ///< The size of the buffer used for reading/writing files.
#define ALPHABET_SIZE 256                    ///< The total number of possible byte values (ASCII + Extended ASCII).
#define MAGIC 0xDEADEAEF                     ///< The 32-bit magic number used to verify valid Huffman compressed files.
#define MAX_CODE_SIZE (ALPHABET_SIZE / 8)    ///< The maximum length of a Huffman code in bytes (256 bits).
#define MAX_TREE_SIZE (3 * ALPHABET_SIZE - 1) ///< The maximum possible size of the serialized Huffman tree.