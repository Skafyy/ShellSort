#Shell_OpenMP.c rövid útmutató:

A program indítása után ellenőrzötten bekér a felhasználótól egy tömbméretet és a mérettől függően random számokkal tölti fel a tömböt. Miután ez megtörtént, a getSpacing függvény kiszámolja a bekért tömbmérettől függően a lépésközöket és ezekkel dolgozik a shellSort függvényben. A shellSort függvény valósítja meg a feltöltött tömbbel és a lépésközökkel a Shell rendezést. Itt az OpenMP segítségével párhuzamosítjuk a részekre bontott al-tömböket. A program kiírja a képernyőre a:
-Feltöltött rendezetlen tömböt, 
-A lépésközöket és a lépésközök darabszámát
-A rendezett tömböt
-Az x db elem rendezéséhez szükséges eltelt időt mp-ben.

#Felhasznált technológia: OpenMP

#Fordítás:
gcc -fopenmp Shell_OpenMP.c Shell_OpenMP

#Futtatás:
Shell_OpenMP






