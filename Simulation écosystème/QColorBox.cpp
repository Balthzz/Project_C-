#include "QColorBox.h"


#include <QPainter>
#include <QColorDialog>


QColorBox::QColorBox(QWidget * parent)
	: QColorBox(Qt::black, parent)
{
}

QColorBox::QColorBox(QColor const & color, QWidget * parent)
	: QFrame(parent)
	, mInteractiveColor{ true }
	, mColor(color)
{
}

bool QColorBox::isInteractiveColor() const
{
	return mInteractiveColor;
}

void QColorBox::setInteractiveColor(bool interactive)
{
	mInteractiveColor = interactive;
}

QColor QColorBox::color() const
{
	return mColor;
}

void QColorBox::setColor(QColor const & color)
{
	mColor = color;
	update();
	emit colorChanged(mColor);
}

void QColorBox::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (mInteractiveColor) {
		setColor(QColorDialog::getColor(mColor, this, "Select color"));
	}
}

void QColorBox::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(mColor);
	painter.drawRect(rect());
}

