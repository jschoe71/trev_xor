﻿#ifndef DefArrayClass 
#define DefArrayClass

#include <stdint.h>
#include <stdio.h>
class ArrayClass {
	public:
	ArrayClass();
	ArrayClass(FILE *, int);
	ArrayClass(FILE *, int, const char);
	ArrayClass(FILE *, int, const char, const char);

	void InitZero(int);												//n비트의 0으로 초기화 하는 함수
	void Init(int,  unsigned int *);								//n비트의 배열으로 초기화하는 함수
	void print(char *);
	void print(char *, const char);
	void fprint(FILE *);
	void fprint(FILE *, char *);
	void fprint(FILE *, char *, const char);
	//void GenerateU01(FILE *, int );

	unsigned int *Data;
	int Len;
	int Block;
};


#endif