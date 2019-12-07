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
    for (string::const_iterator itr = content.begin(); itr != content.end(); ++itr) {
        if (frequency_table.find(*itr) != frequency_table.end())
            ++frequency_table[*itr];
        else 
            frequency_table[*itr] = 1;
    }

    // Debug msg
    // cout << "frequency_table==================" << endl;
    // cout << "[" << content << "]" << endl;
    // for (map<char, int>::const_iterator itr = frequency_table.begin(); itr != frequency_table.end(); ++itr) {
    //     cout << "[" << itr->first << "][" << itr->second << "]" << endl;
    // }
    // cout << "=================================" << endl;

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

// Convert an 8-bit binary string to dec
int binToDec(const string& binString) {
    int decimal = 0;
    string::const_iterator itr = binString.begin();

    for (int i = 7; i >= 0; --i, ++itr) {
        if (*itr == '1')
            decimal += power(i);
    }
    return decimal;
}

// Convert decimal to 8-bit zero extende binary string
void decToBin(int decimal, string& binString) {
    while (decimal != 0) {
        binString = ((decimal % 2 == 0) ? "0" : "1") + binString;
        decimal /= 2;
    }
    while (binString.length() != 8)
        binString = "0" + binString;
}


string result = "";
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

    // binCode stores 0s and 1s
    string binCode = "";

    // Store the Huffman code of all of the char in content into binCode
    for (string::const_iterator itr = content.begin(); itr != content.end(); ++itr)
        binCode += encoded_table.find(*itr)->second;

    // Debug msg
    // cout << "binCode [" << binCode << "]" << endl;
    result = binCode;

    // int cutOffNum = binCode.length() % 8;

    // if (cutOffNum != 0) {
    //     // Fill (8 - cutOffNum) 0s to the end
    //     for (int i = 0; i < (8 - cutOffNum); ++i)
    //         binCode += "0";

    //     // Convert cutOffNum to binary header
    //     string header = "";
    //     decToBin(cutOffNum, header);

    //     binCode = header + binCode;
    // }

    // // Debug msg
    // // cout << "Final binCode [" << binCode << "]" << endl;

    // char key;
    // int i = 0;
    // while (ifile.get(key)) {
    //     // Convert the binary string to decimal byte by byte
    //     unsigned char bin = binToDec(binCode.substr(i, 8));

    //     // XOR encryption, write to file, then print written value in hex
    //     bin = bin ^ (unsigned char)key;
    //     ofile << bin;
    //     cout << setfill('0') << setw(2) << hex << (int)bin;
    //     i += 8;
    // }

    // cout << dec << "\n" << endl;
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

    // char byte;
    // char key;
    // while (key_file.get(key)) {
    //     ifile.get(byte);

    //     // XOR decrpytion, convert it to binary string, then stored it into bit_str
    //     unsigned code = (unsigned char)byte ^ (unsigned char)key;
    //     string binString = "";
    //     decToBin((int)code, binString);
    //     bit_str += binString;
    // }

    // // Debug msg
    // // cout << "bit_str = [" << bit_str << "]" << endl;

    // // Handle the cut off
    // int cutOffLength = bit_str.length() - 8 - binToDec(bit_str.substr(0, 8));
    // bit_str = bit_str.substr(8, cutOffLength);

    // Debug msg
    // cout << "Final bit_str = [" << bit_str << "]" << endl;

    ifile.close();
    key_file.close();

    // using huffman tree you've built before to decode the bit string
    // string plaintext = tree.decode(bit_str);
    string plaintext = tree.decode(result);
    cout << plaintext << endl << endl;
}
