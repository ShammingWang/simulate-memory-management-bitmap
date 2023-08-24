#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "disk.h"

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refreshTable();

private slots:

    void on_selectDiskButton_clicked();

    //void on_tableView_activated(const QModelIndex &index);

    void on_allocateButton_clicked();

    void on_exitButton_clicked();

    void on_pushButton_clicked();

    void on_releaseButton_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* model;
    Disk *disk; // ѡ����ť������ʱ����ʼ��
};
#endif // MAINWINDOW_H
