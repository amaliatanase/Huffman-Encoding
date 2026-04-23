#include <iostream>
#include <filesystem>
#include <fstream>
#include <stack>
#include "Huffman.h"

void Huffman::computeHistogram(const std::string &inputFilePath, uint64_t histogram[256]) {
    for (int i = 0 ;i < ALPHABET_SIZE; i++)
        histogram[i] = 0;
    std::ifstream inFile(inputFilePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening the file." << inputFilePath << std::endl;
        return;
    }
    char buffer[BLOCK];
    while (inFile.read(buffer, BLOCK) || inFile.gcount() > 0) {
        std::streamsize bytesRead = inFile.gcount(); //the exact number of available bytes

        for (std::streamsize i = 0; i < bytesRead; i++) {
            uint8_t symbol = static_cast<uint8_t>(buffer[i]); //for each element in the buffer we cast it and add it to the histogram
            histogram[symbol]++;
        }
    }
    inFile.close();
    histogram[0]++;
    histogram[255]++;
}

Node* Huffman::buildHuffmanTree(const uint64_t histogram[256]) {
    std::priority_queue<Node *, std::vector<Node *>, CompareNode> pq;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (histogram[i] > 0) { //add in the pq only the symbols that occur at least once
            Node* newNode = new Node(static_cast<uint8_t>(i), histogram[i]);
            pq.push(newNode);
        }
    }
    while (pq.size() > 1) {
        Node* leftChild =  pq.top();
        pq.pop();
        Node* rightChild =  pq.top();
        pq.pop();
        Node* parent = Node::join(leftChild, rightChild);
        pq.push(parent);
    }
    //the last remaining node in the pq is the root
    Node* root = pq.top();
    pq.pop();
    return root;
}

void Huffman::generateCodeTable(Node* root, Code& currentCode, Code codeTable[ALPHABET_SIZE]) {
    if (root == nullptr) return;
    if (root->left == nullptr && root->right == nullptr) {
        codeTable[root->symbol] = currentCode;
        return;
    }
    if (root->left != nullptr) {
        currentCode.push_bit(0);
        generateCodeTable(root->left, currentCode, codeTable);
        uint8_t dummy; //a temp variable to catch the value of the last bit when backtracking
        currentCode.pop_bit(dummy);
    }
    if (root->right != nullptr) {
        currentCode.push_bit(1);
        generateCodeTable(root->right, currentCode, codeTable);
        uint8_t dummy;
        currentCode.pop_bit(dummy);
    }
}

Node *Huffman::reconstructTree(const uint8_t tree_dump[], uint32_t dump_size) {
    std::stack<Node *> st;
    for (uint32_t i = 0; i < dump_size; i++) {
        if (tree_dump[i] == 'L') {
            uint8_t symbol = tree_dump[++i];
            Node* leaf = new Node(symbol, 0);
            st.push(leaf);
        }
        else if (tree_dump[i] == 'T'){
            Node* rightChild = st.top();
            st.pop();
            Node* leftChild = st.top();
            st.pop();
            Node* parent = Node::join(leftChild, rightChild);
            st.push(parent);
        }
    }
    Node* root = st.top();
    st.pop();
    return root;
}

void Huffman::writeTreeDump(Node* root, std::ofstream& outFile) {
    if (root == nullptr) return;
    writeTreeDump(root->left, outFile);
    writeTreeDump(root->right, outFile);
    //if root is a leaf it writes L followed by the symbol of the leaf
    if (root->left == nullptr && root->right == nullptr) {
        char type = 'L';
        outFile.write(&type, sizeof(char));
        outFile.write(reinterpret_cast<const char*>(&root->symbol), sizeof(uint8_t));
    }
    //otherwise it just writes I
    else {
        char type = 'T';
        outFile.write(&type, sizeof(char));
    }
}

void Huffman::compressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    //generate the histogram
    uint64_t histogram[ALPHABET_SIZE];
    computeHistogram(inputFilePath, histogram);
    int uniqueSymbols = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (histogram[i] > 0)
            uniqueSymbols++;
    //generate the tree and the code table
    Node* root = buildHuffmanTree(histogram);
    Code codeTable[ALPHABET_SIZE];
    Code emptyCode;
    generateCodeTable(root, emptyCode, codeTable);
    std::ifstream inFile(inputFilePath, std::ios::binary);
    std::ofstream outFile(outputFilePath, std::ios::binary);

    if (!inFile || !outFile) {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }
    //generate the header
    Header h;
    h.signature = MAGIC;
    h.tree_size = (3 * uniqueSymbols) - 1;
    h.file_size = std::filesystem::file_size(inputFilePath);
    outFile.write(reinterpret_cast<const char*>(&h), sizeof(Header));
    //write the tree on the disk
    writeTreeDump(root, outFile);

    //read the text and write the codes
    uint8_t writeBuffer = 0;
    int bitsInBuffer = 0;

    char readBuffer[BLOCK];
    while (inFile.read(readBuffer, BLOCK) || inFile.gcount() > 0) {
        std::streamsize bytesRead = inFile.gcount();
        //for each symbol we convert into the corresponding code
        for (std::streamsize i = 0; i < bytesRead; i++) {
            uint8_t symbol = static_cast<uint8_t>(readBuffer[i]);
            Code currentCode = codeTable[symbol];

            //traverse through the bits of the code
            for (uint32_t j = 0; j < currentCode.size(); j++) {
                uint8_t bit = currentCode.get_bit(j);

                //we write the newly added bits at the end of the old ones by shifting them
                writeBuffer = (writeBuffer << 1) | bit;
                bitsInBuffer++;

                //when we have reached a byte it can be written on the disk
                if (bitsInBuffer == 8) {
                    outFile.write(reinterpret_cast<const char*>(&writeBuffer), sizeof(writeBuffer));
                    writeBuffer = 0;  //empty the buffer
                    bitsInBuffer = 0; //reset the counter
                }
            }
        }
    }
    //if there are any bytes left
    if (bitsInBuffer > 0) {
        //if there is less than a byte left, the bits need to be shifted in order to cover 8 bits
        writeBuffer = writeBuffer << (8 - bitsInBuffer);
        outFile.write(reinterpret_cast<const char*>(&writeBuffer), sizeof(writeBuffer));
    }
    //close the files
    inFile.close();
    outFile.close();
}

void Huffman::decompressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inFile(inputFilePath, std::ios::binary);
    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!inFile || !outFile) {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }
    //reading and checking the header
    Header h;
    inFile.read(reinterpret_cast<char*>(&h), sizeof(Header));
    if (h.signature != MAGIC) {
        std::cerr << "Error: Invalid file (MAGIC signature incorrect)!" << std::endl;
        return;
    }
    //reconstruct the tree
    uint8_t* tree_dump = new uint8_t[h.tree_size];
    inFile.read(reinterpret_cast<char*>(tree_dump), h.tree_size);
    Node* root = reconstructTree(tree_dump, h.tree_size);
    delete[] tree_dump;
    //read bit by bit and decode
    Node* currentNode = root;
    uint64_t decodedSymbols = 0;
    uint8_t byte;
    //read from the infile until the original number of symbols is reached
    while (decodedSymbols < h.file_size && inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; i--) {
            //extract the bit on position i
            uint8_t bit = (byte >> i) & 1;
            // 0 -> left, 1 -> right
            if (bit == 0) currentNode = currentNode->left;
            else currentNode = currentNode->right;
            //check if we have reached a leaf
            if (currentNode->left == nullptr && currentNode->right == nullptr) {
                outFile.put(static_cast<char>(currentNode->symbol)); //write the symbol
                decodedSymbols++;
                currentNode = root; //reset to the root for the next symbol
            }
            //stop if file_size is reached
            if (decodedSymbols == h.file_size)
                break;
        }
    }
    //close the files
    inFile.close();
    outFile.close();
}