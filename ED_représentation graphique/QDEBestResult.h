#pragma once



#include <QPlainTextEdit>
#include "QDEAdapter.h"


class QDEBestResult : public QPlainTextEdit
{
	Q_OBJECT

public:
	QDEBestResult(QWidget * parent = nullptr);
	QDEBestResult(QDEBestResult const &) = default;
	QDEBestResult(QDEBestResult &&) = default;
	QDEBestResult& operator=(QDEBestResult const &) = default;
	QDEBestResult& operator=(QDEBestResult &&) = default;
	~QDEBestResult() override = default;

public slots:
	void updateBestResult(QDEAdapter const & de);
	void reset();
};
