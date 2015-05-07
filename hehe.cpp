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
























