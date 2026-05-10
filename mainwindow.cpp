#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Program her zaman Ana Menü (0. indeks) ile başlasın
    if (ui->stackedWidget->count() > 0) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- NAVİGASYON (SAYFA GEÇİŞLERİ) ---
void MainWindow::on_btnUcuslarGit_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_btnYolcularGit_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btnRotaGit_clicked() { ui->stackedWidget->setCurrentIndex(1); }

// --- VERİ YAPISI FONKSİYONLARI ---

// 1. PNR SORGULAMA (BST Arama)
// ... (Constructor ve Destructor kısımları aynı kalacak)

// 1. PNR SORGULAMA (BST Arama)
void MainWindow::on_pnrSorgulaBtn_clicked() {
    QString pnr = ui->pnrInput->text();
    if (pnr.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen PNR giriniz!");
        return;
    }
    Yolcu y = sistem.pnrIleYolcuBul(pnr.toStdString());
    if (!y.ad.empty()) {
        ui->yolcuBilgiLabel->setText(QString::fromStdString("Yolcu: " + y.ad + " " + y.soyad));
    } else {
        ui->yolcuBilgiLabel->setText("Yolcu bulunamadı.");
    }
}

// 2. KULE YÖNETİMİ (Priority Queue
// DİKKAT: Designer'da bu butonun adı "kuleIndirBtn" olmalı!
void MainWindow::on_kuleIndirBtn_clicked() {
    string ucak = sistem.siradakiUcagiIndir();
    if(ucak != "Kuyruk Bos") {
        // Eğer bir ListWidget kullanıyorsan addItem, Table kullanıyorsan insertRow yapmalısın
        // ui->tableWidget_2->insertRow(0);
        QMessageBox::information(this, "Kule", QString::fromStdString(ucak + " iniş izni aldı."));
    }
}

// 3. BAGAJ TAHLİYE (Stack) - AZ ÖNCE EKSİK OLAN BUYDU!
// 3. BAGAJ TAHLİYE (Stack Mantığı)
void MainWindow::on_bagajTahliyeBtn_clicked() {
    // Giriş kutusundan sefer numarasını alıyoruz
    string seferNo = ui->pnrInput->text().toStdString();

    // Sistemden bagaj listesini çekiyoruz
    vector<string> bagajlar = sistem.bagajlariTahliyeEt(seferNo);

    // Listeyi temizliyoruz
    ui->kargoListe->clear();

    if (bagajlar.empty()) {
        ui->kargoListe->addItem("Bu sefere ait bagaj bulunamadı.");
        return;
    }

    // Bagajları ekrana tek tek yazdırıyoruz
    for (const string& b : bagajlar) {
        ui->kargoListe->addItem(QString::fromStdString(b));
    }
}

// 4. ROTA HESAPLAMA (Dijkstra Mantığı)
void MainWindow::on_rotaHesaplaBtn_clicked() {
    // ComboBox'lardan şehirleri alıyoruz
    string kalkis = ui->kalkisCombo->currentText().toStdString();
    string varis = ui->varisCombo->currentText().toStdString();

    // En kısa rotayı hesaplıyoruz
    string sonuc = sistem.enKisaRota(kalkis, varis);

    // Sonucu ekrandaki yazı alanına yazdırıyoruz
    ui->rotaSonucLabel->setPlainText(QString::fromStdString(sonuc));
}