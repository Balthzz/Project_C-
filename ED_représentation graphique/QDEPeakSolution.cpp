#include "QDEPeakSolution.h"


#include <PeakFunctionMinSolution.h>
#include <PeakFunctionMaxSolution.h>
#include <PeakFunctionMinMaxSolution.h>

#include <QSurfaceDataArray>
#include <QCustom3DVolume>

#include <QLabel>
#include <QFrame>
#include <QGroupBox>
#include <QRadioButton>
#include <QFormLayout>
#include <QHBoxLayout>

#include <Q3DCamera>
#include <QDebug>


const size_t QDEPeakSolution::sXRes{ 49 };
const size_t QDEPeakSolution::sZRes{ sXRes };
const qreal QDEPeakSolution::sXMin{ -3.0 };
const qreal QDEPeakSolution::sXMax{ 3.0 };
const qreal QDEPeakSolution::sZMin{ -3.0 };
const qreal QDEPeakSolution::sZMax{ 3.0 };
const QString QDEPeakSolution::sInternalResourceMeshFileName(":/DEApplication/point");
const QString QDEPeakSolution::sExternalResourceMeshFileName("point.obj");
const qreal QDEPeakSolution::sMeshOffset{ 0.25 };
const qreal QDEPeakSolution::sMeshScale{ 0.005 };

const QString QDEPeakSolution::sParametersTitle("Paramètres");
const QString QDEPeakSolution::sVisualizationTitle("Visualisation");
const QString QDEPeakSolution::sDescription("Effectue la recherche des points minimum et maximum de la célèbre courbe peak de Matlab.");

const QColor QDEPeakSolution::sMinColor(0xff, 0xbb, 0x00);
const QColor QDEPeakSolution::sMaxColor(0xff, 0x00, 0xbb);


QDEPeakSolution::QDEPeakSolution(QWidget *parent)
	: QDESolutionPanel(parent),
		mMinPeakRadio{ new QRadioButton },
		mMaxPeakRadio{ new QRadioButton },
		mMinMaxPeakRadio{ new QRadioButton },
		mSurface{ new Q3DSurface },
		mSurfaceWidget{ QWidget::createWindowContainer(mSurface) },
		mMeshFileName(),
		mMeshAvailable{},
		mMinColor(1, 1, QImage::Format_RGB32),
		mMaxColor(1, 1, QImage::Format_RGB32)
{
	QFile internalResourceFile(sInternalResourceMeshFileName);
	if (internalResourceFile.exists()) {
		mMeshFileName = sInternalResourceMeshFileName;
	}
	if (mMeshFileName.isEmpty()) {
		QFile externalResourceFile(sExternalResourceMeshFileName);
		if (externalResourceFile.exists()) {
			mMeshFileName = sExternalResourceMeshFileName;
		}
	}
	mMeshAvailable = !mMeshFileName.isEmpty();

	mMinColor.fill(sMinColor);
	mMaxColor.fill(sMaxColor);

	mMinMaxPeakRadio->setChecked(true);

	QFormLayout * inputLayout{ new QFormLayout };
	inputLayout->addRow("Point minimum", mMinPeakRadio);
	inputLayout->addRow("Point maximum", mMaxPeakRadio);
	inputLayout->addRow("Points minimum et maximum", mMinMaxPeakRadio);
	QGroupBox * inputGroupBox{ new QGroupBox(sParametersTitle) };
	inputGroupBox->setLayout(inputLayout);

	QWidget * inputsWidget{ new QWidget };
	QVBoxLayout * inputsLayout{ new QVBoxLayout };
	inputsLayout->addWidget(inputGroupBox);
	inputsLayout->addStretch();
	inputsWidget->setLayout(inputsLayout);

	QGroupBox * visualizationGroupBox{ new QGroupBox(sVisualizationTitle) };
	QVBoxLayout * visualizationLayout{ new QVBoxLayout };
	visualizationLayout->addWidget(mSurfaceWidget);
	visualizationGroupBox->setLayout(visualizationLayout);
	mSurfaceWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QWidget * ioWidget{ new QWidget };
	QHBoxLayout * ioLayout{ new QHBoxLayout };
	ioLayout->addWidget(inputGroupBox);
	ioLayout->addWidget(visualizationGroupBox);
	ioWidget->setLayout(ioLayout);

	QLabel * infoLabel{ new QLabel(sDescription) };
	QFrame * sepFrame{ new QFrame };
	sepFrame->setFrameStyle(static_cast<int>(QFrame::HLine) | static_cast<int>(QFrame::Raised));
	
	QVBoxLayout * mainLayout{ new QVBoxLayout };
	mainLayout->addWidget(infoLabel);
	mainLayout->addWidget(sepFrame);
	mainLayout->addWidget(ioWidget);

	setLayout(mainLayout);

	connect(mMinPeakRadio, &QRadioButton::clicked, this, &QDESolutionPanel::parameterChanged);
	connect(mMaxPeakRadio, &QRadioButton::clicked, this, &QDESolutionPanel::parameterChanged);
	connect(mMinMaxPeakRadio, &QRadioButton::clicked, this, &QDESolutionPanel::parameterChanged);

	mSurface->axisX()->setTitle("x value");
	mSurface->axisY()->setTitle("z value");
	mSurface->axisZ()->setTitle("y value");
	mSurface->axisX()->setTitleVisible(true);
	mSurface->axisY()->setTitleVisible(true);
	mSurface->axisZ()->setTitleVisible(true);
	mSurface->axisX()->setAutoAdjustRange(true);
	mSurface->axisY()->setAutoAdjustRange(true);
	mSurface->axisZ()->setAutoAdjustRange(true);
	mSurface->axisX()->setLabelAutoRotation(90.0f);
	mSurface->axisY()->setLabelAutoRotation(90.0f);
	mSurface->axisZ()->setLabelAutoRotation(90.0f);
	mSurface->axisX()->setSegmentCount(6);
	mSurface->axisY()->setSegmentCount(10);
	mSurface->axisZ()->setSegmentCount(6);
	mSurface->axisX()->setSubSegmentCount(1);
	mSurface->axisY()->setSubSegmentCount(1);
	mSurface->axisZ()->setSubSegmentCount(1);
	mSurface->axisX()->setRange(sXMin, sXMax);
	mSurface->axisZ()->setRange(sZMin, sZMax);
	mSurface->axisY()->setRange(-10.0f, 10.0f);
	QSurfaceDataArray * data{ new QSurfaceDataArray };
	data->reserve(sXRes);
	for (size_t x{}; x < sXRes; ++x) {
		double xVal{ sXMin + (sXMax - sXMin) / double(sXRes - 1) * double(x) };
		QSurfaceDataRow * dataRow{ new QSurfaceDataRow(sZRes) };
		for (size_t z{}; z < sZRes; ++z) {
			double zVal{ sZMin + (sZMax - sZMin) / double(sZRes - 1) * double(z) };
			(*dataRow)[z].setPosition(QVector3D(zVal, de::PeakFunctionSolution::peak(xVal, zVal), xVal));
		}
		*data << dataRow;
	}
	QSurface3DSeries * peakFunc{ new QSurface3DSeries };
	peakFunc->dataProxy()->resetArray(data);
	mSurface->addSeries(peakFunc);


	// Connect le signal 'timeout' du 'timer' de l'auto-rotation de la caméra.
	QObject::connect(&mAutoRotateTimer, &QTimer::timeout, this, &QDEPeakSolution::rotateCamera);
	// Démarre l'auto-rotation
	mAutoRotateTimer.start(30);
}

void QDEPeakSolution::rotateCamera()
{
	float rotation{ mSurface->scene()->activeCamera()->xRotation() };
	rotation += 0.5f;
	if (rotation >= 360.0f) rotation -= 360.0f;
	mSurface->scene()->activeCamera()->setXRotation(rotation);
}

QString QDEPeakSolution::tabTitle() const
{
	return QString("Fonction peak");
}

de::SolutionStrategy * QDEPeakSolution::buildSolution() const
{
	switch (currentMode()) {
		case PeakMode::Minimum:
			return new de::PeakFunctionMinSolution;
		case PeakMode::Maximum:
			return new de::PeakFunctionMaxSolution;
		case PeakMode::MinMax:
		default:
			return new de::PeakFunctionMinMaxSolution;
	}
}

QDEPeakSolution::PeakMode QDEPeakSolution::currentMode() const
{
	if (mMinPeakRadio->isChecked()) {
		return PeakMode::Minimum;
	}

	if (mMaxPeakRadio->isChecked()) {
		return PeakMode::Maximum;
	}

	return PeakMode::MinMax;
}

void QDEPeakSolution::updateVisualization(QDEAdapter const & de)
{
	if (mMeshAvailable) {
		// Approche inefficace : to do : doit réutiliser les points existants si la population ne change pas
		mSurface->removeCustomItems();
		for (size_t i{}; i < de.actualPopulation().size(); ++i) {
			switch (currentMode()) {
				case PeakMode::Minimum:
					addPoint(de.actualPopulation()[i][0], de.actualPopulation()[i][1], false);
					break;
				case PeakMode::Maximum:
					addPoint(de.actualPopulation()[i][0], de.actualPopulation()[i][1], true);
					break;
				case PeakMode::MinMax:
				default:
					addPoint(de.actualPopulation()[i][0], de.actualPopulation()[i][1], true);
					addPoint(de.actualPopulation()[i][2], de.actualPopulation()[i][3], false);
					break;
			}
		}
	}
}

void QDEPeakSolution::addPoint(qreal x, qreal z, bool maximumPoint)
{
	addPoint(x, de::PeakFunctionSolution::peak(x, z), z, maximumPoint);
}

void QDEPeakSolution::addPoint(qreal x, qreal y, qreal z, bool maximumPoint)
{
	QCustom3DItem * point{ new QCustom3DItem };
	point->setMeshFile(mMeshFileName);

	point->setTextureImage(maximumPoint ? mMaxColor : mMinColor);

	//point->setPosition(QVector3D(z, y + (maximumPoint ? sMeshOffset : -sMeshOffset), x));
	point->setPosition(QVector3D(z, y + sMeshOffset, x));
	point->setScaling(QVector3D(sMeshScale, sMeshScale, sMeshScale));

	mSurface->addCustomItem(point);
}

