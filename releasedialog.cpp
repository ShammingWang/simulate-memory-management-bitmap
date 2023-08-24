#include "releasedialog.h"
#include "ui_releasedialog.h"

ReleaseDialog::ReleaseDialog(Disk* disk, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReleaseDialog)
{
    ui->setupUi(this);
    this->disk = disk;

}

ReleaseDialog::~ReleaseDialog()
{
    delete ui;
}

void ReleaseDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(!disk) return;
    QString text = "";
    for(QChar c : ui->lineEdit->text()) {
        if(c.isDigit()) {
            text.push_back(c);
        }
    }

    QString maxn = QString::number(u32(1) * disk->n * disk->m - 1); // 注意这里需要减去1 因为输入的是下标
    if(text.length() > maxn.length() || (text.length() == maxn.length() && QString::compare(text, maxn) > 0)) {
        QMessageBox::information(nullptr, "警告", "输入的数字过大，超过了" + maxn);
        text = "";
    }
    ui->lineEdit->setText(text);
}



void ReleaseDialog::on_confirmButton_clicked()
{
    disk->block_release(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
    this->close();
}


void ReleaseDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(!disk) return;
    QString text = "";
    for(QChar c : ui->lineEdit_2->text()) {
        if(c.isDigit()) {
            text.push_back(c);
        }
    }

    QString maxn = QString::number(u32(1) * disk->n * disk->m);
    if(text.length() > maxn.length() || (text.length() == maxn.length() && QString::compare(text, maxn) > 0)) {
        QMessageBox::information(nullptr, "警告", "输入的数字过大，超过了" + maxn);
        text = "";
    }
    ui->lineEdit_2->setText(text);
}

void ReleaseDialog::on_cancelButton_clicked()
{
    this->close();
}


