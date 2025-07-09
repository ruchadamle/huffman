#include <iostream>
#include <string>
#include "Huffman.h"

/**
 * Prints how to use command to user
 */
void printInstructions() {
    std::cout << "How to use:\n"
              << "huffman compress <input_file> <output_file>\n"
              << "huffman decompress <input_file> <output_file>\n";
}

int main(int argc, char* argv[]) {
    // if incorrect number of args provided, show how to use command
    if (argc != 4) {
        printInstructions();
        return 1;
    }

    // either compress or decompress
    std::string command = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    Huffman huffman;

    try {
        if (command == "compress") {
            huffman.compress(input_file, output_file);
            std::cout << "Compression completed: " << output_file << std::endl;
        } else if (command == "decompress") {
            huffman.decompress(input_file, output_file);
            std::cout << "Decompression completed: " << output_file << std::endl;
        } else {
            std::cerr << "Unknown command: " << command << "\n";
            printInstructions();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}