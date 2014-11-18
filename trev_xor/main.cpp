#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS  // to avoid fopen error in VC++
#endif
#include <stdio.h>
#include <time.h>
/*
#include <iostream>
#include <fstream>
#include <string>
*/


//#pragma warning (disable : 4996)

using namespace std;

#include "extractor_XOR.cpp"

#define TIMER_SAMPLE_CNT 1
#define SAMPLE_CNT 2
//3GHz CPU기준. 다른 CPU인 경우 변경
#define CPUCYCLE 4000000000.

void doTiming();
unsigned int measure_extractor_per_cycle(unsigned int dtMin);
unsigned int measure_extractor_per_clock(unsigned int dtMin);
unsigned int calibrate();
unsigned int calibrate_per_clock();
unsigned int HiResTime(void);

void main()
{
	//사용예
	
	FILE *fp, *fp2, *fp3;
//	string line;
//	unsigned long int c;
	

	fp = fopen("LPN.txt","r");		//Random source 입력
	//ifstream fpaa("LPN.txt");
	fp2 = fopen("T.txt","w");		//U_t 입력
	fp3 = fopen("RND.txt","w+");	//출력 저장
	
/*	while (getline(fpaa, line))
	{
		cout << line << endl;
		c += line.length();
	}
	*/
	fseek(fp, 0L, SEEK_END);
	unsigned long int num_of_input_bits = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	printf("number of bits = %d", num_of_input_bits);
	int iterations = num_of_input_bits / X_LEN;
	//변수 선언 및 초기화 
	//Random source, U_t는 ArrayClass의 Data, Len, Block을 채우는 다른 방식(Init 함수 등)으로 입력받아도 됨
	//ArrayClass X(fp,X_LEN, 'b');			//Random source 입력 받기
	//ArrayClass T(fp2,T_LEN);		//U_t 입력 받기
	//ArrayClass T(fp2, T_LEN, 'b', 'r');
	ArrayClass RND;					//출력을 저장할 변수 선언

	int i = 0;
	for (i = 0; i < iterations; ++i){
		ArrayClass X(fp, X_LEN, 'b');			//Random source 입력 받기
		ArrayClass T(fp2, T_LEN, 'b', 'r');
		extractor(&X, &T, &RND);			//extractor수행
		X.fprint(fp3, "Random SRC ", 'b');
		T.fprint(fp3, "U_t        ", 'b');
		RND.fprint(fp3, "Random val ", 'b');				//출력
	}
	
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	//속도 측정
	doTiming();
	getchar();
	
}

void doTiming()
{
    unsigned int calibration;
    unsigned int measure_extractor_val;
	unsigned int calibration2;
	unsigned int measure_extractor_val2;
    double measur_sec;
    calibration = calibrate();

    measure_extractor_val = measure_extractor_per_cycle(calibration);
    printf("Cycles for gen %5d bits    : %d\n",OUT_LEN, measure_extractor_val);
	printf("bits/Cycles                  : %lf\n",(double)OUT_LEN/(double)measure_extractor_val);
	printf("Throuthput(bits/sec)by cycle : %lf\n\n",(double)OUT_LEN/(double)measure_extractor_val*CPUCYCLE);
	
	
	calibration2 = calibrate_per_clock();
	measure_extractor_val2 = measure_extractor_per_clock(calibration2);
    printf("Second for gen %5d bits    : %lf\n",OUT_LEN, (double)measure_extractor_val2/1000./SAMPLE_CNT);
	printf("Throuthput(bits/sec)by clock : %lf\n",OUT_LEN/((double)measure_extractor_val2/1000./SAMPLE_CNT));
    printf("\n");
}

unsigned int measure_extractor_per_cycle(unsigned int dtMin)
{
    unsigned int tMin = 0xFFFFFFFF;         /* big number to start */
    unsigned int t0,t1,i;
	unsigned int count;
	FILE *fp, *fp2;
	fp = fopen("X.txt","r");
	fp2 = fopen("T.txt","r");

	ArrayClass X(fp,X_LEN);
	ArrayClass T(fp2,T_LEN);
	ArrayClass RND;

    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = HiResTime();
		extractor(&X,&T,&RND);
        t1 = HiResTime();
		
        if (tMin > t1-t0 - dtMin)       /* keep only the minimum time */
            tMin = t1-t0 - dtMin;
        }
    /* now tMin = # clocks required for running RoutineToBeTimed() */
    
    return tMin;
}

unsigned int calibrate()
{
    unsigned int dtMin = 0xFFFFFFFF;        /* big number to start */
    unsigned int t0,t1,i;

    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = HiResTime();
        t1 = HiResTime();
        if (dtMin > t1-t0)              /* keep only the minimum time */
            dtMin = t1-t0;
        }
	
    return dtMin;
}


/************** Timing routine (for performance measurements) ***********/
/* By Doug Whiting */
/* unfortunately, this is generally assembly code and not very portable */
#if defined(_M_IX86) || defined(__i386) || defined(_i386) || defined(__i386__) || defined(i386) || \
    defined(_X86_)   || defined(__x86_64__) || defined(_M_X64) || defined(__x86_64)
#define _Is_X86_    1
#endif

#if  defined(_Is_X86_) && (!defined(__STRICT_ANSI__)) && (defined(__GNUC__) || !defined(__STDC__)) && \
    (defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW_H) || defined(__GNUC__))
#define HI_RES_CLK_OK         1          /* it's ok to use RDTSC opcode */

#if defined(_MSC_VER) // && defined(_M_X64)
#include <intrin.h>
#pragma intrinsic(__rdtsc)         /* use MSVC rdtsc call where defined */
#endif

#endif

unsigned int HiResTime(void)           /* return the current value of time stamp counter */
    {
#if defined(HI_RES_CLK_OK)
    unsigned int x[2];
#if   defined(__BORLANDC__)
#define COMPILER_ID "BCC"
    __emit__(0x0F,0x31);           /* RDTSC instruction */
    _asm { mov x[0],eax };
#elif defined(_MSC_VER)
#define COMPILER_ID "MSC"
#if defined(_MSC_VER) // && defined(_M_X64)
    x[0] = (unsigned int) __rdtsc();
#else
    _asm { _emit 0fh }; _asm { _emit 031h };
    _asm { mov x[0],eax };
#endif
#elif defined(__MINGW_H) || defined(__GNUC__)
#define COMPILER_ID "GCC"
    asm volatile("rdtsc" : "=a"(x[0]), "=d"(x[1]));
#else
#error  "HI_RES_CLK_OK -- but no assembler code for this platform (?)"
#endif
    return x[0];
#else
    /* avoid annoying MSVC 9.0 compiler warning #4720 in ANSI mode! */
#if (!defined(_MSC_VER)) || (!defined(__STDC__)) || (_MSC_VER < 1300)
    FatalError("No support for RDTSC on this CPU platform\n");
#endif
    return 0;
#endif /* defined(HI_RES_CLK_OK) */
    }



unsigned int measure_extractor_per_clock(unsigned int dtMin)
{
    unsigned int tMin = 0xFFFFFFFF;         /* big number to start */
    unsigned int t0,t1,i,j;
	unsigned int count;
	FILE *fp, *fp2;
	fp = fopen("X.txt","r");
	fp2 = fopen("T.txt","r");

	ArrayClass X(fp,X_LEN);
	ArrayClass T(fp2,T_LEN);
	ArrayClass RND;

    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = clock();
		for(j=0;j<SAMPLE_CNT;j++)
		{
			extractor(&X,&T,&RND);
		}
        t1 = clock();
        if (tMin > t1-t0 - dtMin)       /* keep only the minimum time */
            tMin = t1-t0 - dtMin;
        }
    /* now tMin = # clocks required for running RoutineToBeTimed() */
    
    return tMin;
}

unsigned int calibrate_per_clock()
{
    unsigned int dtMin = 0xFFFFFFFF;        /* big number to start */
    unsigned int t0,t1,i;
	int j;

    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = clock();
		for(j=0;j<SAMPLE_CNT;)
		{
			j++;
		}
        t1 = clock();
        if (dtMin > t1-t0)              /* keep only the minimum time */
            dtMin = t1-t0;
        }
    return dtMin;
}
