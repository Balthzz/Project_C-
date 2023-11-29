#include <random>

#include "Interval.h"

std::mt19937 Interval::mEngine;

Interval::Interval()
	: Interval::Interval(0.0, 1.0)
{
}

Interval::Interval(double lower, double upper)
	: mLower{ lower }, mUpper{ upper }, distance(mLower, mUpper)
{
}

double Interval::randomize()
{
	return distance(Interval::mEngine);
}

void Interval::setLower(double lower)
{
	mLower = lower;
}

void Interval::setUpper(double upper)
{
	set(mLower, upper);
}

double Interval::getLower()
{
	return mLower;
}

double Interval::getUpper()
{
	return mUpper;
}

void Interval::set(double lower, double upper)
{
	if (lower > upper)
		lower = upper;

	mLower = lower;
	mUpper = upper;
}
