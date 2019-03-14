#include "MitkDrawingTool.h"
#include "ui_MitkDrawingTool.h"
#include "QmitkSegmentationOrganNamesHandling.cpp"
#include "QmitkNewSegmentationDialog.h"
#include <mitkIOUtil.h>
#include "QmitkToolGUI.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "DataManager.h"

MitkDrawingTool::MitkDrawingTool(mitk::DataStorage *datastorage, QWidget *parent) :
    GuiModuleBase(parent),
    ui(new Ui::MitkDrawingTool),
    m_DataStorage(datastorage),
    m_LastToolGUI(nullptr)
{
    ui->setupUi(this);

    this->m_ToolManager = mitk::ToolManagerProvider::GetInstance()->GetToolManager();
    assert(m_ToolManager);

    m_ToolManager->SetDataStorage(*m_DataStorage);
    m_ToolManager->InitializeTools();

    ui->labelSetWidget->SetDataStorage(m_DataStorage);
    ui->labelSetWidget->SetOrganColors(mitk::OrganNamesHandling::GetDefaultOrganColorString());
    //ui->labelSetWidget->hide();

    ui->toolSelectionBox->SetToolManager(*m_ToolManager);
    ui->toolSelectionBox->setEnabled(true);
    ui->toolSelectionBox->SetGenerateAccelerators(true);
    ui->toolSelectionBox->SetDisplayedToolGroups(tr("Add Subtract Correction Paint Erase").toStdString());
    ui->toolSelectionBox->SetLayoutColumns(3);
    ui->toolSelectionBox->SetEnabledMode(QmitkToolSelectionBox::AlwaysEnabled);

    connect(ui->newLabelPushBtn, SIGNAL(clicked()), this, SLOT(OnCreateNewLabel()));
    connect(ui->ConfirmSegBtn, SIGNAL(clicked()), this, SLOT(OnConfirmSegmentation()));
    connect(ui->toolSelectionBox, SIGNAL(ToolSelected(int)), this, SLOT(OnManualTool2DSelected(int)));
    
}

MitkDrawingTool::~MitkDrawingTool()
{
  if (m_WorkingMaskNode)
  {
    this->OnMitkDataNodeAboutToGetRemoved(m_WorkingMaskNode);
  }

  delete ui;
}

void MitkDrawingTool::SetMitkImageViewer(MitkImageViewer* mitkImageViewer)
{
  connect(mitkImageViewer, SIGNAL(LoadedNewMask(mitk::DataNode::Pointer)),
    this, SLOT(OnLoadedNewMask(mitk::DataNode::Pointer))
  );
  connect(mitkImageViewer, SIGNAL(MitkDataNodeAboutToGetRemoved(mitk::DataNode::Pointer)),
    this, SLOT(OnMitkDataNodeAboutToGetRemoved(mitk::DataNode::Pointer))
  );
}

void MitkDrawingTool::OnLoadedNewMask(mitk::DataNode::Pointer dataNode)
{
  // Connect the mask to the tool
  m_LoadedMaskNode = dataNode;

  mitk::DataNode::Pointer m_WorkingMaskNode = mitk::DataNode::New();
  m_WorkingMaskNode->SetData(dynamic_cast<mitk::Image *>(dataNode->GetData()));
  m_WorkingMaskNode->SetName("Mask");

  //this->m_DataStorage->Add(m_WorkingMaskNode, m_LoadedMaskNode);

  //this->m_ToolManager->SetWorkingData(m_WorkingMaskNode);
  this->m_ToolManager->SetWorkingData(m_LoadedMaskNode);
  this->m_ToolManager->SetReferenceData(m_LoadedMaskNode);
  
  if (ui->labelSetWidget->isHidden())
    ui->labelSetWidget->show();
  ui->labelSetWidget->ResetAllTableWidgetItems();

  mitk::RenderingManager::GetInstance()->InitializeViews(
    m_WorkingMaskNode->GetData()->GetTimeGeometry(), 
    mitk::RenderingManager::REQUEST_UPDATE_ALL, true
  );

  // Check if this was triggered by pressing the new label button
  if (m_WaitingOnLabelsImageCreation)
  {
	  this->OnCreateNewLabel();
    m_WaitingOnLabelsImageCreation = false;
  }
}

void MitkDrawingTool::OnMitkDataNodeAboutToGetRemoved(mitk::DataNode::Pointer dataNode)
{
  // TODO: Save changes
}

// void MitkDrawingTool::OnEnableSegmentation()
// {
//   m_LabelsImageName.clear();

//   QString path = m_DataManager->GetDataPath(m_CurrentData);
//   QFileInfo f(path);
//   mitk::DataNode::Pointer referenceNode = this->m_DataStorage->GetNamedNode(f.baseName().toStdString().c_str());

//   if (!referenceNode)
//   {
//     QMessageBox::information(
//       this, "New Segmentation Session", "Please load and select a patient image before starting some action.");
//     return;
//   }

//   m_ToolManager->ActivateTool(-1);

//   mitk::Image* referenceImage = dynamic_cast<mitk::Image*>(referenceNode->GetData());
//   assert(referenceImage);

//   m_LabelsImageName = QString::fromStdString(m_DataManager->GetSubjectName(
// 	  m_DataManager->GetSubjectIdFromDataId(m_CurrentData)
//   ).toStdString());
//   m_LabelsImageName.append("-mask");

//   bool ok = false;
//   m_LabelsImageName = QInputDialog::getText(this, "New Segmentation Session", "New name:", QLineEdit::Normal, m_LabelsImageName, &ok);

//   if (!ok)
//   {
//     return;
//   }

//   mitk::LabelSetImage::Pointer workingImage = mitk::LabelSetImage::New();
//   try
//   {
//     workingImage->Initialize(referenceImage);
//   }
//   catch (mitk::Exception& e)
//   {
//     MITK_ERROR << "Exception caught: " << e.GetDescription();
//     QMessageBox::information(this, "New Segmentation Session", "Could not create a new segmentation session.\n");
//     return;
//   }

//   mitk::DataNode::Pointer workingNode = mitk::DataNode::New();
//   workingNode->SetData(workingImage);
//   workingNode->SetName(m_LabelsImageName.toStdString());

//   if (!this->m_DataStorage->Exists(workingNode))
//   {
//     this->m_DataStorage->Add(workingNode, referenceNode);
//   }

//   this->m_ToolManager->SetWorkingData(workingNode);
//   this->m_ToolManager->SetReferenceData(referenceNode);

//   OnCreateNewLabel();
// }

// void MitkDrawingTool::CreateNewSegmentation()
// {
//   // Create empty segmentation working image
//   mitk::DataNode::Pointer workingImageNode = mitk::DataNode::New();
//   mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingImageNode->GetData());
//   const std::string organName = "test";
//   mitk::Color color; // actually it dosn't matter which color we are using
//   color.SetRed(1);   // but CreateEmptySegmentationNode expects a color parameter
//   color.SetGreen(0);
//   color.SetBlue(0);
//   mitk::Tool* firstTool = m_ToolManager->GetToolById(0);
//   workingImageNode = firstTool->CreateEmptySegmentationNode(workingImage, organName, color);
//   this->m_DataStorage->Add(workingImageNode);
//   if (workingImageNode.IsNotNull())
//     this->m_ToolManager->SetWorkingData(workingImageNode);
//   mitk::DataNode::Pointer origNode = this->m_DataStorage->GetNamedNode("LoadedData");
//   if (origNode.IsNotNull())
//     this->m_ToolManager->SetReferenceData(origNode);
//   mitk::RenderingManager::GetInstance()->InitializeViews(workingImageNode->GetData()->GetTimeGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
// }

// void MitkDrawingTool::OnDisableSegmentation()
// {
//   if (m_LastToolGUI)
//   {
//     ui->toolGUIArea->layout()->removeWidget(m_LastToolGUI);
//     m_LastToolGUI->setParent(nullptr);
//     delete m_LastToolGUI; // will hopefully notify parent and layouts
//     m_LastToolGUI = nullptr;

//     QLayout *layout = ui->toolGUIArea->layout();
//     if (layout)
//     {
//       layout->activate();
//     }
//   }
// }

void MitkDrawingTool::OnCreateNewLabel()
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode* workingNode = m_ToolManager->GetWorkingData(0);
  if (!workingNode)
  {
    QMessageBox::information(
      this, "New Segmentation Session", "Please load and select a patient image before starting some action.");
    return;
  }

  mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingNode->GetData());
  if (!workingImage)
  {
    QMessageBox::information(
      this, "New Segmentation Session", "Please load and select a patient image before starting some action.");
    return;
  }

  QmitkNewSegmentationDialog* dialog = new QmitkNewSegmentationDialog(this);
  dialog->SetSuggestionList(mitk::OrganNamesHandling::GetDefaultOrganColorString());
  dialog->setWindowTitle("New Label");

  int dialogReturnValue = dialog->exec();
  if (dialogReturnValue == QDialog::Rejected)
  {
    return;
  }

  QString labelsImageName = dialog->GetSegmentationName();
  if (labelsImageName.isEmpty())
  {
    labelsImageName = "Unnamed";
  }
  workingImage->GetActiveLabelSet()->AddLabel(labelsImageName.toStdString(), dialog->GetColor());

  if (ui->labelSetWidget->isHidden())
    ui->labelSetWidget->show();
  ui->labelSetWidget->ResetAllTableWidgetItems();

  mitk::RenderingManager::GetInstance()->InitializeViews(workingNode->GetData()->GetTimeGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
}

void MitkDrawingTool::OnConfirmSegmentation()
{
  // QString pathToSave = QDir::currentPath();
  // if (m_DataManager->GetSubjectIdFromDataId(m_CurrentData) != -1)
  // {
  //   pathToSave = m_DataManager->GetSubjectPath(m_DataManager->GetSubjectIdFromDataId(m_CurrentData));
  // }

  // QString filename = QFileDialog::getSaveFileName(this, tr("Save Segmentation Mask"),
  //   pathToSave, tr("Images (*.nrrd)"));
  
  // // if (!filename.endsWith(".nii.gz"))
  // // {
	//  //  filename = filename + ".nii.gz";
  // // }

  // if (!filename.isEmpty())
  // {
  //   mitk::DataNode::Pointer segData = this->m_DataStorage->GetNamedNode(m_LabelsImageName.toStdString());
  //   mitk::IOUtil::Save(segData->GetData(), filename.toStdString());

  //   //remove after saving
  //   m_DataStorage->Remove(m_DataStorage->GetNamedNode(m_LabelsImageName.toStdString()));
    
  //   if (m_DataManager->GetSubjectIdFromDataId(m_CurrentData) != -1)
  //   {
	// 	m_DataManager->AddDataToSubject(
	// 		m_DataManager->GetSubjectIdFromDataId(m_CurrentData),
	// 		filename,
	// 		"Mask"
	// 	);
  //   }
  // }

  // //re-render here
  // mitk::RenderingManager::GetInstance()->RequestUpdateAll();

  // //hide ourself
  // this->hide();
}

void MitkDrawingTool::OnManualTool2DSelected(int id)
{
  if (id >= 0)
  {
    std::string text = m_ToolManager->GetToolById(id)->GetName();
    
    if (text == "Paint")
    {
      //this->OnDisableSegmentation();

      mitk::Tool *tool = m_ToolManager->GetActiveTool();

      itk::Object::Pointer possibleGUI = tool->GetGUI("Qmitk", "GUI").GetPointer(); 
      QmitkToolGUI *gui = dynamic_cast<QmitkToolGUI *>(possibleGUI.GetPointer());
      m_LastToolGUI = gui;
      if (gui)
      {
        gui->SetTool(tool);

        gui->setParent(ui->toolGUIArea);
        gui->move(gui->geometry().topLeft());
        gui->show();

        QLayout *layout = ui->toolGUIArea->layout();
        if (!layout)
        {
          layout = new QVBoxLayout(ui->toolGUIArea);
        }
        if (layout)
        {
          layout->addWidget(gui);
          layout->activate();
        }
      }
    }
    else
    {
      //this->OnDisableSegmentation();
    }
 
  }

}