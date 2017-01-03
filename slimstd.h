#ifndef SLIMSTD_H
#define SLIMSTD_H

float sqr(float x) {
	return x * x;
}

float qsqrt(float number) // exact quake 3 quick sqrt implementation
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;							// evil floating point bit level hacking
	i = 0x5F3759DF - (i >> 1);					// what the fuck? 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));		// 1st iteration
	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed

	return y;
}

#endif