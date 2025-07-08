#ifndef NODE_H
#define NODE_H

/**
 * @struct Node
 *
 * This struct provides functionalities for a node to be used in a tree in the Huffman
 * compression algorithm. Each node has a letter to store the char it holds, a weight
 * for its frequency, a zero for its left child, a one for its right child, and a code
 * for its Huffman code that is generated after traversing the Huffman tree.
 */
struct Node {

    /**
     * Parameterized constructor
     * @param letter the char the node contains
     * @param weight the weight (how often the char shows up) of the node
     */
    Node(char letter, int weight) {
        this->letter = letter;
        this->weight = weight;
        this->zero = nullptr;
        this->one = nullptr;
        this->code = "";

    }

    /**
     * Parameterized constructor
     * @param letter the char the node contains
     * @param weight the weight (how often the char shows up) of the node
     * @param zero the node's left child
     * @param one the node's right child
     */
    Node(char letter, int weight, Node *zero, Node *one) {
        this->letter = letter;
        this->weight = weight;
        this->zero = zero;
        this->one = one;
        this->code = "";
    }

    char letter;        // letter character to be stored
    Node* zero;         // the node's left child
    Node* one;          // the node's right child
    int weight;         // count for how many times the character is used in the file
    std::string code;   // binary string code created from the huffman tree
};


#endif //NODE_H


#ifndef COMPARE_WEIGHTS
#define COMPARE_WEIGHTS
/**
 * Class needed for the to compare Nodes in the priority queue
 * @see https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
 */
struct compareWeights{
    /**
     * Compares the weights of two nodes to insert nodes in order into priority queue
     * @param lhs the left node to compare
     * @param rhs the right node to compare
     * @return true if lhs is ordered before rhs, false if lhs is ordered after rhs
     */
    bool operator()(Node* const& lhs, Node* const& rhs){
        return lhs->weight > rhs->weight;
    }
};
#endif // COMPARE_WEIGHTS