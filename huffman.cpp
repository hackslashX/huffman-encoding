#include"huffman.h"

using namespace std;

HuffmanTree::HuffmanTree(string i, string o) {
	// Set the filePath
	this->inputPath = i;
	this->outputPath = o;
}

HuffmanTree::~HuffmanTree() {
	deleteTree(hTree.heapStructure[0].leftNode);
	deleteTree(hTree.heapStructure[0].rightNode);	
}

void HuffmanTree::encode() {
	cout << "[OK]\n~ Extracting input stream... ";
	// Grab the data to encode
	ifstream inputFile;
	inputFile.open(inputPath); // Open the input file
	stringstream temporaryStream;
	temporaryStream << inputFile.rdbuf(); // Feed the file data to a temporary stream
	dataStream = temporaryStream.str(); // Copy it over to our actual string object
	inputFile.close(); // Close the file
	
	cout << "[OK]\n~ Building Frequency Table... ";
	// We'll start by building a frequency table
	buildFrequencyTable();

	cout << "[OK]\n~ Building Tree... ";
	// Let's build a minHeap structure
	copyTo_minHeap();
	buildMinHeap();

	// Start the Magic
	while (hTree.size() != 1) {
		// Extract the first minimum element
		keyNode extract1 = hTree.heapStructure[0];
		hTree.heapStructure.erase(hTree.heapStructure.begin());
		buildMinHeap(); // Again build minHeap

		// Extract the second element
		keyNode extract2 = hTree.heapStructure[0];
		hTree.heapStructure.erase(hTree.heapStructure.begin());

		// Create a new keyNode with left and right nodes as the extracted nodes
		keyNode toInsert; 
		toInsert.frequency = extract1.frequency + extract2.frequency;
		toInsert.leftNode = new keyNode;
		toInsert.leftNode->data = extract1.data;
		toInsert.leftNode->frequency = extract1.frequency;
		toInsert.leftNode->leftNode = extract1.leftNode;
		toInsert.leftNode->rightNode = extract1.rightNode;
		toInsert.rightNode = new keyNode;
		toInsert.rightNode->data = extract2.data;
		toInsert.rightNode->frequency = extract2.frequency;
		toInsert.rightNode->leftNode = extract2.leftNode;
		toInsert.rightNode->rightNode = extract2.rightNode;
		hTree.heapStructure.push_back(toInsert);

		// Again minHeapify the new structure
		buildMinHeap();
	}

	cout << "[OK]\n~ Writing Codes to Map... ";
	// Let's build the codes
	writeCodesToMap("", &hTree.heapStructure[0]);

	cout << "[OK]\n~ Constructing Output Data... ";
	// Constrcut the encoded stream (containing 0s and 1s)
	writeEncodedStream();
}

void HuffmanTree::decode() {
	// Retrieve the encoded stream
	cout << "[OK]\n~ Extracting encoded stream... ";
	bitsetR bitStr("");
	vector<p> tempVector;
	{
		ifstream file(inputPath);
		boost::archive::binary_iarchive inFile(file);
		inFile >> tempVector;
		inFile >> bitStr;
	}

	cout << "[OK]\n~ Constructing needed data... ";
	// Construct a barebone tree from the input vector
	constructTreeFromVector(tempVector);

	// Convert the binary stream to string
	to_string(bitStr.x, dataStream);

	cout << "[OK]\n~ Decoding input stream... ";
	// Construct the final output string
	decodeStream(&hTree.heapStructure[0]);
}

void HuffmanTree::decodeStream(keyNode* node) {
	int pos = 0; int streamSize = dataStream.length();
	while (pos != streamSize) {
		while (true) {
			if (dataStream[pos] == '0') 
				node = node->leftNode;
			else if (dataStream[pos] == '1') 
				node = node->rightNode;
			pos++;
			if ((dataStream[pos] == '0' && node->leftNode == NULL) || (dataStream[pos] == '1' && node->rightNode == NULL) || pos == streamSize)
				break;
		}
		outputStream += node->data;
		node = &hTree.heapStructure[0];
	}
}

void HuffmanTree::writeCodesToMap(string code, keyNode* root) {
	if (root->leftNode != NULL) {
		writeCodesToMap(code + '0', root->leftNode);
	}
	if (root->rightNode != NULL) {
		writeCodesToMap(code + '1', root->rightNode);
	}
	if (root->leftNode == NULL && root->rightNode == NULL) {
		codeTable.insert(pair<char, string>(root->data, code));
	}
}

void HuffmanTree::writeEncodedStream() {
	int inputStreamSize = dataStream.length();
	outputStream = "";
	for (int i = 0; i < inputStreamSize; i++) {
		char tempChr = dataStream[i];
		outputStream += codeTable.find(tempChr)->second;
	}
}
