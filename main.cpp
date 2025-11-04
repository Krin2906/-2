//
// Created by Manju Muralidharan on 10/19/25.

// Huffman-like Variable-Bit Encoder
// Pipeline overview:
// 1) Read input.txt and count the frequency of lowercase letters.
// 2) Create leaf nodes in parallel arrays (charArr, weightArr, leftArr, rightArr)
// 3) Use the MinHeap to create the encoding tree. Combine the two smallest nodes until there is only one root.
// 4) Iterative traversal using std::stack: assign codes using left = 0, right = 1.
// 5) Print the code table and the encoded message.
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"
using namespace std;

// Global arrays for node information
const int MAX_NODES = 64;
int weightArr[MAX_NODES];
int leftArr[MAX_NODES];
int rightArr[MAX_NODES];
char charArr[MAX_NODES];

// Function prototypes
void buildFrequencyTable(int freq[], const string& filename);
int createLeafNodes(int freq[]);
int buildEncodingTree(int nextFree);
void generateCodes(int root, string codes[]);
void encodeMessage(const string& filename, string codes[]);

int main() {
    int freq[26] = {0};

    // Step 1: Read file and count letter frequencies
    buildFrequencyTable(freq, "input.txt");

    // Step 2: Create leaf nodes for each character with nonzero frequency
    int nextFree = createLeafNodes(freq);

    // Safety check: if we didn't find any letters in the file, we can't do anything
    if (nextFree == 0) {
        // Still need to print the expected format even with no data
        cout << "Character : Code\n\nEncoded message:\n\n";
        return 0;  // exit early since there's nothing to encode
    }

    // Step 3: Build encoding tree using your heap
    int root = buildEncodingTree(nextFree);
    if (root == -1) {
        cout << "Character : Code\n\nEncoded message:\n\n";
        return 0;
    }

    // Step 4: Generate binary codes using an STL stack
    string codes[26];
    generateCodes(root, codes);

    // Step 5: Encode the message and print output
    encodeMessage("input.txt", codes);

    return 0;
}

/*------------------------------------------------------
    Function Definitions (Students will complete logic)
  ------------------------------------------------------*/

// Step 1: Read file and count frequencies
void buildFrequencyTable(int freq[], const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    char ch;
    while (file.get(ch)) {
        // Convert uppercase to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Count only lowercase letters
        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
    file.close();

    cout << "Frequency table built successfully.\n";
}

// Step 2: Create leaf nodes for each character
int createLeafNodes(int freq[]) {
    int nextFree = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            charArr[nextFree] = 'a' + i;
            weightArr[nextFree] = freq[i];
            leftArr[nextFree] = -1;
            rightArr[nextFree] = -1;
            nextFree++;
        }
    }
    cout << "Created " << nextFree << " leaf nodes.\n";
    return nextFree;
}

// Step 3: Build the encoding tree using heap operations
int buildEncodingTree(int nextFreeIdx) {
    MinHeap nodeHeap;   // our working heap for combining nodes

    //  Toss all initial leaf nodes into the heap
    // (each index represents a symbol or something)
    for (int i = 0; i < nextFreeIdx; i++) {
        nodeHeap.push(i, weightArr);
    }

    // No symbols? No tree.
    if (nodeHeap.size == 0) {
        return -1;
    }

    // Combine nodes until we’re left with a single root
    while (nodeHeap.size > 1) {
        // grab the two smallest weights
        int left = nodeHeap.pop(weightArr);
        int right = nodeHeap.pop(weightArr);

        // Not trying to overflow the node buffer
        if (nextFreeIdx >= MAX_NODES) {
            cerr << "[error] Ran out of node space while building tree!" << endl;
            break; // Could throw but this will do for now
        }

        // make a new internal node
        int parent = nextFreeIdx++;
        leftArr[parent] = left;
        rightArr[parent] = right;

        weightArr[parent] = weightArr[left] + weightArr[right]; // internal node’s weight is the sum of its children

        charArr[parent] = '\0';  // internal marker no character here

        // stick the new parent back into the heap
        nodeHeap.push(parent, weightArr);


    }

    //  The last remaining item in the heap is our root
    int rootNode = nodeHeap.pop(weightArr);


    if (nodeHeap.size != 0) { // check (heap should now be empty)
        cerr << "[warn] Heap not empty after building? Something’s fishy.\n";
    }

    return rootNode;
}



// Step 4: Use an STL stack to generate codes
void generateCodes(int rootNode, string codes[]) {
    // Clear any previous codes
    for (int i = 0; i < 26; i++) codes[i] = "";

    if (rootNode == -1) return; // no symbols so nothing to do

    struct Frame {
        int node;
        string path;
    };

    stack<Frame> s;
    s.push({rootNode, ""}); // start at root

    while (!s.empty()) {
        Frame cur = s.top(); s.pop();

        // leaf if it has a character
        if (charArr[cur.node] != '\0') {
            char letter = charArr[cur.node];
            if (letter >= 'a' && letter <= 'z') {
                string code = cur.path.empty() ? "0" : cur.path; // singlechar edge case
                codes[letter - 'a'] = code;
            }
            continue;
        }

        // push children (right first, then left)
        int L = leftArr[cur.node];
        int R = rightArr[cur.node];

        if (R != -1) s.push({R, cur.path + "1"}); // right = 1
        if (L != -1) s.push({L, cur.path + "0"}); // left  = 0
    }
}


// Step 5: Print table and encoded message
void encodeMessage(const string& fileName, string codes[]) {
    // print the code table
    cout << "Character : Code" << endl;
    for (int i = 0; i < 26; i++) {
        if (codes[i] != "") {
            char letter = 'a' + i;
            cout << letter << " : " << codes[i] << endl;
        }
    }

    cout << "\nEncoded message:\n";

    // open the file and start encoding
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        cerr << "[error] Could not open input file: " << fileName << endl;
        cout << "" << endl;
        return;
    }

    string encoded = "";
    char ch;

    while (inFile.get(ch)) {
        // lowercase conversio
        if (ch >= 'A' && ch <= 'Z') {
            ch = ch - 'A' + 'a';
        }

        // only encode lowercase alphabet chars
        if (ch >= 'a' && ch <= 'z') {
            int idx = ch - 'a';
            string codeForChar = codes[idx];

            if (codeForChar.size() > 0) {
                encoded += codeForChar;
            } else {

            }
        }
        //Everything else (spaces, punctuation) just ignored
    }

    // Finally, print encoded string
    cout << encoded << endl;

    inFile.close();
}
