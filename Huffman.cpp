#include "Huffman.h"

void Huffman::compress(const std::string &input_file, const std::string &output_file) {
    // make a frequency table for chars
    std::unordered_map<char, int> frequency = createFrequencyTable(input_file);
    // build the Huffman tree based on the created table
    buildHuffmanTree(frequency);
    // encode the file using our Huffman tree
    encodeFile(input_file, output_file);
    // free the memory allocated by the tree
    freeTree();
}

void Huffman::decompress(const std::string &input_file, const std::string &output_file) {
    // decode the file
    decodeFile(input_file, output_file);
    // free the memory allocated by the tree
    freeTree();
}

std::unordered_map<char, int> Huffman::createFrequencyTable(const std::string& input_file) {
    // make a map to store the frequencies in
    // the char is each unique character in the file
    // the int is the amount of times it appears in the file
    std::unordered_map<char, int> frequency;

    // open the file
    std::ifstream huffman_input(input_file);
    char current_char;

    // read characters from the file until there are no more characters to be read
    while (huffman_input.get(current_char)) {
        // increment the frequency of the character
        frequency[current_char]++;
    }

    // EOF char that only appears once
    frequency['\x03'] = 1;

    // return the map with the frequencies of each character
    return frequency;
}

void Huffman::buildHuffmanTree(const std::unordered_map<char, int> &frequency) {
    // ensures the tree is clear before we try building it
    freeTree();

    // make a queue to store the nodes
    // the lowest weight nodes have the highest priority
    std::priority_queue<Node*, std::vector<Node*>, compareWeights> node_queue;

    // adds all the chars with their weights as nodes into the queue
    for (std::pair<char, int> pair : frequency) {
        node_queue.push(new Node(pair.first, pair.second));
    }

    // loop continues while there is more than one node in the queue
    while (node_queue.size() > 1) {
        // get the two smallest nodes in the queue
        Node* left_node = node_queue.top();
        node_queue.pop();
        Node* right_node = node_queue.top();
        node_queue.pop();

        // combine the two nodes by adding their weights together
        // this node is not a leaf, so it has the null character
        Node* new_node = new Node('\0', left_node->weight + right_node->weight,
                                  left_node, right_node);
        // push this combined node back into the queue
        node_queue.push(new_node);
    }

    // set our root to our top-most node
    root = node_queue.top();

    // remove our root to ensure that the priority queue only contains nodes that still
    // need to be used in the future
    node_queue.pop();

    // generate all the Huffman codes for the newly created tree
    generateHuffmanCodes(root, "");
}

void Huffman::generateHuffmanCodes(Node *tree, const std::string &code_string) {
    // if the current node is nullptr, we need to return
    // base case #1, empty tree
    if (tree == nullptr) {
        return;
    }

    // if our current node is a leaf node (meaning that it contains a character),
    // then store the code into our map
    // base case #2, we reach a leaf node
    if (tree->letter != '\0') {
        huffman_codes[tree->letter] = code_string;
    }

    // recursive case (general case)
    // traverses left
    generateHuffmanCodes(tree->zero, code_string + "0");
    // traverses right
    generateHuffmanCodes(tree->one, code_string + "1");
}

void Huffman::encodeFile(const std::string &input_file, const std::string &output_file) {
    // open the input file
    std::ifstream huffman_input;
    huffman_input.open(input_file);
    // throw an error if we can't open our input file
    if (!huffman_input.is_open()) {
        throw std::runtime_error("Failed to open input file.");
    }

    // open storage to write in the output file
    // throw error if we cannot open the output file
    if (!storage.open(output_file, "write")) {
        throw std::runtime_error("Failed to open output file.");
    }

    // make an iterator
    std::unordered_map<char, std::string>::iterator it;
    // make a variable to store the header
    // the header will contain "instructions" to decode the file in the format:
    // [char][Huffman code][\36]
    // \36 is a record separator character in ASCII to separate the characters and their Huffman code
    std::string header_string;

    // iterate through the map and add in each char, associated code, and record separator
    for (it = huffman_codes.begin(); it != huffman_codes.end(); it++)
    {
        header_string += it->first + it->second + "\36";
    }

    // set the file header
    storage.setHeader(header_string);

    // variable for the current character we are reading from the input file
    char current_char;

    // read each character from the input file
    while (huffman_input.get(current_char)) {
        // insert the encoded binary into the output file as soon as we read it in
        storage.insert(huffman_codes[current_char]);
    }

    // close file since we don't need it anymore
    huffman_input.close();

    // add flag to signify that we reached the end of the file
    // 'x03' is an ASCII char that signifies EOF
    storage.insert(huffman_codes['\x03']);

    // close the file
    storage.close();
}

void Huffman::decodeFile(const std::string &input_file, const std::string &output_file) {
    // open the storage to read from the file
    if (!storage.open(input_file, "read")) {
        throw std::runtime_error("Failed to open input file for reading.");
    }

    // open the file to output in
    std::ofstream decoded_file;
    decoded_file.open(output_file);

    // throw an error if we cannot open the file
    if (!decoded_file.is_open()) {
        throw std::runtime_error("Failed to open output file.");
    }

    // get the header
    std::string header = storage.getHeader();

    // reconstruct the tree
    reconstructTree(header);

    // initialize variables to decode
    std::string file_bits;
    // start at the root of the tree
    Node* current_node = root;

    // iterate through each bit stored in the binary string
    while (storage.extract(file_bits)) {
        for (char bit : file_bits) {
            // if the bit is 0, then go to the zero position (left)
            if (bit == '0') {
                current_node = current_node->zero;
                // otherwise, if the bit is 1, then go to the one position (one)
            } else if (bit == '1') {
                current_node = current_node->one;
            }

            // if it's a leaf node, then it must contain a char
            if (current_node->letter != '\0') {
                // if we reach our EOF char, break
                if (current_node->letter == '\x03') {
                    break;
                }
                // output the char
                decoded_file << current_node->letter;
                // reset to root after each outputted char
                current_node = root;
            }
        }
    }

    // close the files
    decoded_file.close();
    // close the file opened for reading
    storage.close();
}

void Huffman::reconstructTree(std::string &header) {
    // make a map to store the reconstructed Huffman tree in
    std::unordered_map<char, std::string> reconstructed_code_map;
    // variable to store the position of our record separator
    size_t position;

    // loop keeps running as long as there is a record separator in between each position
    // if find() cannot find the record separator, it returns npos
    while ((position = header.find("\36")) != std::string::npos) {
        // the first char is the encoded char
        char encoded_char = header[0];
        // after the encoded char, its Huffman code follows
        // the code is from after the encoded char to before the record separator
        std::string huffman_code = header.substr(1, position - 1);
        // store the key (char) and value (Huffman code) in the code map
        reconstructed_code_map[encoded_char] = huffman_code;
        // erase the segment we just analyzed to move onto the next one
        header.erase(0, position + 1);
    }

    // initialize a dummy root for our reconstructed tree
    root = new Node('\0', 0);

    // iterate through each key-value pair in the map
    for (std::pair<const char, std::string> pair : reconstructed_code_map) {
        // char to store the letter of a leaf node
        char letter = pair.first;
        // variable to store the binary code associated with a lead node
        std::string code = pair.second;
        // start constructing at the root of the tree
        Node* current = root;

        // traverse the string of code, bit by bit
        for (char bit : code) {
            if (bit == '0') {
                // if the 0 node isn't already created, create it
                if (!current->zero) {
                    current->zero = new Node('\0', 0);  // Create a left child with dummy values
                }
                // move to the zero child
                current = current->zero;
            } else { // bit == '1'
                // if the 1 node isn't already created, create it
                if (!current->one) {
                    current->one = new Node('\0', 0);  // Create a right child with dummy values
                }
                // move to the one child
                current = current->one;
            }
        }
        // after all the bits are traversed, we have reached a leaf node that contains the char
        current->letter = letter;
    }
}

void Huffman::freeTree() {
    // call helper function
    freeNodes(root);
    root = nullptr;
}

void Huffman::freeNodes(Node* node) {
    // base case
    if (node == nullptr) {
        return;
    }

    // use post-order traversal to delete nodes
    freeNodes(node->zero);
    freeNodes(node->one);
    delete node;
}