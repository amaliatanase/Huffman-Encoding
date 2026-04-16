# Huffman Project: Encoder and Decoder (C++)

This project implements the **Huffman** *lossless* data compression and decompression algorithm. It consists of two command-line utilities that work together to reduce file sizes and accurately restore them to their original state.

## 🚀 Features
* **Encoder (`encoder`)**: Calculates character frequencies, builds an optimal Huffman tree, and writes a compressed binary file (`.huff`) using bit-level manipulation (bit-packing).
* **Decoder (`decoder`)**: Reads the header and serialized tree structure from the compressed file to reconstruct the original text. It supports output redirection to `stdout` if no output file is specified.
* **Detailed Statistics (`-s`)**: Both applications can calculate and display the percentage of space saved after compression.

## 🛠️ Compilation

The project uses the C++17 standard (specifically for `std::filesystem`). To compile the two utilities, open a terminal in the project folder and run:

```bash
# Compile the Encoder
g++ -std=c++17 encoder_main.cpp Huffman.cpp Node.cpp Code.cpp -o encoder

# Compile the Decoder
g++ -std=c++17 decoder_main.cpp Huffman.cpp Node.cpp Code.cpp -o decoder
```

## 📖Usage
### 1. Compression (Encoder)
To compress a file, use the `encoder` utility:
```bash
./encoder -i original_text.txt -o compressed_file.huff -s
```

### 2. Decompression (Decoder)
To restore a compressed file, use the `decoder` utility:
```bash
./decoder -i compressed_file.huff -o recovered_text.txt -s
```
*(If you omit the `-o` flag, the decoded text will be printed directly to the terminal / `stdout`)*.

### Available Options:
* `-h` : Displays the help message and exits.
* `-i <file>` : Specifies the input file.
* `-o <file>` : Specifies the output file.
* `-s` : Displays detailed compression statistics (original size, compressed size, and space saved).

## 📚Documentation
The detailed codebase documentation was generated using **Doxygen**.
To view the documentation locally, open the following file in any web browser:
`html/index.html`