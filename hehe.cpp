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

int convertToInt(string str)
{
	unsigned int x;   
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;

    return x; 
}

string converTableToStr(Table param)
{
	
}

Table Round(Table param, unsigned long long RCfixed)
{
	/* Perform one round of computation as defined in the Keccak-f permutation

       A: current state (5×5 matrix)
       RCfixed: value of round constant to use (integer)
      */

    unsigned long long A[][] = param.cell; 
    unsigned long long B[][] = new unsigned long long[5][5]; 
    unsigned long long C[] = new unsigned long long[5]; 
    unsigned long long D[] = new unsigned long long[5]; 

    for (int i = 0; i < 5; ++i)
    {
    	C[i] = 0; 
    	D[i] = 0; 
    	for (int j = 0; j < 5; ++j)
    	{
    		B[i][j] = 0; 
    	}
    }

    //Theta step
    for (int x = 0; x < 5; ++x)
    	C[x] = pow(A[x][0], pow(A[x][1], pow(A[x][2], pow(A[x][3], A[x][4])))); 

    for (int x = 0; x < count; ++x)
    	D[x] = pow(C[(x - 1) % 5], rot(C[(x + 1) % 5], 1)); 

    for (int x = 0; x < 5; ++x)
    	for (int y = 0; y < 5; ++y)
    		A[x][y] = pow(A[x][y], D[x]); 

    //Rho and Pi steps
    for (int i = 0; i < 5; ++i)
    	for (int i = 0; i < 5; ++i)
    		B[y][(2 * x + 3 * y) % 5] = rot(A[x][y], r[x][y]); 

    //Chi step
    for (int i = 0; i < 5; ++i)
    	for (int i = 0; i < 5; ++i)
    		A[x][y] = pow(B[x][y], ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]); 

    //Iota step
    A[0][0] = pow(A[0][0], RCfixed); 

    Table rev = new Table(A); 

    return A
}

Table KeccakF(Table param)
{
	/* Perform Keccak-f function on the state A

       A: 5×5 matrix containing the state
       verbose: a boolean flag activating the printing of intermediate computations
     */

    for(int i = 0; i < nr; ++i)
    	A = Round(param, RC[i] % (1 << w))

    return A
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
    unsigned int my_byte; 
    if((n - 8) <= l && l <= (n - 2))
    {
    	if(nbr_bits_filled == 0)
    		my_byte = 0; 
    	else
    		my_byte = convertToInt(characters.substr(nr_bytes_filled * 2, 2)); 
    	my_byte = (my_byte >> (8 - nbr_bits_filled)); 
    	my_byte = my_byte + pow(2, nbr_bits_filled) + pow(2, 7); 
    	spirntf(&my_byte, "%02X", my_byte); 
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
    	spirntf(&my_byte, "%02X", my_byte); 
    	characters = characters.substr(0, nr_bytes_filled * 2) + my_byte; 
    	while((8 * characters.length() / 2) % n < (n - 8))
    	{
    		characters = characters + '00'; 
    	}
    	characters = characters + '80'
    }
    return characters; 
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
	S = unsigned long long[5][5]; 

	//Padding of messages
	string P = pad10star1(characters, length, r); 

	//Absorbing phase
	for(int i = 0; i < P.length * 8 / 2 / r; ++i)
	{
		Table table = convertStrToTable(P.substr(i * (2 * r / 8), (2 * r / 8)) + string((c/8), '00')); 
		Pi = table.cell; 

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
			S = KeccakF(S).cell; 
	}

	return Z.substr(0, 2 * n / 8); 
}
























