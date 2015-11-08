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
void wpiszWierzcholki(int x, int k);
void drukTabOryg(int tryb);
int poprzedniki(int kolumna, int listaPoprz[]);
int nastepniki(int wiersz, int listaNast[]);
int znajdzPoprzKraw(int wiersz);
int znajdzNastKraw(int kolumna);

const char* nazwaPlikuZrodlowego = "test.csv";
const char* nazwaPlikuDocelowego = "testOryg.csv";
int main()
{
    odczytajZPliku(nazwaPlikuZrodlowego);
    czyJednograf();
    czySprzez();
    przenumTab();
    drukTab(1);
    generujGrafOryg();
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
        printf("Nie mo¿na otworzyæ pliku %s", nazwaPliku);
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
    char buf[500];
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
                    lx = 1;
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
    for (x = 0; x <rozmMacierz; x++)
        for (y = x + 1; y < rozmMacierz; y++)
            for (int z = 0; z < rozmMacierz; z ++)
                if (macierz[z][x] == 1 && macierz[z][y] == 1)
                    return 1;
    return 0;
}


void przenumTab()
{
    int n = 1;

    for(int w=0; w < rozmMacierz; w++)
    {
        for (int k=0;  k < rozmMacierz; k++)
        {
            if (macierz[w][k] == 1)
                macierz[w][k] = n++;
        }
    }
    rozmMacierzOryg = n-1;
}


void generujGrafOryg()
{
    for(int w=0; w < rozmMacierz; w++)

        for (int k=0;  k < rozmMacierz; k++)
            if(macierz[w][k] > 0)
                wpiszWierzcholki(w, k);

}

void wpiszWierzcholki(int w, int k)
{
    int listaPoprz[MAX_TAB];
    int listaNast[MAX_TAB];
    int liczbaPoprz = 0;
    int liczbaNast = 0;
    int wartosc = 0;

    liczbaPoprz = poprzedniki(w, listaPoprz);
    liczbaNast  = nastepniki(k, listaNast);

    wartosc = macierz[w][k];

    if(liczbaPoprz > 0)
        for(int i = 0; i < liczbaPoprz; i++)
            macierzOryg[listaPoprz[i]-1][wartosc-1] = 1;
    else
    {
        int kraw = znajdzPoprzKraw(w);
        if(kraw > 0 )
            macierzOryg[kraw-1][wartosc-1] = 1;
        else
        {
            macierzOryg[rozmMacierzOryg][wartosc-1] = 1;
            rozmMacierzOryg++;
        }
    }
    if(liczbaNast > 0)
        for(int i = 0; i < liczbaNast; i++)
            macierzOryg[wartosc-1][listaNast[i]-1] = 1;
    else
    {
        int kraw = znajdzNastKraw(k);
        if(kraw > 0)
            macierzOryg[wartosc-1][kraw-1] = 1;
        else
        {
            macierzOryg[wartosc-1][rozmMacierzOryg] = 1;
            rozmMacierzOryg++;

        }
    }
}


int znajdzNastKraw(int kolumna)
{
    for(int i=0; i < rozmMacierz; i++)
    {
        int kraw = macierz[i][kolumna];
        if( kraw > 0)
            for(int j = 0; j < rozmMacierzOryg; j++)
            {
                if( macierzOryg[kraw - 1][j] > 0)
                    return j + 1;
            }
    }
    return 0;
}

int znajdzPoprzKraw(int wiersz)
{
    for(int i=0; i < rozmMacierz; i++)
    {
        int kraw = macierz[wiersz][i];
        if( kraw > 0)
            for(int j = 0; j < rozmMacierzOryg; j++)
            {
                if( macierzOryg[j][kraw - 1] > 0)
                    return j + 1;
            }
    }
    return 0;
}



int poprzedniki(int kolumna, int listaPoprz[])
{
    int poprzedni = 0;

    for(int i=0; i < rozmMacierz; i++)
        if( macierz[i][kolumna] > 0 )
        {
            listaPoprz[poprzedni] = macierz[i][kolumna];
            poprzedni++;
        }
    return poprzedni;
}


int nastepniki(int wiersz, int listaNast[])
{
    int nastepny = 0;
    for(int i=0; i < rozmMacierz; i++)
        if( macierz[wiersz][i] > 0 )
        {
            listaNast[nastepny] = macierz[wiersz][i];
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
                    printf ("macierz[%d][%d]=%d\n", w, k, macierz[w][k]);
            }
            else
                printf ("macierz[%d][%d]=%d\n", w, k, macierz[w][k]);
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
                    printf ("macierzOryg[%d][%d]=%d\n", w, k, macierzOryg[w][k]);
            }
            else
                printf ("macierzOryg[%d][%d]=%d\n", w, k, macierzOryg[w][k]);
}
