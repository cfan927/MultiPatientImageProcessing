#ifndef CUSTOM_QMITK_LABEL_SET_WIDGET_H
#define CUSTOM_QMITK_LABEL_SET_WIDGET_H

#include <ui_CustomQmitkLabelSetWidgetControls.h>

#include "MitkSegmentationUIExports.h"

#include "mitkColorSequenceRainbow.h"
#include "mitkLabel.h"
#include "mitkNumericTypes.h"

class QmitkDataStorageComboBox;
class QCompleter;

namespace mitk
{
  class LabelSetImage;
  class LabelSet;
  class Label;
  class DataStorage;
  class ToolManager;
  class DataNode;
}

class /*MITKSEGMENTATIONUI_EXPORT*/ CustomQmitkLabelSetWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CustomQmitkLabelSetWidget(QWidget *parent = nullptr);
  ~CustomQmitkLabelSetWidget() override;

  void SetDataStorage(mitk::DataStorage *storage);

  void SetOrganColors(const QStringList &organColors);

  void UpdateControls();

  virtual void setEnabled(bool enabled);

  QStringList &GetLabelStringList();

signals:

  /// \brief Send a signal when it was requested to go to a label.
  void goToLabel(const mitk::Point3D &);
  void resetView();

public slots:

  /**
  * @brief Updates the current labels in the label set widget table. For each label (widget item) the 'UpdateTableWidgetItem' is called.
  *
  *   Updating means setting the color box of the table, setting the column with and fill it with the label name.
  *   Furthermore the two push buttons for locking and showing/hiding the layer are checked/unchecked.
  *   This functions only changes the appearance of the table widget and no render window update is necessary.
  */
  void UpdateAllTableWidgetItems();
  /**
  * @brief Resets the current labels in the label set widget table. For each label a widget item is inserted into the table.
  *
  *   Resetting means removing all rows of the widget table and inserting new rows (labels) from the active label set (= layer) of the current working node.
  *   The currently active label is selected and 'Number of labels' is set.
  *   As this function is typically used after one label has been removed or the reference node has been changed (e.g.) the render windows have to be updated.
  */
  void ResetAllTableWidgetItems();
  void SelectLabelByPixelValue(mitk::Label::PixelType pixelValue);

private slots:

  // LabelSet dependent
  void OnOpacityChanged(int);
  void OnUnlockAllLabels(bool);
  void OnLockAllLabels(bool);
  void OnSetAllLabelsVisible(bool);
  void OnSetAllLabelsInvisible(bool);
  void OnSetOnlyActiveLabelVisible(bool);
  void OnRandomColor(bool);
  void OnRemoveLabel(bool);
  void OnRemoveLabels(bool);
  void OnRenameLabel(bool);
  void OnLockedButtonClicked();
  void OnVisibleButtonClicked();
  void OnColorButtonClicked();
  void OnItemClicked(QTableWidgetItem *item);
  void OnItemDoubleClicked(QTableWidgetItem *item);
  void OnTableViewContextMenuRequested(const QPoint &);
  void InsertTableWidgetItem(mitk::Label *label);
  void UpdateTableWidgetItem(QTableWidgetItem *item);
  // reaction to "returnPressed" signal from ...
  void OnSearchLabel();
  // reaction to the button "Change Label"
  void OnActiveLabelChanged(int pixelValue);

  // LabelSetImage Dependet
  void OnCreateDetailedSurface(bool);
  void OnCreateSmoothedSurface(bool);
  // reaction to the signal "createMask" from QmitkLabelSetTableWidget
  void OnCreateMask(bool);
  void OnCreateMasks(bool);
  // reaction to the signal "createCroppedMask" from QmitkLabelSetTableWidget
  void OnCreateCroppedMask(bool);
  void OnCombineAndCreateMask(bool);
  void OnCombineAndCreateSurface(bool);
  void OnEraseLabel(bool);
  void OnEraseLabels(bool);
  // reaction to signal "mergeLabel" from QmitkLabelSetTableWidget
  void OnMergeLabel(bool);
  void OnMergeLabels(bool);
  // reaction to the button "Import Segmentation"
  void OnImportSegmentation();
  // reaction to the button "Import Labeled Image"
  void OnImportLabeledImage();

  // reaction to signal "labelListModified" from QmitkLabelSetTableWidget
  void OnLabelListModified(const QStringList &list);
  // reaction to the signal "toggleOutline" from QmitkLabelSetTableWidget
  void OnToggleOutline(bool);

private:
  enum TableColumns
  {
    NAME_COL = 0,
    LOCKED_COL,
    COLOR_COL,
    VISIBLE_COL
  };

  void WaitCursorOn();

  void WaitCursorOff();

  void RestoreOverrideCursor();

  void OnThreadedCalculationDone();

  void InitializeTableWidget();

  int GetPixelValueOfSelectedItem();

  mitk::LabelSetImage *GetWorkingImage();

  mitk::DataNode *GetWorkingNode();

  typename Ui::CustomQmitkLabelSetWidgetControls m_Controls;

  mitk::ColorSequenceRainbow m_ColorSequenceRainbow;

  mitk::DataStorage *m_DataStorage;

  QCompleter *m_Completer;

  mitk::ToolManager *m_ToolManager;

  QStringList m_OrganColors;

  QStringList m_LabelStringList;
};

#endif // ! CUSTOM_QMITK_LABEL_SET_WIDGET_H