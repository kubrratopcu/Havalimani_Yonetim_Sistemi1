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
    // --- Sayfa Geçiş Butonları ---
    void on_btnKuleGit_clicked();
    void on_btnRotaGit_clicked();
    void on_btnYolcularGit_clicked();
    void on_btnGeriDon1_clicked();      // Rota sayfasından dönüş
    void on_btnGeriDon3_clicked();      // Yolcu sayfasından dönüş
    void on_btnGeriDonKule_clicked();   // Kule sayfasından dönüş

    // --- İşlem Butonları ---
    void on_pnrSorgulaBtn_clicked();
    void on_kuleIndirBtn_clicked();
    void on_bagajTahliyeBtn_clicked();
    void on_rotaHesaplaBtn_clicked();

    // --- Dinamik Filtreleme ---
    void on_kalkisCombo_currentTextChanged(const QString &arg1);  // Doğrusu bu olmalı

private:
    Ui::MainWindow *ui;
    HavayoluSistemi sistem;
};

#endif // MAINWINDOW_H-