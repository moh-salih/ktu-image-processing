# Gelişmiş Görüntü İşleme Uygulaması

**Karadeniz Teknik Üniversitesi — Yazılım Mühendisliği Bölümü**  
**Ders:** Görüntü İşleme (2024–2025 Bahar)  

---

## Özellikler
- 20’den fazla manuel olarak kodlanmış görüntü işleme filtresi  
  (Gaussian, Median, Sobel, Prewitt, Erosion, Dilation, Otsu, Kapur, Centroid…)
- Gerçek zamanlı önizleme ve dinamik histogram görüntüleme  
- Modüler mimari: `core/` (işlem motoru) & `gui/` (arayüz)  
- Çapraz platform desteği: Windows, Linux, macOS  

---

## Kurulum & Derleme

```bash
git clone https://github.com/moh-salih/ktu-image-processing.git
cd ktu-image-processing
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## License

This project is licensed under the MIT License—see the [LICENSE](./LICENSE) file for details.
