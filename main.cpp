
#include"huffman.h"
#include<chrono>
#include<boost/filesystem.hpp>

using namespace std;


int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "USAGE: huffman [OPTION] InputFile OutputFile\n;";
		cout << "Options:\n-c\tCompress the given InputFile to OutputFile\n-d\tDecompress the given InputFile to OutputFile\n";
		return 1;
	}

	cout << "------ Firing up Huffman Routine ------\n";

	if (strcmp(argv[1],"-c") == 0) { // Start Compressing
		cout << "~ Checking if file exists... ";
		// Check if the input file exists
		ifstream file(argv[2]);
		if (!file.good()) {
			cout << "\n!! Error: Can't Open Input File\nProgram Aborted\n";
			return 1;
		}
		cout << "[OK]\n~ Creating Object and Firing Compression... ";
		HuffmanTree *hObject = new HuffmanTree(argv[2], argv[3]);
		// Start Timer //
		auto start = chrono::high_resolution_clock::now();
		hObject->encode();
		cout << "[OK]\n~ Writing everything to file and cleaning up... ";
		hObject->writeToFile(true);
		delete hObject;
		cout << "[OK]\n------            Done           ------\n";
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<double, milli> duration = end - start;
		auto originalSize = boost::filesystem::file_size(argv[2]);
		int newSize = boost::filesystem::file_size(argv[3]);
		cout << "Compression: " << float(newSize) / float(originalSize)*100.0 << "% (" << originalSize << " -> " << newSize << ")\n";
		cout << "Duration: " << duration.count() << " millisconds" << endl;
		return 1;
	}

	if (strcmp(argv[1],"-d") == 0) {
		cout << "~ Checking if file exists... ";
		// Check if the input file exists
		ifstream file(argv[2]);
		if (!file.good()) {
			cout << "\n!! Error: Can't Open Input File\nProgram Aborted\n";
			return 1;
		}
		cout << "[OK]\n~ Creating Object and Firing Decompression... ";
		HuffmanTree *hObject = new HuffmanTree(argv[2], argv[3]);
		// Start Timer //
		auto start = chrono::high_resolution_clock::now();
		hObject->decode();
		cout << "[OK]\n~ Writing everything to file and cleaning up... ";
		hObject->writeToFile(false);
		delete hObject;
		cout << "[OK]\n------            Done           ------\n";
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<double, milli> duration = end - start;
		auto originalSize = boost::filesystem::file_size(argv[2]);
		int newSize = boost::filesystem::file_size(argv[3]);
		cout << "Decompression: " << originalSize << " -> " << newSize << "\n";
		cout << "Duration: " << duration.count() << " milliseconds" << endl;
		return 1;
	}
}
