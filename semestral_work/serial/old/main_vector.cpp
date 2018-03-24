#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace std;

#define REP 1
#define VEC_SIZE 256
#define sym char

/*
	Succesfully vectorized.
*/

int naive(const sym* string, size_t strLen, const sym* pattern, size_t pattLen, int start = 0) {
	// if(pattLen > strLen)
		// return -1;

	size_t index = start;
	size_t end = strLen - pattLen + 1;

	uint8_t shRes[VEC_SIZE];
	uint64_t* lgRes = (uint64_t*)shRes;
	size_t vectorized_size = pattLen - (pattLen % VEC_SIZE);
	size_t rest_start = vectorized_size;
	size_t chunk_size = (VEC_SIZE * sizeof(uint8_t)) / sizeof(uint64_t);

	while(index < end) {
		bool found = true;

		//vectorize comparison of the VEC_SIZE bytes chunks
		for(size_t x = 0; x < vectorized_size; x += VEC_SIZE) {
			for(size_t i = 0; i < VEC_SIZE; i++) {
				shRes[i] = string[index + x + i] != pattern[x + i];
			}

			//to archieve speed up, loop through the result by bigger steps
			for(size_t i = 0; i < chunk_size; i++) {
				if(lgRes[i] > 0) {
					found = false;
					break;
				}
			}

			if(!found) {
				break;
			}
		}

		//compare the rest of pattern => size may not be dividable by VEC_SIZE
		if(found) {
			for(size_t x = rest_start; x < pattLen; x++) {
				if(string[index + x] != pattern[x]) {
					found = false;
					break;
				}
			}
		}
		
		if(found) {
			// cout << "Naive: pattern found at index: " << index << endl;
			return index;
		}

		index++;
	}
	// cout << "Naive: pattern not found." << endl;
	return -1;
}

void prepareKMP(const sym* pattern, int pattLen, int* shifts) {
  int x = 1;
  int k;
  int shift;
  shifts[0] = -1;
  while(x < pattLen) {
  	k = x - 1;
  	shift = k;
  	do {
  		shift = shifts[shift];
  	} while(shift != -1 && pattern[shift] != pattern[k]);
  	shifts[x] = shift + 1;
  	x++;
  }
}

int kmp(const sym* string, int strLen, const sym* pattern, int pattLen, int* shifts, int start = 0) {
	if(strLen < pattLen)
		return -1;

	size_t index = start;
	int shift = 0;
	size_t end = strLen;
	while (index < end && shift < pattLen) {
		while (shift > -1 && pattern[shift] != string[index]) {
			shift = shifts[shift];
		}
		index++;
		shift++;
	}

	if (shift >= pattLen) {
		// cout << "KMP2: pattern found at index: " << (index - pattLen) << endl;
		return index - pattLen;
	} 

	// cout << "KMP2: pattern not found" << endl;
	return -1;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cout << "Usage: ./semestralka <kmp|naive> <input file>" << endl;
		return 1;
	}

	ifstream input;
	input.open(argv[2]);

	string text;
	string pattern;
	string* patterns = nullptr;

	int pattCnt = -1;
	if(input.is_open()) {
		getline(input, text);
		input >> pattCnt;
		input.ignore(numeric_limits<streamsize>::max(), '\n');
		patterns = new string[pattCnt];
		for(int x = 0; x < pattCnt; x++) {
			getline(input, pattern);
			patterns[x] = pattern;
		}
	} else {
		cout << "File does not exist." << endl;
		return 1;
	}
	input.close();

	cout << "Patterns loaded: " << pattCnt << endl;
	cout << "Input text length: " << text.length() << endl;

	cout << "Method running: " << argv[1] << endl;
	if(strcmp(argv[1], "naive") == 0) {
		for(int rep = 0; rep < REP; rep++)
			for(int x = 0; x < pattCnt; x++) {
				int index = -1;
				do {
					index = naive(text.c_str(), text.length(), patterns[x].c_str(), patterns[x].length(), index + 1);
				} while(index >= 0);
			}
	} else {
		for(int x = 0; x < pattCnt; x++) {
			int* shifts = new int[patterns[x].length()];
			prepareKMP(patterns[x].c_str(), patterns[x].length(), shifts);
			int index = -1;
			do {
				index = kmp(text.c_str(), text.length(), patterns[x].c_str(), patterns[x].length(), shifts, index + 1);
			} while(index >= 0);
			delete[] shifts;
		}
	}

	delete[] patterns;
	return 0;
}
