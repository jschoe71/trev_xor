/*
void ComputeSi(int i, unsigned int *indices) : Si 생성 함수
i를 입력받아 Si의 원소(indices)생성
GF(MOD)을 이용 : 기본 설정 MOD = 131
각 원소(index)는 0 ~ MOD^2-1의 값
i는 최대 MOD^DEGREE - 1 까지 가능(최대 출력 길이)
*/

#include "config.h"
#include "MakeSi.h"

void ComputeSi(int i, unsigned int *indices)
{
	unsigned int count;
	unsigned int a;
	unsigned int x;
	unsigned int c;
	unsigned int res;
	unsigned int coeff[DEGREE+1] = {0};
#ifdef EXPENSIVE_SANITY_CHECKS
	if (i >= (MOD<<(DEGREE-1)) {
		printf("Internal error: Bit index out of range");
		printf("(total length: %d, requested: %d)",Src->Len,i);
		exit(-1);
	}
#endif
	c = i;
	for (count = 0; count <= DEGREE; count++) {  // DEGREE=2로 정의되어 있음. 즉 2차 다항식
		coeff[count] = c % MOD;  // 다항식 계수 결정 : 낮은 차수부터 (즉, 상수항)
		c /= MOD;  // 상수항 제거 : 계수가 MOD보다 작은 수이므로. 왜냐하면 GF(p) 상의 다항식이라서
	}
	for(a = 0;a<NUM_Si;a++)
	{
		res = coeff[0];  // 상수항
		x = a;
		for(count=1;count<=DEGREE;count++)
		{
			res = (res + x *coeff[count]) % MOD;
			x = (x * a) % MOD;
		}
		indices[a] = res + (a*MOD);
	}
}