#include "allocatedialog.h"
#include "ui_allocatedialog.h"

#include <QMessageBox>
#include <QIntValidator>

#include <iostream>

AllocateDialog::AllocateDialog(Disk* disk, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllocateDialog)
{
    ui->setupUi(this);
    ui->radio1->setChecked(true); // 默认勾选连续
    this->disk = disk;

    if(disk == nullptr) {
        QMessageBox::information(nullptr, "警告", "请先初始化磁盘再进行分配操作");
        return;
    }
    QIntValidator validator(0, u32(1) * disk->n * disk->m, ui->lineEdit); // 限制用户输入的范围
    ui->lineEdit->setValidator(&validator);

}

AllocateDialog::~AllocateDialog()
{
    delete ui;
}

void AllocateDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(!disk) return;
    QString text = "";
    for(QChar c : ui->lineEdit->text()) {
        if(c.isDigit()) {
            text.push_back(c);
        }
    }

    QString maxn = QString::number(u32(1) * disk->n * disk->m - disk->getRest());
    if(text.length() > maxn.length() || (text.length() == maxn.length() && QString::compare(text, maxn) > 0)) {
        QMessageBox::information(nullptr, "警告", "输入的数字过大，超过了" + maxn);
        text = "";
    }
    ui->lineEdit->setText(text);
}

void AllocateDialog::on_confirmButton_clicked()
{
    //std::cout << "clicked" << std::endl;
    if(ui->radio1->isChecked()) { // 用户勾选了是连续的
        disk->block_allocate(ui->lineEdit->text().toInt(), 1);
    } else {
        disk->block_allocate(ui->lineEdit->text().toInt(), 0);
    }
    this->close();
}

void AllocateDialog::on_cancelButton_clicked()
{
    this->close();
}
