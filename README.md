# Segmentacija slike upotrebom algoritma za detekciju ivica na ADSP-21489 razvojnoj platformi

U okviru zadatka implementirana je segmentacija slike korištenjem 4 različita detektora ivica (_Kirsch_, _Prewitt_, _Sobel_ i _quick_). Segmentirana slika je zatim kodovana odgovarajućim vrijednostima koje pripadaju istom segmentu, nakon čega je izvršeno bojenje slike radi vizuelnog prikaza segmentacije.

## Korišteni hardverski i softverski resursi

### Hardverski resursi
- **ADSP-21489** razvojna platforma
- PC (_x86-64, Windows 10 OS_)

### Softverski resursi
- _CrossCore Embedded Studio_ (za izvršavanje na ADSP-21489)
- _Visual Studio Code_ (za izvršavanje na PC-u)

## Priloženi fajlovi

U folderu **_ImageSegmentation_** sadržan je sav izvorni kod projektnog zadatka. _Source_ kod fajlovi koji implementiraju tražene funkcionalnosti su:

- **_read_image.c_** fajl u _Visual Studio Code_-u čita ulazne slike date u _bmp_ formatu i smiješta nizove njihovih piksela u **_test_img1.h_** i **_test_img2.h_** fajlove koji se kasnije uvoze u CCES i koriste za obradu slike.

- **_write_image.c_** fajl sadrži kod koji u _Vsual Studio Code-u_ čita piksele iz **_edge_detected_pixels.h_** i **_colored_pixels.h_** fajlova dobijenih u _CCES_ i upisuje vrijednosti tako dobijenih piksela slike u izlaznu sliku u _bmp_ formatu.

Folder **_ImageProcessing_** sadrži projekat kreiran u _CCES_ koji implementira algoritme koji vrši čitanje ulazne slike iz odgovarajućeg _header_ fajla (**_test_img1.h_** ili **_test_img2.h_**), segmentaciju, kodovanje, bojenje slike i upis piksela izlazne slike u odgovarajuće _header_ fajlove (_edge_detected_pixels.h_ i _colored_pixels.h_). Izvorni kod koji implementira pomenute algoritme za obrade slike dat je u fajlovima:

- **_ImageProcessing.h_** fajl sadrži definiciju korištenih konstanti, definiciju maski za algoritme detekcije ivica i deklaracije svih implementiranih funkcija uz opis njihove funkcionalnosti i parametara funkcija.

- **_ImageProcessing.c_** fajl sadrži pretprocesorske direktive korištene pri kompajliranju i optimizaciji, definiciju globalnih nizova korištenih za smiještanje piksela slike, definicije svih funkcija iz _ImageProcessing.h_ fajla i implementaciju **_main_** funkcije u kojoj se pozivaju sve funkcije korštene pri obradi slike.

## Pokretanje i izvršavanje

Pokretanje se sprovodi sljedećim nizom koraka:

- Pokretanje **_read_image.c_** iz _Visual Studio Code_-a za dobijanje _header_ fajlova sa pikselima ulaznih testnih slika.

- _Build_-anje projekta **_ImageProcessing_**  u _CCES_ i spuštanje na ciljnu ADSP-21489 razvojnu platformu (otvaranjem _Debug_ perspektive).

- Pokretanje **write_image.c_** iz _Visual Studio Code_-a za dobijanje izlaznih slika u _bmp_ formatu.

## Izvještaj i rezultati izvršavanja

U _pdf_ dokumentu **_Report_** dat je izvještaj sa detaljnijim opisom funkcionalnosti realizovanih pri izradi projektnog zadatka. Ovaj dokument, takođe, sadrži objašnjene detalje optimizacije i zauzeća resursa.

Folder **_Results_** sadrži rezultate izvršavanja implementiranih algoritama nad ulaznim testnim slikama **_test_img1.bmp_** i **_test_img_2.bmp_**. 

- Podfolder **_test_img1_results_** sadrži originalnu sliku **_test_img1.bmp_** dimenzija **200x200** i rezultate izvršavanja sva 4 algoritma za detekciju ivica nad tom slikom.

- Podfolder **test_img2_results_** sadrži originalnu sliku **_test_img2.bmp_** dimenzija **100x100** i rezultate izvršavanja sva 4 algoritma za detekciju ivica nad tom slikom.

