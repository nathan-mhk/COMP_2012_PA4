#include <iostream>

#include "utils.h"
#include "hftree.h"
using namespace std;

int main()
{
    Utils ut;
    if (ut.loadDictionary("text.txt")) {
        ut.buildTree();
        ut.setEncodedTable();
        cout << "[start compressing file as encode_paper.bin ...]" << endl;
        ut.saveBinDictionary("encode_paper.bin");
        cout << "[compressed file encode_paper.bin saved. ]" << endl << endl;

        cout << "[start decoding...]" << endl;
        ut.decode("encode_paper.bin");
    }
    return 0;
}
