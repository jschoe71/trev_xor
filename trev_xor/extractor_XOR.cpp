#include "extractor_XOR.h"
#include "MakeSi.h"

inline bool get_bit(ArrayClass *Src, unsigned int i) {
// 0 <= i <= Src.Len-1 
#ifdef EXPENSIVE_SANITY_CHECKS
	if (i > Src->Len-1) {
		printf("Internal error: Bit index out of range");
		printf("(total length: %d, requested: %d)",Src->Len,i);
		exit(-1);
	}
#endif
	unsigned int idx = i/32;
	unsigned int chunk = Src->Data[idx];

	bool bit = (bool)(chunk & (1<<(31-(i%32))));
	return bit;
}

inline bool ond_bit_extract(ArrayClass *X, unsigned int *Sidx)
{
	bool res = 0;
	int i;
	for(i=0;i<T2_LEN;i++)
	{
		res ^= get_bit(X,Sidx[i]);
	}
	return res;
}


inline void extractor(ArrayClass *X, ArrayClass *T, ArrayClass *RND)
{
	int i,j,k;
	unsigned int Si[L_LEN] = {0};
	unsigned int Sidx[T2_LEN] = {0};
	RND->InitZero(OUT_LEN);

	for(i=0;i<OUT_LEN;i++)
	{
		//S 계산(index)
		ComputeSi(i,Si);
		//T에서 Si위치 값을 뽑아서 T2_LEN개의 indices 계산
		for(j=0;j<T2_LEN;j++)
		{
			Sidx[j] = 0;
			for(k=0;k<LOG_X_LEN;k++)
			{
				if(get_bit(T,Si[LOG_X_LEN*j+k]))
				{
					Sidx[j] ^= 1<<(LOG_X_LEN-k-1);
				}
			}
		}
		
		if(ond_bit_extract(X,Sidx))
		{
			RND->Data[i/32] |= 1<<(31-(i%32));
		}
	}
}