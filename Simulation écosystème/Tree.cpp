#include "Tree.h"

#include "Entity.h"

#include <qnamespace.h>

Tree::Tree(QPointF initialPos, QBrush const& brush)
	: Entity(brush), mSize{ 100 }, mCountBeforeGrowth{ 5000 / 30 }, mCurrentCount{ 0 },
	mSizePx{ static_cast<double>(mSize) / 25 },mMaxSize{200}
{
	setPos(initialPos);
	mShape << QPointF(-mSizePx / 2, -mSizePx / 2)
		<< QPointF(mSizePx / 2, -mSizePx / 2)
		<< QPointF(mSizePx / 2, mSizePx / 2)
		<< QPointF(-mSizePx / 2, mSizePx / 2);
}

void Tree::disperseSeeds()
{

}

void Tree::calculateSizePx() 
{
	mSizePx = static_cast<double>(mSize) / 25;
}

void Tree::updateShape()
{
	mShape.clear();
	mShape << QPointF(-mSizePx / 2, -mSizePx / 2)
		<< QPointF(mSizePx / 2, -mSizePx / 2)
		<< QPointF(mSizePx / 2, mSizePx / 2)
		<< QPointF(-mSizePx / 2, mSizePx / 2);
}

void Tree::grow()
{
	if (mSize < mMaxSize) {
		mCurrentCount++;
		if (mCurrentCount >= mCountBeforeGrowth) {
			mSize += 20;
			calculateSizePx();
			updateShape();
			mCurrentCount = 0;
		}
	}
	else if (mSize <= 0) {
		mSize = 0;
	}
}

void Tree::advance(int phase)
{
	grow();
	//disperseSeeds();
}

void Tree::eaten()
{
	mSize -= 25;
	if (mSize <= 0) {
		mSize = 0;
	}
	else {
		calculateSizePx();
		updateShape();
	}
}

int Tree::getSize()
{
	return mSize;
}
