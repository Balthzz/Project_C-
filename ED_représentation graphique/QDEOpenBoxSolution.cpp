#include "QDEOpenBoxSolution.h"


#include <QFrame>
#include <QGroupBox>
#include <QScrollBar>
#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPainter>


const QString QDEOpenBoxSolution::sDescription("Le problème de la boîte ouverte consiste à déterminer la taille de la coupe au quatre coins d'une surface rectangulaire permettant d'optimizer le volume du cube issu du pliage de cette feuille.");


QDEOpenBoxSolution::QDEOpenBoxSolution(QWidget * parent)
	:	QDESolutionPanel(parent),
		mVisualizationLabel{ new QLabel },
		mWidthScrollBar{ nullptr },
		mHeightScrollBar{ nullptr }
{
	QLabel * title{ new QLabel(sDescription) };

	QFrame * sepFrame{ new QFrame };
	sepFrame->setFrameStyle(static_cast<int>(QFrame::HLine) | static_cast<int>(QFrame::Raised));

	mVisualizationLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	mVisualizationLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QGroupBox * parameterGroupBox{ new QGroupBox("Paramètres") };
	QFormLayout * parameterLayout{ new QFormLayout };
	parameterLayout->addRow("Largeur", buildScrollBarLayout(mWidthScrollBar));
	parameterLayout->addRow("Hauteur", buildScrollBarLayout(mHeightScrollBar));
	parameterGroupBox->setLayout(parameterLayout);

	QWidget * inputWidget{ new QWidget };
	QVBoxLayout * inputLayout{ new QVBoxLayout };
	inputLayout->addWidget(parameterGroupBox);
	inputLayout->addStretch();
	inputWidget->setLayout(inputLayout);

	QGroupBox * visualizationGroupBox{ new QGroupBox("Visualisation") };
	QVBoxLayout * visualizationLayout{ new QVBoxLayout };
	visualizationLayout->addWidget(mVisualizationLabel);
	visualizationGroupBox->setLayout(visualizationLayout);

	QWidget * ioWidget{ new QWidget };
	QHBoxLayout * ioLayout{ new QHBoxLayout };
	ioLayout->addWidget(parameterGroupBox);
	ioLayout->addWidget(visualizationGroupBox);
	ioWidget->setLayout(ioLayout);

	QVBoxLayout * mainLayout{ new QVBoxLayout };
	mainLayout->addWidget(title);
	mainLayout->addWidget(sepFrame);
	mainLayout->addWidget(ioWidget);

	setLayout(mainLayout);
}

QString QDEOpenBoxSolution::tabTitle() const
{
	return QString("Open box problem");
}

de::SolutionStrategy* QDEOpenBoxSolution::buildSolution() const
{
	return new OpenBoxStrategy(mWidthScrollBar->value(), mHeightScrollBar->value());
}

QHBoxLayout * QDEOpenBoxSolution::buildScrollBarLayout(QScrollBar * & sb)
{
	const int minSize{ 10 };
	const int maxSize{ 1000 };
	const int defSize{ 100 };
	const int minWidth{ 150 };

	sb = new QScrollBar;
	sb->setOrientation(Qt::Horizontal);
	sb->setRange(minSize, maxSize);
	sb->setValue(defSize);
	sb->setMinimumWidth(minWidth);

	QLabel * label{ new QLabel(QString::number(defSize)) };

	connect(sb, &QScrollBar::valueChanged, label, static_cast<void(QLabel::*)(int)>(&QLabel::setNum));
	connect(sb, &QScrollBar::valueChanged, this, &QDESolutionPanel::parameterChanged);

	QHBoxLayout * layout{ new QHBoxLayout };
	layout->addWidget(sb);
	layout->addWidget(label);

	return layout;
}

void QDEOpenBoxSolution::updateVisualization(QDEAdapter const & de)
{
	static const QColor boxColor(211, 203, 156);
	static const QColor cornerColor(boxColor.lighter(125));
	static const QColor titleColor(QColor::fromHsv(cornerColor.hue(), 32, cornerColor.value()));
	static const QColor subTitleColor(QColor::fromHsv(cornerColor.hue(), 32, cornerColor.value() / 4 * 3));
	static const QColor backgroundColor(96, 96, 96);
	static const QString title("Représentation de chaque solution triée");
	static const QString subTitle("du haut vers le bas et de gauche à droite");

	QSize labelImageSize(mVisualizationLabel->size().width() - 1, mVisualizationLabel->size().height() - 1);

	double bigOffset{ 25.0 };
	double smallOffset{ 10.0 };
	double smallGridCount{ std::ceil(std::sqrt(de.actualPopulation().size() - 1)) };
	double bigSmallRatio{ 3.0 };
	double hRatio{ (labelImageSize.width() - (smallGridCount + 1) * smallOffset) / smallGridCount };
	double vRatio{ (labelImageSize.height() - smallGridCount * smallOffset - 2.0 * bigOffset) / (smallGridCount + bigSmallRatio) };
	double imageSize{ std::min(hRatio, vRatio) };
	double bigImageSize{ imageSize * bigSmallRatio };
	double smallImageSize{ imageSize };

	QPointF bigImageCenter(labelImageSize.width() / 2.0, bigOffset + bigImageSize / 2.0);
	QPointF smallUpperLeftImageCenter(labelImageSize.width() / 2.0 - (smallGridCount - 1.0) / 2.0 * (smallImageSize + smallOffset), 2.0 * bigOffset + bigImageSize + smallImageSize / 2.0);

	QPixmap pixmap(labelImageSize);
	pixmap.fill(backgroundColor);

	const int offsetText{ 8 };
	QPainter painter(&pixmap);
	QFont font(painter.font());
	font.setFamily("Courier new");
	font.setPointSize(12);
	font.setBold(true);
	QFontMetrics titleFontMetrics(font);
	painter.setFont(font);
	painter.setPen(titleColor);
	painter.drawText(QPoint(offsetText, offsetText + titleFontMetrics.height()), title);
	font.setPointSize(10);
	font.setBold(false);
	font.setItalic(true);
	QFontMetrics subTitleFontMetrics(font);
	painter.setFont(font);
	painter.setPen(subTitleColor);
	painter.drawText(QPoint(offsetText + titleFontMetrics.horizontalAdvance(title) - subTitleFontMetrics.horizontalAdvance(subTitle),
							offsetText + titleFontMetrics.height() + subTitleFontMetrics.height()), subTitle);

	drawBox(painter, bigImageCenter, bigImageSize, de.actualPopulation()[0][0], boxColor, cornerColor);
	for (size_t i{ 1 }; i < de.actualPopulation().size(); ++i) {
		size_t row{ (i - 1) / static_cast<size_t>(smallGridCount) };
		size_t column{ (i - 1) % static_cast<size_t>(smallGridCount) };
		QPointF rowColOffset(column * (smallImageSize + smallOffset), row * (smallImageSize + smallOffset));
		drawBox(painter, smallUpperLeftImageCenter + rowColOffset, smallImageSize, de.actualPopulation()[i][0], boxColor, cornerColor);
	}


	mVisualizationLabel->setPixmap(pixmap);
	QSize s{ mVisualizationLabel->size() };
}

void QDEOpenBoxSolution::drawBox(QPainter & painter, QPointF const & boxCenterPos, double boxSize, double cutSize, QColor const & boxColor, QColor const & cornerColor)
{
	double width{ static_cast<double>(mWidthScrollBar->value()) };
	double height{ static_cast<double>(mHeightScrollBar->value()) };
	double scale{ boxSize / std::max(width, height) };

	QRectF box(boxCenterPos.x() - width / 2.0 * scale, boxCenterPos.y() - height / 2.0 * scale, width * scale, height * scale);
	painter.setPen(Qt::NoPen);
	painter.setBrush(boxColor);
	painter.drawRect(box);

	painter.setBrush(cornerColor);
	QSizeF cornerSize(cutSize * scale, cutSize * scale);
	painter.drawRect(QRectF(box.topLeft(), cornerSize));
	painter.drawRect(QRectF(box.topRight() - QPointF(cornerSize.width(), 0.0), cornerSize));
	painter.drawRect(QRectF(box.bottomLeft() - QPointF(0.0, cornerSize.height()), cornerSize));
	painter.drawRect(QRectF(box.bottomRight() - QPointF(cornerSize.width(), cornerSize.height()), cornerSize));
}









#include <OptimizationMaximization.h>
#include <FitnessIdentity.h>
#include <sstream>
#include <iomanip>

QDEOpenBoxSolution::OpenBoxStrategy::OpenBoxStrategy(double width, double height)
	: mWidth{ width }, mHeight{ height }
{
	// Problème à une dimension : a[0] représente la taille x du carré à découper.
	mSolutionBounds.resize(1);
	mSolutionBounds[0].set(0.0, std::min(height, width) / 2.0);
	setOptimizationStrategy(new de::OptimizationMaximization);
	setFitnessStrategy(new de::FitnessIdentity);
}

std::string QDEOpenBoxSolution::OpenBoxStrategy::toString(de::Solution const & ag) const
{
	std::stringstream sStream;
	sStream << "Taille originale   : " << mWidth << " x " << mHeight << std::endl
			<< "Taille de la coupe : " << std::setprecision(6) << std::fixed << ag[0] << " x " << ag[0] << std::endl
			<< "Volume de la boite : " << std::setprecision(6) << std::fixed << ag.fitness();

	return sStream.str();
}

double QDEOpenBoxSolution::OpenBoxStrategy::process(de::Solution const & ag)
{
	if (ag[0] <= 0.0 || ag[0] >= std::min(mWidth, mHeight) / 2.0) {
		return 0.0;
	}

	return ag[0] * (mWidth - 2.0 * ag[0]) * (mHeight - 2.0 * ag[0]);
}

