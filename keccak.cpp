#include "utils.h"

class Keccak {
    private: 
	unsigned long long RC[];
	int r[5][5];
	int c;
	int w;
	int b;
	int l; 
	int nr;

	int convertToInt(string str)
	{
		unsigned int x;   
		std::stringstream ss;
		ss << std::hex << str;
		ss >> x;

		return x; 
	}

    /**
     * Bitwise rotation (to the left) of n bits considering the
     *      string of bits is w bits long.
     */
    unsigned long long rot(unsigned long long x, int n)
    {
        n = n % w;
        return ((x >> (w - n)) + (x << n)) % (1 << w); 
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
		char format[20];
        sprintf(format, "%%0%dX", w / 4);
        char laneHexBE_cstr[20];
        sprintf(laneHexBE_cstr, format, lane);
		string laneHexBE(laneHexBE_cstr);

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
    void printState(Table state, string info)
    {
		cout << "Current value of state: " << info << endl;
        for (int y = 0; y < 5; y++)
        {
            string line = "";
            for (int x = 0; x < 5; x++)
            {
                line += "\t" + hex(state.cell[x][y]);
            }
			cout << line << endl;
		}
	}

    /**
     * Convert a string of bytes to its 5×5 matrix representation.
     * @param s String of bytes of hex-coded bytes (e.g. '9A2C...').
     */
    Table convertStrToTable(string s)
    {
        // Check that input parameters.
        if (w % 8 != 0)
        {
            // Raise exception. "w is not a multiple of 8"
        }

        if (s.length() != 2 * b / 8)
        {
            // Raise exception. "string can't be divided in 25 blocks of w bits\
            // i.e. string must have exactly b bits"
        }

        // Convert
		Table output;

        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                int offset = 2 * ((5 * y + x) * w) / 8;
                output.cell[x][y] = fromHexStringToLane(
                                    s.substr(offset, 2 * w / 8));
            }
        }

        return output;
    }


	Table Round(Table param, unsigned long long RCfixed)
	{
		/* Perfor one round of computation as defined in the Keccak-f permutation

		   A: current state (5×5 matrix)
		   RCfixed: value of round constant to use (integer)
		  */

		unsigned long long A [5][5]; 
		for(int i = 0; i < 5; i++) {
			for(int j = 0; j < 5; j++){
				A[i][j] = param.cell[i][j];
			}
		}
		unsigned long long B[5][5]; 
		unsigned long long C[5]; 
		unsigned long long D[5]; 

		for (int i = 0; i < 5; ++i)
		{
			C[i] = 0; 
			D[i] = 0; 
			for (int j = 0; j < 5; ++j)
				B[i][j] = 0; 
		}

		//Theta step
		for (int x = 0; x < 5; ++x)
			C[x] = pow(A[x][0], pow(A[x][1], pow(A[x][2], pow(A[x][3], A[x][4])))); 

		for (int x = 0; x < 5; ++x)
			D[x] = pow(C[(x - 1) % 5], rot(C[(x + 1) % 5], 1)); 

		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
				A[x][y] = pow(A[x][y], D[x]); 
		}

		//Rho and Pi steps
		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
				B[y][(2 * x + 3 * y) % 5] = rot(A[x][y], r[x][y]); 
		}

		//Chi step
		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
				A[x][y] = pow(B[x][y], ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]));
		}

		//Iota step
		A[0][0] = pow(A[0][0], RCfixed); 

		Table rev(A); 

		return rev;
	}

	Table KeccakF(Table param)
	{
		/* Perform Keccak-f function on the state A

		   A: 5×5 matrix containing the state
		   verbose: a boolean flag activating the printing of intermediate computations
		 */
		for(int i = 0; i < nr; ++i)
			param = Round(param, RC[i] % (1 << w));

		return param;
	}

	string pad10star1(string characters, int length, int n)
	{
		/* Pad M with the pad10*1 padding rule to reach a length multiple of r bits
		   characters: message string of hex characters ('9AFC...')
		   length: 
		   n: length in bits (must be a multiple of 8)
		   Example: pad10star1('BA594E0FB9EBBD30', 60, 8, returns 'BA594E0FB9EBBD93' 
		 */

		//Check the parameter n
		if(n % 8 != 0)
			//throw new KaccakError("n must be a multiple of 8"); 

		//Check the length of the provided string
		if(characters.length() % 2 != 0)
			//Pad with one '0' to reach correct length (don't know test
			//vectors coding)
			characters = characters + '0'; 

		if(length > characters.length() / 2 * 8)
			//throw new KaccakError("The string is too short to contain the number of bits announced");

		int nr_bytes_filled = length / 8; 
		int nbr_bits_filled = length % 8; 
		int l = length % n; 
		unsigned int my_byte; 
		if((n - 8) <= l && l <= (n - 2))
		{
			if(nbr_bits_filled == 0)
				my_byte = 0; 
			else
				my_byte = convertToInt(characters.substr(nr_bytes_filled * 2, 2)); 
			my_byte = (my_byte >> (8 - nbr_bits_filled)); 
			my_byte = my_byte + pow(2, nbr_bits_filled) + pow(2, 7); 
			sprintf(&my_byte, "%02X", my_byte); 
			characters = characters.substr(0, nr_bytes_filled * 2) + my_byte; 
		}
		else
		{
			if(nbr_bits_filled == 0)
				my_byte = 0; 
			else
				my_byte = convertToInt(characters.substr(nr_bytes_filled * 2, 2)); 
			my_byte = (my_byte >> (8 - nbr_bits_filled)); 
			my_byte = my_byte + pow(2, nbr_bits_filled); 
			sprintf(&my_byte, "%02X", my_byte); 
			characters = characters.substr(0, nr_bytes_filled * 2) + my_byte; 
			while((8 * characters.length() / 2) % n < (n - 8))
			{
				characters = characters + "00"; 
			}
			characters = characters + "80";
		}
		return characters; 
	}

	string hash(string characters, int length, int r = 1024, int c = 576, int n = 1024)
	{
		/*
		 * Compute the Keccak[r,c,d] sponge function on message M

		characters: string of hex characters ('9AFC...')
		length: length in bits
		r: bitrate in bits (defautl: 1024), shall be defined in the class
		c: capacity in bits (default: 576), shall be defined in the class
		n: length of output in bits (default: 1024),
		verbose: print the details of computations(default:False)
		*/

		if(r < 0) || (r % 8 != 0)
		//	throw new KeccakError("r must be a multiple of 8 in this implementation"); 
		if(n % 8 != 0)
		//	throw new KeccakError("outputLength must be a multiple of 8"); 

		//Compute lane length (in bits)
		w = (r + c) / 25; 

		//Initialization of state
		Table S; 

		//Padding of messages
		string P = pad10star1(characters, length, r); 

		//Absorbing phase
		for(int i = 0; i < P.length * 8 / 2 / r; ++i)
		{
			Table table = convertStrToTable(P.substr(i * (2 * r / 8), (2 * r / 8)) + string((c/8), "00")); 
			Pi = table.cell; 

			for(int y = 0; y < 5; ++y)
				for(int x = 0; x < 5; ++x)
					S[x][y] = pow(S[x][y], Pi[x][y]); 
			S = KeccakF(S); 
		}

		//Squeezing phase
		string Z = ""; 
		int outputLength = n; 
		while(outputLength > 0)
		{
			string str = converTableToStr(S); 
			Z += str.substr(0, r * 2 / 8); 
			outputLength -= r; 
			if(outputLength > 0)
				S = KeccakF(S).cell; 
		}

		return Z.substr(0, 2 * n / 8); 
	}

    public: 
    //Constructions 
    Keccak() {
		int _r[5][5] = {
			{0, 36, 3, 41, 18},
			{1, 44, 10, 45, 2},
			{62, 6, 43, 15, 61},
			{28, 55, 25, 21, 56},
			{27, 20, 39, 8, 14},
		}; 
		memcpy(r, _r, sizeof(int)*25);
		unsigned long long _RC[] = {
			0x0000000000000001,
			0x0000000000008082,
			0x800000000000808A,
			0x8000000080008000,
			0x000000000000808B,
			0x0000000080000001,
			0x8000000080008081,
			0x8000000000008009,
			0x000000000000008A,
			0x0000000000000088,
			0x0000000080008009,
			0x000000008000000A,
			0x000000008000808B,
			0x800000000000008B,
			0x8000000000008089,
			0x8000000000008003,
			0x8000000000008002,
			0x8000000000000080,
			0x000000000000800A,
			0x800000008000000A,
			0x8000000080008081,
			0x8000000000008080,
			0x0000000080000001,
			0x8000000080008008
		};
		memcpy(RC, _RC, sizeof(unsigned long long) * 24);
    }

    /**
     * Set the value of the parameter b (and thus w,l and nr).
     * @param b Must be choosen among [25, 50, 100, 200, 400, 800, 1600]
     */
    void setB(int _b)
    {
        b = _b;
        w = b / 25;
        l = (int)log2(w);
        nr = 12 + 2 * l;
    }

};

