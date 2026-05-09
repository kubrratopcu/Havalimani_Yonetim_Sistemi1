#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Gazi Havalimanı Yönetim Sistemi");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 1. PNR SORGULAMA (Hash Table)
void MainWindow::on_pnrSorgulaBtn_clicked()
{
    QString pnr = ui->pnrInput->text();
    if (pnr.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen PNR giriniz!");
        return;
    }

    // Backend: HavayoluSistemi::pnrIleYolcuBul(string pnr)
    Yolcu y = sistem.pnrIleYolcuBul(pnr.toStdString());

    if (!y.ad.empty()) {
        QString mesaj = QString::fromStdString("Yolcu: " + y.ad + " " + y.soyad);
        ui->yolcuBilgiLabel->setText(mesaj);
    } else {
        ui->yolcuBilgiLabel->setText("Yolcu bulunamadı.");
    }
}

// 2. KULE YÖNETİMİ (Priority Queue)
void MainWindow::on_kuleIndirBtn_clicked()
{
    // Backend: HavayoluSistemi::siradakiUcagiIndir()
    string ucak = sistem.siradakiUcagiIndir();

    if(ucak != "Kuyruk Bos") {
        ui->kuleListe->addItem(QString::fromStdString(ucak + " iniş izni aldı."));
    }
}

// 3. KARGO TAHLİYE (Stack) - SEFER NO HATASI BURADA ÇÖZÜLDÜ
void MainWindow::on_bagajTahliyeBtn_clicked()
{
    // Arayüzdeki LineEdit'ten sefer numarasını alıyoruz
    string seferNo = ui->seferInput->text().toStdString();

    if(seferNo.empty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen Sefer No giriniz!");
        return;
    }

    // Backend: HavayoluSistemi::bagajlariTahliyeEt(string seferNo)
    vector<string> bagajlar = sistem.bagajlariTahliyeEt(seferNo);

    ui->kargoListe->clear();
    for(const string& b : bagajlar) {
        ui->kargoListe->addItem(QString::fromStdString(b));
    }
}

// 4. ROTA HESAPLAMA (Graph)
void MainWindow::on_rotaHesaplaBtn_clicked()
{
    string kalkis = ui->kalkisCombo->currentText().toStdString();
    string varis = ui->varisCombo->currentText().toStdString();

    // Backend: HavayoluSistemi::enKisaRota(string kalkis, string varis)
    string sonuc = sistem.enKisaRota(kalkis, varis);
    ui->rotaSonucLabel->setText(QString::fromStdString(sonuc));
}