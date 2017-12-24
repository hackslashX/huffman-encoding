/*
Huffman Coding Interface
--
--
--
*/

// Class Includes
#include<map>
#include<vector>
#include<fstream>
#include<bitset>
#include<string>
#include<sstream>
#include<iostream>
#include"boost_serialization.h"

// A common key node that contains the frequency the character and forms a part of our tree
struct keyNode {
	char data = 0;       // The character itself
	int frequency = 0;       // Occurances of the character in the data stream.
	keyNode *leftNode = nullptr;
	keyNode *rightNode = nullptr; // Left and right children of the root node (the keyNode itself)

private:
	friend class boost::serialization::access;
	template <class Archive> void serialize(Archive &ar, unsigned) {
		ar &data;
		ar &frequency;
		ar &leftNode;
		ar &rightNode;
	}
};

struct minHeap {
	std::vector<keyNode> heapStructure; // A vector of many keyNodes
	size_t size() const { return heapStructure.size(); }

private:
	friend class boost::serialization::access;
	template <class Archive> void serialize(Archive &ar, unsigned) {
		ar &heapStructure;
	}
};

class HuffmanTree {
private:
	// Input Data Stream
	std::string dataStream;

	// Filepath
	std::string inputPath;
	std::string outputPath;

	// Tables
	std::map<char, int> frequencyTable;
	std::map<char, std::string> codeTable;

	// Huffman Tree
	minHeap hTree;

	// Output Data Stream
	std::string outputStream;

	// Private Implementations
	void buildFrequencyTable();
	void minHeapify(int);
	void buildMinHeap();
	void copyTo_minHeap();
	void writeCodesToMap(std::string, keyNode*);
	void writeEncodedStream();
	void decodeStream(keyNode*);
	void constructTreeFromVector(std::vector<p>);

	// Destructor Implementation
	void deleteTree(keyNode*);

public:
	HuffmanTree(std::string, std::string);
	~HuffmanTree();
	void encode();
	void decode();
	// Stream to Output File on Storage
	void writeToFile(bool); // True = Write Encoded Stream, False = Write Decoded Stream

};
