#include <iostream>   // cout, endl
#include <fstream>    // dosya okuma (ifstream)
#include <sstream>    // stringstream (satır parçalama)
#include <map>        // map veri yapısı
#include "Modeller.h" // Sefer ve Bagaj struct/class'ları

using namespace std;

// =============================================
// SEFERLERİ DOSYADAN OKUYUP SİSTEME YÜKLER
// =============================================
void seferleriYukle(map<string, Sefer>& seferSistemi) {

    // seferler.txt dosyasını aç
    ifstream dosya("seferler.txt");
    string satir;

    // Eğer dosya açılamazsa fonksiyondan çık
    if (!dosya.is_open()) return;

    // Dosyayı satır satır oku
    while (getline(dosya, satir)) {

        // Satırı parçalamak için stringstream kullan
        stringstream ss(satir);

        // CSV formatındaki verileri değişkenlere ayır
        string seferNo, ucakId, kalkis, varis, zaman;
        getline(ss, seferNo, ','); // sefer numarası
        getline(ss, ucakId, ',');  // uçak ID
        getline(ss, kalkis, ',');  // kalkış noktası
        getline(ss, varis, ',');   // varış noktası
        getline(ss, zaman, ',');   // uçuş zamanı (timestamp)

        // Yeni bir Sefer nesnesi oluştur
        Sefer s;

        // Okunan verileri nesneye aktar
        s.seferNo = seferNo;
        s.ucakId = stoi(ucakId);     // string → int dönüşüm
        s.ucusZamani = stoll(zaman); // string → long long dönüşüm

        // TEST AMAÇLI:
        // Her sefere PNR111 yolcusunu ekliyoruz
        // (bagaj eşleşmesini göstermek için)
        s.yolcuPnrListesi.push_back("PNR111");

        // Map içine ekle (key = seferNo)
        seferSistemi[seferNo] = s;
    }

    // Dosyayı kapat
    dosya.close();

    // Bilgi mesajı
    cout << "[-] Sefer Sistemi: Seferler yuklendi." << endl;
}


// =============================================
// BAGAJLARI OKUYUP İLGİLİ SEFERİN STACK'İNE EKLER
// =============================================
void bagajlariYukle(map<string, Sefer>& seferSistemi) {

    // bagajlar.txt dosyasını aç
    ifstream dosya("bagajlar.txt");
    string satir;

    // Dosya açılamazsa çık
    if (!dosya.is_open()) return;

    // Dosyayı satır satır oku
    while (getline(dosya, satir)) {

        // Satırı parçalamak için stringstream kullan
        stringstream ss(satir);

        // Bagaj bilgileri
        string bId, pnr, agirlik;

        getline(ss, bId, ',');     // bagaj ID
        getline(ss, pnr, ',');     // yolcu PNR
        getline(ss, agirlik, ','); // bagaj ağırlığı

        // Bagaj nesnesi oluştur
        Bagaj b = {
            stoi(bId),     // ID → int
            pnr,           // PNR
            stof(agirlik)  // ağırlık → float
        };

        // =============================================
        // BAGAJI DOĞRU SEFERE YERLEŞTİRME
        // =============================================

        // Tüm seferleri dolaş
        for (auto& cift : seferSistemi) {

            // Seferdeki yolcuları kontrol et
            for (string yPnr : cift.second.yolcuPnrListesi) {

                // Eğer bagajın sahibi bu yolcuysa
                if (yPnr == pnr) {

                    // Bagajı o seferin kargo bölümüne ekle
                    // (STACK yapısı → LIFO mantığı)
                    cift.second.kargoBolumu.push(b);
                }
            }
        }
    }

    // Dosyayı kapat
    dosya.close();

    // Bilgi mesajı
    cout << "[-] Kargo Sistemi: Bagajlar ucaklarin yiginina (Stack) yuklendi." << endl;
}
void bagajlariTahliyeEt(Sefer& secilenSefer) {
    cout << "\n[LIFO] " << secilenSefer.seferNo << " seferi bagajlari bosaltiliyor..." << endl;

    if (secilenSefer.kargoBolumu.empty()) {
        cout << "(!) Bu seferde bagaj bulunmamaktadir." << endl;
        return;
    }

    int sira = 1;
    while (!secilenSefer.kargoBolumu.empty()) {
        Bagaj b = secilenSefer.kargoBolumu.top(); // En üstteki bagajı al
        cout << sira << ". Tahliye Edilen Bagaj -> ID: " << b.id
             << " | Sahibi (PNR): " << b.pnr_sahibi << endl;

        secilenSefer.kargoBolumu.pop(); // Yığından çıkar
        sira++;
    }
    cout << "[✔] Tum bagajlar teslim noktasına gonderildi." << endl;
}