#ifndef Q_COLOR_BOX_H
#define Q_COLOR_BOX_H


#include <QFrame>


class QColorBox : public QFrame
{
	Q_OBJECT

public:
	QColorBox(QWidget * parent = nullptr);
	QColorBox(QColor const & color, QWidget * parent = nullptr);

	bool isInteractiveColor() const;
	void setInteractiveColor(bool interactive = true);

	QColor color() const;
	void setColor(QColor const & color);

signals:
	void colorChanged(QColor color);

protected:
	bool mInteractiveColor;
	QColor mColor;

	void mouseDoubleClickEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event) override;
};

#endif // Q_COLOR_BOX_H