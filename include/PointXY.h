#ifndef POINTXY_H
#define POINTXY_H

#include "./include/defines.h"

/**
 * @brief The PointXY class class providing basic point functionality. Can be used as pair of numbers as well
 */

class PointXY
{
    public:

        long double X; /**< point x */
        long double Y; /**< point y */

        /**
         * @brief PointXY basic constructor
         * @param X x
         * @param Y y
         */
        PointXY(long double X = 0, long double Y = 0); /**** Konstruktor z podanymi wartosciami ****/
        /**
         * @brief PointXY copy constructor
         * @param tmp
         */
        PointXY(const PointXY& tmp); /**** Konstruktor jako kopia podanego punktu ****/
        ~PointXY();

        /**
         * @brief changeTo change point parameters
         * @param X
         * @param Y
         */
        void changeTo (long double X, long double Y); /**** Zmienia wartosc X i Y klasy do ... ****/
        /**
         * @brief changeTo copy parameters from other point to this one
         * @param tmp reference point
         */
        void changeTo (const PointXY& tmp);                 /**** Zmienia wartosc X i Y klasy do wartosci z klasy podanej ****/

        /**
         * @brief moveBy move point by specified offset
         * @param X x offset
         * @param Y y offset
         */
        void moveBy (long double X, long double Y); /**** Przesuwa punkt o podane wartosci ****/
        /**
         * @brief moveBy move point by offset specified as point
         * @param tmp
         */
        void moveBy (const PointXY& tmp);                 /**** Przesuwa punkt o wartosc X i Y z klasy podanej ****/

        /**
         * @brief getX return x coordinate
         * @return x pos
         */
        inline long double getX() const {return this->X;} /**** Zwraca wartość X klasy ****/
        /**
         * @brief getY return y coordinate
         * @return y pos
         */
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
