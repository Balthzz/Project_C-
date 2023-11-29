#pragma once

#include <qtypes.h>
#include <QPoint.h>
#include <QGraphicsScene.h>

namespace warper {
	qreal warp(qreal value, qreal begin, qreal end);
	void warpP(QPointF& point, QGraphicsScene* scene);
}