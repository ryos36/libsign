#include "temp.h"
#include <stdio.h>

enum {
	JG = 0xFFFFFFFFFFUL,
	JS = ( JG << 63 ),

	TAG_MASK          = 0x7UL,
	TAG_SHIFT         = 0x3,

	LONG_INT_BIT_SIZE = ((sizeof(long int) == 8)?64:32),
	MINUS_BIT  = (0x1UL << (LONG_INT_BIT_SIZE - 1)),

	SHORT_NUM_MASK = ~TAG_MASK,
	SHORT_ABS_NUM_MASK = (SHORT_NUM_MASK & ~MINUS_BIT),
	//MINUS_MASK = SHORT_NUM_MASK & ~( SHORT_ABS_NUM_MASK >> (TAG_SHIFT - 1UL))
	MINUS_MASK = SHORT_NUM_MASK & ~( SHORT_NUM_MASK >> TAG_SHIFT ),
	ABS_NUM_MASK = (SHORT_NUM_MASK & ~MINUS_BIT) >> TAG_SHIFT,
};

class X {
public:
};

union u {
	int x;
	int y;
	void *xx;

	u &operator= (X *x0) {
		xx = x0;
		return *this;
	}
};


int
main()
{
	u u;
	X x;

	u = &x;
	printf("%d\n", _bit_size<>::bit_size);
	printf("%lx\n", JG);
	printf("%lx\n", JS);
	printf("MINUS_BIT %lx\n", MINUS_BIT);
	printf("SHORT_NUM_MASK %lx\n", SHORT_NUM_MASK);
	printf("SHORT_NUM_MASK %lx\n", SHORT_ABS_NUM_MASK >> (TAG_SHIFT - 1));
	printf("SHORT_ABS_NUM_MASK %lx\n", SHORT_ABS_NUM_MASK);
	printf("MINUS_MASK %lx\n", MINUS_MASK);
	printf("ABS_NUM_MASK %lx\n", ABS_NUM_MASK);

}
