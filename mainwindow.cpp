#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QLineF>
// Şehirlerin ekrandaki (X, Y) konumlarını tutan harita
//önemli!!!! rota ıcın eklendı gorsel rota
// Şehirlerin ekrandaki (X, Y) koordinat haritası (TÜM 18 ŞEHİR EKLENDİ)
std::map<string, QPointF> sehirKonumlari = {
    {"Istanbul", {80, 80}},     {"Bursa", {110, 110}},     {"Eskisehir", {180, 140}},
    {"Ankara", {280, 150}},     {"Izmir", {40, 220}},      {"Mugla", {70, 300}},
    {"Bodrum", {40, 290}},      {"Antalya", {200, 320}},   {"Konya", {300, 250}},
    {"Samsun", {400, 60}},      {"Trabzon", {560, 70}},    {"Erzurum", {640, 140}},
    {"Van", {760, 220}},        {"Adana", {420, 310}},     {"Gaziantep", {500, 320}},
    {"Sanliurfa", {560, 330}},  {"Diyarbakir", {620, 280}},{"Mardin", {640, 340}}
};
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


// Şehirlerin ekrandaki (X, Y) koordinat haritası
// Bu koordinatları kendi ekran tasarımına göre (View boyutuna göre) değiştirebilirsin.

//---------------rota
void MainWindow::on_rotaHesaplaBtn_clicked() {
    std::string kalkis = ui->kalkisCombo->currentText().toStdString();
    std::string varis = ui->varisCombo->currentText().toStdString();

    if (kalkis == varis) {
        ui->rotaSonucLabel->setPlainText("Hata: Kalkış ve varış şehri aynı olamaz!");
        return;
    }

    // Algoritmayı çalıştır ve arayüze yaz
    std::string sonuc = sistem.enKisaRota(kalkis, varis);
    ui->rotaSonucLabel->setPlainText(QString::fromStdString(sonuc));

    // Görselleştirmeyi tetikle
    grafiGorsellestir(sonuc);
}
void MainWindow::grafiGorsellestir(string rotaSonucu) {
    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    auto& graf = sistem.getGraf();

    // 1. ADIM: Tüm yolları (Graf kenarlarını) ince, soluk gri çizgilerle çiz
    for (auto const& [sehir, komsular] : graf) {
        for (auto const& komsu : komsular) {
            if (sehirKonumlari.count(sehir) > 0 && sehirKonumlari.count(komsu.first) > 0) {
                QLineF line(sehirKonumlari[sehir], sehirKonumlari[komsu.first]);
                scene->addLine(line, QPen(QColor(100, 100, 100, 150), 1));
            }
        }
    }

    // 2. ADIM: Gelen metni temizle ("En Kisa Rota: Ankara -> Antalya\nMesafe..." içinden sadece şehirleri al)
    QString hamMetin = QString::fromStdString(rotaSonucu);
    QString sadeceRota = hamMetin;

    if (hamMetin.contains("Rota: ")) {
        int baslangic = hamMetin.indexOf("Rota: ") + 6;
        int bitis = hamMetin.indexOf("\n", baslangic);
        if (bitis == -1) bitis = hamMetin.length();
        sadeceRota = hamMetin.mid(baslangic, bitis - baslangic);
    }

    QStringList duraklar = sadeceRota.split(" -> ");

    // 3. ADIM: Dijkstra'nın geçtiği rotadaki çizgileri KALIN ve KIRMIZI yap
    for (int i = 0; i < duraklar.size() - 1; ++i) {
        string mevcut = duraklar[i].trimmed().toStdString();
        string sonraki = duraklar[i+1].trimmed().toStdString();

        if (sehirKonumlari.count(mevcut) > 0 && sehirKonumlari.count(sonraki) > 0) {
            QLineF rotaLine(sehirKonumlari[mevcut], sehirKonumlari[sonraki]);
            // Çizgiyi kırmızı ve daha kalın (5) yapıyoruz
            scene->addLine(rotaLine, QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));
        }
    }

    // 4. ADIM: Şehir düğümlerini (Node) çiz
    for (auto const& [isim, nokta] : sehirKonumlari) {

        // Bu şehir rotamızın içinde geçiyor mu diye kontrol et
        bool rotadaMi = false;
        for (const QString& durak : duraklar) {
            if (durak.trimmed().toStdString() == isim) {
                rotadaMi = true;
                break;
            }
        }

        // Rotadaki şehirleri KIRMIZI ve daha BÜYÜK (24px) yap, diğerlerini koyu mavi (16px) bırak
        QColor renk = rotadaMi ? Qt::red : QColor(20, 40, 80);
        int cap = rotadaMi ? 24 : 16;
        int offset = cap / 2;

        scene->addEllipse(nokta.x() - offset, nokta.y() - offset, cap, cap, QPen(Qt::white, 2), QBrush(renk));

        // Şehir ismini yazdır
        QGraphicsTextItem* text = scene->addText(QString::fromStdString(isim));
        text->setPos(nokta.x() + offset, nokta.y() + offset - 10);

        // Rotadaki şehirlerin ismini beyaz ve KALIN yap, diğerleri gri kalsın
        if (rotadaMi) {
            text->setDefaultTextColor(Qt::white);
            QFont font = text->font();
            font.setBold(true);
            text->setFont(font);
        } else {
            text->setDefaultTextColor(Qt::gray);
        }
    }
}