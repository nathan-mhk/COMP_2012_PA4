#include "utils.h"

Utils::Utils()  {}

Utils::~Utils()
{
    tree.release();
}

bool Utils::loadDictionary(const string& text_dic)
{
    // load the file with name text_dic, and save all characters in this->content
    ifstream load_file(text_dic);
    if (!load_file.is_open()) {
        cout << "Failed to load the article. Exit." << endl;
        return false;
    }

    ostringstream buf;
    char ch;
    while (buf && load_file.get(ch))
        buf.put(ch);
    this->content = buf.str();
    // move out the end useless '\r' of a file
    this->content.pop_back();

    load_file.close();

    // scan the string this->content, calculate the frequency table
    // insert your code here ...
    // DEBUG
    for (string::const_iterator itr = content.begin(); itr != content.end(); ++itr) {
        if (frequency_table.find(*itr) != frequency_table.end())
            frequency_table[*itr] = 0;
        else
            ++frequency_table[*itr];
    }
    return true;
}

void Utils::buildTree()
{
    tree.loadMap(frequency_table);
}

void Utils::setEncodedTable()
{
    tree.encode(encoded_table);
}

inline int power(int pow) {
    int value = 1;
    while (pow != 0) {
        value *= 2;
        --pow;
    }
    return value;
}

void Utils::saveBinDictionary(const string& bin_file)
{
    // load key file
    ifstream ifile("xor_key.bin", ios::binary);
    if (!ifile.is_open()) {
        cout << "cannot load key file, exit." << endl;
        return;
    }

    ofstream ofile(bin_file, ios::binary);

    // assuming that the length of bin_code is multiple of 8
    // so just for every 8 continuous {0,1} characters, intepret it as a binary byte number
    // for last byte you write, its valid length may be less than 8, fill the invalid bits with 0
    // eg., last byte contains only 3 valid bits, 110. You should fill it as 1100 0000, and then
    // you need to write integer 3 (bin form: 0000 0011) as one byte at the beginning of your binary file.
    // after saving data into .bin file, you should print out its hex form in command line
    // insert your code here ...
    // DEBUG

    // binCode stores 0s and 1s
    string binCode = "";

    // Store the Huffman code of all of the char in content into binCode
    for (string::const_iterator itr = content.begin(); itr != content.end(); ++ itr) {
        map<char, string>::const_iterator item = encoded_table.find(*itr);
        binCode += item->second;
    }

    int cutOffNum = binCode.length() % 8;
    
    // Fill (8 - cutOffNum) 0s to the end
    for (int i = 0; i < (8 - cutOffNum); ++i)
        binCode += "0";

    if (cutOffNum != 0) {
        // Convert to 8-bit bin
        string header = "";
        while (cutOffNum != 0) {
            header = (cutOffNum % 2 == 0 ? "0" : "1") + header;
            cutOffNum /= 2;
        }

        //Extent it to 8-bit
        for (int i = 0; i < (8 - header.length()); ++i)
            header = "0" + header;
        
        binCode = header + binCode;
    }

    char key;
    string::const_iterator itr = binCode.begin();
    while (ifile.get(key)) {
        int dec = 0;
        unsigned char bin;

        // Convert 8-bit bin to dec
        for (int i = 7; i >= 0; --i, ++itr) {
            if (*itr == '1') {
                dec += power(i);
            }
        }
        bin = dec;
        bin = bin ^ ((unsigned char)key);
        ofile << bin;
        cout << hex << (int)bin;
    }
    cout << endl;
    ofile.close();
    ifile.close();
}

void Utils::decode(const string& bin_file)
{
    ifstream ifile(bin_file, ios::binary);
    if (!ifile.is_open()) {
        cout << "cannot open .bin file, stop decoding." << endl;
        return;
    }

    ifstream key_file("xor_key.bin", ios::binary);
    if (!key_file.is_open()) {
        cout << "cannot load key file, exit." << endl;
        return;
    }
    
    // the string used to search on huffman tree to decode as plaintext
    string bit_str = "";

    // bin_file: stores a binary huffman code with possible extra bits at the end
    // key_file: decryption XOR key
    // search in the encoded table
    // insert your code here ...
    // TODO
    






    ifile.close();
    key_file.close();

    // using huffman tree you've built before to decode the bit string
    string plaintext = tree.decode(bit_str);
    cout << plaintext << endl << endl;
}
