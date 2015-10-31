#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>

using namespace std;

char * nazwaPliku;
int wierzcholek=0;

void podajIodczytaj();
void odczytajZPliku(char * nazwaPliku);



void podajIodczytaj()
{
    char nazwa [30];
    printf("Podaj nazwe pliku: ");
    scanf("%s", nazwa);
    odczytajZPliku(nazwa);
}

void odczytajZPliku(char * nazwaPliku)
{
    fstream plik;
    string line;



    plik.open(nazwaPliku, ios::in);
    {
        while(getline(plik,line))
        {
            const char * buf = line.c_str();
            sscanf(buf, "%d", &wierzcholek);
            printf("Linia: %s\n", buf);

        }
    }
    plik.close();
    printf("Odczyt z pliku zakonczony powodzeniem\n");
}



int main()
{
  podajIodczytaj();
  printf("Przed wierzochlkiem\n");

  printf("%d\n", wierzcholek);

printf("po wierzochlkiem\n");

  return 0;
}
