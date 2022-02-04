# Rendszerközeli Programozás Project Dokumentáció:

 Mári Ferenc

 ZC44G0

# Felhasználói útmutatás a programhoz

 A program futtatásához Linux rendszer, egy C compiler és Internetkapcsolatra van szükség.

 Fordításkor szükséges a -fopenmp kapcsoló használata.
```
 gcc compiler esetén a parancs a fordításához: gcc main.c -fopenmp

futtatás: /a.out Így később kell fájlt választani

./a.out—help Segítség,Információ


./a.out—version Verzió Információ
./a.out cpu.bmp cpu.bmp fájl dekódolása
```
 A fájl kiválasztása után.

 A program jelzi hogy sikerült-e vagy nem elküldeni és automatikusan leáll.

 A kód Leírása.


A programban definiált alprogramokat ki szerveztem egy külön saját header állományba (feri.h).

# 1.

## 1; int main(int argc, char* argv[])

 Indítási opciók:

 --help:-> void printHelp()

 Tájékoztatást ad a felhasználónak a futtatás lehetséges opcióiról.

 --version: - > void printVersioninfo()

 Ki írja a program verziószámát, elkészültének dátumát és a fejlesztő nevét.

 egy fájlnév:

 A dekódolandó fájl neve. A program megpróbája megnyitni a megadott fájlt.

 Egyszerre csak 1 db fájl!

 Ha nem adunk meg opciót akkor a fájlt később választjuk ki.


# 2.
## 2.1; char* TestArray(int *NumCh)@ depricated,használja: random_char()

 Az „abc” stringet elhelyezi a megfelelő kódolással egy dinamikusan lefoglalt memória

területre és azt visszaadja.

## 2.2; char* Unwrap(char* Pbuff, int NumCh)

A függvény a megadott címről dekódolt szöveg számára pontosan elegendő memóriaterületet foglal
és ide menti a bit shiftelés-el dekódolt üzenetet.

Majd felszabadítja paraméterként megadott mem.területet és a dekódolt szöveg címével tér vissza.

Ha a memóriafoglalás sikertelen, hibaüzenetet ír az alapértelmezett hiba felületre és a program 1
értéket adva leáll.

## A rendelkezésre álló összes processzormagon párhuzamosan történik, közel azonos terheléssel!

# 3.

## 1; char* ReadPixels(int f, int* NumCh)

A olvasásra megnyitott fájl leíróból olvassa be a képfájl teljes tartalmát egy megfelelő méretű
dinamikusan foglalt memóriaterületre és ennek a címével tér vissza.

Visszatérés előtt a Numch ba menti a kódolt karakterek számát.

Ha a memóriafoglalás sikertelen, a program hibaüzenetet ír ki és 1-el véget ér.

## 2; **int BrowseForOpen()**

A függvény ki írja az aktuális felhasználó alapértelmezett könyvtárának tartalmát.

A felhasználó ezek közül választ egy (fájlt,helyesen gépelve) és azt bemenetként megadja a
programnak.

Amikor a felhasználó egy reguláris fájl nevét adja meg bemenetként akkor a program megkísérli
dekódolni.

A függvény a megnyitás során kapott fájlleíróval tér vissza.


# 4.

## 1; int Post(char *neptunID, char *message, int NumCh)

A dekódolt szöveget egy web szerverre továbbítja megadott Neptun azonosítót és a (dekódolt)
„message” et.

A függvény az „IP” ip című csomópont „PORT” portjára „BUFFER” méretű szövegeket tud küldeni.

Ha a szerver jelzi, hogy megkapta az üzenetet akkor 0-val tér vissza minden más esetben a program

 pozitív értékkel és egy hibaüzenettel tér vissza.

# 5.

## 2; void WhatToDo(int sig)

Egy szignálkezelő eljárás, amely két szignált is képes kezelni.

Ha **SIGALRM** szignál, akkor ki ír egy hibaüzenetet, hogy a program túl sokáig futott, majd leáll 1
hibakóddal!

Ha **SIGINT** szignál, akkor létre hoz egy gyermek folyamatot, amely arról tájékoztatja a felhasználót,
hogy a ctrl+c billentyűkombináció jelenleg nem állítja meg a programot, majd a gyerek be fejeződik
egy saját magának küldött SIGKILL szignál által!


