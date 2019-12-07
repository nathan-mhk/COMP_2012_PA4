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

    // Construct a min priority queue
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, CompareWeight> pq;

    for (map<char, int>::const_iterator itr = frequency_table.begin(); itr != frequency_table.end(); ++itr) {
        HuffmanTreeNode* node = new HuffmanTreeNode;
        node->key = itr->first;
        node->weight = itr->second;
        pq.push(node);
    }

    // Build the tree
    while (pq.size() != 1) {
        HuffmanTreeNode* parent = new HuffmanTreeNode;
        HuffmanTreeNode* left = pq.top();
        pq.pop();
        HuffmanTreeNode* right = pq.top();
        pq.pop();
        
        // Debug msg
        // cout << "Merging==========================" << endl;
        // cout << "left:  [" << left->key << "], w(" << left->weight << ")" << endl;
        // cout << "right: [" << right->key << "], w(" << right->weight << ")" << endl;

        mergeTree(left, right, parent);

        // Debug msg
        // cout << "Merge result:" << endl;
        // cout << "parent: [" << parent->key << "], w(" << parent->weight << "), hf[" << parent->huffman_code << "]" << endl;
        // cout << "Left:   [" << parent->leftChild->key << "], w(" << parent->leftChild->weight << "), hf[" << parent->leftChild->huffman_code << "]" << endl;
        // cout << "Right:  [" << parent->rightChild->key << "], w(" << parent->rightChild->weight << "), hf[" << parent->rightChild->huffman_code << "]" << endl;
        // cout << "=================================" << endl;

        pq.push(parent);
    }
    // Attach the root
    hfTree = pq.top();
    pq.pop();
}

void updateHFCode(HuffmanTreeNode* parent, const string& binChar) {
    parent->huffman_code = binChar + parent->huffman_code;

    if ((parent->leftChild != NULL) && (parent->rightChild != NULL)) {
        updateHFCode(parent->leftChild, binChar);
        updateHFCode(parent->rightChild, binChar);
    }
}

void HuffmanTree::mergeTree(HuffmanTreeNode *bt1, HuffmanTreeNode *bt2,
                       HuffmanTreeNode *pt)
{
    // insert your code here ...
    HuffmanTreeNode*& left = bt1;
    HuffmanTreeNode*& right = bt2;
    HuffmanTreeNode*& parent = pt;

    // Update the Huffman code of all children
    updateHFCode(left, "0");
    updateHFCode(right, "1");

    // Construct the tree
    parent->key = '\0';
    parent->weight = left->weight + right->weight;
    parent->leftChild = left;
    parent->rightChild = right;
}


void HuffmanTree::encode(map<char, string>& encoded_table)
{
    // go through every path from root to leaf, then get the encoding of each character(leaf node)
    // you are NOT allowed to use any recursive method here, otherwise you'll get no marks for this function
    // try level-order traversal on this Huffman Tree
    // result encoded_table: char -- character set, string -- encoding of leaf node, like "00", "1001"...

    // insert your code here ...
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

    // Debug msg
    // for (map<char, string>::const_iterator itr = encoded_table.begin(); itr != encoded_table.end(); ++itr)
    //     cout << "[" << itr->first << "] = [" << itr->second << "]" << endl;
}

string HuffmanTree::decode(const string& bin_str)
{
    // decode a binary string to plaintext
    // insert your code here ...
    // FIXME

    string text = "";
    const HuffmanTreeNode* node = hfTree;
    
    for (string::const_iterator itr = bin_str.begin(); itr != bin_str.end(); ++itr) {
        if (*itr == '0')
            node = node->leftChild;
        else
            node = node->rightChild;
        
        if ((node->leftChild == NULL) && (node->rightChild == NULL)) {
            text += node->key;
            node = hfTree;
        }
    }
    return text;
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

