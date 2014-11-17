/*
void ComputeSi(int i, unsigned int *indices) : Si ���� �Լ�
i�� �Է¹޾� Si�� ����(indices)����
GF(MOD)�� �̿� : �⺻ ���� MOD = 131
�� ����(index)�� 0 ~ MOD^2-1�� ��
i�� �ִ� MOD^DEGREE - 1 ���� ����(�ִ� ��� ����)
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
	for (count = 0; count <= DEGREE; count++) {  // DEGREE=2�� ���ǵǾ� ����. �� 2�� ���׽�
		coeff[count] = c % MOD;  // ���׽� ��� ���� : ���� �������� (��, �����)
		c /= MOD;  // ����� ���� : ����� MOD���� ���� ���̹Ƿ�. �ֳ��ϸ� GF(p) ���� ���׽��̶�
	}
	for(a = 0;a<NUM_Si;a++)
	{
		res = coeff[0];  // �����
		x = a;
		for(count=1;count<=DEGREE;count++)
		{
			res = (res + x *coeff[count]) % MOD;
			x = (x * a) % MOD;
		}
		indices[a] = res + (a*MOD);
	}
}