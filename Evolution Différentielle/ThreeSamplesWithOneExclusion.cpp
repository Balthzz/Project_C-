#include "ThreeSamplesWithOneExclusion.h"


#include <chrono>
#include <algorithm>
#include <numeric>


ThreeSamplesWithOneExclusion::ThreeSamplesWithOneExclusion(size_t sourceSize)
{
	if (!prepare(sourceSize)) {
		throw std::invalid_argument("ThreeSamplesWithOneExclusion::ThreeSamplesWithOneExclusion source size must be greater or equal to 4.");
	}
}

bool ThreeSamplesWithOneExclusion::prepare(size_t sourceSize)
{
	// input validation
	if (sourceSize < 4) {
		return false;
	}

	// initialization
	mEngine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	mSource.resize(sourceSize);
	size_t value{};
	for (auto& i : mSource) {
		i = value++;
	}

	return true;
}

bool ThreeSamplesWithOneExclusion::select(size_t excludedIndex, size_t& sample0, size_t& sample1, size_t& sample2)
{
	// input validation
	if (mSource.size() < 4 || excludedIndex >= mSource.size()) {
		return false;
	}

	// swap the excluded index
	std::swap(mSource[0], mSource[excludedIndex]);

	// generate all samples
	size_t randIndex1{ std::uniform_int_distribution<size_t>(1, mSource.size() - 1)(mEngine) };
	std::swap(mSource[1], mSource[randIndex1]);

	size_t randIndex2{ std::uniform_int_distribution<size_t>(2, mSource.size() - 1)(mEngine) };
	std::swap(mSource[2], mSource[randIndex2]);

	sample0 = mSource[1];
	sample1 = mSource[2];
	sample2 = mSource[std::uniform_int_distribution<size_t>(3, mSource.size() - 1)(mEngine)];

	// 'unswap' for next selection : reverse order is required!
	std::swap(mSource[2], mSource[randIndex2]);
	std::swap(mSource[1], mSource[randIndex1]);
	std::swap(mSource[0], mSource[excludedIndex]);

	return true;
}

