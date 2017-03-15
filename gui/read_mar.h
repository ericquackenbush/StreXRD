/* Copyright 2017 Eric Quackenbush */
#pragma once
#include <vector>
#include <iostream>

typedef unsigned char BYTE;
typedef short int WORD;

class ReadMar {
	unsigned char *cimage;
	std::streampos fileSize;
	std::vector<std::string> list;
	int imageSize;
	std::string filename;
	void byte_swap(unsigned char*, int);
	void get_pck(int, BYTE*, WORD*);
	static void unpack_word(BYTE*, int, int, WORD*);
public:
	ReadMar(std::string);
	~ReadMar();
	int get_image_size();
	int get_file_size();
	void get_header();
	void print_header(std::string);
	std::vector<int> read_file();
};