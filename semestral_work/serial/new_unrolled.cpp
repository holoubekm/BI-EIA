#include "header.cpp"

void naive(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {

	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		size_t pattLen = patterns[patt].length();
		size_t unrollCnt = 4;
		size_t pattLenUnroll = pattLen / unrollCnt;
		size_t pattLenRest = pattLen % unrollCnt;

		size_t end = strLen - pattLen;
		for(size_t strIndex = 0; strIndex <= end; strIndex++) {
			bool found = true;
			for(size_t x = 0; x < pattLenUnroll; x+=4) {
				if(
					text[strIndex + x + 0] != pattern[x + 0] ||
					text[strIndex + x + 1] != pattern[x + 1] ||
					text[strIndex + x + 2] != pattern[x + 2] ||
					text[strIndex + x + 3] != pattern[x + 3]
					) {
					found = false;
					break;
				}
			}

			for(size_t x = pattLen - pattLenRest; found && x < pattLen; x++) {
				if(text[strIndex + x] != pattern[x])
					found = false;
			}

			#ifdef DEBUG
			if(found) {
				cout << "Naive: pattern found at strIndex: " << strIndex << endl;
			}
			#endif
		}
	}
}

void computeShifts(const uint8_t* pattern, size_t pattLen, int* shifts) {
	shifts[0] = -1;
	for (int index = 0; index < pattLen; ++index) {
		shifts[index + 1] = shifts[index];
		while (shifts[index + 1] > -1 && pattern[shifts[index + 1]] != pattern[index]) {
			shifts[index + 1] = shifts[shifts[index + 1]];
		}
		shifts[index + 1]++;
	}
}

void kmp(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {
	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		size_t pattLen = patterns[patt].length();

		int* shifts = new int[pattLen + 1];
		computeShifts(pattern, pattLen, shifts);

		int shift = 0;
		for(size_t index = 0; index < strLen; ++index) {
			while (shift > -1 && pattern[shift] != text[index]) {
				shift = shifts[shift];
			}
			shift++;

			if(shift >= pattLen) {
				cnt+=+;
				shift = shifts[pattLen];
				#ifdef DEBUG
					cout << "KMP: pattern found at index: " << (index - pattLen + 1) << endl;
				#endif
			}
		}
		delete[] shifts;
	}
}

#include "footer.cpp"
