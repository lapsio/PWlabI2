#ifndef VECTORXY_H
#define VECTORXY_H

#include "./include/defines.h"

#include <iostream>
#include <math.h>
#include "PointXY.h"

/**
 * @brief The VectorXY class Class providing basic vector functionality
 */
class VectorXY
{
    private:
        PointXY Beg; /**** Punkt poczatku wektora ****/ /**< vector beginning */
        PointXY End; /**** Punkt konca wektora ****/ /**< vector end */
    public:

        /**
         * @brief VectorXY basic constructor
         * @param xBeg x begin
         * @param yBeg y begin
         * @param xEnd x end
         * @param yEnd y end
         */
        VectorXY(long double xBeg = 0,long double yBeg = 0,long double xEnd = 1,long double yEnd = 1); /**** Konstruktor z podanymi wartosciami dla poczatku i konca ****/
        /**
         * @brief VectorXY constructor from points
         * @param first begin
         * @param last end
         */
        VectorXY(const PointXY& first, const PointXY& last);                                           /**** Konstruktor z podanymi dwoma punktami (class PointXY) ****/
        /**
         * @brief VectorXY copy constructor
         * @param tmp reference
         */
        VectorXY(const VectorXY& tmp);                                                        /**** Konstruktor kopiujacy wartosci z podanego wektora ****/
        ~VectorXY();

        /**
         * @brief getBegin
         * @return begin of vector
         */
        inline const PointXY& getBegin() const {return this->Beg;} /**** Kopiuje punkt poczatkowy ****/
        /**
         * @brief getEnd
         * @return end of vector
         */
        inline const PointXY& getEnd() const {return this->End;} /**** Kopiuje punkt koncowy ****/

        /**
         * @brief setBegin set beginning point of vector to specified value
         * @param A new value
         */
        void setBegin(const PointXY& A);                          /**** Ustawia poczatek zgodny z podanym punktem ****/
        void setBegin(long double a, long double b);        /**** Ustawia poczatek o podanych wspolrzednych ****/
        /**
         * @brief setEnd set endinf point of vector to specified value
         * @param A new value
         */
        void setEnd(const PointXY& A);                            /**** Ustawia koniec zgodny z podanym punktem ****/
        void setEnd(long double a, long double b);          /**** Ustawia koniec o podanych wspolrzednych ****/
        /**
         * @brief setVector set vector to new one
         * @param beg new beginning
         * @param end new ending
         */
        void setVector(const PointXY& beg, const PointXY& end);          /**** Ustawia vector dla podanych punktow ****/

        /**
         * @brief height returns height of vector (Y axis length)
         * @return height of vector
         */
        long double height() const;   /**** Zwraca wysokosc  ****/
        /**
         * @brief width returns width of vector (X axis length)
         * @return width of vector
         */
        long double width() const;     /**** Zwraca szerokosc  ****/
        /**
         * @brief sizeXY returns pair of width:heigth as Point class
         * @return xy size
         */
        PointXY sizeXY() const;   /**** zwraca wysokosc i dlugosc czyli punkt koncowy ****/
        /**
         * @brief size Euclidean length of vector
         * @return length
         */
        long double size() const;      /**** Liczy dlugosc wektora ****/

        /**
         * @brief moveTo move vector to specified position
         * @param x desired x pos
         * @param y desired y pos
         * @param end flag indicating which point of vector should be aligned
         */
        void moveTo (long double x, long double y, bool end = false); /**** Przesuwa koniec wektora lub poczatek wektora do podanego punktu zachowujac dlugosc  ****/
        /**
         * @brief moveBy move vector by specified offset
         * @param x x offset
         * @param y y offset
         */
        void moveBy (long double x, long double y);             /**** Przesuwa koniec wektora i poczatek wektora o podane wartosci  ****/
        void moveBy (PointXY& point);                           /**** Przesuwa koniec wektora i poczatek wektora o podany punkt(wektor o poczatku (0,0) ****/

        /**
         * @brief centerOn center vector on specified point
         * @param x desired x
         * @param y desired y
         */
        void centerOn (long double x, long double y);           /**** Przesuwa srodek wektora do podanych wspolrzednych zachowujac dlugosc  ****/
        void centerOn (PointXY& point);                         /**** Przesuwa srodek wektora do podanego punktu zachowujac dlugosc  ****/

        /**
         * @brief scaleBy scale vector by specified fraction (from point 0,0)
         * @param k fraction
         */
        void scaleBy(long double k);                                /**** Skaluje wektor o k, czyli mnozymy punkt poczatku i konca przez k  ****/
        /**
         * @brief scaleTo scale vector to specified Euclidean length (from point 0,0)
         * @param siz desired size
         */
        void scaleTo(long double siz);                              /**** Skaluje wektor w taki sposob zeby dlugosc sie zgadzala z podana ****/
        /**
         * @brief scaleLengthTo scale vector by specified fraction (from beginning point)
         * @param k
         * @param end
         */
        void scaleLengthTo(long double k , bool end = false);       /**** Skaluje wektor w taki sposob zeby dlugosc sie zgadzala z podana i poczatek lub koniec pozostal bez zmian ****/
        /**
         * @brief scaleLengthBy scale vector to specified Euclidean length (from point 0,0)
         * @param k
         * @param end
         */
        void scaleLengthBy(long double k , bool end = false);       /**** Skaluje wektor o k, czyli mnozymy punkt poczatku i konca przez k i poczatek lub koniec pozostaje bez zmian ****/

        /**
         * @brief flip swap beginning point with ending one
         */
        void flip();

        /**
         * @brief intersects indicates whether 2 vectors intersect or not
         * @param A vector A
         * @param B vector B
         * @return whether A intersects with B
         */
        static bool intersects(const VectorXY& A,const  VectorXY& B);                          /**** Sprawdza czy dwa wektory sie przecinaja ****/
        /**
         * @brief scalarProduct scalar product of 2 detached vector
         * @param a point A
         * @param b commont point of 2 vectors
         * @param c point B
         * @return
         */
        static long double scalarProduct(PointXY * a, PointXY * b, PointXY * c);

        /**
         * @brief flipAcrossVector flip one vector across another
         * @param A vector to be flipped
         * @param B vector to flip against
         */
        static void flipAcrossVector(VectorXY& A, VectorXY& B);             /**** Symetria wektora wzgledem wektora, czyli odbija punkt konca i poczatku wzgledem wektora ****/
        /**
         * @brief flipAcrossVector flip point actoss vector
         * @param pt point
         * @param vec vector
         */
        static void flipAcrossVector(PointXY& pt, VectorXY& vec);             /**** Symetria punktu wzgledem wektora ****/

        /**
         * @brief projectOntoVector projectile point onto vector
         * @param pt point
         * @param vec vector
         */
        static void projectOntoVector(PointXY& pt, VectorXY& vec);              /**** Rzutowanie punktu na wektor ****/
        /**
         * @brief projectOntoVector projectile vector onto another vector
         * @param A vector to be projected
         * @param B vector to be projected on
         */
        static void projectOntoVector(VectorXY& A, VectorXY& B);               /**** Rzutowanie wektora na wektor ****/

        VectorXY operator +(VectorXY vect) const;
        VectorXY operator -(VectorXY vect) const;
        void operator +=(VectorXY vect);
        void operator -=(VectorXY vect);
        VectorXY operator *(double k) const; /**** Operator dla mnozenia (X,Y) * k ****/
        void operator *=(double k); /**** Operator dla mnozenia (X,Y) * k ****/
        bool operator ==(const VectorXY& vect) const;
        bool operator !=(const VectorXY& vect) const;

/*****************************************************************************************/
/*************Funkcje zaprzyjaznione przeciazenia operatorow poza klasa******************/
/***************************************************************************************/

        friend VectorXY operator*( const double & k,const VectorXY & vect ); // Operator dla mnozenia k * (X,Y)
#ifdef _DEBUG
        void show();
#endif
};


#endif // VECTORXY_H
