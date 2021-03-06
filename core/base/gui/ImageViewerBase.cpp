#include "ImageViewerBase.h"

#include <QDebug>

ImageViewerBase::ImageViewerBase(QWidget *parent) : GuiModuleBase(parent)
{
	qDebug() << QString("ImageViewerBase::ImageViewerBase");
}

void ImageViewerBase::SetDataView(DataViewBase* dataView)
{
	qDebug() << QString("ViewerBase::SetDataView()");
	// No need to save a reference to the DataView
	// Data are fetched from the DataManager

	m_DataView = dataView;

	connect(dataView, SIGNAL(SelectedSubjectChanged(long)),
		this, SLOT(SelectedSubjectChangedHandler(long))
	);
	connect(dataView, SIGNAL(DataAddedForSelectedSubject(long)),
		this, SLOT(DataAddedForSelectedSubjectHandler(long))
	);
	connect(dataView, SIGNAL(DataRemovedFromSelectedSubject(long)),
		this, SLOT(DataRemovedFromSelectedSubjectHandler(long))
	);
	connect(dataView, SIGNAL(SelectedDataChanged(long)),
		this, SLOT(SelectedDataChangedHandler(long))
	);
	connect(dataView, SIGNAL(DataCheckedStateChanged(long, bool)),
		this, SLOT(DataCheckedStateChangedHandler(long, bool))
	);
}

void ImageViewerBase::SetOpacitySlider(QSlider* slider)
{
	qDebug() << QString("ViewerBase::SetOpacitySlider()");

	connect(slider, SIGNAL(valueChanged(int)),
		this, SLOT(OpacitySliderHandler(int))
	);
}

void ImageViewerBase::OpacitySliderHandler(int value)
{
	qDebug() << QString("ViewerBase::OpacitySliderHandler()");
}

void ImageViewerBase::SelectedSubjectChangedHandler(long uid)
{
	qDebug() << QString("ViewerBase::SelectedSubjectChangedHandler()") << uid;
}

void ImageViewerBase::SelectedDataChangedHandler(long iid)
{
	qDebug() << QString("ViewerBase::SelectedDataChangedHandler()") << iid;
}

void ImageViewerBase::DataAddedForSelectedSubjectHandler(long iid)
{
	qDebug() << QString("ViewerBase::DataAddedForSelectedSubjectHandler()") << iid;
}

void ImageViewerBase::DataRemovedFromSelectedSubjectHandler(long iid)
{
	qDebug() << QString("ViewerBase::DataRemovedFromSelectedSubjectHandler()") << iid;
}

void ImageViewerBase::DataCheckedStateChangedHandler(long iid, bool checkState)
{
	qDebug() << QString("ViewerBase::DataCheckedStateChangedHandler()") << iid;
}

DataViewBase* ImageViewerBase::GetDataView()
{
	return m_DataView;
}