#include <iostream>
#include "./include/PointXY.h"

using namespace std;

/*************************************************************/
/********************** Konstruktory ************************/
/***********************************************************/

PointXY::PointXY(long double X, long double Y)
{
    this->X = X;
    this->Y = Y;
}

PointXY::PointXY(const PointXY& tmp)
{
    this -> X = tmp.X;
    this -> Y = tmp.Y;
}

PointXY::~PointXY()
{
    //dtor
}

void PointXY::changeTo (long double X, long double Y) /**** Zmienia wartosc X i Y klasy do ... ****/
{
    this->X = X;
    this->Y = Y;
}

void PointXY::changeTo (const PointXY& tmp) /**** Zmienia wartosc X i Y klasy do wartosci z klasy podanej ****/
{
    this->X = tmp.X;
    this->Y = tmp.Y;
}

void PointXY::moveBy(long double X, long double Y) /**** Przesuwa punkt o podane wartosci ****/
{
    this->X = this->X + X;
    this->Y = this->Y + Y;
}

void PointXY::moveBy(const PointXY& tmp) /**** Przesuwa punkt o wartosc X i Y z klasy podanej ****/
{
    this->X = this->X + tmp.X;
    this->Y = this->Y + tmp.Y;
}

/*************************************************************/
/************************ OPERATORY *************************/
/***********************************************************/

PointXY PointXY::operator+(PointXY point) const /***DODAJE PUNKTY***/
{
    PointXY tmp;
    tmp.X = X + point.X;
    tmp.Y = Y + point.Y;

    return tmp;
}

PointXY PointXY::operator-(PointXY point) const /***ODEJMUJE PUNKTY***/
{
    PointXY tmp;
    tmp.X = X - point.X;
    tmp.Y = Y - point.Y;

    return tmp;
}

void PointXY::operator+=(PointXY point)/***DODAJE PUNKTY i PRZYPISUJE WYNIK DODAWANIA***/
{
    this -> changeTo(this -> X + point.X, this -> Y + point.Y);
    return;
}

void PointXY::operator-=(PointXY point) /***ODEJMUJE PUNKTY i PRZYPISUJE WYNIK ODEJMOWANIA***/
{
    this -> changeTo(this -> X - point.X, this -> Y - point.Y);
    return;
}

PointXY PointXY:: operator*( double k ) const /***MNOZENIE PRZEZ SKALAR POSTACI (X,Y)*k ***/
{
    PointXY tmp;
    tmp.X = this -> X * k;
    tmp.Y = this -> Y * k;

    return tmp;
}

void PointXY:: operator*=( double k ) /***MNOZENIE PRZEZ SKALAR POSTACI (X,Y)*k i PRZYPISUJE WYNIK MNOZENIA***/
{
    long double m, l;

    m = this -> X * k;
    l = this -> Y * k;

    this -> changeTo(m,l);

    return;
}

bool PointXY:: operator==(const PointXY& point) const /***POROWNANIE DWOCH PUNKTOW == ***/
{
    if (this -> X != point.X)
        return false;
    if (this -> Y != point.Y)
        return false;

    return true;
}

bool PointXY:: operator!=(const PointXY& point) const /***POROWNANIE DWOCH PUNKTOW != ***/
{
    if ((this -> X == point.X) && (this -> Y == point.Y))
        return false;

    return true;
}


PointXY operator*( const double & k,const PointXY & point ) /***MNOZENIE PRZEZ SKALAR POSTACI k*(X,Y) ***/
{
    PointXY tmp;
    tmp.X = point.X * k;
    tmp.Y = point.Y * k;

    return tmp;
}


void PointXY:: show()
{
    cout << endl << "(" << this -> X <<","<< this -> Y << ")" << endl << endl;
}
