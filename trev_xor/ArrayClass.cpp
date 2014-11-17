#include "ArrayClass.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctime>
#include "config.h"



ArrayClass::ArrayClass()
{
	Len = 0;
	Block = 0;
}

ArrayClass::ArrayClass(FILE *fp, int n)
	//파일로 부터 입력 받는 함수
	// n은 비트 길이, 파일의 값은 16진수(Not binary)
	// 한양대 원 코드의 함수
{
	int i;
	uint32_t temp;
	if(!Data)
		free(Data);
	Len = n;
	Block = (Len+31)/32;
	Data = (uint32_t *)malloc(sizeof(uint32_t) * Block);  // 4 bytes = 32 bits, 갯수가 Block인 uint32_t 형의 변수열
	for(i =0 ; i < Block-1 ; i++)  // FIXME: Block --> Block-1로 수정
	{
		fscanf_s(fp, "%08X",&Data[i]);  // file을 8 character 단위로 읽어들임 -> 16진수 표기라면 32비트가 됨
		temp = Data[i];
	}
	//16진수(4바이트)기준으로 값 설정
	Data[Block-1] = Data[Block-1]<<(32-4*(((Len%32)+3)/4));  // last block that may not fill 4 byte storage
}

ArrayClass::ArrayClass(FILE *fp, int n, const char b )  // bit로 저장된 파일인 경우
// 내가 추가함 
{
	int i, j;
	char ch;
	uint32_t temp;
	if(!Data)
		free(Data);
	Len = n;
	Block = Len/32; // X_LEN % 32 == 0 이 만족되어 있음
	Data = (uint32_t*)calloc(Block, sizeof(uint32_t));  // initialize with zero
	for( i=0 ; i<Block ; ++i )
	{
		for( j=0 ; j<32 ; ++j ){
			fscanf_s(fp, "%c", &ch);
			temp = (uint32_t)(ch-'0');
			temp <<= (31-j);
			Data[i]|=temp;
		}
	}
}

ArrayClass::ArrayClass(FILE *fp, int t, const char b, const char r ) 
// 이것도 내가 추가
// uniform random seed 파일 생성
{
	int i, j;
	char ch;
	uint32_t temp;

	srand((unsigned int)time(NULL));

	if(!Data)
		free(Data);
	Len = t;
	Block = Len/32;
	Data = (uint32_t *)calloc(Block, sizeof(uint32_t));
	for( i=0 ; i<Block ; ++i )
	{
		for( j=0 ; j<32 ; ++j )
		{
			temp = (uint32_t)(rand()%2);
			temp <<= (31-j);
			Data[i]|=temp;
		}
	}
}


void ArrayClass::InitZero(int n)												//n비트의 0으로 초기화 하는 함수
{
	int i;
	if(!Data)
		free(Data);
	Len = n;
	Block = (Len+31)/32;
	Data = (uint32_t *)malloc(sizeof(uint32_t) * Block);
	for(i=0;i<Block;i++)
	{
		Data[i] = 0;
	}
}

void ArrayClass::Init(int n, unsigned int *RowData)								//n비트의 배열로 초기화하는 함수
{
	int i;
	if(!Data)
		free(Data);
	Len = n;
	Block = (Len+31)/32;
	Data = (uint32_t *)malloc(sizeof(uint32_t) * Block);
	for(i=0;i<Block;i++)
	{
		Data[i] = RowData[i];
	}
}

void ArrayClass::print(char *Name)
{
	int i;
	printf("%s : ",Name);
	for(i=0;i<Block;i++)
	{
		printf("%08X",Data[i]);
	}
	printf("\n");
}

void ArrayClass::print(char *Name, const char b)
{
	int i, j;
	uint32_t mask=1;
	mask <<= 31;
	printf("%s : ", Name);
	for( i=0 ; i<Block; i++)
	{
		for( j=0 ; j<32 ; ++j )
		{
			printf("%c", Data[i]&mask );
			Data[i] <<= 1;
		}
	}
}

void ArrayClass::fprint(FILE *fp)
{
	int i;
	for(i=0;i<Block;i++)
	{
		fprintf(fp,"%08X",Data[i]);
	}
	fprintf(fp,"\n");
}
void ArrayClass::fprint(FILE *fp, char * Name)
{
	int i;
	fprintf(fp,"%s : ",Name);
	for(i=0;i<Block;i++)
	{
		fprintf(fp,"%08X",Data[i]);
	}
	fprintf(fp,"\n");
}

void ArrayClass::fprint(FILE *fp, char *Name, const char b)
{
	int i, j;
	uint32_t mask=1;
	mask <<= 31;
	fprintf(fp, "%s : ", Name);
	for( i=0 ; i<Block; i++)
	{
		for( j=0 ; j<32 ; ++j )
		{
			fprintf(fp, "%c", Data[i]&mask? '1' : '0' );
			Data[i] <<= 1;
		}
	}
	fprintf(fp,"\n");
}

