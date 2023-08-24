#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "allocatedialog.h"
#include "releasedialog.h"


#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>


#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // 构造函数
    disk = nullptr;//new Disk("D:\\BaiduSyncdisk\\qt\\QtProject\\bitmap_2_8.txt111"); // 让当前打开的磁盘为空
    model = new QStandardItemModel(); // 创建QT的标准数据显示模型
    model->setRowCount(0);    // 设置行数为 0
    model->setColumnCount(0); // 设置列数为 0

    //MyTableModel *model = new MyTableModel();
    ui->tableView->setModel(model); // 用数据模型绑定tabelView对象
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 列宽自适应
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 行高自适应
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->show();

    ui->lineEdit->setReadOnly(true);

    refreshTable();

}

void MainWindow::refreshTable() {
    if(this->disk == nullptr) {
        model->setRowCount(0);    // 设置行数为 0
        model->setColumnCount(0); // 设置列数为 0
    } else {
        model->setRowCount(disk->n);    // 设置行数为 n
        model->setColumnCount(disk->m); // 设置列数为 m

        // 设置列头
        for(u16 j = 0;j < disk->m;j ++) {
            model->setHeaderData(j, Qt::Horizontal, QString::number(j));
        }
        // 设置行头
        for(u16 i = 0;i < disk->m;i ++) {
            model->setHeaderData(i, Qt::Vertical, QString::number(i));
        }

        for(u16 i = 0;i < disk->n;i ++) {
            for(u16 j = 0;j < disk->m;j ++) {
                bool err = false;
                bool bit = disk->getbit(i, j, err);
                if(err) {
                    QMessageBox::information(nullptr, "警告", QString::fromStdString("越界访问磁盘（"+to_string(i)+"行，"+to_string(j)+"列）"));
                    continue;
                }
                model->setData(model->index(i, j), bit?"1":"0"); //一个个数据进行刷新
                QModelIndex index = model->index(i, j, QModelIndex());
                QStandardItem* item = model->itemFromIndex(index);
                if(item)
                {
                    item->setTextAlignment(Qt::AlignCenter); // item 居中对其
                    item->setBackground(bit?QColor(255, 0, 0):QColor(0, 255, 0)); //RGB
                }
            }
        }

    }
    ui->tableView->reset(); // 整表刷新
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectDiskButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择一个txt文件作为磁盘位示图"), QDir::homePath(), tr("Text Files (*.txt)"));
        if (!fileName.isEmpty()) {
            // 处理打开文件的逻辑
            //std::cout << fileName.toStdString() << std::endl;
            if(disk) {
                delete disk; // 如果有意义 保存上一个磁盘
            }
            disk = new Disk(fileName.toStdString()); // 只用字符串初始化磁盘
            if(disk->bitmap == nullptr) {
                delete disk;
                disk = nullptr;
                QMessageBox::information(nullptr, "警告", "文件名中含有的整数数量不为2");
            }
            ui->lineEdit->setText(fileName);
            refreshTable();

        } else {
            QMessageBox::information(nullptr, "警告", "文件选择退出");
        }

}



void MainWindow::on_allocateButton_clicked()
{
    if(this->disk == nullptr) {
        QMessageBox::information(nullptr, "警告", "请先初始化磁盘再进行分配操作");
        return;
    }
    AllocateDialog* allocateDialog = new AllocateDialog(this->disk);
    allocateDialog->exec();//
    refreshTable();
    //std::cout << "refresh" << std::endl;
}

void MainWindow::on_exitButton_clicked()
{
    if(this->disk) {
        delete this->disk;
    }
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    if(this->disk == nullptr) {
        QMessageBox::information(nullptr, "警告", "请先初始化磁盘再进行分配操作");
        return;
    }
    this->disk->sort();
    refreshTable();
}

void MainWindow::on_releaseButton_clicked()
{
    if(this->disk == nullptr) {
        QMessageBox::information(nullptr, "警告", "请先初始化磁盘再进行分配操作");
        return;
    }
    ReleaseDialog* releaseDialog = new ReleaseDialog(this->disk);
    releaseDialog->exec();
    refreshTable();
}
