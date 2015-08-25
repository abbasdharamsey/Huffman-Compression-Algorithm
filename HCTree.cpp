#include "HCTree.hpp"
#include "HCNode.hpp"
#include <queue>
#include <vector>
#include <stack>

/** Method to build Huffman tree.
 *  Simply takes in a vector of frequencies whose indexes correspond to 
 *  ASCII. The ones with 0 frequency are ignored, rest are used to build
 *  the tree.
 */
void HCTree::build(const vector<int>& freqs) {
    // Used to hold nodes that will make the tree
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> priorityQueue;

    // Create a vector of leaves about the vector given
    for (unsigned int i = 0; i < freqs.size(); ++i) {
        if (freqs[i]) {
            leaves[i] = new HCNode(freqs[i], i);
            priorityQueue.push(leaves[i]);
        }
    }

    // If there is only one node, it will be made into a root
    if (priorityQueue.size() == 1) {
        HCNode* toPush = new HCNode(0, 0);
        priorityQueue.push(toPush);
    }

    // If there are several nodes, they need to be rearranged into a Huffman tree
    while (priorityQueue.size() > 1) {
        // Get two nodes with lowest counts
        HCNode* firstPop = priorityQueue.top();
        priorityQueue.pop();
        HCNode* secondPop = priorityQueue.top();
        priorityQueue.pop();

        // Create intermediary node for these two
        HCNode* intermediaryNode = new HCNode(firstPop->count + secondPop->count, 0);
        
        // Put them into tree by setting pointers
        firstPop->p = intermediaryNode;
        secondPop->p = intermediaryNode;
        intermediaryNode->c0 = firstPop;
        intermediaryNode->c1 = secondPop;
        
        // Intermediary is pushed into priority queue for algorithm
        priorityQueue.push(intermediaryNode);
    }

    if (priorityQueue.size()) {
        root = priorityQueue.top();
        priorityQueue.pop();
    }
}

void HCTree::encode(byte symbol, BitOutputStream& out) const {
    // Get node for starting point
    HCNode* head = leaves[symbol];
    stack<char> encodeStack;    // 0s and 1s are pushed here to reverse later

    while (head->p) {
        if (head == head->p->c0)
            encodeStack.push(0);

        else if (head == head->p->c1)
            encodeStack.push(1);

        head = head->p;
    }
    
    while (!encodeStack.empty()) {
        out.writeBit(encodeStack.top());
        encodeStack.pop();
    }
}

int HCTree::decode(BitInputStream& in) const {
    HCNode* head = root;

    while (head->c0 || head->c1) {
        int readbt = in.readBit();
        if (readbt == 0)
            head = head->c0;

        else if (readbt == 1) 
            head = head->c1;
    }

    return head->symbol;
}



/** ================== PRE-CHECKPOINT FUNCTIONS BELOW ================== **/

/** Method to encode any ASCII character using the Huffman tree built using build.
 *  Finds leaf with symbol, traverses up and writes 0s and 1s to stream out based
 *  on algorithm.
 */
void HCTree::encode(byte symbol, ofstream& out) const {
    // Get node for starting point
    HCNode* working = leaves[symbol];
    stack<char> encodeStack;    // 0s and 1s are pushed here to reverse later

    // Traverse up, adding 0s and 1s to the stack as dictated by algorithm
    while (working->p) {
        if (working == working->p->c0) {
            encodeStack.push('0');
            working = working->p;
        }

        else if (working == working->p->c1) {
            encodeStack.push('1');
            working = working->p;
        }
    }

    // Add chars to stream
    while (!encodeStack.empty()) {
        out << encodeStack.top();
        encodeStack.pop();
    }
}

/** Method to decode any ASCII character using the Huffman tree built using build.
 *  Starts at root, traverses down to a leaf using 0s and 1s from stream in, returns
 *  the values.
 */
int HCTree::decode(ifstream& in) const {
    // Root is the starting point when decoding
    HCNode* working = root;
    char currentChar;   // Stores chars as they are read in

    // Traverse down toward leaf using 0s and 1s from stream
    while (working->c0 || working->c1) {
        currentChar = in.get();

        if (currentChar == '0')
            working = working->c0;
        else
            working = working->c1;
    }

    // Returns symbol of the leaf node it ends up at
    return working->symbol;
}
