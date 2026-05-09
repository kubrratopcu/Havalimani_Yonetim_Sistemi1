#pragma once // Bu dosyanın derleyici tarafından sadece bir kez okunmasını sağlar (çakışmaları önler)

#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <string>
#include "Modeller.h" // Sefer, Yolcu, YolcuNode gibi yapıların (struct) tanımları buradan geliyor

using namespace std;

// Merkezi Yönetici Sınıfımız
class HavayoluSistemi {
private:
    // ==========================================
    // KAPSÜLLENMİŞ (PRIVATE) VERİ YAPILARI
    // Bu değişkenlere arayüzden veya başka bir sınıftan doğrudan ulaşılamaz.
    // Sadece bu sınıfın içindeki fonksiyonlar buralara veri ekleyip silebilir.
    // ==========================================

    map<string, Sefer> seferSistemi;             // Sefer numarasına göre (Örn: "TK202") sefer bilgilerini tutar (O(log n) erişim)
    priority_queue<Ucak> kule;                   // Uçakları yakıt ve varış zamanına göre otomatik sıraya sokan kule sistemi (Heap mantığı)
    map<string, vector<pair<string, int>>> graf; // Şehirler arası rotaları ve mesafeleri tutan komşuluk listesi (Dijkstra için)
    unordered_map<string, Yolcu> harita;         // Yolcuları PNR numarasına göre anında bulmayı sağlayan Hash Table (O(1) erişim)
    YolcuNode* yolcuAgaciKoku;                   // Yolcuları isimlerine göre alfabetik sıralamak için kullanılacak İkili Arama Ağacının (BST) kök düğümü

public:
    // ==========================================
    // DIŞA AÇIK (PUBLIC) FONKSİYONLAR
    // Arayüz (GUI) veya main.cpp sadece bu bölümdeki fonksiyonları çağırabilir.
    // ==========================================

    // Yapıcı (Constructor) Metot: Sistem başlatıldığında otomatik çalışır
    HavayoluSistemi();

    // Yıkıcı (Destructor) Metot: Sistem kapatıldığında otomatik çalışır (Bellek temizliği için)
    ~HavayoluSistemi();

    // Bellek (RAM) temizliği yapan yardımcı C++ fonksiyonları (delete işlemleri)
    void agaciTemizle(YolcuNode* kok);
    void seferListesiniTemizle(PnrNode*& bas);
};