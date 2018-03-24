#include "header.cpp"

void naive(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {
	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		size_t pattLen = patterns[patt].length();

		size_t end = strLen - pattLen;
		for(size_t strIndex = 0; strIndex <= end; strIndex++) {
			bool found = true;
			for(size_t x = 0; x < pattLen; x++) {
				if(text[strIndex + x] != pattern[x]){
					found = false;
					break;
				} INS(1);
			}
			INS(2);
			if(found) {
				cnt++; INS(1);
				NAIVE_RES(strIndex);
			}
		}
	}
}

void computeShifts(const uint8_t* pattern, size_t pattLen, int* shifts) {
	shifts[0] = -1;
	for (int index = 0; index < pattLen; ++index) {
		shifts[index + 1] = shifts[index];
		while (shifts[index + 1] > -1 && pattern[shifts[index + 1]] != pattern[index]) {
			shifts[index + 1] = shifts[shifts[index + 1]]; INS(1);
		}
		shifts[index + 1]++; INS(3);
	}
}

void kmp(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt, int* shifts) {
	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		size_t pattLen = patterns[patt].length();

		computeShifts(pattern, pattLen, shifts);

		int shift = 0;
		for(size_t index = 0; index < strLen; index++) {
			while (shift > -1 && pattern[shift] != text[index]) {
				shift = shifts[shift]; INS(2);
			}
			shift++; INS(3);

			if(shift >= pattLen) {
				shift = shifts[pattLen];
				cnt++; INS(2);
				KMP_RES(index - pattLen + 1);
			}
		}
	}
}

#include "footer.cpp"
