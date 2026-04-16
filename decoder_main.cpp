/**
* @file decoder_main.cpp
 * @brief Command-line utility for decompressing Huffman encoded files.
 * * Reconstructs the original file using the embedded tree dump.
 * Supports standard flags (-i, -o, -h, -s). Defaults output to stdout if none provided.
 */
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include "Huffman.h"

void printHelp() {
    std::cout << "Usage: ./decoder [Options]\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << " -h            : Help\n" << std::endl;
    std::cout << " -i <infile>   : Compressed file (.huff)\n" << std::endl;
    std::cout << " -o <outfile>  : File to save the original text\n" << std::endl;
    std::cout << " -s            : Outputs detailed statistics about the compression" << std::endl;
}

int main(int argc, char *argv[]) {
    std::string inputFile = "";
    std::string outputFile = "";
    bool showStats = false;
    int option;

    while ((option = getopt(argc, argv, "hi:o:s")) != -1) {
        switch (option) {
            case 'h':
                printHelp();
                return 0;
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 's':
                showStats = true;
                break;
            default: printHelp();
                return 1;
        }
    }
    std::string actualOutput = outputFile;
    if (actualOutput == "")
        actualOutput = "/dev/stdout";
    if (inputFile == "") {
        std::cerr << "Error: Input file (-i) is required." << std::endl;
        return 1;
    }
    Huffman huffman;
    huffman.decompressFile(inputFile, actualOutput);
    if (showStats) {
        try {
            uintmax_t compressedSize = std::filesystem::file_size(inputFile);
            uintmax_t uncompressedSize = std::filesystem::file_size(actualOutput);
            double spaceSaving = 0;
            if (uncompressedSize > 0)
                spaceSaving = 100.0 * (1.0 - (static_cast<double>(compressedSize) / uncompressedSize));
            std::cerr << "Compressed file size: " << compressedSize << " bytes" << std::endl;
            std::cerr << "Decompressed file size: " << uncompressedSize << " bytes" << std::endl;
            std::cerr << "Space saving: " << spaceSaving << " %" << std::endl;
        }catch (const std::exception& e) {
            std::cerr << "Error calculating statistics: " << e.what() << std::endl;
        }
    }

    if (outputFile == "")
        std::cerr << "Decompression successful to stdout!" << std::endl;
    else
        std::cerr << "Decompression successful to " << outputFile << std::endl;
    return 0;
}