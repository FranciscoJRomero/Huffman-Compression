//Francisco Romero
//Cs 220
//Professor: Akira Kawaguchi

#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<bitset>
#include"Huffman.h"
namespace HuffmanCoding{
	
	Node::Node(){
		this->right = NULL;
		this->left = NULL;
		this->leaf = true;
		this->frequency = 0;
	}
	
	Node::Node(Node* left, Node* right){
		this->right = right;
		this->left = left;
		this->leaf = false;
		this->frequency = left->frequency + right->frequency;
	}
	
	
	void Node::setWeight(int a){
		this->frequency = a;
	}

	Node Node::operator=(Node A){
		this->letter	= A.letter;
		this->frequency = A.frequency;
		this->right		= A.right;
		this->left		= A.left;
		this->leaf		= A.leaf;
		return *this;
	}
	
	bool Comparitor::operator()(Node &a, Node &b){
		return a.frequency > b.frequency;
	}

	void Node::Copy(Node * Left, Node * Right)
	{
		left = Left;
		right = Right;
		frequency = Left->frequency + Right->frequency;
		leaf = false;
	}
	
	int FileSize(std::string Filename){
		std::ifstream File(Filename.c_str());
		File.seekg(0, File.end);
		return	File.tellg();
	}

	void genCode(Node* root, std::map<char, std::string> &Codes, std::string Code){
		if (root->leaf){
			root->code = Code;
			Codes.insert(std::pair<char, std::string>(root->letter, root->code));
			std::cout << "Char: " << root->letter << "\tCode: " << root->code << std::endl;
			return;
		}
		genCode(root->right, Codes, Code + "1");
		genCode(root->left, Codes, Code + "0");
	}

	unsigned long freqCount(unsigned long* frequencies, std::string File){
		std::ifstream input(File.c_str());
		//char buffer = input.get();
		char buffer;
		unsigned long characters = 0;
		while (input.read(&buffer, 1)) {
			++characters;
			++frequencies[int(buffer)];
		}
		/*while (buffer != EOF){
			std::cout << buffer;
			++characters;
			++frequencies[int(buffer)];
			buffer = input.get();
		}*/
		input.close();
		return characters;
	}


	std::string encode_(std::map<char, std::string> &Codes, char * source, int bufferSize){
		std::string finalCode;
		for (int i = 0; i < bufferSize; ++i){
			finalCode += Codes.find(source[i])->second;
		}
		return finalCode;
	}

	void decode(unsigned long * source, Node root, int sourceSize, unsigned long maxBits, std::string Output){

		std::bitset<32>	decodingP;
		Node	*iterator = &root;
		unsigned long BitCount(0);
		
		std::ofstream output(Output.c_str());
		for (int i = 0; i < sourceSize; ++i){
			decodingP = source[i];
			for (int j = 31; j >= 0; --j) {
				if (decodingP.test(j) == true){
					iterator = iterator->right;
				}
				else{
					iterator = iterator->left;
				}
				if (iterator->leaf){
					output << iterator->letter;
					iterator = &root;
				}
				++BitCount;
				if (BitCount == maxBits) break;
			}
			if (BitCount == maxBits) break;
		}
		output.close();
	}

	void decodeFile(Node* Root, int index, unsigned long Bits, std::string CompressedFile, std::string output){

		unsigned long bufferSize = FileSize(CompressedFile.c_str());
		std::ifstream inputS(CompressedFile.c_str(), std::ios::binary);
		unsigned long* data = new unsigned long[bufferSize];
		inputS.read((char*) data, sizeof(unsigned long) *index);
		inputS.close();
		decode(data, *Root, index, Bits, output);
		delete [] data;
	}

	void encodeFile(std::string decodingString, unsigned long ArrayOfBits, std::string CompressedFile){
		std::ofstream outputC(CompressedFile.c_str(), std::ios::binary);
		unsigned long *charCodes = new unsigned long[ArrayOfBits];
		unsigned long buffer = 0;
		unsigned long index = 0;
		unsigned long counter = 0;
		unsigned long stringLen = decodingString.length();
		for (unsigned int i = 0; i < stringLen; ++i){
			buffer <<= 1;
			buffer |= decodingString[i] == '1' ? 1 : 0;
			++counter;
			if (counter % 32 == 0){
				charCodes[index++] = buffer;
			}
			if (((i + 1) == stringLen) && ((counter %32) != 0)){
				buffer <<= (32 - counter % 32);
				charCodes[index++] = buffer;
			}
		}
		outputC.write((char*) charCodes, sizeof(unsigned long) *index);
		delete [] charCodes;
		outputC.close();
	}

	Huffman::Huffman() {
		unsigned long* begin = frequencies;
		unsigned long* end = begin + 256;
		while (begin < end) {
			*(begin++) = 0;
		}
		init();
	}

	Huffman::~Huffman() {
		delete [] NodeArray;
	}
	void Huffman::init() {
		std::cout << "Enter your input File name to Compress: ";
		std::cin >> FileName;
		std::cout << "Enter the name of your Compressed File: ";
		std::cin >> CompressedFile;
		std::cout << "Enter the name of your output File: ";
		std::cin >> OutputFile;
		std::cout << std::endl;
		register unsigned long bufferSizeC = freqCount(frequencies, FileName);

		std::ifstream inputFile(FileName.c_str());
		if (inputFile.is_open() == false) {
			openedsuccessfully = false;
			return;
		} else {
			std::cout << "File Opened SuccessFully!\n";
			openedsuccessfully = true;
		}

		char* bufferC = new char[bufferSizeC + 1];
		inputFile.read(bufferC, bufferSizeC);
		inputFile.close();
		bufferC[bufferSizeC] = '\0';

		unsigned long counter = 0;
		unsigned long totalChar = 0;

		for (int i = 0; i < 256; ++i){
			if (frequencies[i]>0) {
				++totalChar;
				counter += frequencies[i];
			}
		}

		std::cout << "Total characters:  " << counter << " \nUnique characters: " << totalChar << std::endl;

		int totalNodes = (totalChar - 1) * 3;
		NodeArray = new Node[totalNodes];

		int j = 0;
		for (unsigned int i = 0; i < totalChar; ++i){
			while (frequencies[j] == 0) ++j;
			NodeArray[i].setWeight(frequencies[j]);
			NodeArray[i].letter = char(j);
			++j;
		}

		for (unsigned int i = 0; i < totalChar; ++i){
			pq1.push(NodeArray[i]);
		}


		for (int i = 0, parent = 1; i < totalNodes; ++i, ++parent){
			if (parent == 3){
				NodeArray[i].Copy(&NodeArray[i - 2], &NodeArray[i - 1]);
				parent = 0;
				pq1.push(NodeArray[i]);
			}
			else{
				NodeArray[i] = pq1.top();
				pq1.pop();
			}
		}
		this->Root = &NodeArray[totalNodes - 1];

		genCode(this->Root, Codes);

		EncodedString = encode_(Codes, bufferC, bufferSizeC);
		unsigned long BitLength = 32;
		ArrayOfBits = (EncodedString.length() / BitLength) +
			(EncodedString.length() % BitLength ? 1 : 0);
		delete [] bufferC;
		
	}

	void Huffman::encode() {
		if (!openedsuccessfully) return;
		encodeFile(EncodedString, ArrayOfBits, CompressedFile);
	}

	void Huffman::decode() {
		if (!openedsuccessfully) return;
		decodeFile(Root, ArrayOfBits, EncodedString.length(), CompressedFile, OutputFile);
	}
}