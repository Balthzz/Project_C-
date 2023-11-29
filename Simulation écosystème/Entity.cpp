#include "Entity.h"

#include <QPainter>

Entity::Entity(QBrush const& brush)
{
	mBrush = brush;
}

void Entity::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(mBrush);
	painter->drawPolygon(mShape);
}

QRectF Entity::boundingRect() const
{
	return mShape.boundingRect();
}
