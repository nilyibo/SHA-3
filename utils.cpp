// Util functions.

#include <sstream>

/**
 * Convert a string to its uppercase.
 */
string upper(string s)
{
    int length = s.length();
    for (int i = 0; i < length; i++)
    {
        char c = s[i];
        if ('a' <= c && c <= 'z')
        {
            s[i] = c - 'a' + 'A';
        }
    }

    return s;
}

string hex(unsigned long long n)
{
    std::stringstream s;
    s << std::hex << n;
    return s.str();
}

string strToHexStr(string input)
{
    int length = input.length();
    string output = "";
    for (int n = 0; n < length; n++)
    {
        char c = input[n];
        output += hex((unsigned long long)c);
    }

    return output;
}

