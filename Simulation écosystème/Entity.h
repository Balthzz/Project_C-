#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPolygonF>

class Entity : public QGraphicsItem
{
public:
	QBrush mBrush;
	QPolygonF mShape;

	Entity(QBrush const& brush);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	QRectF boundingRect() const override;
};

#endif