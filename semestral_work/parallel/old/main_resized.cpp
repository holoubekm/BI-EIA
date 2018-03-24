#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace std;
#define sym char
#define REP 1


/*

	Slightly more intelligent approarch to naive solution

*/

int naive(const sym* string, size_t strLen, const sym* pattern, size_t pattLen, int start = 0) {
	if(pattLen > strLen)
		return -1;

	int scale = sizeof(uint64_t) / sizeof(char);
	uint64_t* patternA = (uint64_t*)pattern;
	uint64_t pattALen = pattLen / scale;
	size_t index = start;
	size_t end = strLen - pattLen + 1;
	while(index < end) {
		bool found = true;
		for(size_t x = 0; x < pattALen; x++) {
			if(((uint64_t*)(string + index))[x] != patternA[x]){
				found = false;
				break;
			}
		}

		if(found) {
			// cout << "Naive: pattern found at index: " << index << endl;
			return index;
		}
		index++;
	}
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
