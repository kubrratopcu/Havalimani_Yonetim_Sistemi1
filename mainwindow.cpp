#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (ui->stackedWidget->count() > 0) {
        ui->stackedWidget->setCurrentIndex(0);
    }

    QStringList aktifSehirler = {
        "Adana", "Ankara", "Antalya", "Bodrum", "Bursa", "Diyarbakir",
        "Erzurum", "Eskisehir", "Gaziantep", "Istanbul", "Izmir",
        "Konya", "Mardin", "Mugla", "Samsun", "Sanliurfa", "Trabzon", "Van"
    };
    aktifSehirler.sort();
    ui->kalkisCombo->addItems(aktifSehirler);
    ui->kalkisCombo->setCurrentIndex(-1);
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setHorizontalHeaderLabels({"Uçuş No", "Yakıt", "Havayolu", "Durum"});

    KuleYonetimi geciciKule = sistem.getKule();
    int satir = 0;
    while (!geciciKule.bosMu()) {
        Ucak u = geciciKule.enOncelikliyiGetir();
        geciciKule.pop();

        ui->tableWidget_2->insertRow(satir);
        ui->tableWidget_2->setItem(satir, 0, new QTableWidgetItem(QString::number(u.id)));
        ui->tableWidget_2->setItem(satir, 1, new QTableWidgetItem(QString::number(u.yakit) + "%"));
        ui->tableWidget_2->setItem(satir, 2, new QTableWidgetItem(QString::fromStdString(u.havayolu)));

        QString oncelikMetni = u.acilDurum ? "ACİL DURUM" : "Normal";
        ui->tableWidget_2->setItem(satir, 3, new QTableWidgetItem(oncelikMetni));
        satir++;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_kalkisCombo_currentTextChanged(const QString &arg1) {
    // 1. İşlem yaparken arayüzün kafası karışmasın diye sinyalleri durdur
    ui->varisCombo->blockSignals(true);
    ui->varisCombo->clear();

    // 2. Eğer kalkış şehri henüz seçilmediyse boş bırak
    if (arg1.isEmpty()) {
        ui->varisCombo->blockSignals(false);
        return;
    }

    // 3. Varış listesi için tüm şehirleri tam liste halinde oluştur
    QStringList tumSehirler = {
        "Adana", "Ankara", "Antalya", "Bodrum", "Bursa", "Diyarbakir",
        "Erzurum", "Eskisehir", "Gaziantep", "Istanbul", "Izmir",
        "Konya", "Mardin", "Mugla", "Samsun", "Sanliurfa", "Trabzon", "Van"
    };

    // 4. Kalkış olarak seçtiğimiz şehri varış listesinden ÇIKAR (Ankara'dan Ankara'ya gidilmez)
    tumSehirler.removeOne(arg1);
    tumSehirler.sort();

    // 5. Temizlenmiş ve sıralanmış tam listeyi varış kutusuna ekle
    ui->varisCombo->addItems(tumSehirler);
    ui->varisCombo->setCurrentIndex(-1); // Sayfa açıldığında boş görünsün

    // 6. Sinyalleri geri aç
    ui->varisCombo->blockSignals(false);
}
void MainWindow::on_btnKuleGit_clicked() {
    // 1. Önce Kule sayfasına geçiş yap (Burdaki sayının doğru olduğundan emin olmalıyız)
    ui->stackedWidget->setCurrentIndex(2);

    // 2. Tabloyu sıfırla ve başlıkları GÜNCELLE
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setHorizontalHeaderLabels({"Uçuş No", "Yakıt", "Havayolu", "Durum"});

    // 3. Arka planda hazır bekleyen uçakları çek
    KuleYonetimi geciciKule = sistem.getKule();

    // 4. Tabloyu satır satır doldur
    int satir = 0;
    while (!geciciKule.bosMu()) {
        Ucak u = geciciKule.enOncelikliyiGetir();
        geciciKule.pop();

        ui->tableWidget_2->insertRow(satir);
        ui->tableWidget_2->setItem(satir, 0, new QTableWidgetItem(QString::number(u.id)));
        ui->tableWidget_2->setItem(satir, 1, new QTableWidgetItem(QString::number(u.yakit) + "%"));
        ui->tableWidget_2->setItem(satir, 2, new QTableWidgetItem(QString::fromStdString(u.havayolu)));

        QString oncelikMetni = u.acilDurum ? "ACİL" : "Normal";
        ui->tableWidget_2->setItem(satir, 3, new QTableWidgetItem(oncelikMetni));

        satir++;
    }
}
void MainWindow::on_btnYolcularGit_clicked() {
    ui->pnrInput->clear();
    ui->yolcuBilgiLabel->setText("");
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnRotaGit_clicked() {
    ui->kalkisCombo->blockSignals(true);
    ui->varisCombo->blockSignals(true);

    ui->kalkisCombo->setCurrentIndex(-1);
    ui->varisCombo->clear();
    ui->rotaSonucLabel->clear();

    ui->stackedWidget->setCurrentIndex(1);

    ui->kalkisCombo->blockSignals(false);
    ui->varisCombo->blockSignals(false);
}

void MainWindow::on_pnrSorgulaBtn_clicked() {
    QString pnr = ui->pnrInput->text().trimmed();
    if (pnr.isEmpty()) {
        ui->yolcuBilgiLabel->setText("Lütfen bir PNR giriniz!");
        return;
    }

    Yolcu y = sistem.pnrIleYolcuBul(pnr.toStdString());

    if (!y.ad.empty()) {
        string ekBilgi = sistem.yolcununUcusBilgisiniGetir(pnr.toStdString());
        QString temizKoltuk = QString::fromStdString(y.koltukNo).trimmed();
        QString yazi = QString::fromStdString("Yolcu: " + y.ad + " " + y.soyad) +
                       " | Koltuk: " + temizKoltuk + " " + QString::fromStdString(ekBilgi).trimmed();
        ui->yolcuBilgiLabel->setText(yazi);
    } else {
        ui->yolcuBilgiLabel->setText("Yolcu sistemde bulunamadı.");
    }
}

void MainWindow::on_kuleIndirBtn_clicked() {
    std::string ucak = sistem.siradakiUcagiIndir();

    if(ucak != "Kuyruk Bos") {
        QMessageBox::information(this, "Kule Kontrol", QString::fromStdString(ucak + " için iniş izni verildi."));

        if (ui->tableWidget_2->rowCount() > 0) {
            ui->tableWidget_2->removeRow(0);
        }
    } else {
        QMessageBox::warning(this, "Kule Kontrol", "İniş bekleyen uçak yok!");
    }
}

void MainWindow::on_bagajTahliyeBtn_clicked() {
    string seferNo = ui->seferNoInput->text().toStdString();
    vector<string> bagajlar = sistem.bagajlariTahliyeEt(seferNo);
    ui->kargoListe->clear();
    if (bagajlar.empty()) {
        ui->kargoListe->addItem("Bu sefere ait bagaj bulunamadı.");
        return;
    }
    for (const string& b : bagajlar) {
        ui->kargoListe->addItem(QString::fromStdString(b));
    }
}

void MainWindow::on_btnGeriDon1_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_btnGeriDonKule_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_btnGeriDon3_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
    ui->pnrInput->clear();
    ui->kargoListe->clear();
    ui->yolcuBilgiLabel->setText("");
    ui->seferNoInput->clear();
}

void MainWindow::on_rotaHesaplaBtn_clicked() {
    std::string kalkis = ui->kalkisCombo->currentText().toStdString();
    std::string varis = ui->varisCombo->currentText().toStdString();

    if (kalkis == varis) {
        ui->rotaSonucLabel->setPlainText("Hata: Kalkış ve varış şehri aynı olamaz!");
        return;
    }

    std::string sonuc = sistem.enKisaRota(kalkis, varis);
    ui->rotaSonucLabel->setPlainText(QString::fromStdString(sonuc));
}

