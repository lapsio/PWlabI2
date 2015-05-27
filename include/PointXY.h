#ifndef POINTXY_H
#define POINTXY_H

#include "./include/defines.h"

class PointXY
{
    public:

        long double X;
        long double Y;

        PointXY(long double X = 0, long double Y = 0); /**** Konstruktor z podanymi wartosciami ****/
        PointXY(const PointXY& tmp); /**** Konstruktor jako kopia podanego punktu ****/
        ~PointXY();

        void changeTo (long double X, long double Y); /**** Zmienia wartosc X i Y klasy do ... ****/
        void changeTo (const PointXY& tmp);                 /**** Zmienia wartosc X i Y klasy do wartosci z klasy podanej ****/

        void moveBy (long double X, long double Y); /**** Przesuwa punkt o podane wartosci ****/
        void moveBy (const PointXY& tmp);                 /**** Przesuwa punkt o wartosc X i Y z klasy podanej ****/

        inline long double getX() const {return this->X;} /**** Zwraca wartość X klasy ****/
        inline long double getY() const {return this->Y;} /**** Zwraca wartość Y klasy ****/

        PointXY operator +(const PointXY point) const;
        PointXY operator -(const PointXY point) const;
        void operator +=(const PointXY point);
        void operator -=(const PointXY point);
        PointXY operator *(double k) const; /**** Operator dla mnozenia (X,Y) * k ****/
        void operator *=(double k); /**** Operator dla mnozenia (X,Y) * k ****/
        bool operator ==(const PointXY& point) const;
        bool operator !=(const PointXY& point) const;


/*****************************************************************************************/
/*************Funkcje zaprzyjaznione przeciazenia operatorow poza klasa******************/
/***************************************************************************************/

        friend PointXY operator*( const double & k,const PointXY & point ); // Operator dla mnozenia k * (X,Y)
#ifdef _DEBUG
        void show () const;
#endif
};



#endif // POINTXY_H
