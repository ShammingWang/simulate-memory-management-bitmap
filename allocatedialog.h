#ifndef ALLOCATEDIALOG_H
#define ALLOCATEDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "disk.h"

namespace Ui {
class AllocateDialog;
}

class AllocateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AllocateDialog(Disk* disk, QWidget *parent = nullptr);
    ~AllocateDialog();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

    //void on_lineEdit_windowIconTextChanged(const QString &iconText);

private:
    Ui::AllocateDialog *ui;
    Disk* disk;
    //MainWindow* window;
};

#endif // ALLOCATEDIALOG_H
