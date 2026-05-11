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
// 81 İlin yaklaşık (X,Y) Türkiye haritası koordinatları
std::map<string, QPointF> sehirKonumlari = {
    {"Adana", {520, 340}}, {"Adiyaman", {620, 310}}, {"Afyon", {240, 220}}, {"Agri", {810, 160}},
    {"Amasya", {480, 110}}, {"Ankara", {360, 170}}, {"Antalya", {260, 360}}, {"Artvin", {730, 60}},
    {"Aydin", {80, 290}}, {"Balikesir", {90, 170}}, {"Bilecik", {180, 140}}, {"Bingol", {710, 220}},
    {"Bitlis", {780, 240}}, {"Bolu", {270, 110}}, {"Burdur", {210, 310}}, {"Bursa", {140, 130}},
    {"Canakkale", {40, 140}}, {"Cankiri", {380, 120}}, {"Corum", {440, 130}}, {"Denizli", {150, 290}},
    {"Diyarbakir", {680, 280}}, {"Edirne", {50, 60}}, {"Elazig", {640, 230}}, {"Erzincan", {640, 170}},
    {"Erzurum", {720, 150}}, {"Eskisehir", {220, 170}}, {"Gaziantep", {580, 350}}, {"Giresun", {590, 90}},
    {"Gumushane", {620, 120}}, {"Hakkari", {840, 310}}, {"Hatay", {500, 400}}, {"Isparta", {230, 300}},
    {"Mersin", {460, 370}}, {"Istanbul", {120, 90}}, {"Izmir", {60, 250}}, {"Kars", {800, 100}},
    {"Kastamonu", {380, 80}}, {"Kayseri", {480, 230}}, {"Kirklareli", {80, 50}}, {"Kirsehir", {420, 200}},
    {"Kocaeli", {160, 100}}, {"Konya", {340, 280}}, {"Kutahya", {190, 200}}, {"Malatya", {600, 250}},
    {"Manisa", {80, 220}}, {"Kahramanmaras", {560, 300}}, {"Mardin", {710, 330}}, {"Mugla", {100, 340}},
    {"Mus", {750, 220}}, {"Nevsehir", {440, 240}}, {"Nigde", {450, 270}}, {"Ordu", {550, 90}},
    {"Rize", {690, 70}}, {"Sakarya", {190, 100}}, {"Samsun", {490, 70}}, {"Siirt", {770, 280}},
    {"Sinop", {430, 50}}, {"Sivas", {550, 180}}, {"Tekirdag", {80, 80}}, {"Tokat", {510, 140}},
    {"Trabzon", {640, 80}}, {"Tunceli", {670, 200}}, {"Sanliurfa", {630, 360}}, {"Usak", {150, 240}},
    {"Van", {830, 240}}, {"Yozgat", {440, 170}}, {"Zonguldak", {280, 70}}, {"Aksaray", {410, 250}},
    {"Bayburt", {670, 120}}, {"Karaman", {360, 330}}, {"Kirikkale", {390, 160}}, {"Batman", {730, 280}},
    {"Sirnak", {800, 310}}, {"Bartin", {310, 60}}, {"Ardahan", {780, 70}}, {"Igdir", {840, 130}},
    {"Yalova", {130, 110}}, {"Karabuk", {330, 80}}, {"Kilis", {560, 380}}, {"Osmaniye", {530, 360}},
    {"Duzce", {230, 100}}
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
        "Adana", "Adiyaman", "Afyon", "Agri", "Aksaray", "Amasya", "Ankara", "Antalya", "Ardahan", "Artvin",
          "Aydin", "Balikesir", "Bartin", "Batman", "Bayburt", "Bilecik", "Bingol", "Bitlis", "Bolu", "Burdur",
          "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Duzce", "Edirne", "Elazig", "Erzincan",
          "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Igdir", "Isparta", "Istanbul",
          "Izmir", "Kahramanmaras", "Karabuk", "Karaman", "Kars", "Kastamonu", "Kayseri", "Kirikkale", "Kirklareli", "Kirsehir",
          "Kilis", "Kocaeli", "Konya", "Kutahya", "Malatya", "Manisa", "Mardin", "Mersin", "Mugla", "Mus",
          "Nevsehir", "Nigde", "Ordu", "Osmaniye", "Rize", "Sakarya", "Samsun", "Sanliurfa", "Siirt", "Sinop",
          "Sirnak", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Usak", "Van", "Yalova", "Yozgat", "Zonguldak"
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
        "Adana", "Adiyaman", "Afyon", "Agri", "Aksaray", "Amasya", "Ankara", "Antalya", "Ardahan", "Artvin",
        "Aydin", "Balikesir", "Bartin", "Batman", "Bayburt", "Bilecik", "Bingol", "Bitlis", "Bolu", "Burdur",
        "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Duzce", "Edirne", "Elazig", "Erzincan",
        "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Igdir", "Isparta", "Istanbul",
        "Izmir", "Kahramanmaras", "Karabuk", "Karaman", "Kars", "Kastamonu", "Kayseri", "Kirikkale", "Kirklareli", "Kirsehir",
        "Kilis", "Kocaeli", "Konya", "Kutahya", "Malatya", "Manisa", "Mardin", "Mersin", "Mugla", "Mus",
        "Nevsehir", "Nigde", "Ordu", "Osmaniye", "Rize", "Sakarya", "Samsun", "Sanliurfa", "Siirt", "Sinop",
        "Sirnak", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Usak", "Van", "Yalova", "Yozgat", "Zonguldak"
    };

    tumSehirler.removeOne(arg1); // Kalkış şehrini varıştan çıkarır
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
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    auto& graf = sistem.getGraf();

    // 1. ADIM: Rotayı metinden temizle
    QString hamMetin = QString::fromStdString(rotaSonucu);
    QStringList satirlar = hamMetin.split("\n");
    QString rotaSatiri = satirlar.isEmpty() ? "" : satirlar[0];
    rotaSatiri.replace("En Kisa Rota: ", "");
    QStringList duraklar = rotaSatiri.split(" -> ");

    // 2. ADIM: Tüm yolları arka plan dokusu olarak çiz
    for (auto const& [sehir, komsular] : graf) {
        for (auto const& komsu : komsular) {
            if (sehirKonumlari.count(sehir) > 0 && sehirKonumlari.count(komsu.first) > 0) {
                QLineF line(sehirKonumlari[sehir], sehirKonumlari[komsu.first]);
                scene->addLine(line, QPen(QColor(100, 100, 100, 40), 1));
            }
        }
    }

    // 3. ADIM: Aktif Rotayı KALIN ve KIRMIZI çiz
    for (int i = 0; i < duraklar.size() - 1; ++i) {
        string mevcut = duraklar[i].trimmed().toStdString();
        string sonraki = duraklar[i+1].trimmed().toStdString();

        if (sehirKonumlari.count(mevcut) > 0 && sehirKonumlari.count(sonraki) > 0) {
            QLineF rotaLine(sehirKonumlari[mevcut], sehirKonumlari[sonraki]);
            scene->addLine(rotaLine, QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap));
        }
    }

    // 4. ADIM: 81 Şehrin HEPSİNİ Ekrana Bas (İsimleriyle Birlikte)
    for (auto const& [isim, nokta] : sehirKonumlari) {

        bool rotadaMi = false;
        for (const QString& durak : duraklar) {
            if (durak.trimmed().toStdString() == isim) {
                rotadaMi = true; break;
            }
        }

        // Düğüm (Nokta) boyutu ve rengi
        QColor renk = rotadaMi ? Qt::red : QColor(70, 90, 130);
        int cap = rotadaMi ? 16 : 8; // Rotadakiler büyük, diğerleri normal
        int offset = cap / 2;

        // Noktayı çiz
        scene->addEllipse(nokta.x() - offset, nokta.y() - offset, cap, cap,
                          QPen(rotadaMi ? Qt::white : Qt::transparent, rotadaMi ? 2 : 0), QBrush(renk));

        // Şehir İsmini Yaz (ARTIK HEPSİ YAZILIYOR)
        QGraphicsTextItem* text = scene->addText(QString::fromStdString(isim));
        QFont font = text->font();

        if (rotadaMi) {
            // Rotadaki şehirlerin özellikleri (Büyük, Kalın, Beyaz)
            font.setPointSize(10);
            font.setBold(true);
            text->setDefaultTextColor(Qt::white);
            text->setZValue(1); // Çakışma olursa kırmızı yazı hep ÜSTTE dursun
        } else {
            // Rotada OLMAYAN şehirlerin özellikleri (Küçük, İnce, Gri)
            font.setPointSize(6);
            font.setBold(false);
            text->setDefaultTextColor(QColor(150, 150, 150));
            text->setZValue(0); // Altta kalsın
        }

        text->setFont(font);
        text->setPos(nokta.x() + offset, nokta.y() - offset - (rotadaMi ? 10 : 5));
    }
}