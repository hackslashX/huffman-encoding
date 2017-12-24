#include"huffman.h"

using namespace std;

void HuffmanTree::buildFrequencyTable() {
	int streamSize = dataStream.length();
	for (int i = 0; i < streamSize; i++) {
		char character = dataStream[i];
		if (frequencyTable.find(character) == frequencyTable.end())
			frequencyTable.insert(pair<char, int>(character, 1));
		else
			frequencyTable[character]++;
	}
}

void HuffmanTree::copyTo_minHeap() {
	for (auto i = frequencyTable.begin(); i != frequencyTable.end(); i++) {
		keyNode newKeyNode;
		newKeyNode.data = i->first;
		newKeyNode.frequency = i->second;
		newKeyNode.rightNode = NULL;
		newKeyNode.leftNode = NULL;
		hTree.heapStructure.push_back(newKeyNode);
	}
}

void HuffmanTree::minHeapify(int i) {
	int parent = i;
	int left = (i * 2) + 1;
	int right= (i * 2) + 2;
	if (hTree.size() > left && hTree.heapStructure[left].frequency < hTree.heapStructure[parent].frequency)
		parent = left;
	if (hTree.size() > right && hTree.heapStructure[right].frequency < hTree.heapStructure[parent].frequency)
		parent = right;
	if (parent != i) {
		swap(hTree.heapStructure[i], hTree.heapStructure[parent]);
		minHeapify(parent);
	}
}

void HuffmanTree::buildMinHeap() {
	for (int i = 0; i < hTree.size() / 2 + 1; i++) {
		minHeapify(i);
	}
}

void HuffmanTree::writeToFile(bool mode) {
	if (mode) {
		bitsetR bitStorage(outputStream);

		// Convert Map to Vector (Boost Library bug fix)
		vector<p> tempVector;
		for (auto i = codeTable.begin(); i != codeTable.end(); i++) {
			p newPair;
			newPair.a = i->first; newPair.bits = i->second;
			tempVector.push_back(newPair);
		}

		// Serialize the data
		{
			ofstream file(outputPath, ios::out);
			boost::archive::binary_oarchive outFile(file);
			outFile << tempVector;
			outFile << bitStorage;
		}
	}
	else {
		ofstream file(outputPath, ios::out);
		file << outputStream;
		file.close();
	}
}

void HuffmanTree::constructTreeFromVector(vector<p> in) {
	// Insert a new empty node in our tree
	keyNode varKeyNode;
	hTree.heapStructure.push_back(varKeyNode);
	for (auto i = in.begin(); i != in.end(); i++) {
		// Extract character and bitstream
		char varChr = i->a;
		string varString;
		to_string(i->bits.x, varString);
		keyNode* refPointer = &hTree.heapStructure[0];
		// Traverse and insert
		for (size_t x = 0; x != varString.length(); x++) {
			if (varString[x] == '0') { // Go left
				if (refPointer->leftNode == NULL)
					refPointer->leftNode = new keyNode;
				refPointer = refPointer->leftNode;
			}
			else if(varString[x] == '1') { // Go Right
				if (refPointer->rightNode == NULL)
					refPointer->rightNode = new keyNode;
				refPointer = refPointer->rightNode;
			}
		}
		// Insert the value
		refPointer->data = varChr;
		refPointer = NULL;
	}
}

void HuffmanTree::deleteTree(keyNode* node) {
	if (node == NULL) return;
	deleteTree(node->leftNode);
	deleteTree(node->rightNode);
	delete node;
	node = NULL;
}
