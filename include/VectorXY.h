#ifndef VECTORXY_H
#define VECTORXY_H

#include <iostream>
#include <math.h>
#include "PointXY.h"


class VectorXY
{
    private:
        PointXY Beg; /**** Punkt poczatku wektora ****/
        PointXY End; /**** Punkt konca wektora ****/
    public:

        VectorXY(long double xBeg = 0,long double yBeg = 0,long double xEnd = 1,long double yEnd = 1); /**** Konstruktor z podanymi wartosciami dla poczatku i konca ****/
        VectorXY(const PointXY& first, const PointXY& last);                                           /**** Konstruktor z podanymi dwoma punktami (class PointXY) ****/
        VectorXY(const VectorXY& tmp);                                                        /**** Konstruktor kopiujacy wartosci z podanego wektora ****/
        ~VectorXY();

        PointXY getBegin(); /**** Kopiuje punkt poczatkowy ****/
        PointXY getEnd(); /**** Kopiuje punkt koncowy ****/

        void setBegin(const PointXY& A);                          /**** Ustawia poczatek zgodny z podanym punktem ****/
        void setBegin(long double a, long double b);        /**** Ustawia poczatek o podanych wspolrzednych ****/
        void setEnd(const PointXY& A);                            /**** Ustawia koniec zgodny z podanym punktem ****/
        void setEnd(long double a, long double b);          /**** Ustawia koniec o podanych wspolrzednych ****/
        void setVector(const PointXY& beg, const PointXY& end);          /**** Ustawia vector dla podanych punktow ****/

        long double height();   /**** Zwraca wysokosc  ****/
        long double width();     /**** Zwraca szerokosc  ****/
        PointXY sizeXY();   /**** zwraca wysokosc i dlugosc czyli punkt koncowy ****/
        long double size();      /**** Liczy dlugosc wektora ****/

        void moveTo (long double x, long double y, bool end = false); /**** Przesuwa koniec wektora lub poczatek wektora do podanego punktu zachowujac dlugosc  ****/
        void moveBy (long double x, long double y);             /**** Przesuwa koniec wektora i poczatek wektora o podane wartosci  ****/
        void moveBy (PointXY& point);                           /**** Przesuwa koniec wektora i poczatek wektora o podany punkt(wektor o poczatku (0,0) ****/

        void centerOn (long double x, long double y);           /**** Przesuwa srodek wektora do podanych wspolrzednych zachowujac dlugosc  ****/
        void centerOn (PointXY& point);                         /**** Przesuwa srodek wektora do podanego punktu zachowujac dlugosc  ****/

        void scaleBy(long double k);                                /**** Skaluje wektor o k, czyli mnozymy punkt poczatku i konca przez k  ****/
        void scaleTo(long double siz);                              /**** Skaluje wektor w taki sposob zeby dlugosc sie zgadzala z podana ****/
        void scaleLengthTo(long double k , bool end = false);       /**** Skaluje wektor w taki sposob zeby dlugosc sie zgadzala z podana i poczatek lub koniec pozostal bez zmian ****/
        void scaleLengthBy(long double k , bool end = false);       /**** Skaluje wektor o k, czyli mnozymy punkt poczatku i konca przez k i poczatek lub koniec pozostaje bez zmian ****/

        void flip();

        static bool intersects(const VectorXY& A,const  VectorXY& B);                          /**** Sprawdza czy dwa wektory sie przecinaja ****/
        static long double scalarProduct(PointXY * a, PointXY * b, PointXY * c);

        static void flipAcrossVector(VectorXY& A, VectorXY& B);             /**** Symetria wektora wzgledem wektora, czyli odbija punkt konca i poczatku wzgledem wektora ****/
        static void flipAcrossVector(PointXY& pt, VectorXY& vec);             /**** Symetria punktu wzgledem wektora ****/

        static void projectOntoVector(PointXY& pt, VectorXY& vec);              /**** Rzutowanie punktu na wektor ****/
        static void projectOntoVector(VectorXY& A, VectorXY& B);               /**** Rzutowanie wektora na wektor ****/

        VectorXY operator +(VectorXY vect);
        VectorXY operator -(VectorXY vect);
        void operator +=(VectorXY vect);
        void operator -=(VectorXY vect);
        VectorXY operator *(double k); /**** Operator dla mnozenia (X,Y) * k ****/
        void operator *=(double k); /**** Operator dla mnozenia (X,Y) * k ****/
        bool operator ==(const VectorXY& vect);
        bool operator !=(const VectorXY& vect);

/*****************************************************************************************/
/*************Funkcje zaprzyjaznione przeciazenia operatorow poza klasa******************/
/***************************************************************************************/

        friend VectorXY operator*( const double & k,const VectorXY & vect ); // Operator dla mnozenia k * (X,Y)

        void show();
};


#endif // VECTORXY_H
