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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_kalkisCombo_currentTextChanged(const QString &arg1)
{
    if (arg1.isEmpty() || arg1 == "") {
        ui->varisCombo->clear();
        return;
    }

    ui->varisCombo->blockSignals(true);
    ui->varisCombo->clear();

    std::string kalkisStr = arg1.toStdString();
    auto& komsularMap = sistem.getGraf();

    if (komsularMap.count(kalkisStr)) {
        QStringList varisSehirleri;
        for (const auto& komsu : komsularMap[kalkisStr]) {
            varisSehirleri << QString::fromStdString(komsu.first);
        }
        varisSehirleri.sort();
        ui->varisCombo->addItems(varisSehirleri);
        ui->varisCombo->setCurrentIndex(-1);
    }

    ui->varisCombo->blockSignals(false);
}

void MainWindow::on_btnKuleGit_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
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

