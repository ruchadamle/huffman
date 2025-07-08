#include <iostream>
#include "Huffman.h"

using namespace std;

int main() {
    Huffman *compressor = new Huffman();
    int choice = 0;
    string in_file, out_file;

    cout << "Would you like to:" << endl
         << "1) Compress a file" << endl
         << "2) Decompress a file" << endl;

    cin >> choice;
    if (1 == choice) {
        cout << "Enter the path of the file to be compressed: ";
        cin >> in_file;
        cout << "Enter the path where you'd like the compressed file to be saved: ";
        cin >> out_file;
        compressor->compress(in_file, out_file);

    } else if (2 == choice) {
        cout << "Enter the path of the file to be decompressed: ";
        cin >> in_file;
        cout << "Enter the path where you'd like the decompressed file to be saved: ";
        cin >> out_file;
        compressor->decompress(in_file, out_file);

    } else {
        cout << "That is not a valid choice." << endl;
    }

    return 0;
}
