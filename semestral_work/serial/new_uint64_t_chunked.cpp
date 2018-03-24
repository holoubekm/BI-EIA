#include "header.cpp"

void naive(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {
	const uint8_t* textChunked = nullptr;
	for(size_t chunk = 0; chunk < strLen; chunk += CACHE_LINE_SIZE) {
		for(size_t patt = 0; patt < pattCnt; patt++) {
			uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
			uint64_t* newPattern = (uint64_t*)pattern;
			size_t pattLen = patterns[patt].length();
			size_t newPattLen = pattLen / 8;

			size_t chunkEnd = min((size_t)CACHE_LINE_SIZE, strLen - chunk);
			chunkEnd = min(chunkEnd, strLen - newPattLen);
			textChunked = text + chunk;

			for(size_t strIndex = 0; strIndex < chunkEnd; strIndex++) {
				bool found = true;
				for(size_t x_8 = 0, x_64 = 0; x_8 < newPattLen; x_8++, x_64 += 8) {
					if(*((uint64_t*)(textChunked + strIndex + x_64)) != newPattern[x_8]){
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
	const uint8_t* textChunked = nullptr;
	for(size_t chunk = 0; chunk < newStrLen; chunk += CACHE_LINE_SIZE) {
		for(size_t patt = 0; patt < pattCnt; patt++) {
			uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
			size_t pattLen = patterns[patt].length();

			uint64_t* newPattern = (uint64_t*)pattern;
			size_t newPattLen = pattLen / 8;
			size_t chunkEnd = min(CACHE_LINE_SIZE + newPattLen, newStrLen - chunk);
			textChunked = text + chunk;
			computeShifts(newPattern, newPattLen, shifts);

			int shift = 0;
			for(size_t offset = 0; offset < 8; offset++) {
			const uint64_t* newText = (const uint64_t*)(textChunked + offset);
				for(size_t index = 0; index < chunkEnd; index++) {
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
}

#include "footer.cpp"
