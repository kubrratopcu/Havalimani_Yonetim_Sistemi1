#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "HavayoluSistemi.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnUcuslarGit_clicked();
    void on_btnYolcularGit_clicked();
    void on_btnRotaGit_clicked();

    void on_pnrSorgulaBtn_clicked();
    void on_kuleIndirBtn_clicked();
    void on_bagajTahliyeBtn_clicked();
    void on_rotaHesaplaBtn_clicked();

private:
    Ui::MainWindow *ui;
    HavayoluSistemi sistem;
};

#endif // MAINWINDOW_H-