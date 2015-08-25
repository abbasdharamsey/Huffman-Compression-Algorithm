#include "HCTree.hpp" 
#include <string>
#include <vector>

using namespace std;

/** Entry point for compress. Uses HCTree methods to employ
 *  Huffman coding trees to compress a file.
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

    // Reads files by character, incrementing ASCII frequencies in freqs
    while (inputFileStream.good()) {
        if (inputFileStream.peek() == EOF) break;
        unsigned int currentChar = inputFileStream.get();
        freqs[currentChar]++;
    }

    // Create tree
    HCTree* mainTree = new HCTree();
    mainTree->build(freqs);

    // Open output stream
    ofstream outputFileStream;
    outputFileStream.open(outputFile);

    // Write all counts to file as 4-byte ints
    for (unsigned int i = 0; i < 256; i++) {
        unsigned int writeInt = freqs[i];
        outputFileStream.write((char*)&writeInt, sizeof(int));
    }

    // Clear input stream becuase it needs to be used again
    inputFileStream.clear();
    inputFileStream.seekg(0);

    // Create new BitOutputStream to write to file
    BitOutputStream* out = new BitOutputStream(outputFileStream);

    // Read characters, use Huffman tree to encode into 0s and 1s
    while (inputFileStream.good()) {
        if (inputFileStream.peek() == EOF){
            out->flush();
            break;
        }
        unsigned int currentChar = inputFileStream.get();
        mainTree->encode(currentChar, *out);
    }

    // Closes streams
    inputFileStream.close();
    outputFileStream.close();
    delete mainTree;
    return 0;
}
