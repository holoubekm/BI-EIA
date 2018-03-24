#include "header.cpp"

void naive(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {
	const uint8_t* textChunked = nullptr;
	for(size_t chunk = 0; chunk < strLen; chunk += CACHE_LINE_SIZE) {
		for(size_t patt = 0; patt < pattCnt; patt++) {
			uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
			size_t pattLen = patterns[patt].length();

			size_t chunkEnd = min((size_t)CACHE_LINE_SIZE, strLen - chunk);
			chunkEnd = min(chunkEnd, strLen - pattLen);
			textChunked = text + chunk;

			for(size_t strIndex = 0; strIndex < chunkEnd; strIndex++) {
				bool found = true;
				for(size_t x = 0; x < pattLen; x++) {
					if(textChunked[strIndex + x] != pattern[x]){
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
	const uint8_t* textChunked = nullptr;
	for(size_t chunk = 0; chunk < strLen; chunk += CACHE_LINE_SIZE) {
		for(size_t patt = 0; patt < pattCnt; patt++) {
			uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
			size_t pattLen = patterns[patt].length();
			size_t chunkEnd = min(CACHE_LINE_SIZE + pattLen, strLen - chunk);
			textChunked = text + chunk;

			computeShifts(pattern, pattLen, shifts);

			int shift = 0;
			for(size_t index = 0; index <= chunkEnd; index++) {
				while (shift > -1 && pattern[shift] != textChunked[index]) {
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
}

#include "footer.cpp"
