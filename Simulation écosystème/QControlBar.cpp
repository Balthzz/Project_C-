#include "QControlBar.h"


#include <QLayout>
#include <QPushButton>
#include "QColorBox.h"


std::array<QColor, static_cast<int>(QControlBar::State::_count_)> const QControlBar::mDefaultStateColor({
	QColor(220, 220, 220),		// to do : must be set from QPalette
	QColor(196, 0, 0),
	QColor(0, 196, 0),
	QColor(232, 222, 0) });

std::array<QString, static_cast<int>(QControlBar::Action::_count_)> const QControlBar::mDefaultActionText({
	"Start" , 
	"Stop" , 
	"Pause" , 
	"Resume" , 
	"Step" });

size_t const QControlBar::sDefaultStateMarkerDepth{ 5 };


QControlBar::QControlBar(Qt::Orientation orientation, Alignment alignment, QWidget* parent)
	: QWidget(parent),
	mStateColor(mDefaultStateColor),
	mActionText(mDefaultActionText),
	mState{ State::NotReady },
	mStartStopButton{ new QPushButton(actionText(Action::Start)) },
	mPauseButton{ new QPushButton(actionText(Action::Pause)) },
	mStepButton{ new QPushButton(actionText(Action::Step)) },
	mStateMarkerColorBox{ new QColorBox }
{
	mStateMarkerColorBox->setInteractiveColor(false);

	QBoxLayout * layout{};
	if (orientation == Qt::Horizontal) {
		layout = new QHBoxLayout;
		mStateMarkerColorBox->setFixedWidth(sDefaultStateMarkerDepth);
	} else {
		mStateMarkerColorBox->setFixedHeight(sDefaultStateMarkerDepth);
		layout = new QVBoxLayout;
	};
	
	if (alignment != Alignment::TowardFirst) {
		layout->addStretch();
	}
	layout->addWidget(mStartStopButton);
	layout->addWidget(mPauseButton);
	layout->addWidget(mStepButton);
	layout->addWidget(mStateMarkerColorBox);
	if (alignment != Alignment::TowardLast) {
		layout->addStretch();
	}

	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	setLayout(layout);

	connect(mStartStopButton, &QPushButton::clicked, this, &QControlBar::toggleStartStop);
	connect(mPauseButton, &QPushButton::clicked, this, &QControlBar::togglePauseResume);
	connect(mStepButton, &QPushButton::clicked, this, &QControlBar::step);

	updateGui();
}
void QControlBar::setActionText(Action action, QString const & text)
{
	mActionText[static_cast<int>(action)] = text;
	updateGui();
}
QString QControlBar::actionText(Action action) const
{
	return mActionText[static_cast<int>(action)];
}
void QControlBar::setStateColor(State state, QColor const& color)
{
	mStateColor[static_cast<int>(state)] = color;
	update();
}

QColor QControlBar::stateColor(State state) const
{
	return mStateColor[static_cast<int>(state)];
}

QControlBar::State QControlBar::state() const
{
	return mState;
}

void QControlBar::setReady(bool isReady)
{
	if (isReady && mState == State::NotReady) {
		mState = State::Stopped;
	}
	if (!isReady) {
		if (mState == State::Running) {
			stop();
		}
		mState = State::NotReady;
	}
	updateGui();
}

void QControlBar::toggleStartStop()
{
	switch (mState) {
		case QControlBar::State::Stopped:
			start();
			break;
		case QControlBar::State::Running:
			stop();
			break;
	}
}

void QControlBar::togglePauseResume()
{
	switch (mState) {
		case QControlBar::State::Running:
			pause();
			break;
		case QControlBar::State::Paused:
			resume();
			break;
	}
}

void QControlBar::start()
{
	if (mState == State::Stopped) {
		mState = State::Running;
		emit started();
		updateGui();
	}
}

void QControlBar::stop()
{
	if (mState == State::Running) {
		mState = State::Stopped;
		emit stopped();
		updateGui();
	}
}

void QControlBar::pause()
{
	if (mState == State::Running) {
		mState = State::Paused;
		emit paused();
		updateGui();
	}
}

void QControlBar::resume()
{
	if (mState == State::Paused) {
		mState = State::Running;
		emit resumed();
		updateGui();
	}
}

void QControlBar::step()
{
	if (mState == State::Stopped || mState == State::Paused) {
		emit stepped();
	}
}

void QControlBar::updateGui()
{
	setEnabled(mState != State::NotReady);

	mStartStopButton->setText(actionText(mState == State::Running ? Action::Stop : Action::Start));
	mStartStopButton->setEnabled(mState != State::Paused);

	mPauseButton->setText(actionText(mState == State::Paused ? Action::Resume : Action::Pause));
	mPauseButton->setEnabled(mState == State::Running || mState == State::Paused);

	mStepButton->setText(actionText(Action::Step));
	mStepButton->setEnabled(mState == State::Paused);

	mStateMarkerColorBox->setColor(stateColor(mState));
}

