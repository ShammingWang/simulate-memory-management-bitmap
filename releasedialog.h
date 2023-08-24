#ifndef RELEASEDIALOG_H
#define RELEASEDIALOG_H

#include "disk.h"

#include <QDialog>

namespace Ui {
class ReleaseDialog;
}

class ReleaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReleaseDialog(Disk* disk, QWidget *parent = nullptr);
    ~ReleaseDialog();

private slots:
    //void on_startLineEdit_textChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ReleaseDialog *ui;
    Disk* disk;
};

#endif // RELEASEDIALOG_H
