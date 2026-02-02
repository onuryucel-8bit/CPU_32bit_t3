### Logisim
	
### Assembler 
	
### Derleyici
	------Degisken tanimlama----

	LET a = 1;
	//LET a; gecersiz
	LET abc_t0 = 25;			

	------Atama ve Aritmetik----------

	LET a = 4;
	LET b = 2;
	a = b;
	a = a + 1;
	a = a - 1;
	a = a * 4 + b * 2;

	-----Kosul Yapisi ---------------

	LET a = 4;
	LET b = 0;

	IF a == 4 THEN
		IF b < 0 THEN
			a = a + 1;
		END
		
		b = 2;
	END

	---Donguler----------------------

	LET i = 0;

	WHILE i < 10 THEN			
		i = i + 1;
	END

	---Draw komutu-------------------

	belirtilen x,y noktasina verilen renkte piksel cizer
	DRAW(x,y,color);

	LET posx = 0;
	LET posy = 2;
	LET color = 2;

	DRAW(posx, posy, color);

	WHILE i < 10 THEN
		posx = posx + 1;
		DRAW(posx, posy, color);
	END


	---BUZZ komutu-------------------

	bool => 1 | 0
	BUZZ(x,bool);
	
### Calismakta olan programlar
	- t0_tty_ayi => ascii ayi cizer 						(tty)
	- t1_helloWorld_v(0|1) => klasik merhaba dunya programi (tty)
	- t3_yildiz => 5x5 yildiz karesi cizer 					(tty)
	- t4_agac => cam agaci cizer 						    (RGBVideo)
	- t5_[BadApple] => 32x32 5fps badapple video ciktisi    (RGBVideo)
