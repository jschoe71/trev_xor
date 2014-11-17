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
	//���Ϸ� ���� �Է� �޴� �Լ�
	// n�� ��Ʈ ����, ������ ���� 16����(Not binary)
	// �Ѿ�� �� �ڵ��� �Լ�
{
	int i;
	uint32_t temp;
	if(!Data)
		free(Data);
	Len = n;
	Block = (Len+31)/32;
	Data = (uint32_t *)malloc(sizeof(uint32_t) * Block);  // 4 bytes = 32 bits, ������ Block�� uint32_t ���� ������
	for(i =0 ; i < Block-1 ; i++)  // FIXME: Block --> Block-1�� ����
	{
		fscanf_s(fp, "%08X",&Data[i]);  // file�� 8 character ������ �о���� -> 16���� ǥ���� 32��Ʈ�� ��
		temp = Data[i];
	}
	//16����(4����Ʈ)�������� �� ����
	Data[Block-1] = Data[Block-1]<<(32-4*(((Len%32)+3)/4));  // last block that may not fill 4 byte storage
}

ArrayClass::ArrayClass(FILE *fp, int n, const char b )  // bit�� ����� ������ ���
// ���� �߰��� 
{
	int i, j;
	char ch;
	uint32_t temp;
	if(!Data)
		free(Data);
	Len = n;
	Block = Len/32; // X_LEN % 32 == 0 �� �����Ǿ� ����
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
// �̰͵� ���� �߰�
// uniform random seed ���� ����
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


void ArrayClass::InitZero(int n)												//n��Ʈ�� 0���� �ʱ�ȭ �ϴ� �Լ�
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

void ArrayClass::Init(int n, unsigned int *RowData)								//n��Ʈ�� �迭�� �ʱ�ȭ�ϴ� �Լ�
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

