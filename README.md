# Opis projektu
#### Windziarz biurowy [MS]
Program sterujący działaniem wind wraz z symulatorem „czasu rzeczywistego”. Zadaniem
programu jest sterowanie systemem wind według określonych zasad. Istotne jest aby
system obsługiwał n wind obsługujących w m grupach o pięter. Każda z grup obsługuje
piętra w podanym zakresie (p_min, p_max) + parter.
#### Windziarz DELUX [MS] - rozszerzenie
Program sterujący działaniem wind w systemie, w którym panele wyboru pięter umieszczone
są w holach wind a nie w samych windach.

# Oprogramowanie
Do poprawnego działania wymagana jest biblioteka **Google Test**

W celu instalacji:
```bash
sudo apt-get install cmake libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make

# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib
```

# Struktura
* src - pliki źródłowe
* tests - testy
