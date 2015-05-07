#include <iostream>
#include <string>
#include "keccak.cpp"

using namespace std;

int main()
{
    Keccak k;
    string input = "This is a test string";
    string output = k.hash(input);
    cout << "Input: " << input << endl
        << "Output: " << output << endl;
    return 0;
}
