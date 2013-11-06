#ifndef PROBLEMSDIALOG_H
#define PROBLEMSDIALOG_H


#include <QDialog>
#include <QUrl>
#include <iplugindiagnose.h>


namespace Ui {
class ProblemsDialog;
}


class ProblemsDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit ProblemsDialog(std::vector<MOBase::IPluginDiagnose*> diagnosePlugins, QWidget *parent = 0);
  ~ProblemsDialog();

  bool hasProblems() const;
private:

  void runDiagnosis();

private slots:

  void selectionChanged();
  void urlClicked(const QUrl &url);

  void startFix();
private:

  Ui::ProblemsDialog *ui;
  std::vector<MOBase::IPluginDiagnose *> m_DiagnosePlugins;
};

#endif // PROBLEMSDIALOG_H
