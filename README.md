Hedefler
=======
-Donen kup
-Donen 3d Model
-Bad Apple
-Mandelbrot
-Pong
-Xox minmax
=======

-------
Logisim
Assembler
Compiler
Emu
utils/ROMPrinter
utils/CharsetROMPrinter
-------

----AssemblerV4----

	+Tekrar eden kisimlari fonksiyon veya macro haline getir
	+Ziplama komutlarini parser`a ekle

	+Tokens.h adli dosya olustur enum tokentype i oraya tasi
	-.ascii ve .text ekle
	-Hata algilama kismini iyilestir
	+Cikti dizisini tanimla

	-TODO kisimlarini yaz	
	+"Fonksiyon" komutlarini parser`a ekle
	~#define makrolarini ekle

	+#include "stdmath.asm" dosya okuma sistemini ekle		
	
-----------------

- Makefile ekle

+Yakalama(Fetch) Dongusu
	t0 
	Mar = pc
	pc++
	t1
	MDR = ram[MAR]
	t2
	IR = MDR

- ROM yazicisini kontrol et	
- LOAD rx,Sayi ROM kontrolBitleri
- Komut kumesini iyilestir
- Emulator .u yaz
- Teletype Ciktisi
- Fibonacci
- Float(Kesirli sayi) islemlerini ekle 16-32 IEEE veya sabit noktali tanim olucak
- pi ve sinx hesaplayan program ciktisini TTY birimine gonder
- Stack(Yigin) [ayri RAM veya RAM_0 icinde belirli bir alan] tanimla
- Ekran kartini tasarla
- Ekran karti x,y nokta registerlerini tanimla
- Ekran karti islemci birimini ROM ile bagla
- Ekran kartini bagla
- Bresenham cizgi algoritma devresini ekran kartina ekle
- Ekran kartina piksel degerlerini gondererek basit cizim yap
- Ekran kartina Ascii karakter yolla
- RLE fonksiyonu yaz
- Bad Apple dosyasini proje icin hazirla(fps dusur, RLE den gecir)
- Bad Apple denemesi
- Buzzer devresini tasarla
- Buzzer devresini bagla
- Bad Apple ses ciktisini WAV donustur
- WAV okuyucusu yaz
- Ses dosyasini RLE den gecir
- Bad Apple ses Denemesi
- Bad Apple video denemesi
- Derleyici BASIC + C tasarla
- C Derleyicisi yaz
- Mandelbrot 
- Pong
- MinMax xox
- Son hata Ayiklamalari
- Rafa kaldiris