#ifndef POINTXY_H
#define POINTXY_H


class PointXY
{
    private:

        long double X;
        long double Y;

    public:

        PointXY(long double X = 0, long double Y = 0); /**** Konstruktor z podanymi wartosciami ****/
        PointXY(const PointXY& tmp); /**** Konstruktor jako kopia podanego punktu ****/
        ~PointXY();

        void changeTo (long double X, long double Y); /**** Zmienia wartosc X i Y klasy do ... ****/
        void changeTo (const PointXY& tmp);                 /**** Zmienia wartosc X i Y klasy do wartosci z klasy podanej ****/

        void moveBy (long double X, long double Y); /**** Przesuwa punkt o podane wartosci ****/
        void moveBy (const PointXY& tmp);                 /**** Przesuwa punkt o wartosc X i Y z klasy podanej ****/

        long double getX(); /**** Zwraca wartość X klasy ****/
        long double getY(); /**** Zwraca wartość Y klasy ****/

        PointXY operator +(PointXY point);
        PointXY operator -(PointXY point);
        void operator +=(PointXY point);
        void operator -=(PointXY point);
        PointXY operator *(double k); /**** Operator dla mnozenia (X,Y) * k ****/
        void operator *=(double k); /**** Operator dla mnozenia (X,Y) * k ****/
        bool operator ==(const PointXY& point);
        bool operator !=(const PointXY& point);


/*****************************************************************************************/
/*************Funkcje zaprzyjaznione przeciazenia operatorow poza klasa******************/
/***************************************************************************************/

        friend PointXY operator*( const double & k,const PointXY & point ); // Operator dla mnozenia k * (X,Y)

        void show ();
};



#endif // POINTXY_H