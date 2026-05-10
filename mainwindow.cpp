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
    // Kullanıcının girdiği metni al [cite: 534]
    // mainwindow.cpp içinde pnrSorgulaBtn kısmını bul ve ilk satırı şöyle değiştir:
    QString pnr = ui->pnrInput->text().trimmed();

    // Boş giriş yapıldıysa uyar [cite: 535-538]
    if (pnr.isEmpty()) {
        ui->yolcuBilgiLabel->setText("Lütfen bir PNR giriniz!");
        return;

    }

    // Sistemden yolcuyu bul [cite: 539]
    Yolcu y = sistem.pnrIleYolcuBul(pnr.toStdString());

    // Yolcu bulunduysa ekrana yaz, bulunamadıysa uyarı ver [cite: 540-544]
    if (!y.ad.empty()) {
        // Yolcunun adını, soyadını ve koltuğunu yazdırıyoruz
        ui->yolcuBilgiLabel->setText(QString::fromStdString("Yolcu: " + y.ad + " " + y.soyad + " | Koltuk: " + y.koltukNo));
    } else {
        ui->yolcuBilgiLabel->setText("Yolcu sistemde bulunamadı.");
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
    // Aynı kutudan bu kez Sefer Numarasını (Örn: TK1920) alıyoruz [cite: 558]
    string seferNo = ui->seferNoInput->text().toStdString();
    // Sistemden bagaj listesini çekiyoruz [cite: 559]
    vector<string> bagajlar = sistem.bagajlariTahliyeEt(seferNo);

    // Eski listeyi temizliyoruz [cite: 560]
    ui->kargoListe->clear();

    // Eğer o sefere ait bagaj yoksa ekrana bilgi ver
    if (bagajlar.empty()) {
        ui->kargoListe->addItem("Bu sefere ait bagaj bulunamadı veya sefer yok.");
        return;
    }

    // Bagajları sırasıyla listeye ekle [cite: 561-563]
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
// Rota sayfasından Ana Menüye dönüş
void MainWindow::on_btnGeriDon1_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// Uçuşlar sayfasından Ana Menüye dönüş
void MainWindow::on_btnGeriDon2_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// Yolcular sayfasından Ana Menüye dönüş
void MainWindow::on_btnGeriDon3_clicked() {
    // Sayfayı ana menüye (0. indeks) çevir
    ui->stackedWidget->setCurrentIndex(0);

    // Kullanıcı deneyimi için: Sayfadan çıkarken eski bilgileri temizle
    ui->pnrInput->clear();
    ui->kargoListe->clear();
    ui->yolcuBilgiLabel->setText("");
}