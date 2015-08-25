#include "HCTree.hpp"
#include <string>
#include <vector>

using namespace std;

/** Entry point for uncompress. Uses HCTree methods to employ
 *  Huffman coding trees to decompress a file.
 */
int main(int argc, const char* argv[]) {
    // Expects exactly 2 arguments
    if (argc != 3) {
        cerr << "Incorrect format. Use `./compress inputFile outputFile`." << endl;
        return -1;
    }

    // Convert names of files to strings to compare
    string inputFile = string(argv[1]);
    string outputFile = string(argv[2]);

    // If file names are the same, show error
    if (inputFile.compare(outputFile) == 0) {
        cerr << "Incorrect format. inputFile and outputFile cannot be the same." << endl;
        return -1;
    }

    // Open input stream
    ifstream inputFileStream;
    inputFileStream.open(inputFile);
    
    // Check for empty file
    inputFileStream.seekg(0, ios::end);
    if (inputFileStream.tellg() == 0) {
        cerr << "Empty file cannot be compressed." << endl;
        return 1;
    }

    // If everything is fine, resets seekg
    inputFileStream.seekg(0);
    // Vector of frequencies that will go to HCTree::build()
    vector<int> freqs (256, 0);

    // Check if stream is good
    if (!inputFileStream.good()) {
        cerr << "Error opening inputFile." << endl;
        return 1;
    }

    int count;       // Used to hold counts as they are read in
    // Read in counts, add to freqs
    int bytes = 0;
    for (unsigned int i = 0; i < 256; i++) {
        inputFileStream.read((char*)&count, sizeof(int));
        freqs[i] = count;
        bytes += count;
    }

    // freqs used to rebuild the new Huffman tree
    HCTree* mainTree = new HCTree();
    mainTree->build(freqs);

    // Open output stream
    ofstream outputFileStream;
    outputFileStream.open(outputFile);

    BitInputStream* in = new BitInputStream(inputFileStream);

    char currentChar; // Used to hold chars as they are read in
    // Read file, use rebuilt tree to decode contents
    while (bytes) {
        //if (inputFileStream.peek() == EOF) break;
        currentChar = (char) mainTree->decode(*in);
        outputFileStream.put(currentChar);
        bytes--;
    }

    // Close streams
    inputFileStream.close();
    outputFileStream.close();
    delete mainTree;
    return 0;
}
