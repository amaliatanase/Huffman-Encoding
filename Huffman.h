/**
* @file Huffman.h
 * @brief Core logic for the Huffman compression and decompression algorithms.
 */
#pragma once
#include "defines.h"
#include "Node.h"
#include <vector>
#include <queue>
#include "Code.h"

/**
 * @struct Header
 * @brief Represents the metadata written at the beginning of a compressed file.
 */
struct Header {
    uint32_t signature; ///< Magic number to verify file validity (0xDEADEAEF).
    uint32_t tree_size; ///< Size of the serialized Huffman tree dump in bytes.
    uint64_t file_size; ///< Original size of the uncompressed file in bytes.
};

/**
 * @struct CompareNode
 * @brief Comparator used by the priority queue to order nodes by frequency.
 */
struct CompareNode {
    /**
     * @brief Compares two nodes based on their frequencies.
     * @return True if n1 has a greater frequency than n2 (min-heap logic).
     */
    bool operator() (Node* const& n1, Node* const& n2) {
        return n1->getFrequency() > n2->getFrequency();
    }
};

/**
 * @class Huffman
 * @brief Handles the complete lifecycle of Huffman encoding and decoding.
 */
class Huffman {
public:
    /**
     * @brief Computes the frequency of each byte in the input file.
     * @param inputFilePath Path to the file to be analyzed.
     * @param histogram Array of size ALPHABET_SIZE where frequencies will be stored.
     */
    void computeHistogram(const std::string& inputFilePath, uint64_t histogram[ALPHABET_SIZE]);

    /**
     * @brief Builds the optimal Huffman tree using a priority queue.
     * @param histogram The pre-computed array of character frequencies.
     * @return Pointer to the root node of the generated Huffman tree.
     */
    Node* buildHuffmanTree(const uint64_t histogram[ALPHABET_SIZE]);

    /**
     * @brief Generates the binary codes for all characters by traversing the tree.
     * @param root Pointer to the root of the Huffman tree.
     * @param currentNode A temporary Code object used during traversal (must be empty initially).
     * @param codeTable Array where the resulting codes will be stored, indexed by character.
     */
    void generateCodeTable(Node* root, Code& currentNode, Code codeTable[ALPHABET_SIZE]);

    /**
     * @brief Rebuilds a Huffman tree from its serialized post-order dump.
     * @param tree_dump Array containing the 'L' (Leaf) and 'T' (Interior) byte sequence.
     * @param dump_size The length of the tree_dump array.
     * @return Pointer to the root of the reconstructed Huffman tree.
     */
    Node* reconstructTree(const uint8_t tree_dump[], uint32_t dump_size);

    /**
     * @brief Serializes the Huffman tree to a file using post-order traversal.
     * @param root Pointer to the root of the tree to be dumped.
     * @param outFile The output file stream where the tree will be written.
     */
    void writeTreeDump(Node* root, std::ofstream& outFile);

    /**
     * @brief Compresses an input file using Huffman encoding.
     * @param inputFilePath Path to the original file to compress.
     * @param outputFilePath Path where the compressed binary file (.huff) will be saved.
     */
    void compressFile(const std::string& inputFilePath, const std::string& outputFilePath);

    /**
     * @brief Decompresses a Huffman encoded file back to its original state.
     * @param inputFilePath Path to the compressed file (.huff).
     * @param outputFilePath Path where the restored data will be written (or /dev/stdout).
     */
    void decompressFile(const std::string& inputFilePath, const std::string& outputFilePath);

    void freeTree(Node* root);
};



