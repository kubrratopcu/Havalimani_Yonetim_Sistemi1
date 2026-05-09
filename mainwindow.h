#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HavayoluSistemi.h" // Backend dosyan

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        // Slotlar: Tasarımda butonlara verdiğin isimlerle aynı olmalı
        void on_pnrSorgulaBtn_clicked();
    void on_kuleIndirBtn_clicked();
    void on_bagajTahliyeBtn_clicked(); // Yeni eklendi
    void on_rotaHesaplaBtn_clicked();


private:
    Ui::MainWindow *ui;
    HavayoluSistemi sistem; // Senin yazdığın backend sınıfı
};
#endif // MAINWINDOW_H