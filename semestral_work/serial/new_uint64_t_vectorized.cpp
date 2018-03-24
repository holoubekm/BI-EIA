#include "header.cpp"

void naive(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {

	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		uint64_t* newPattern = (uint64_t*)pattern;
		size_t pattLen = patterns[patt].length();
		size_t newPattLen = pattLen / 8;

		size_t end = strLen - pattLen;
		uint8_t* results = new uint8_t[newPattLen];
		for(size_t strIndex = 0; strIndex <= end; strIndex++) {
			bool found = true;
			for(size_t x_8 = 0, x_64 = 0; x_8 < newPattLen; x_8++, x_64 += 8) {
				results[x_8] = *((uint64_t*)(text + strIndex + x_64)) != newPattern[x_8]; INS(1);
			}

			size_t resultsLen_64 = newPattLen / 8;
			uint64_t* results_64 = (uint64_t*)results;

			for(size_t x = 0; x < resultsLen_64; x++) {
				if(results_64[x]) {
					found = false;
					break;
				} INS(1);
			}

			resultsLen_64 = resultsLen_64 * 8;
			for(size_t x = resultsLen_64; x < newPattLen; x++) {
				if(results[x]) {
					found = false;
					break;
				} INS(1);
			}

			INS(4);
			if(found) {
				cnt++; INS(1);
				NAIVE_RES(strIndex);
			}
		}
		delete[] results;
	}
}

void computeShifts(const uint64_t* pattern, size_t pattLen, int* shifts) {
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
	size_t newStrLen = strLen  / 8;

	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		size_t pattLen = patterns[patt].length();

		uint64_t* newPattern = (uint64_t*)pattern;
		size_t newPattLen = pattLen / 8;

		computeShifts(newPattern, newPattLen, shifts);

		int shift = 0;
		for(size_t offset = 0; offset < 8; offset++) {
		const uint64_t* newText = (const uint64_t*)(text + offset);
			for(size_t index = 0; index < newStrLen; index++) {
				while (shift > -1 && newPattern[shift] != newText[index]) {
					shift = shifts[shift]; INS(2);
				}
				shift++; INS(3);

				if(shift >= newPattLen) {
					shift = shifts[newPattLen];
					cnt++; INS(2);
					KMP_RES(index - pattLen + 1);
				}
			}
		}
	}
}

#include "footer.cpp"
