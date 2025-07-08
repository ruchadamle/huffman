#include <string>
#include <queue>
#include <fstream>
#include <unordered_map>
#include "Node.h"
#include "Storage/Storage.h"

#ifndef HUFFMAN_H
#define HUFFMAN_H

/**
 * @class Huffman
 *
 * This class provides functionalities to compress and decompress a given text file using the Huffman
 * compression algorithm.
 */
class Huffman {
private:
    Node* root;                                          // the tree's root
    std::unordered_map<char, std::string> huffman_codes; // map to store all the huffman codes
    Storage storage;                                     // storage used to store binary code

    /**
     * Creates a frequency table for how often the file's characters appear
     * @param input_file the file to read from
     * @return unordered_map with frequency of each character in the text file
     */
    std::unordered_map<char, int> createFrequencyTable(const std::string &input_file);

    /**
     * Builds a Huffman tree based on a given map of a char and its frequency
     * @param frequency hashmap that contains a char as the key and its frequency as the value
     */
    void buildHuffmanTree(const std::unordered_map<char, int>& frequency);

    /**
     * Generates Huffman codes from a given Huffman tree
     * @param root the tree to traverse
     * @param code_string the variable in which the binary codes are stored in
     */
    void generateHuffmanCodes(Node* root, const std::string& code_string);

    /**
     * Encodes the input file and prints the encoded version into the output file
     * @param input_file the file to be encoded
     * @param output_file the encoded file
     */
    void encodeFile(const std::string& input_file, const std::string& output_file);

    /**
     * Decodes the input file and prints the decoded version into the output file
     * @param input_file the file to be decoded
     * @param output_file the decoded file
     */
    void decodeFile(const std::string& input_file, const std::string& output_file);

    /**
     * Reconstructs a Huffman tree given a header containing chars and their respective Huffman codes
     * Note: the header needs to be in [char][Huffman code][\36] format to work
     * @param header the header from which the tree can be reconstructed from
     */
    void reconstructTree(std::string &header);

    /**
     * Cleans up the Huffman tree by deallocating all nodes
     */
    void freeTree();

    /**
     * Helper method for freeTree() to delete all the nodes in the tree
     * @param node the current node to be deleted, starting at the root of the tree
     */
    void freeNodes(Node* node);

public:
    /**
     * Compresses a given input file using Huffman compression and outputs the compressed
     * version to another file
     *
     * @param input_file the file to compress
     * @param output_file the compressed file
     */
    void compress(const std::string &input_file, const std::string &output_file);

    /**
     * Decompresses a file that used Huffman compression and outputs the decompressed
     * version to another file
     *
     * @param input_file the file to decompress
     * @param output_file the decompressed file
     */
    void decompress(const std::string &input_file, const std::string &output_file);
};

#endif //HUFFMAN_H
