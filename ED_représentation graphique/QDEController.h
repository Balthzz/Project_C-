#pragma once

#include <QGroupBox>
#include <QTimer>
#include "QDEAdapter.h"

class QPushButton;
class QSlider;
class QLabel;


class QDEController : public QGroupBox
{
	Q_OBJECT

public:
	QDEController(QDEAdapter const & dEAdapter, QWidget * parent = nullptr);
	QDEController(QDEController const &) = default;
	QDEController(QDEController &&) = default;
	QDEController& operator=(QDEController const &) = default;
	QDEController& operator=(QDEController &&) = default;
	~QDEController() override = default;

signals:
	void evolutionRequired();
	void resetRequired();
	void evolutionStarted();
	void evolutionStopped();

public slots:
	void endSimulation();
	void resetSimulation();

private slots:
	void startSimulation();
	void updateGui();
	void updateSpeed();
	void updateCurrentGeneration();

private:
	QDEAdapter const & mDEAdapter;

	QPushButton * mStepButton{};
	QPushButton * mResetButton{};
	QPushButton * mStartButton{};
	QSlider * mSpeedSlider{};
	QLabel * mSpeedLabel{};
	QLabel * mCurrentGenerationLabel{};
	
	QTimer mSimulationTimer;
	qreal getTimerValue();

	static const qreal mSlowerTimeout;
	static const size_t mMinimumSpeedRange;
	static const size_t mMaximumSpeedRange;
	static const size_t mDefaultSpeedRange;
};
