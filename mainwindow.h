#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsScene> // Görsel rota çizimi için gerekli sahne kütüphanesi
#include <string>         // Standart metin işlemleri için
#include "HavayoluSistemi.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// MainWindow
 // Uygulamanın ana arayüz penceresini ve kullanıcı etkileşimlerini yöneten sınıftır.

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // =====================================================
    // SAYFA GEÇİŞ SİNYALLERİ (Navigation)
    // =====================================================
    void on_btnKuleGit_clicked();      // Kule yönetim ekranına geçer
    void on_btnRotaGit_clicked();      // Rota hesaplama ekranına geçer
    void on_btnYolcularGit_clicked();  // Yolcu/Bagaj sorgulama ekranına geçer
    void on_btnGeriDon1_clicked();      // Rota sayfasından ana menüye döner
    void on_btnGeriDon3_clicked();      // Yolcu sayfasından ana menüye döner
    void on_btnGeriDonKule_clicked();   // Kule sayfasından ana menüye döner

    // =====================================================
    // MANTIKSAL İŞLEM SİNYALLERİ (Logic)
    // =====================================================
    void on_pnrSorgulaBtn_clicked();   // Girilen PNR ile yolcu verilerini getirir
    void on_kuleIndirBtn_clicked();    // Kuyruktaki uçağa iniş izni verir
    void on_bagajTahliyeBtn_clicked(); // Sefer numarasına göre bagajları boşaltır
    void on_rotaHesaplaBtn_clicked();  // Dijkstra algoritmasını başlatır

    // =====================================================
    // DİNAMİK ARAYÜZ GÜNCELLEMELERİ
    // =====================================================
    // Kalkış şehri değiştiğinde varış listesini otomatik filtreler
    void on_kalkisCombo_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;         // Arayüz elemanlarına erişim sağlayan işaretçi
    HavayoluSistemi sistem;     // Arka plandaki veri yapılarını yöneten ana sistem nesnesi

    // =====================================================
    // GÖRSELLEŞTİRME VE GRAFİK BİLEŞENLERİ
    // =====================================================
    QGraphicsScene *sahne;      // Rota haritasının çizileceği alan


    // Hesaplanan en kısa yolu harita üzerinde görselleştirir.
    // rota: Algoritmadan gelen durak noktalarını içeren metin.

    void grafiGorsellestir(std::string rota);
};

#endif // MAINWINDOW_H