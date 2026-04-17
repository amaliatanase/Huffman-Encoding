/**
* @file Code.h
 * @brief Defines a stack-based structure to hold bits for Huffman codes.
 */
#pragma once
#include "defines.h"

/**
 * @class Code
 * @brief Represents a binary code as a stack of bits.
 * * This class is used to dynamically build the binary path (0s and 1s)
 * while traversing the Huffman tree.
 */
class Code {
public:
    /**
     * @brief Default constructor. Initializes an empty code.
     */
    Code();

    /**
     * @brief Checks if the code stack is empty.
     * @return True if empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Checks if the code stack has reached its maximum capacity.
     * @return True if full, false otherwise.
     */
    bool full() const;

    /**
     * @brief Gets the current number of bits in the code.
     * @return The size of the code sequence.
     */

    /**
     * @brief Pushes a single bit (0 or 1) onto the code stack.
     * @param bit The bit to add.
     * @return True if successful, false if the stack is full.
     */
    bool push_bit(uint8_t bit);

    /**
     * @brief Pops the last bit off the code stack.
     * @param bit Reference to a variable where the popped bit will be stored.
     * @return True if successful, false if the stack is empty.
     */
    bool pop_bit(uint8_t& bit);

    /**
     * @brief Retrieves a bit at a specific index without removing it.
     * @param index The position of the bit to retrieve.
     * @return The bit at the specified index, or 0 if out of bounds.
     */
    uint8_t get_bit(uint32_t index) const;
    uint32_t size() const;
private:
    uint32_t top;                       ///< The current index of the top of the stack.
    uint8_t bits[MAX_CODE_SIZE];        ///< Array holding the individual bits.
};