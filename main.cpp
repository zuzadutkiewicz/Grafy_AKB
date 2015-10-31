#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

using namespace std;
void odczytajZPliku(const char * nazwaPliku);

int main()
{
    odczytajZPliku("test.csv");
    return 0;
}

    void odczytajZPliku(const char * nazwaPliku)
{
    char buflicz[10];
    fstream plik;
    string line;
    plik.open(nazwaPliku, ios::in);
    {
        int linia = 0;
        int kolumna = 0;
        while(getline(plik,line))
        {
            linia++;
            const char * buf = line.c_str();
            int i = 0;
            int j = 0;
            while(true) {
                if(buf[i] != '\t' && buf[i] !='\0') {
                    buflicz[j]=buf[i];
                    j++;
                } else {
                    int dana = 0;
                    kolumna++;
                    buflicz[j] = '\0';
                    dana = atoi(buflicz);
                    // sscanf(buflicz, "%d", &dana);
                    printf("linia: %d kolumna: %d liczba: %d\n", linia, kolumna, dana);
                    j = 0;
                }
                if(buf[i] == '\0')
                    break;
            i++;
            }
            kolumna = 0;
        }
    }
    plik.close();
    printf("Odczyt z pliku zakonczony powodzeniem.\n");
}
