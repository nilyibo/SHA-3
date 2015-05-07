#include "includes.h"
// Util functions.

string upper(string s);
string hex(unsigned long long n);
string strToHexStr(string input);

struct Table {
	unsigned long long cell[5][5];
	Table(){
		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++)
				cell[i][j] = 0;
	};
	Table(unsigned long long arr[][5]) {
		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++)
				cell[i][j] = arr[i][j];
	}
};

#include "utils.cpp"

