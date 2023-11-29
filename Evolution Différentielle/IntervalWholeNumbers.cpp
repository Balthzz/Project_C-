#include <random>

#include "IntervalWholeNumbers.h"

std::mt19937 IntervalWholeNumbers::mEngine;

IntervalWholeNumbers::IntervalWholeNumbers()
	: IntervalWholeNumbers::IntervalWholeNumbers(1, 10)
{
}

IntervalWholeNumbers::IntervalWholeNumbers(int lower, int upper)
	: mLower{ lower }, mUpper{ upper }, distance(mLower, mUpper)
{
}

int IntervalWholeNumbers::randomize()
{
	return distance(IntervalWholeNumbers::mEngine);
}

void IntervalWholeNumbers::setLower(int lower)
{
	mLower = lower;
}

void IntervalWholeNumbers::setUpper(int upper)
{
	set(mLower, upper);
}

double IntervalWholeNumbers::getLower()
{
	return mLower;
}

double IntervalWholeNumbers::getUpper()
{
	return mUpper;
}

void IntervalWholeNumbers::set(int lower, int upper)
{
	if (lower > upper)
		lower = upper;

	mLower = lower;
	mUpper = upper;
}
