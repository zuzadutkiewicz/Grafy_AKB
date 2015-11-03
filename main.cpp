#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define MAX_TAB 100
#define MAX_MAC_ORYG 1000

using namespace std;

int macierz[MAX_TAB][MAX_TAB];
int rozmMacierz = 0;

int macierzOryg[MAX_MAC_ORYG][MAX_MAC_ORYG];
int rozmMacierzOryg = 0;


void odczytajZPliku(const char * nazwaPliku);
int czyJednograf();
int sprawdzSprzez(int x, int y);
int wybierzWierzcholki();
int sprawdzLin(int x, int y);
void przenumTab();
void drukTab(int z, int k);

int main()
{
    odczytajZPliku("test.csv");
    czyJednograf();
    wybierzWierzcholki();
    przenumTab();

    return 0;
}

void odczytajZPliku(const char * nazwaPliku)
{
    char buflicz[10];
    fstream plik;
    string line;
    plik.open(nazwaPliku, ios::in);
    int linia = 0;
    int kolumna = 0;
    while(getline(plik,line))
    {
        const char * buf = line.c_str();
        int i = 0;
        int j = 0;
        //int liczKolumn = 0;
        linia++;
        while(true)
        {
            if(buf[i] != '\t' && buf[i] !='\0')
            {
                buflicz[j]=buf[i];
                j++;
            }
            else
            {
                int dana = 0;
                kolumna++;
                buflicz[j] = '\0';
                dana = atoi(buflicz);
                macierz[linia-1][kolumna-1] = dana;
                printf("linia: %d kolumna: %d liczba: %d\n", linia, kolumna, dana);
                j = 0;
            }
            if(buf[i] == '\0')
                break;
            i++;
        }
        // liczKolumn = kolumna;
        kolumna = 0;
    }
    //liczKolumn == linia;
    rozmMacierz = linia;
    plik.close();
    printf("Odczyt z pliku zakonczony powodzeniem.\n");
}

// sprawdz czy macierz zawiera inne liczby niz 0 lub 1
// zwaracane wartosci
//      1 - gdy nie jest
//      0 - gdy jest

int czyJednograf()
{
    int z = 0;
    int k = 0;
    for(z=0; z < rozmMacierz; z++)
    {
        for (k=0;  k < rozmMacierz; k++)
        {
            if (macierz[z][k] != 1 && macierz[z][k] != 0)
            {
                printf("Podany graf nie jest 1-grafem. Kolumna: %d, wiersz %d\n", k,z);
                return 1;
            }
        }
    }
    printf("Podany graf jest 1-grafem.\n");
    return 0;
}

int sprawdzSprzez(int x, int y)
{
    int znaczRoz = 0;
    int znaczIden = 0;
    int z = 0;
    for (z =0; z < rozmMacierz; z ++)
    {
        if ((macierz[x][z] == 0 && macierz[y][z] == 1))
        {
            znaczRoz = 1;
        }
        else if (macierz[x][z] == 1 && macierz[y][z] == 0)
        {
            znaczRoz = 1;
        }
        else if (macierz[x][z] == 1 && macierz[y][z] == 1)
        {
            znaczIden = 1;
        }
    }
    if(znaczIden == 1 &&znaczRoz == 1)
        return 0;
    else if (znaczIden == 1 && znaczRoz == 0)
        return 1;
    return 2;
}

int wybierzWierzcholki()
{
    int x = 0;
    int y = 0;
    int z = 0;
    int lx = 0;

    for (x = 0; x <rozmMacierz; x++)
    {
        for (y = x + 1; y < rozmMacierz; y++)
        {
            z = sprawdzSprzez(x,y);
            if (z == 0)
            {
                printf("Graf nie jest grafem sprzezonym. \n");
                return 0;
            }
            else if (z == 1)
            {
                if( sprawdzLin(x, y) == 1 )
                    lx = 1;
            }
        }
        printf("Graf jest grafem sprzezonym. \n");
        if (lx == 0)
        {
            printf("Graf jest liniowy. \n");
        }
        else if (lx == 1)
        {
            printf("Graf nie jest grafem liniowym. \n");
        }


    }
    return 1;
}
int sprawdzLin(int x,int y)
{
    int z= 0;

    for (x = 0; x <rozmMacierz; x++)
    {
        for (y = x + 1; y < rozmMacierz; y++)
        {
            for (z =0; z < rozmMacierz; z ++)
            {
                if (macierz[z][x] == 1 && macierz[z][y] == 1)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void przenumTab()
{
    int z = 0;
    int k = 0;
    int n = 1;
    for(z=0; z < rozmMacierz; z++)
    {
        for (k=0;  k < rozmMacierz; k++)
        {
            if (macierz[z][k] == 1)
            {
                macierz[z][k] = n++;
             //   n++;
            }

        }
    }
    drukTab(z,k);
}

void drukTab(int z,int k)
{
    {
        int z = 0;
        int k = 0;

        for(z=0; z < rozmMacierz; z++)
        {
            for (k=0;  k < rozmMacierz; k++)
            {
                printf ("macierz[%d][%d]=%d\n", z, k, macierz[z][k]);
            }
        }
    }
}
