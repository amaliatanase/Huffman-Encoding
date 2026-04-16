/**
* @file Node.h
 * @brief Defines the structure of a Huffman tree node.
 */
#pragma once
#include <cstdint>

/**
 * @class Node
 * @brief Represents a single element (leaf or interior node) in the Huffman tree.
 */
class Node {
public:
    /**
     * @brief Constructor for a new Node.
     * @param symbol The 8-bit character this node represents.
     * @param frequency The number of times this symbol appears in the input data.
     */
    Node(uint8_t symbol, uint64_t frequency);
    /**
     * @brief Joins two child nodes to create an interior parent node.
     * * The new parent node will have the symbol '$' and a frequency equal
     * to the sum of its children's frequencies.
     * * @param leftChild Pointer to the node that will become the left child.
     * @param rightChild Pointer to the node that will become the right child.
     * @return Pointer to the newly dynamically allocated parent node.
     */
    static Node* join(Node* leftChild,Node* rightChild);
    friend class Huffman;
    /**
     * @brief Gets the frequency of the node.
     * @return The occurrence count of the symbol.
     */
    uint64_t getFrequency() const {return frequency;};
private:
    Node *left;          ///< Pointer to the left child node.
    Node *right;         ///< Pointer to the right child node.
    uint8_t symbol;      ///< The character stored in this node.
    uint64_t frequency;  ///< The weight/frequency of this node.
};


