/*
Load, check adjoint graph and generate source graph based on adjoint graph.
Copyright (C) 2015  Zuzanna Dutkiewicz

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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

typedef struct element
{
    int poczatek;
    int koniec;
} Element;

Element macierzPom[MAX_MAC_ORYG];
int rozMacierzPom = 0;

int macierzOryg[MAX_MAC_ORYG][MAX_MAC_ORYG];
int rozmMacierzOryg = 0;


void odczytajZPliku(const char * nazwaPliku);
void zapiszDoPliku(const char * nazwaPliku);
int czyJednograf();
int sprawdzSprzez(int x, int y);
int czySprzez();
int sprawdzLin(int x, int y);
void przenumTab();
void drukTab(int tryb);
void generujGrafOryg();
void wpiszWierzcholki(int x);
void drukTabOryg(int tryb);
int poprzedniki(int wierzcholek, int listaPoprz[]);
int nastepniki(int wierzcholek, int listaNast[]);
int ustawPoprzedniki(int punkt, int liczbaPoprz, int listaPoprz[]);
int ustawNastepniki (int punkt, int liczbaNast, int listaNast[]);
void drukujMacierzPom(int tryb);
void macierzPomDoOryg();

const char* nazwaPlikuZrodlowego = "test.csv";
const char* nazwaPlikuDocelowego = "testOryg.csv";


int main()
{
    int ret = 0;
    int ret1 = 0;
    odczytajZPliku(nazwaPlikuZrodlowego);
    drukTab(1);
    ret = czyJednograf();
    if (ret == 1)
        exit(1);
    ret1 = czySprzez();
    if(ret1 == 0)
        exit(1);
    generujGrafOryg();
    drukujMacierzPom(1);
    drukTabOryg(1);
    zapiszDoPliku(nazwaPlikuDocelowego);

    return 0;
}


void odczytajZPliku(const char * nazwaPliku)
{
    char buflicz[10];
    int wiersz = 0;
    int kolumna = 0;

    fstream plik;
    string line;
    plik.open(nazwaPliku, ios::in);
    if(plik.good() != true)
    {
        printf("Nie mozna otworzyc pliku %s", nazwaPliku);
        exit(1);
    }

    while(getline(plik,line))
    {
        const char * buf = line.c_str();
        int i = 0;
        int j = 0;
        //int liczKolumn = 0;
        wiersz++;
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
                macierz[wiersz-1][kolumna-1] = dana;
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
    rozmMacierz = wiersz;
    plik.close();
    printf("Odczyt z pliku %s zakonczony powodzeniem.\n", nazwaPliku);
}


void zapiszDoPliku(const char * nazwaPliku)
{
    fstream plik;
    plik.open(nazwaPlikuDocelowego, ios::out);
    char buf[MAX_TAB *3];
    for(int w=0; w < rozmMacierzOryg; w++)
    {
        for (int k=0;  k < rozmMacierzOryg; k++)
        {
            if(k == 0)
                sprintf(buf,"%d", macierzOryg[w][k]);
            else
                sprintf(buf,"%s\t%d", buf, macierzOryg[w][k]);
        }
        plik << buf << endl;
    }
    plik.close();
    printf("Zapis danych do plik %s zakonczony powodzeniem.\n", nazwaPliku);
}


// sprawdz czy macierz zawiera inne liczby niz 0 lub 1
// zwaracane wartosci
//      1 - gdy nie jest
//      0 - gdy jest

int czyJednograf()
{
    for(int w=0; w < rozmMacierz; w++)
        for (int k=0;  k < rozmMacierz; k++)
            if (macierz[w][k] != 1 && macierz[w][k] != 0)
            {
                printf("Podany graf nie jest 1-grafem. wiersz: %d, kolumna %d\n", w,k);
                return 1;
            }

    printf("Podany graf jest 1-grafem.\n");
    return 0;
}


int sprawdzSprzez(int w1, int w2)
{
    int znaczRoz = 0;
    int znaczIden = 0;

    for (int k =0; k < rozmMacierz; k ++)
        if ((macierz[w1][k] == 0 && macierz[w2][k] == 1))
            znaczRoz = 1;
        else if (macierz[w1][k] == 1 && macierz[w2][k] == 0)
            znaczRoz = 1;
        else if (macierz[w1][k] == 1 && macierz[w2][k] == 1)
            znaczIden = 1;

    if(znaczIden == 1 && znaczRoz == 1)
        return 0;
    else if (znaczIden == 1 && znaczRoz == 0)
        return 1;

    return 2;
}


int czySprzez()
{
    int lx = 0;

    for (int x = 0; x <rozmMacierz; x++)
        for (int y = x + 1; y < rozmMacierz; y++)
        {
            int z = sprawdzSprzez(x,y);
            if (z == 0)
            {
                printf("Graf nie jest grafem sprzezonym. \n");
                return 0;
            }
            else if (z == 1)
            {
                if( sprawdzLin(x, y) == 1 )
                {
                    printf("Nieliniowosc na elementach x=%d (%c), y=%d (%c)\n", x,x+ 'A', y, y +'A');
                    lx = 1;
                }

            }
        }

    printf("Graf jest grafem sprzezonym. \n");
    if (lx == 0)
        printf("Graf jest liniowy. \n");
    else if (lx == 1)
        printf("Graf nie jest grafem liniowym. \n");

    return 1;
}


int sprawdzLin(int x,int y)
{
    int zbiorNiepusty =0;
    // przegladanie poprzednikow - gdy maja chociaz jeden wspolny poprzednik
    // to graf nieliniowy
    for(int poz = 1; poz < rozmMacierz; poz++)
        if(macierz[poz][x] == 1 && macierz[poz][y] == 1)
            return 1;
    // brak wspolnych poprzednikow - graf liniowy (a raczej tylko te dwa punkty)
    return 0;
}


void generujGrafOryg()
{
    int listaPoprz[MAX_MAC_ORYG];
    int listaNast[MAX_MAC_ORYG];

    // ustawienie macierzy pomocniczej
    for(int punkt = 0; punkt < rozmMacierz; punkt++)
    {
        macierzPom[punkt].poczatek = -1;
        macierzPom[punkt].koniec = -1;
    }

    for(int punkt = 0; punkt < rozmMacierz; punkt++)
    {
        int liczbaPoprz = poprzedniki(punkt, listaPoprz);
        int liczbaNast  = nastepniki(punkt, listaNast);

        if(liczbaPoprz > 0)
            rozMacierzPom = ustawPoprzedniki( punkt, liczbaPoprz, listaPoprz);
        else
            macierzPom[punkt].poczatek = rozMacierzPom++;

        if(liczbaNast > 0)
            rozMacierzPom = ustawNastepniki( punkt, liczbaNast, listaNast);
        else
            macierzPom[punkt].koniec = rozMacierzPom++;
    }
    macierzPomDoOryg();
}


void macierzPomDoOryg()
{
    for(int punkt = 0; punkt < rozmMacierz; punkt++)
    {
        int pocz = macierzPom[punkt].poczatek;
        int kon = macierzPom[punkt].koniec;
        if( pocz != -1 &&  kon != -1)
        {
            macierzOryg[pocz][kon] = macierzOryg[pocz][kon] + 1;
            rozmMacierzOryg = rozmMacierzOryg < pocz ? pocz : rozmMacierzOryg;
            rozmMacierzOryg = rozmMacierzOryg < kon  ? kon  : rozmMacierzOryg;
        }
    }
    rozmMacierzOryg++;
}


void drukujMacierzPom(int tryb)
{
    printf("**** Drukowanie macierzy pomocniczej. ****\n");
    printf("Rozmiar macierz=%d\n", rozmMacierz);
    printf("Rozmiar macierzy pomocniczej=%d\n", rozMacierzPom);

    for(int punkt = 0; punkt < rozmMacierz; punkt++)
        if(tryb == 1)
        {
            if(macierzPom[punkt].poczatek != -1 || macierzPom[punkt].koniec != -1)
                printf(" %d -> %c -> %d\n", macierzPom[punkt].poczatek,punkt + 'A', macierzPom[punkt].koniec);
        }
        else
            printf(" %c -> [%d,%d]\n",punkt + 'A', macierzPom[punkt].poczatek, macierzPom[punkt].koniec);

}


int ustawPoprzedniki(int pkt, int lbPop, int lsPop[])
{
    int wartElem = macierzPom[pkt].poczatek;

    for(int elem = 0; elem < lbPop; elem++)
    {
        int wartKol = macierzPom[lsPop[elem]].koniec;

        if(wartElem == -1 && wartKol != -1)
            wartElem = wartKol;
        else if(wartElem != -1 && wartKol != -1 && wartElem != wartKol)
        {
            printf("Niepoprawna wartosc wartElem=%d wartKol=%d indeks macierzPom(lsPop[elem])=%d.\n", wartElem, wartKol, lsPop[elem]);
            printf("Przerywam dzialanie programu\n");
            exit(1);
        }
    }

    // wypelnienie odpowiednia wartoscia tablicy elementow
    if(wartElem == -1)
    {
        wartElem = rozMacierzPom;
        rozMacierzPom++;
    }

    macierzPom[pkt].poczatek = wartElem;

    for(int elem = 0; elem < lbPop; elem++)
        macierzPom[lsPop[elem]].koniec = wartElem;

    return rozMacierzPom;
}


int ustawNastepniki(int pkt, int lbNa, int lsNa[])
{
    int wartElem = macierzPom[pkt].koniec;

    // szukaj wartosci nastepnika
    for(int elem = 0; elem < lbNa; elem++)
    {
        if(wartElem == -1 && macierzPom[lsNa[elem]].poczatek != -1)
            wartElem = macierzPom[lsNa[elem]].poczatek;
        else if(wartElem != -1 && macierzPom[lsNa[elem]].poczatek != -1 && wartElem != macierzPom[lsNa[elem]].poczatek)
        {
            printf("Niepoprawna wartosc wartElem=%d maOrygPo[lsNa[el]].poczatek=%d indeks macierzPom(listaNast[elem])=%d.\n", wartElem, macierzPom[lsNa[elem]].poczatek, lsNa[elem]);
            printf("Przerywam dzialanie programu.\n");
            exit(1);
        }
    }

    // wypelnienie odpowiednia wartoscia tablicy elementow
    if(wartElem == -1)
    {
        wartElem = rozMacierzPom;
        rozMacierzPom++;
    }

    macierzPom[pkt].koniec = wartElem;

    for(int elem = 0; elem < lbNa; elem++)
        macierzPom[lsNa[elem]].poczatek = wartElem;

    return rozMacierzPom;

}


int poprzedniki(int wierzcholek, int listaPoprz[])
{
    int poprzedni = 0;

    for(int i=0; i < rozmMacierz; i++)
        if( macierz[i][wierzcholek] > 0 )
        {
            listaPoprz[poprzedni] = i;
            poprzedni++;
        }
    return poprzedni;
}


int nastepniki(int wierzcholek, int listaNast[])
{
    int nastepny = 0;
    for(int i=0; i < rozmMacierz; i++)
        if( macierz[wierzcholek][i] > 0 )
        {
            listaNast[nastepny] = i;
            nastepny++;
        }

    return nastepny;
}


void drukTab(int tryb)
{
    printf("\nMacierz sprzezona%s:\n", (tryb == 1 ? " (drukowane tylko elemnty != 0)": ""));
    printf("rozmMacierz=%d\n", rozmMacierz);
    for(int w=0; w < rozmMacierz; w++)
        for (int k=0;  k < rozmMacierz; k++)
            if(tryb == 1)
            {
                if(macierz[w][k] != 0)
                    printf (" %c -> %c (%d)\n", w + 'A', k + 'A', macierz[w][k]);
            }
            else
                printf (" %c -> %c (%d)\n", w + 'A', k + 'A', macierz[w][k]);
}


void drukTabOryg(int tryb)
{
    printf("\nMacierz oryginalna%s:\n", (tryb == 1 ? " (drukowane tylko elemnty != 0)": ""));
    printf("rozmMacierzOryg=%d\n", rozmMacierzOryg);
    for(int w=0; w < rozmMacierzOryg; w++)
        for (int k=0;  k < rozmMacierzOryg; k++)
            if(tryb == 1)
            {
                if(macierzOryg[w][k] != 0)
                    printf (" %d -> %d (%d)\n", w , k, macierzOryg[w][k]);
            }
            else
                printf (" %d -> %d (%d)\n", w, k, macierzOryg[w][k]);
}
