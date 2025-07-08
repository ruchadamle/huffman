### Introduction 
Huffman's algorithm is a lossless data compression algorithm that compresses files by assigning shorter binary codes to characters that 
appear frequently in the text that the algorithm is to compress and assigning longer binary codes to characters that are 
not as frequent. This program utilizes this algorithm to compress .txt files.

### Usage Information
To compress a file, use ```compress()```, for example: 
```
Huffman *compressor = new Huffman();
string in_file, out_file;

compressor->compress(in_file, out_file);
```
```void compress(string input_file, string output_file)``` takes a .txt file, input_file, to compress it
and outputs the compressed version into output_file. It is recommended to save the file as .huf.txt instead of
.txt to differentiate uncompressed and compressed files.

This compression function can compress files to around 55-60% of their original size. For instance, when ```compress()```
is used:

| **Original File Size** | **Compressed File Size** |
|------------------------|--------------------------|
| 6.16 MB                | 3.43 MB                  |
| 1.2 MB                 | 703 KB                   |
| 20.2 KB                | 11.3 KB                  |

To decompress a compressed file, use ```decompress()```, for example:
```
Huffman *compressor = new Huffman();
string in_file, out_file;

compressor->decompress(in_file, out_file);
```
```void decompress(string input_file, string output_file)``` takes a file compressed by the Huffman algorithm, input_file, and then
decompresses it to its original state. Since this is a lossless compression algorithm, no information
is lost with compression, and the decompressed file will match the original uncompressed file. 


### Implementation Details
**compress():**

First, a hashtable is made to store the frequency of each char. The whole file is parsed, and each time that char appears
in the file, its frequency is incremented. 

Based on this frequency, a Huffman tree is created. First, a priority queue, where the Nodes with the smallest weights
get the highest priority, are created. Each key-value node is inserted into the queue as a pair. Then, the two nodes with
the smallest weights are extracted from the queue and combined into a single node with a combined weight. The zero (left child) of this
parent node is the first extracted node, and the one (right child) is the second extracted node. This process is done until
there is one node left in the queue, meaning that a tree has been formed. 

After this, our newly created tree is traversed to generate Huffman codes for each leaf node, since
those are the only nodes that contain chars.

Next, a header is added to the file where our compressed data will be outputted in that contains the chars and their respective
Huffman codes. Then, each character is read from the original input file, converted to its Huffman 
code, then outputted into the output file. Then, an EOF character is added so that when the file is decompressed, the 
function knows when to stop decoding. Finally, our Huffman tree is freed to avoid memory leaks.

**decompress():**

First, the Huffman tree is recreated based on the header data by creating a hashtable and then creating a tree based on 
the Huffman codes contained in there.

Then, each bit in the encoded file is traversed, converted to its char representative, and then outputted to the output file
until the EOF file character is reached, at which point the decoding process ends. 
The reconstructed tree is then deleted to avoid memory leaks.
