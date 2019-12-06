#include "hftree.h"



HuffmanTree::HuffmanTree() : hfTree(NULL) {}

struct CompareWeight {
    const char& minChar(const HuffmanTreeNode* hfTree) {
        if ((hfTree->leftChild == NULL) && (hfTree->rightChild == NULL)) {
            // Non-tree node
            return hfTree->key;
        } else {
            // Tree node
            const char& left = minChar(hfTree->leftChild);
            const char& right = minChar(hfTree->rightChild);
            return ((left < right) ? left : right);
        }
    }

    bool operator()(const HuffmanTreeNode* hft1, const HuffmanTreeNode* hft2) {
        if (hft1->weight > hft2->weight)
            return true;
        else if (hft1->weight == hft2->weight) {
            // Get the min-ASCII char, either the node itself or the leaf
            const char& hft1Char = minChar(hft1);
            const char& hft2Char = minChar(hft2);
            return (hft1Char > hft2Char);
        } else 
            return false;
    }
};

void HuffmanTree::loadMap(map<char, int> frequency_table)
{
    // create an minimum priority queue in STL
    // insert your code here ...
    // DEBUG

    // Construct a min priority queue
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, CompareWeight> pq;

    for (map<char, int>::const_iterator itr = frequency_table.begin(); itr != frequency_table.end(); ++itr) {
        HuffmanTreeNode* node;
        node->key = itr->first;
        node->weight = itr->second;
        pq.push(node);
    }

    // Build the tree
    while (pq.size != 2) {
        HuffmanTreeNode* parent;
        HuffmanTreeNode* left = pq.top();
        pq.pop();
        HuffmanTreeNode* right = pq.top();
        pq.pop();
        mergeTree(left, right, parent);

        pq.push(parent);
    }
    // Attach the root
    hfTree->leftChild = pq.top();
    pq.pop();
    hfTree->rightChild = pq.top();
    pq.pop();
}

void updateHFCode(HuffmanTreeNode* left, HuffmanTreeNode* right, HuffmanTreeNode* parent) {
    if ((left != NULL) && (right != NULL)) {
        string parentCode = parent->huffman_code;
        left->huffman_code = parentCode + left->huffman_code;
        right->huffman_code = parentCode + right->huffman_code;

        updateHFCode(left->leftChild, left->rightChild, left);
        updateHFCode(right->leftChild, right->rightChild, right);
    }
}

void HuffmanTree::mergeTree(HuffmanTreeNode *bt1, HuffmanTreeNode *bt2,
                       HuffmanTreeNode *pt)
{
    // insert your code here ...
    // DEBUG
    HuffmanTreeNode*& left = bt1;
    HuffmanTreeNode*& right = bt2;
    HuffmanTreeNode*& parent = pt;

    // Update the Huffman code of all children
    left->huffman_code = "0";
    right->huffman_code = "1";

    updateHFCode(left->leftChild, left->rightChild, left);
    updateHFCode(right->leftChild, right->rightChild, right);

    // Construct the tree
    parent->leftChild = left;
    parent->rightChild = right;
    parent->weight = left->weight + right->weight;
}


void HuffmanTree::encode(map<char, string>& encoded_table)
{
    // go through every path from root to leaf, then get the encoding of each character(leaf node)
    // you are NOT allowed to use any recursive method here, otherwise you'll get no marks for this function
    // try level-order traversal on this Huffman Tree
    // result encoded_table: char -- character set, string -- encoding of leaf node, like "00", "1001"...

    // insert your code here ...
    // DEBUG
    // Non-recursive level-order traversal
    queue<HuffmanTreeNode*> hftQueue;
    if (hfTree != NULL)
        hftQueue.push(hfTree);

    while (!hftQueue.empty()) {
        HuffmanTreeNode* head = hftQueue.front();
        hftQueue.pop();

        if ((head->leftChild == NULL) && (head->rightChild == NULL)) {
            // Leaf node
            encoded_table[head->key] = head->huffman_code;
        } else {
            // Tree node
            hftQueue.push(head->leftChild);
            hftQueue.push(head->rightChild);
        }
    }
}

string HuffmanTree::decode(const string& bin_str)
{
    // decode a binary string to plaintext
    // insert your code here ...
    // DEBUG
    string text = "";
    const HuffmanTreeNode* node = hfTree;
    for (string::const_iterator itr = bin_str.begin(); itr != bin_str.end(); ++itr) {
        if ((node->leftChild == NULL) && (node->rightChild == NULL)) {
            // Leaf node, retrieve the key and reset node
            text += node->key;
            node = hfTree;
        } else if (*itr == '0')
            node = node->leftChild;
        else
            node = node->rightChild;
    }
}

void HuffmanTree::release()
{
    // release all tree nodes in post-order
    // you must use non-recursive traversal to release the tree
    // you need to print out the weight of nodes with post-order when you delete the node
    // hint: try to use stack
    // if you use recursive deletion, you will lose partial marks of this PA

    cout << "[start releasing the huffman tree...]" << endl;
    if (hfTree == NULL)
        return;

    // store the nodes in vector path with post-order first
    vector<HuffmanTreeNode*> path;

    // insert your code here ...
    // DEBUG
    // Reverse of post-order, CRL
    // Pop the top one first (C), then check:
    // If not leaf node, push L then R to stack, repeat
    // path is filled from last to first
    stack<HuffmanTreeNode*> hftStack;
    hftStack.push(hfTree);

    while (!hftStack.empty()) {
        HuffmanTreeNode* node = hftStack.top();
        hftStack.pop();
        // Insert to the front of the vector
        path.insert(path.begin(), node);

        if ((node->leftChild != NULL) && (node->rightChild != NULL)) {
            hftStack.push(node->leftChild);
            hftStack.push(node->rightChild);
        }
    }

    for (int i = 0; i < path.size(); i++) {
        if (i != path.size() - 1)
            cout << dec << path[i]->weight << " ";
        else
            cout << dec << path[i]->weight << endl;

        // delete the node
        delete path[i];
    }
    cout << "[successfully released.]" << endl;
}

