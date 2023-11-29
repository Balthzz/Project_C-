#include "Warper.h"

namespace warper {
	qreal warp(qreal value, qreal begin, qreal end) {
		qreal const width = end - begin;
		return value - qFloor((value - begin) / width) * width;
	}

	void warpP(QPointF& point, QGraphicsScene* scene) {
		point.setX(warp(point.x(), scene->sceneRect().left(), scene->sceneRect().right()));
		point.setY(warp(point.y(), scene->sceneRect().top(), scene->sceneRect().bottom()));
	}
}