#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "Modeller.h"

using namespace std;

/**
 * @brief Uçakları dosyadan okuyup Kule'nin Öncelikli Kuyruğuna (Heap) ekler.
 * @param kule Referans olarak alınan Priority Queue yapısı.
 */
void ucaklariSistemeYukle(priority_queue<Ucak>& kule) {
    // ifstream: Dosyadan okuma işlemi yapmak için dosya akışını başlatır
    ifstream dosya("ucaklar.txt");
    string satir;

    // Dosyanın başarıyla açılıp açılmadığını güvenlik amacıyla kontrol ediyoruz
    if (!dosya.is_open()) {
        cerr << "Hata: ucaklar.txt dosyasi bulunamadi!" << endl;
        return;
    }

    // Dosyanın sonuna gelene kadar satır satır okuma işlemi yapar
    while (getline(dosya, satir)) {
        // stringstream: Okunan tüm satırı parçalara ayırmak için tampon belleğe alır
        stringstream ss(satir);
        string id, havayolu, yakit, zaman;

        // Satırdaki verileri virgüle (',') göre bölerek ilgili string değişkenlere atar
        getline(ss, id, ',');
        getline(ss, havayolu, ',');
        getline(ss, yakit, ',');
        getline(ss, zaman, ',');

        Ucak u;
        // stoi: String to Integer (Metni tam sayıya çevirir)
        u.id = stoi(id);
        u.havayolu = havayolu;
        u.yakit = stoi(yakit);
        
        // stoll: String to Long Long (Zaman verisi büyük olduğu için long long kullanıyoruz)
        u.varisZamani = stoll(zaman);

        // Uçağı kuyruğa ekleriz. Bu satır çalıştığı an, Modeller.h içindeki operator< 
        // devreye girer ve uçağı zaman/yakıt aciliyetine göre Heap ağacında doğru yere yerleştirir.
        kule.push(u); 
    }
    
    // İşlem bitince RAM'i serbest bırakmak için dosyayı kapatıyoruz
    dosya.close();
}