//Francisco Romero
//Cs 220
//Professor: Akira Kawaguchi

#include<iostream>
#include<ctime>
#include"Huffman.h"

int main(){

	clock_t Begin_Time;											
	HuffmanCoding::Huffman compressor;

	Begin_Time = clock();														
	compressor.encode();
	std::cout << "It took\t" << float(clock() - Begin_Time) / CLOCKS_PER_SEC * 1000 << " msec to Compress the File" << std::endl;

	Begin_Time = clock();														
	compressor.decode();
	std::cout << "It took\t" << float(clock() - Begin_Time) / CLOCKS_PER_SEC * 1000 << " msec to unCompress the File" << std::endl << std::endl;

	system("pause");
	return 0;
}