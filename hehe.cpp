#include <vector>
#include <math.h>

using namespace std; 

class KeccakError: public exception
{
	string err; 
	virtual KeccakError(string message)
	{
		err = message; 
	}

	virtual const string getMsg()
	{
		return err; 
	}
}; 

ostream& operator<<(ostream& os, const KeccakError& err)
{
    os << err.getMsg() << endl;
    return os;
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
    	throw new KaccakError("n must be a multiple of 8"); 

    //Check the length of the provided string
    if(characters.length() % 2 != 0)
    	//Pad with one '0' to reach correct length (don't know test
        //vectors coding)
    	characters = characters + '0'; 

    if(length > characters.length() / 2 * 8)
    	throw new KaccakError('The string is too short to contain the number of bits announced')

    int nr_bytes_filled = length / 8; 
    int nbr_bits_filled = length % 8; 
    int l = length % n; 
    if((n - 8) <= l && l <= (n - 2))
    {
    	if(nbr_bits_filled == 0)
    		
    }
}

string Keccak(string characters, int length, int r = 1024, int c = 576, int n = 1024)
{
	"""Compute the Keccak[r,c,d] sponge function on message M

    characters: string of hex characters ('9AFC...')
    length: length in bits
    r: bitrate in bits (defautl: 1024), shall be defined in the class
    c: capacity in bits (default: 576), shall be defined in the class
    n: length of output in bits (default: 1024),
    verbose: print the details of computations(default:False)
    """

	if(r < 0) || (r % 8 != 0)
		throw new KeccakError('r must be a multiple of 8 in this implementation'); 
	if(n % 8 != 0)
		throw new KeccakError('outputLength must be a multiple of 8'); 

	//Compute lane length (in bits)
	w = (r + c) / 25; 

	//Initialization of state
	S = double[5][5]; 

	//Padding of messages
	string P = pad10star1(characters, length, r); 

	//Absorbing phase
	for(int i = 0; i < P.length * 8 / 2 / r; ++i)
	{
		double[][] Pi = convertStrToTable(P.substr(i * (2 * r / 8), (2 * r / 8)) + string((c/8), '00')); 

		for(int y = 0; y < 5; ++y)
			for(int x = 0; x < 5; ++x)
				S[x][y] = pow(S[x][y], Pi[x][y]); 
		S = KeccakF(S); 
	}

	//Squeezing phase
	string Z = ''; 
	int outputLength = n; 
	while(outputLength > 0)
	{
		string str = converTableToStr(S); 
		Z += str.substr(0, r * 2 / 8); 
		outputLength -= r; 
		if(outputLength > 0)
			S = KeccakF(S); 
	}

	return Z.substr(0, 2 * n / 8); 
}
























