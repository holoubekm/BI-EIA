
int main(int argc, char* argv[]) {
	if(argc != 6) {
		cout << "Usage: ./semestralka <kmp|naive> <#threads> <cache chunk size> <exponent factor> <input file>" << endl;
		return 1;
	}

	int chunk_size = -1;
	try{
		chunk_size = stoi(string(argv[3]));
	} catch (exception ex) {
		chunk_size = -1;
	}

	if(chunk_size <= 8192) {
		cout << "Size of cache chunk is too small" << endl;
		cout << argv[2] << endl;
		return 3;
	}

	int input_exp = -1;
	try{
		input_exp = stoi(string(argv[4]));
	} catch (exception ex) {
		input_exp = -1;
	}

	if(input_exp < 1) {
		cout << "Wrong multiply count." << endl;
		return 3;
	}

	ifstream input;
	input.open(argv[5]);

	string text;
	string pattern;
	string* patterns = nullptr;

	cout << "Executable: " << argv[0] << endl;
	int pattCnt = -1;
	size_t longestPattern = 0;
	if(input.is_open()) {
		getline(input, text);
		for(int x = 0; x < input_exp; x++) 
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

	if(chunk_size < longestPattern) {
		cerr << "The length of the longest pattern is greater than CACHE_LINE_SIZE." << endl << "Exiting..." << endl;
		return 2;
	}

	int threads = -1;
	try{
		threads = stoi(string(argv[2]));
	} catch (exception ex) {
		threads = -1;
	}

	if(threads <= 0) {
		cout << "Wrong number of threads.";
		return 3;
	}

	cout << "Method running: " << argv[1] << endl;
	string method = argv[1];
	double start = 0;
	double end = 0;
	if(method == "naive") {
		start = omp_get_wtime();
		naive((uint8_t*)text.c_str(), text.length(), patterns, pattCnt, threads, chunk_size);
		end = omp_get_wtime();
	} else if(method == "kmp") {
		int* shifts = new int[longestPattern + 1];
		start = omp_get_wtime();
		kmp((uint8_t*)text.c_str(), text.length(), patterns, pattCnt, shifts, threads, chunk_size);
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
	cout << ">>" << "\t" << argv[0] << "\t" << text.length() << "\t" << argv[1] << "\t" << argv[2] << "\t" << argv[3] << "\t" << argv[4] << "\t" << argv[5] << "\t" << ((end - start) * 1000) << "ms";
	#ifdef INSTR
		cout << "\t" << ins;
	#endif
	cout << endl << endl;
	return 0;
}
