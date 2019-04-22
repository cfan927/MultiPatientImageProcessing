#ifndef MITK_IMAGE_VIEWER_H
#define MITK_IMAGE_VIEWER_H

#include <QString>
#include <QWidget>
#include <QList>
#include <mitkDataNode.h>
#include <mitkStandaloneDataStorage.h>
#include <CustomQmitkStdMultiWidget.h>

#include "ImageViewerBase.h"
#include "CustomMitkDataStorage.h"

class MitkImageViewer : public ImageViewerBase
{
	Q_OBJECT

public:
	MitkImageViewer(QWidget *parent = nullptr);
	~MitkImageViewer();

public slots:
	// Slot for slider
	virtual void OpacitySliderHandler(int value) override;

	// Slots for DataViewBase
	virtual void SelectedSubjectChangedHandler(long uid) override;
	virtual void DataRemovedFromSelectedSubjectHandler(long iid) override;
	virtual void SelectedDataChangedHandler(long iid) override;
	virtual void DataCheckedStateChangedHandler(long iid, bool checkState) override;
	/*del*/virtual void OnExportData(long iid, QString fileName);

	// Slot, but usually just called.
	/*del*/void SaveImageToFile(long iid, bool updateDataManager = true);

	// Reference is an nrrd image that shows which labels to use
	/*del*/void ConvertToNrrdAndSave(long iid, long referenceIid = -1, bool updateDataManager = true);

protected:	
	CustomQmitkStdMultiWidget* m_MitkWidget;
	CustomMitkDataStorage*     m_DataStorage;

private:
	bool m_FirstTimeForThisSubject = true;
};

#endif // ! MITK_IMAGE_VIEWER_H
