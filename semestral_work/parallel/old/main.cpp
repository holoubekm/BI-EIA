#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace std;

#define sym char
#define CNT 1

int naive(const sym* string, size_t strLen, const sym* pattern, size_t pattLen, int start = 0) {
	if(pattLen > strLen)
		return -1;

	size_t index = start;
	size_t end = strLen - pattLen + 1;
	while(index < end) {
		bool found = true;
		for(size_t x = 0; x < pattLen; x++) {
			if(index + x < strLen)
			{
				if(string[index + x] != pattern[x]) {
					found = false;
				}
			}
		} 

		if(found) {
			cout << "Naive: pattern found at index: " << index << endl;
			return index;
		}
		index++;
	}
	return -1;
}

void prepareKMP(const sym* pattern, int pattLen, int* shifts) {
	shifts[0] = -1;
	int shift;
	for(int x = 1; x < pattLen; x++) {
		shift = shifts[x - 1];
		while(shift >= 0) {
			if(pattern[shift] == pattern[x - 1])
				break;
			else
				shift = shifts[shift];
		}
		shifts[x] = shift + 1;
	}
}

int kmp(const sym* string, int strLen, const sym* pattern, int pattLen, int* shifts, int start = 0) {
	int index = start;
	int shift = 0;
	while(index < strLen) {
		if(shift == -1) {
			index++;
			shift = 0;
		}
		else if(string[index] == pattern[shift]) {
			index++;
			shift++;
			if(shift == pattLen) {
				// cout << "KMP: pattern found at index: " << (index - pattLen) << endl;
				return index - pattLen;
			}
		}
		else {
			shift = shifts[shift];
		}
	}
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
	string* patterns = NULL;

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

	// for(int x= 0; x < 11; x++) {
	// 	text += text;
	// }

	cout << "Patterns loaded: " << pattCnt << endl;
	cout << "Input text length: " << text.length() << endl;

	cout << "Method running: " << argv[1] << endl;
	if(strcmp(argv[1], "naive") == 0) {
		for(int i = 0; i < CNT; i++)
			for(int x = 0; x < pattCnt; x++) {
				int index = -1;
				do {
					index = naive(text.c_str(), text.length(), patterns[x].c_str(), patterns[x].length(), index + 1);
				} while(index >= 0);
			}
	} else {
		for(int i = 0; i < CNT; i++)
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