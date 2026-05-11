#ifndef KULEYONETIMI_H
#define KULEYONETIMI_H

#include <queue>       // Öncelikli kuyruk veri yapısı için
#include "Modeller.h"  // Ucak modelinin tanımı için

// KULE YÖNETİMİ SINIFI
class KuleYonetimi {
public:
    // Mevcut kuyruğa doğrudan erişim sağlar (Referans yoluyla)
    std::priority_queue<Ucak>& getKuyruk() { return kuyruk; }

    // Yeni bir uçağı öncelik sırasına göre sisteme dahil eder
    void ekle(Ucak u) {
        kuyruk.push(u);
    }

    // İşlemi tamamlanan (inen) en öncelikli uçağı kuyruktan çıkartır
    void pop() {
        if(!kuyruk.empty()) {
            kuyruk.pop();
        }
    }

    // Şu an iniş sırası bekleyen en yüksek öncelikli uçağı döndürür
    Ucak enOncelikliyiGetir() {
        return kuyruk.top();
    }

    // Kulede bekleyen uçak olup olmadığını kontrol eder
    bool bosMu() {
        return kuyruk.empty();
    }

private:
    // Uçakları otomatik olarak sıralayan temel veri yapısı
    std::priority_queue<Ucak> kuyruk;
};

#endif // KULEYONETIMI_H