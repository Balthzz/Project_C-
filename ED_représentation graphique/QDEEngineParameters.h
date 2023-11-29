#pragma once


#include <QGroupBox>
#include <EngineParameters.h>

class QSpinBox;
class QComboBox;
class QScrollBar;
class QLabel;



class QDEEngineParameters : public QGroupBox
{
	Q_OBJECT

public:
	QDEEngineParameters(QWidget * parent = nullptr);
	QDEEngineParameters(QDEEngineParameters const &) = default;
	QDEEngineParameters(QDEEngineParameters &&) = default;
	QDEEngineParameters& operator=(QDEEngineParameters const &) = default;
	QDEEngineParameters& operator=(QDEEngineParameters &&) = default;
	~QDEEngineParameters() override = default;

	de::EngineParameters parameters() const;

public slots:
	void enable();
	void disable();

signals:
	void parameterChanged();

private slots:
	void changeParameters();

private:
	QSpinBox * mPopulationSize{};
	QSpinBox * mGenerationCount{};
	QComboBox * mMutationStrategy{};
	QScrollBar* mMutationFactorValue{};
	QLabel* mMutationFactorLabel{};
	QComboBox * mCrossoverStrategy{};
	QScrollBar* mCrossoverRateValue{};
	QLabel* mCrossoverRateLabel{};
	QComboBox * mSelectionStrategy{};
	QComboBox * mStoppingCriteria{};

};

