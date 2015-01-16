//Francisco Romero
//Cs 220
//Professor: Akira Kawaguchi

#pragma once 
#include<string>
#include<map>
#include<queue>


namespace HuffmanCoding {

	class Node{
	public:
		char		letter;
		int		frequency;
		bool		leaf;
		Node		*left;
		Node		*right;
		std::string 	code;

		Node();
		Node(Node* left, Node* right);


		void	setWeight(int a);
		void	Copy(Node * Left, Node * Right);
		Node	operator=(Node A);
	};

	struct Comparitor{	
		bool operator()(Node &a, Node &b);
	};

	class Huffman {
	private:
		std::priority_queue<Node, std::vector<Node>, Comparitor> pq1;
		std::map<char, std::string> Codes;
		std::string FileName;
		std::string OutputFile;
		std::string CompressedFile;
		std::string EncodedString;
		unsigned long ArrayOfBits;
		unsigned long frequencies[256];
		Node *Root;
		Node *NodeArray;
		bool openedsuccessfully;
	public:
		Huffman();
		~Huffman();
		void init();
		void encode();
		void decode();
	};


	void genCode(Node* root, std::map<char, std::string> &Codes, std::string Code = "");

	void decode(unsigned long *source, Node root, int sourceSize, unsigned long maxBits, std::string Output);

	void decodeFile(Node *Root, int index, unsigned long Bits, std::string CompressedFile, std::string Output);

	void encodeFile(std::string decodingString, unsigned long ArrayOfBits, std::string CompressedFile);

	int FileSize(std::string Filename);

	unsigned long freqCount(unsigned long *frequencies, std::string File);

	std::string encode(std::map<char, std::string> &Codes, char* source, int bufferSize);
}
