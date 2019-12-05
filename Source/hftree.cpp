#include "hftree.h"



HuffmanTree::HuffmanTree() : hfTree(NULL) {}

struct CompareWeight {
    const char& minChar(const HuffmanTreeNode& hfTree) {
        if ((hfTree.leftChild == NULL) && (hfTree.rightChild == NULL)) {
            // non-tree node
            return hfTree.key;
        } else {
            // TODO
            // Tree node
            // Find the min leaf
            const char& left = minChar(*hfTree.leftChild);
            const char& right = minChar(*hfTree.rightChild);
            return ((left < right) ? left : right);
        }
    }

    bool operator()(const HuffmanTreeNode& hft1, const HuffmanTreeNode& hft2) {
        if (hft1.weight > hft2.weight)
            return true;
        else if (hft1.weight == hft2.weight) {
            // TODO
            const char& hft1Char = minChar(hft1);
            const char& hft2Char = minChar(hft2);
            return (hft1Char > hft2Char);

        } else 
            return false;
        
        // return hft1.weight > hft2.weight;
    }
};

void HuffmanTree::loadMap(map<char, int> frequency_table)
{
    // create an minimum priority queue in STL
    // insert your code here ...
    // DEBUG

    // Construct a min priority queue
    priority_queue<HuffmanTreeNode, vector<HuffmanTreeNode>, CompareWeight> pq;
    
    for (map<char, int>::const_iterator itr = frequency_table.begin(); itr != frequency_table.end(); ++itr) {
        HuffmanTreeNode node;
        node.key = itr->first;
        node.weight = itr->second;
        pq.push(node);
    }

    // Build the tree
    while (pq.size != 2) {
        HuffmanTreeNode head;
        head.leftChild = pq.pop;
        head.rightChild = pq.pop;

        head.leftChild->huffman_code = "0";
        head.rightChild->huffman_code = "1";
        
        head.weight = head.leftChild->weight + head.rightChild->weight;

        // HuffmanTreeNode left = pq.pop;
        // HuffmanTreeNode right = pq.pop;
        // mergeTree(&left, &right, &head);
        
        pq.push(head);
    }
    // Attach the root
    hfTree->leftChild = pq.pop;
    hfTree->rightChild = pq.pop;
}

void HuffmanTree::mergeTree(HuffmanTreeNode *bt1, HuffmanTreeNode *bt2,
                       HuffmanTreeNode *pt)
{
    // insert your code here ...
    // DEBUG
    bt1->huffman_code = "0";
    bt2->huffman_code = "1";

    pt->leftChild = bt1;
    pt->rightChild = bt2;

    pt->weight = bt1->weight + bt2->weight;
}


void HuffmanTree::encode(map<char, string>& encoded_table)
{
    // go through every path from root to leaf, then get the encoding of each character(leaf node)
    // you are NOT allowed to use any recursive method here, otherwise you'll get no marks for this function
    // try level-order traversal on this Huffman Tree
    // result encoded_table: char -- character set, string -- encoding of leaf node, like "00", "1001"...

    // insert your code here ...
    // TODO
    // Write INTO the encoded_table
    






}

string HuffmanTree::decode(const string& bin_str)
{
    // decode a binary string to plaintext
    // insert your code here ...
    // TODO
    





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
    // TODO
    // Post-order = LRC
    














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

