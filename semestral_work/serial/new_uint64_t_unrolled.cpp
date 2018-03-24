#include "header.cpp"

void naive(const uint8_t* text, size_t strLen, const string* patterns, size_t pattCnt) {

	for(size_t patt = 0; patt < pattCnt; patt++) {
		uint8_t* pattern = (uint8_t*)patterns[patt].c_str();
		uint64_t* newPattern = (uint64_t*)pattern;
		size_t pattLen = patterns[patt].length();
		size_t newPattLen = pattLen / 8;

		size_t end = strLen - pattLen;
		for(size_t strIndex = 0; strIndex <= end; strIndex++) {
			bool found = true;
			size_t x_8 = 0, x_64 = 0;
			size_t newEnd = newPattLen - (newPattLen % 4);
			for(; x_8 < newEnd; x_8 += 4, x_64 += 32) {
				if(
					(*((uint64_t*)(text + strIndex + x_64 + 0)) != newPattern[x_8 + 0]) ||
					(*((uint64_t*)(text + strIndex + x_64 + 8)) != newPattern[x_8 + 1]) ||
					(*((uint64_t*)(text + strIndex + x_64 + 16)) != newPattern[x_8 + 2]) ||
					(*((uint64_t*)(text + strIndex + x_64 + 24)) != newPattern[x_8 + 3])) {
						found = false;
						break;
					} INS(4);
			}

			for(x_8 = newEnd, x_64 = newEnd * 8; found && x_8 < newPattLen; x_8++, x_64 += 8) {
				if(*((uint64_t*)(text + strIndex + x_64)) != newPattern[x_8]){
					found = false;
					break;
				} INS(1);
			}

			INS(3);
			if(found) {
				cnt++; INS(1);
				NAIVE_RES(strIndex);
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
