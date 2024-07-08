//
// Created by osama on 07/07/24.
//
#include <cassert>

static unsigned long randseed = 1;

void srandom(const unsigned long seed)
{
	randseed = seed;
}

/*
 * Pseudo-random number generator for randomizing the profiling clock,
 * and whatever else we might use it for.  The result is uniform on
 * [0, 2^31 - 1].
 */
unsigned long nrandom()
{
	/*
	* Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
	* From "Random number generators: good ones are hard to find",
	* Park and Miller, Communications of the ACM, vol. 31, no. 10,
	* October 1988, p. 1195.
	*/
	const long x = static_cast<long>(randseed);
	const long hi = x / 127773;
	const long lo = x % 127773;
	long t = 16807 * lo - 2836 * hi;
	if (t <= 0)
		t += 0x7fffffff;
	randseed = t;
	return (t);
}


float frandom(const float flLow, const float flHigh)
{
	const float fl = static_cast<float>(nrandom()) / 2147483648.0f; // float in [0,1)
	return (fl * (flHigh - flLow)) + flLow; // float in [low,high)
}

long random_range(const long from, const long to)
{
	assert(from <= to);
	return (static_cast<long>((nrandom() % (to - from + 1)) + from));
}

float DegreeToRadian(const float degrees)
{
	constexpr float PI = 3.14159265358979323846f;
	return degrees * (PI / 180.0f);
}