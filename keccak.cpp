#include "utils.h"

class Keccak {
    private: 
	int r; 
	int c;
	int w;
	int b;
	int l; 
	int nr;
    /**
     * Bitwise rotation (to the left) of n bits considering the
     *      string of bits is w bits long.
     */
    unsigned long long rot(unsigned long long x, int n)
    {
        n = n % this.w;
        return ((x >> (this.w - n)) + (x << n)) % (1 << this.w); 
    }

    /**
     * Convert a string of bytes written in hexadecimal to a lane value.
     */
    unsigned long long fromHexStringToLane(string s)
    {
        if (s.length() % 2 != 0)
        {
            // TODO: throw exception "The provided string does not end with a full byte"
        }

        string temp = "";
        int nrBytes = s.length() / 2;
        for (int i = 0; i < nrBytes; i++)
        {
            int offset = (nrBytes - i - 1) * 2;
            temp += s.substr(offset, 2);
        }

        unsigned long long retval;
        std::stringstream ss;
        ss << std::hex << temp;
        ss >> retval;

        return retval;
    }

    /**
     * Convert a lane value to a string of bytes written in hexadecimal.
     */
    string fromLaneToHexString(unsigned long long lane)
    {
        string format;
        sprintf(format, "%%0%dX", this.w / 4);
        string laneHexBE;
        sprintf(laneHexBE, format, lane);

        // Perform the modification.
        string temp = "";
        int nrBytes = laneHexBE.length() / 2;
        for (int i = 0; i < nrBytes; i++)
        {
            int offset = (nrBytes - i - 1) * 2;
            temp += laneHexBE.substr(offset, 2);
        }

        return upper(temp);
    }

    /**
     * Print on screen the state of the sponge function preceded by string info.
     * @param state State of the sponge function
     * @param info A string of characters used as identifie
     */
    void printState(unsigned long long[][5] state, string info)
    {
        printf("Current value of state: %s\n", info);
        for (int y = 0; y < 5; y++)
        {
            string line = "";
            for (int x = 0; x < 5; x++)
            {
                line += "\t" + hex(state[x][y]);
            }

            printf("\t%s", line);
        }
    }

    /**
     * Convert a string of bytes to its 5×5 matrix representation.
     * @param s String of bytes of hex-coded bytes (e.g. '9A2C...').
     */
    unsigned long long[][5] convertStrToTable(string s)
    {
        // Check that input parameters.
        if (this.w % 8 != 0)
        {
            // Raise exception. "w is not a multiple of 8"
        }

        if (s.length() != 2 * this.b / 8)
        {
            // Raise exception. "string can't be divided in 25 blocks of w bits\
            // i.e. string must have exactly b bits"
        }

        // Convert
        unsigned long long[5][5] output = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        };

        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                int offset = 2 * ((5 * y + x) * this.w) / 8;
                output[x][y] = this.fromHexStringToLane(
                                    s.substr(offset, 2 * this.w / 8))
            }
        }

        return output;
    }

    public: 
    //Constructions 
    public Keccak() {
    }

    /**
     * Set the value of the parameter b (and thus w,l and nr).
     * @param b Must be choosen among [25, 50, 100, 200, 400, 800, 1600]
     */
    void setB(int b)
    {
        this.b = b;
        this.w = b / 25;
        this.l = (int)log2(this.w);
        this.nr = 12 + 2 * this.l;
    }

};

