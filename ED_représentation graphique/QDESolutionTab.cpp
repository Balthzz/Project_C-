#include "QDESolutionTab.h"


#include "QDESolutionPanel.h"


QDESolutionTab::QDESolutionTab(QWidget *parent)
	: QTabWidget(parent)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	connect(this, &QTabWidget::currentChanged, this, &QDESolutionTab::parameterChanged);
}

de::SolutionStrategy* QDESolutionTab::newSolution() const
{
	QWidget* widget{ currentWidget() };
	
	return widget ? static_cast<QDESolutionPanel*>(widget)->buildSolution() : nullptr;
}

void QDESolutionTab::addSolutionPanel(QDESolutionPanel * dESolutionPanel)
{
	int index{ addTab(dESolutionPanel, dESolutionPanel->tabTitle()) };

	connect(dESolutionPanel, &QDESolutionPanel::parameterChanged, this, &QDESolutionTab::parameterChanged);

	setCurrentIndex(index);
}

void QDESolutionTab::enable()
{
	setEnabled(true);
}

void QDESolutionTab::disable()
{
	setEnabled(false);
}

void QDESolutionTab::updateVisualization(QDEAdapter const & de)
{
	static_cast<QDESolutionPanel*>(currentWidget())->updateVisualization(de);
}

