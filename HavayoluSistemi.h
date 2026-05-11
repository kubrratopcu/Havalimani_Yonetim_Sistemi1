#ifndef HAVAYOLUSISTEMI_H
#define HAVAYOLUSISTEMI_H

#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>

#include "KuleYonetimi.h"
#include "Modeller.h"

using namespace std;


// HAVAYOLU SİSTEMİ SINIFI (Ana Yönetim Sınıfı - Controller)
// Bu sınıf, sistemdeki tüm veri yapılarını (Graph, Hash Table, Tree, Stack, Queue)
// merkezden yöneten ana orkestra şefidir. Nesne Yönelimli Programlama (OOP)
// prensiplerine uygun olarak Kapsülleme (Encapsulation) kullanılmıştır.

class HavayoluSistemi {
private:


    // 1. Ağaç Tabanlı Harita (Red-Black Tree / std::map)
    // Seferleri sefer numarasına göre sıralı tutar ve O(\log n) hızında erişim sağlar.
    map<string, Sefer> seferSistemi;

    // 2. Öncelikli Kuyruk (Priority Queue - Max/Min Heap)
    // Kuledeki uçakları "İlk gelen ilk çıkar (FIFO)" yerine, aciliyet veya yakıt
    // durumuna göre özel bir öncelik (Priority) kuralıyla sıralar.
    KuleYonetimi kuleYonetimi;

    // 3. Çizge / Graf (Adjacency List - Komşuluk Listesi)
    // Türkiye haritasını düğüm (Şehir) ve kenar (Mesafe) mantığıyla RAM'e çizer.
    // Dijkstra algoritmasının üzerinde gezindiği ana altyapıdır.
    map<string, vector<pair<string, int>>> graf;

    // 4. Karma Tablo (Hash Table / std::unordered_map)
    // PNR numarası eşleştirmelerinde inanılmaz bir hız avantajı sağlar.
    // Red-Black Tree'den farklı olarak arama (Search) karmaşıklığı O(1) yani sabit zamandır.
    unordered_map<string, Yolcu> harita;

    // 5. İkili Arama Ağacı Kökü (Binary Search Tree Root)
    // Kendi yazdığımız hiyerarşik manuel ağacın başlangıç (Root) referansıdır.
    YolcuNode* yolcuAgaciKoku;

public:

    // Constructor: Program açılır açılmaz tüm text verilerini (Veritabanını) belleğe çeker.
    HavayoluSistemi();
    // Destructor: Program kapanırken dinamik oluşturulan (new) tüm pointer'ları silip belleği iade eder.
    ~HavayoluSistemi();

    void yolculariYukle();   // PNR kayıtlarını Hash Table'a atar.
    void seferleriYukle();   // Seferleri kurup yolcuları Linked List (Bağlı Liste) ile vagon gibi ekler.
    void bagajlariYukle();   // Kargo sistemini başlatıp bagajları uçakların Stack (Yığın) yapısına iter.
    void rotalariYukle();    // Dijkstra için çift yönlü (Undirected) graf haritasını inşa eder.

    // Kapsülleme (Encapsulation) ilkesi gereği private olan Kule'ye güvenli erişim sağlar.
    KuleYonetimi& getKule() { return kuleYonetimi; }

    std::string siradakiUcagiIndir();                // Kuyruktan uçağı çeker (Pop).
    string enKisaRota(string kalkis, string varis);  // Dijkstra algoritması ile düğümler arası en kısa yolu hesaplar.
    Yolcu pnrIleYolcuBul(string pnr);                // Hash Table (O(1)) veya Tree üzerinden PNR eşleşmesi yapar.
    string yolcununUcusBilgisiniGetir(string pnr);   // Bağlı listeleri gezerek yolcunun hangi seferde olduğunu bulur.
    vector<string> bagajlariTahliyeEt(string seferNo); // LIFO (Son giren ilk çıkar) mantığıyla kargo yığınını boşaltır.

    // --- GETTER ---
    // Arayüzün (UI) combobox'ları doldurabilmesi için Graf verisine referansla erişim izni verir.
    std::map<std::string, std::vector<std::pair<std::string, int>>>& getGraf() { return graf; }

    // --- BELLEK YÖNETİMİ (MEMORY CLEANUP) ---
    // Bellek sızıntısını (Memory Leak) önlemek için özyineli (Recursive) Post-Order Traversal ile ağaç temizler.
    void agaciTemizle(YolcuNode* kok);

    // Uçuşlar bittiğinde bellekte asılı kalmamaları için Bağlı Listenin (Linked List) düğümlerini sırayla imha eder.
    void seferListesiniTemizle(PnrNode*& bas);
};

#endif // HAVAYOLUSISTEMI_H