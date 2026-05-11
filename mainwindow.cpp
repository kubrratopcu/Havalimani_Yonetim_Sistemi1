#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QLineF>

/**
 * @brief Türkiye'nin 81 ilinin yaklaşık (X, Y) koordinat haritası.
 * Bu harita, görsel rota çiziminde şehirleri ekrana yerleştirmek için kullanılır.
 */
std::map<string, QPointF> sehirKonumlari = {
    {"Adana", {520, 340}}, {"Adiyaman", {620, 310}}, {"Afyon", {240, 220}}, {"Agri", {810, 160}},
    // ... (Diğer tüm şehirler)
    {"Duzce", {230, 100}}
};

/**
 * @brief MainWindow Yapıcı Fonksiyonu (Constructor)
 * Arayüzü başlatır, şehir listesini doldurur ve başlangıç tablosunu hazırlar.
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Uygulama ilk açıldığında Ana Sayfa (Index 0) gösterilsin
    if (ui->stackedWidget->count() > 0) {
        ui->stackedWidget->setCurrentIndex(0);
    }

    // ComboBox için şehir listesini tanımla ve alfabetik sırala
    QStringList aktifSehirler = {
        "Adana", "Adiyaman", "Afyon", "Agri", "Aksaray", /* ... */ "Zonguldak"
    };
    aktifSehirler.sort();
    ui->kalkisCombo->addItems(aktifSehirler);
    ui->kalkisCombo->setCurrentIndex(-1); // Seçim boş başlasın

    // Kule Tablosunun (TableWidget) başlıklarını ve yapısını ayarla
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setHorizontalHeaderLabels({"Uçuş No", "Yakıt", "Havayolu", "Durum"});

    // Uygulama başladığında kuledeki mevcut uçakları tabloya doldur
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

/**
 * @brief Kalkış şehri seçildiğinde çalışan fonksiyon.
 * Aynı şehrin hem kalkış hem varış olmaması için varış listesini günceller.
 */
void MainWindow::on_kalkisCombo_currentTextChanged(const QString &arg1) {
    ui->varisCombo->blockSignals(true); // Liste güncellenirken sinyalleri durdur
    ui->varisCombo->clear();

    if (arg1.isEmpty()) {
        ui->varisCombo->blockSignals(false);
        return;
    }

    QStringList tumSehirler = { /* ... tüm şehir listesi ... */ };
    tumSehirler.removeOne(arg1); // Seçilen kalkış şehrini varış listesinden çıkar
    tumSehirler.sort();

    ui->varisCombo->addItems(tumSehirler);
    ui->varisCombo->setCurrentIndex(-1);
    ui->varisCombo->blockSignals(false);
}

/**
 * @brief Kule Yönetimi sayfasına geçiş yapar ve tabloyu günceller.
 */
void MainWindow::on_btnKuleGit_clicked() {
    ui->stackedWidget->setCurrentIndex(2); // Kule Sayfası

    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);

    KuleYonetimi geciciKule = sistem.getKule();
    int satir = 0;
    while (!geciciKule.bosMu()) {
        Ucak u = geciciKule.enOncelikliyiGetir();
        geciciKule.pop();

        ui->tableWidget_2->insertRow(satir);
        ui->tableWidget_2->setItem(satir, 0, new QTableWidgetItem(QString::number(u.id)));
        ui->tableWidget_2->setItem(satir, 1, new QTableWidgetItem(QString::number(u.yakit) + "%"));
        ui->tableWidget_2->setItem(satir, 2, new QTableWidgetItem(QString::fromStdString(u.havayolu)));
        ui->tableWidget_2->setItem(satir, 3, new QTableWidgetItem(u.acilDurum ? "ACİL" : "Normal"));
        satir++;
    }
}

/**
 * @brief PNR kodu ile yolcu sorgulama işlemini gerçekleştirir.
 */
void MainWindow::on_pnrSorgulaBtn_clicked() {
    QString pnr = ui->pnrInput->text().trimmed();
    if (pnr.isEmpty()) {
        ui->yolcuBilgiLabel->setText("Lütfen bir PNR giriniz!");
        return;
    }

    Yolcu y = sistem.pnrIleYolcuBul(pnr.toStdString());

    if (!y.ad.empty()) {
        string ekBilgi = sistem.yolcununUcusBilgisiniGetir(pnr.toStdString());
        QString yazi = QString::fromStdString("Yolcu: " + y.ad + " " + y.soyad) +
                       " | Koltuk: " + QString::fromStdString(y.koltukNo).trimmed() +
                       " " + QString::fromStdString(ekBilgi).trimmed();
        ui->yolcuBilgiLabel->setText(yazi);
    } else {
        ui->yolcuBilgiLabel->setText("Yolcu sistemde bulunamadı.");
    }
}

/**
 * @brief Kuledeki en öncelikli uçağa iniş izni verir ve listeden çıkarır.
 */
void MainWindow::on_kuleIndirBtn_clicked() {
    std::string ucak = sistem.siradakiUcagiIndir();

    if(ucak != "Kuyruk Bos") {
        QMessageBox::information(this, "Kule Kontrol", QString::fromStdString(ucak + " için iniş izni verildi."));
        if (ui->tableWidget_2->rowCount() > 0) ui->tableWidget_2->removeRow(0);
    } else {
        QMessageBox::warning(this, "Kule Kontrol", "İniş bekleyen uçak yok!");
    }
}

/**
 * @brief Sefer numarasına göre bagajları stack'ten tahliye eder (LIFO).
 */
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

/**
 * @brief Dijkstra algoritmasını tetikler ve sonucu hem metin hem görsel olarak sunar.
 */
void MainWindow::on_rotaHesaplaBtn_clicked() {
    std::string kalkis = ui->kalkisCombo->currentText().toStdString();
    std::string varis = ui->varisCombo->currentText().toStdString();

    if (kalkis == varis) {
        ui->rotaSonucLabel->setPlainText("Hata: Kalkış ve varış şehri aynı olamaz!");
        return;
    }

    std::string sonuc = sistem.enKisaRota(kalkis, varis);
    ui->rotaSonucLabel->setPlainText(QString::fromStdString(sonuc));

    grafiGorsellestir(sonuc); // Harita üzerinde çizim yap
}

/**
 * @brief QGraphicsScene kullanarak Türkiye haritası üzerinde rotayı çizer.
 */
void MainWindow::grafiGorsellestir(string rotaSonucu) {
    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    auto& graf = sistem.getGraf();

    // Rotayı parse et (Şehir isimlerini ayır)
    QString hamMetin = QString::fromStdString(rotaSonucu);
    QStringList satirlar = hamMetin.split("\n");
    QString rotaSatiri = satirlar.isEmpty() ? "" : satirlar[0];
    rotaSatiri.replace("En Kisa Rota: ", "");
    QStringList duraklar = rotaSatiri.split(" -> ");

    // Arka Plan: Tüm olası yolları ince çizgilerle çiz
    for (auto const& [sehir, komsular] : graf) {
        for (auto const& komsu : komsular) {
            if (sehirKonumlari.count(sehir) > 0 && sehirKonumlari.count(komsu.first) > 0) {
                scene->addLine(QLineF(sehirKonumlari[sehir], sehirKonumlari[komsu.first]),
                               QPen(QColor(100, 100, 100, 40), 1));
            }
        }
    }

    // Ön Plan: Aktif rotayı kalın kırmızı çizgilerle çiz
    for (int i = 0; i < duraklar.size() - 1; ++i) {
        string mevcut = duraklar[i].trimmed().toStdString();
        string sonraki = duraklar[i+1].trimmed().toStdString();

        if (sehirKonumlari.count(mevcut) > 0 && sehirKonumlari.count(sonraki) > 0) {
            scene->addLine(QLineF(sehirKonumlari[mevcut], sehirKonumlari[sonraki]),
                           QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap));
        }
    }

    // Şehir Düğümleri: 81 ilin tamamını noktalar ve isimlerle çiz
    for (auto const& [isim, nokta] : sehirKonumlari) {
        // Şehrin aktif rotada olup olmadığını kontrol et
        bool rotadaMi = false;
        for (const QString& durak : duraklar) {
            if (durak.trimmed().toStdString() == isim) { rotadaMi = true; break; }
        }

        QColor renk = rotadaMi ? Qt::red : QColor(70, 90, 130);
        int cap = rotadaMi ? 16 : 8;
        int offset = cap / 2;

        scene->addEllipse(nokta.x() - offset, nokta.y() - offset, cap, cap,
                          QPen(rotadaMi ? Qt::white : Qt::transparent, rotadaMi ? 2 : 0), QBrush(renk));

        QGraphicsTextItem* text = scene->addText(QString::fromStdString(isim));
        QFont font = text->font();

        // Rotadaki şehirleri daha belirgin yap
        if (rotadaMi) {
            font.setPointSize(10); font.setBold(true);
            text->setDefaultTextColor(Qt::white);
            text->setZValue(1);
        } else {
            font.setPointSize(6);
            text->setDefaultTextColor(QColor(150, 150, 150));
        }

        text->setFont(font);
        text->setPos(nokta.x() + offset, nokta.y() - offset - (rotadaMi ? 10 : 5));
    }
}