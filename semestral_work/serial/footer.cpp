
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

	cout << "Executable: " << argv[0] << endl;
	int pattCnt = -1;
	size_t longestPattern = 0;
	if(input.is_open()) {
		getline(input, text);
		for(int x = 0; x < INPUT_EXP; x++) 
			text += text;
		cout << "Input text length: " << text.length() << endl;
		input >> pattCnt;
		input.ignore(numeric_limits<streamsize>::max(), '\n');
		patterns = new string[pattCnt];
		cout << "Patterns loaded: " << pattCnt << endl;
		for(int x = 0; x < pattCnt; x++) {
			getline(input, pattern);
			cout << "Pattern " << x << ", length: " << pattern.length() << endl;
			patterns[x] = pattern;
			if(pattern.length() > longestPattern) 
				longestPattern = pattern.length();
		} cout << endl;
	} else {
		cout << "File does not exist." << endl;
		input.close();
		return 1;
	}
	input.close();

	if(pattCnt <= 0) {
		cout << "No patterns loaded" << endl;
		return 1;
	}

	if(CACHE_LINE_SIZE < longestPattern) {
		cerr << "The length of the longest pattern is greater than CACHE_LINE_SIZE." << endl << "Exiting..." << endl;
		return 2;
	}

	cout << "Method running: " << argv[1] << endl;
	string method = argv[1];
	double start = 0;
	double end = 0;
	if(method == "naive") {
		start = omp_get_wtime();
		naive((uint8_t*)text.c_str(), text.length(), patterns, pattCnt);
		end = omp_get_wtime();
	} else if(method == "kmp") {
		int* shifts = new int[longestPattern + 1];
		start = omp_get_wtime();
		kmp((uint8_t*)text.c_str(), text.length(), patterns, pattCnt, shifts);
		end = omp_get_wtime();
		delete[] shifts;
	} else {
		cout << "Wrong method " << method << endl;
		return 1;
	}
	cout << endl;
	cout << "Found: " << cnt << endl;
	cout << "Total time spent in " << method << " method: " << ((end - start) * 1000) << "ms";
	#ifdef INSTR
		cout << ", int operarations: " << ins;
	#endif
	cout << endl;
	cout << argv[0] << "\t" << argv[1] << "\t" << argv[2] << "\t" << ((end - start) * 1000) << "ms";
	#ifdef INSTR
		cout << "\t" << ins;
	#endif
	cout << endl << endl;
	return 0;
}
