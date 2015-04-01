#include "./include/VectorXY.h"

using namespace std;

/*************************************************************/
/********************** Konstruktory ************************/
/***********************************************************/

VectorXY::VectorXY(long double xBeg,long double yBeg,long double xEnd,long double yEnd)
{
    this -> Beg.changeTo(xBeg, yBeg);
    this -> End.changeTo(xEnd, yEnd);
}

VectorXY::VectorXY(const PointXY& first,const PointXY& last)
{
    this -> Beg.changeTo(first);
    this -> End.changeTo(last);
}

VectorXY::VectorXY(const VectorXY& tmp)
{
    this -> Beg.changeTo(tmp.Beg);
    this -> End.changeTo(tmp.End);
}

VectorXY::~VectorXY()
{
    //dtor
}


/*************************************************************/
/************************** Metody **************************/
/***********************************************************/

PointXY VectorXY::getBegin()        /**** Kopiuje punkt poczatkowy ****/
{
    return Beg;
}

PointXY VectorXY::getEnd()          /**** Kopiuje punkt koncowy ****/
{
    return End;
}

void VectorXY::setBegin(const PointXY& A)         /**** Ustawia poczatek zgodny z podanym punktem ****/
{
    this -> Beg.changeTo(A);
}

void VectorXY::setBegin(long double a, long double b)           /**** Ustawia poczatek o podanych wspolrzednych ****/
{
    this -> Beg.changeTo(a,b);
}
void VectorXY::setEnd(const PointXY& A)           /**** Ustawia koniec zgodny z podanym punktem ****/
{
    this -> End.changeTo(A);
}

void VectorXY::setEnd(long double a, long double b)         /**** Ustawia koniec o podanych wspolrzednych ****/
{
    this -> End.changeTo(a,b);
}

void VectorXY:: setVector(const PointXY& beg, const PointXY& end)
{
    this -> setBegin(beg);
    this -> setEnd(end);

    return;
}

long double VectorXY::size()            /**** Liczy dlugosc wektora ****/
{
    long double tmp1;
    PointXY tmp2;

    tmp2 = this -> End - this -> Beg;

    tmp1 = sqrt (tmp2.getX() * tmp2.getX() + tmp2.getY() * tmp2.getY());

    return tmp1;
}

long double VectorXY::height()          /**** Zwraca wysokosc  ****/
{
    long double tmp;

    tmp = this -> End.getY() - this -> Beg.getY();

    return tmp;
}

long double VectorXY::width()           /**** Zwraca szerokosc  ****/
{
    long double tmp;

    tmp = this -> End.getX() - this -> Beg.getX();

    return tmp;
}

PointXY VectorXY::sizeXY()          /**** zwraca wysokosc i dlugosc czyli punkt koncowy - punkt poczatkowy ****/
{
    long double a, b;

    a = width();
    b = height();

    PointXY tmp (a , b);

    return tmp;;
}

void VectorXY:: moveTo (long double x, long double y, bool end)         /**** Przesuwa koniec wektora lub poczatek wektora do podanego punktu zachowujac dlugosc  ****/
{
    long double k, l;
    if (end != true)
    {
        k = this -> End.getX() - (this -> Beg.getX() - x);
        l = this -> End.getY() - (this -> Beg.getY() - y);
        this -> Beg.changeTo(x,y);
        this -> End.changeTo(k,l);
    }
    else
    {
        k = this -> Beg.getX() - (this -> End.getX() - x);
        l = this -> Beg.getY() - (this -> End.getY() - y);
        this -> End.changeTo(x,y);
        this -> Beg.changeTo(k,l);
    }

    return;
}

void VectorXY:: moveBy (long double x, long double y)           /**** Przesuwa koniec wektora i poczatek wektora o podane wartosci  ****/
{
    long double k, l;
    k = this -> Beg.getX() + x;
    l = this -> Beg.getY() + y;
    this -> Beg.changeTo(k,l);

    k = this -> End.getX() + x;
    l = this -> End.getY() + y;
    this -> End.changeTo(k,l);

    return;
}

void VectorXY:: moveBy (PointXY& point)             /**** Przesuwa koniec wektora i poczatek wektora o podany punkt(wektor o poczatku (0,0)) ****/
{
    long double k, l;
    k = this -> Beg.getX() + point.getX();
    l = this -> Beg.getY() + point.getY();
    this -> Beg.changeTo(k,l);

    k = this -> End.getX() + point.getX();
    l = this -> End.getY() + point.getY();
    this -> End.changeTo(k,l);

    return;
}

void VectorXY:: centerOn (long double x, long double y)         /**** Przesuwa srodek wektora do podanych wspolrzednych zachowujac dlugosc  ****/
{
    long double k, l;
    k = End.getX()-(End.getX() + Beg.getX())/2;
    l = End.getY()-(End.getY() + Beg.getY())/2;

    moveTo(x,y);
    moveBy(-k, -l);

    return;
}

void VectorXY:: centerOn (PointXY& point)               /**** Przesuwa srodek wektora do podanego punktu zachowujac dlugosc  ****/
{
    long double k, l;
    k = point.getX();
    l = point.getY();

    centerOn(k, l);

    return;
}

void VectorXY:: scaleBy (long double k)             /**** Skaluje wektor o k, czyli mnozymy punkt poczatku i konca przez k  ****/
{
    long double a , b;

    a = this -> Beg.getX() * k;
    b = this -> Beg.getY() * k;

    this -> Beg.changeTo(a,b);

    a = this -> End.getX() * k;
    b = this -> End.getY() * k;

    this -> End.changeTo(a,b);

    return;
}

void VectorXY:: scaleTo (long double siz)       /**** Skaluje wektor w taki sposob zeby dlugosc sie zgadzala z podana ****/
{
    long double k;
    k = siz / this -> size();

    this -> scaleBy(k);

    return;
}

void VectorXY:: scaleLengthTo(long double k , bool end)         /**** Skaluje wektor w taki sposob zeby dlugosc sie zgadzala z podana i poczatek lub koniec pozostal bez zmian ****/
{
    PointXY tmp;
    if (end != true)
    {
        tmp = this -> getBegin();
        scaleTo(k);
        moveTo(tmp.getX() , tmp.getY());
    }
    else
    {
        tmp = this -> getEnd();
        scaleTo(k);
        moveTo(tmp.getX() , tmp.getY() , true);
    }
    return;
}

void VectorXY:: scaleLengthBy(long double k , bool end) //poprawic
{
    long double a , b;
    PointXY tmp;

    if (end != true)
    {
        tmp = this -> getBegin();
        a = this -> End.getX() *k;
        b = this -> End.getY() *k;
        this -> End.changeTo(a,b);
    }
    else
    {
        a = this -> Beg.getX() *k;
        b = this -> Beg.getY() *k;
        this -> Beg.changeTo(a,b);
    }
    return;
}

void VectorXY:: flip()
{
    PointXY tmp1, tmp2;

    tmp1 = this -> getBegin();
    tmp2 = this -> getEnd();
    this -> setVector(tmp1,tmp2);

    return;
}

long double VectorXY:: scalarProduct(PointXY * a, PointXY * b, PointXY * c)
{
  return (b->getX() - a->getX())*(c->getY() - a->getY()) - (c->getX() - a->getX())*(b->getY() - a->getY());
}


bool VectorXY:: intersects(const VectorXY& A,const  VectorXY& B)   /**** Sprawdza czy dwa wektory sie przecinaja ****/
{

    long double S1, S2, S3, S4;
    PointXY a1, a2, a3;
    a1 = A.Beg;
    a2 = B.Beg;
    a3 = A.End;
    S1 = scalarProduct(&a1, &a2, &a3);

    a1 = A.Beg;
    a2 = B.End;
    a3 = A.End;
    S2 = scalarProduct(&a1, &a2, &a3);

    a1 = A.Beg;
    a2 = B.Beg;
    a3 = B.End;
    S3 = scalarProduct(&a2, &a1, &a3);

    a1 = B.End;
    a2 = B.Beg;
    a3 = A.End;
    S4 = scalarProduct(&a2, &a3, &a1);

    if (((S1>0 && S2<0) || (S1<0 && S2>0)) && ((S3<0 && S4>0) || (S3>0 && S4<0)))
        return true;
    else
        return false;

}


void VectorXY:: flipAcrossVector(PointXY& pt, VectorXY& vec)  /**** Symetria punktu wzgledem wektora ****/
{

  PointXY CenterPt;
  PointXY tmp1;
  PointXY tmp2;

  tmp2 = vec.getBegin();
  tmp1 = tmp2 * (-1);


  vec.moveTo(0,0);

  pt.moveBy(tmp1);


  //(x,-iy)(a,ib)²/(a²+b²) = (xa² - xb² + 2yab) + i(yb² - ya² + 2xab)


  long double nx = (pt.getX() * vec.End.getX() * vec.End.getX()) - (pt.getX() * vec.End.getY() * vec.End.getY()) + (2 * pt.getY() * vec.End.getX() * vec.End.getY());

  nx = nx / ((vec.End.getX() * vec.End.getX()) + (vec.End.getY() * vec.End.getY()));


  long double ny = (pt.getY() * vec.End.getY() * vec.End.getY()) - (pt.getY() * vec.End.getX() * vec.End.getX()) + (2 * pt.getX() * vec.End.getX() * vec.End.getY());

  ny = ny / ( (vec.End.getX() * vec.End.getX()) + (vec.End.getY() * vec.End.getY()));


  pt.changeTo(nx ,ny);


  vec.moveBy(tmp2);//move back

  pt.moveBy(tmp2);

  return;

}

void VectorXY:: flipAcrossVector(VectorXY& A,VectorXY& B)      /**** Symetria wektora wzgledem wektora, czyli odbija punkt konca i poczatku wzgledem wektora ****/
{

    VectorXY:: flipAcrossVector(A.Beg, B);

    VectorXY:: flipAcrossVector(A.End, B);

    return;

}


void VectorXY:: projectOntoVector(PointXY& pt, VectorXY& vec)
{

    long double k, l;

    k = pt.getX();
    l = pt.getY();
    VectorXY:: flipAcrossVector(pt,vec);

    k = k + (pt.getX() - k)/2;

    l = l + (pt.getY() - l)/2;

    pt.changeTo(k,l);

    return;

}



void VectorXY:: projectOntoVector(VectorXY& A, VectorXY& B)
{

    PointXY C,D;

    C = A.getBegin();
    D = A.getEnd();

    VectorXY:: projectOntoVector(C , B);
    VectorXY:: projectOntoVector(D , B);

    A.setVector(C,D);

    return ;
}

/*************************************************************/
/************************ OPERATORY *************************/
/***********************************************************/

VectorXY VectorXY::operator+(VectorXY vect) /***DODAJE PUNKTY***/
{
    VectorXY tmp;
    PointXY point1;
    PointXY point2;
    PointXY point3;

    point1.changeTo(this -> height(), this -> width());
    point2.changeTo(vect.height(), vect.width());
    point3 = point1 + point2;

    tmp.setEnd(point3);

    tmp.moveTo(this -> Beg.getX(), this -> Beg.getY());

    return tmp;
}

VectorXY VectorXY::operator-(VectorXY vect) /***ODEJMUJE PUNKTY***/
{
    VectorXY tmp;
    PointXY point1;
    PointXY point2;
    PointXY point3;

    point1.changeTo(this -> height(), this -> width());
    point2.changeTo(vect.height(), vect.width());
    point3 = point1 - point2;

    tmp.setEnd(point3);

    tmp.moveTo(this -> Beg.getX(), this -> Beg.getY());

    return tmp;
}

void VectorXY::operator+=(VectorXY vect) /***DODAJE PUNKTY i PRZYPISUJE WYNIK DODAWANIA***/
{
    VectorXY tmp;
    PointXY point1;
    PointXY point2;
    PointXY point3;
    long double a, b;

    a = this -> Beg.getX();
    b = this -> Beg.getY();

    point1.changeTo(this -> height(), this -> width());
    point2.changeTo(vect.height(), vect.width());
    point3 = point1 + point2;

    this -> setEnd(point3);
    this -> setBegin(0,0);

    this -> moveTo(a, b);

    return;
}

void VectorXY::operator-=(VectorXY vect) /***ODEJMUJE PUNKTY i PRZYPISUJE WYNIK ODEJMOWANIA***/
{
    VectorXY tmp;
    PointXY point1;
    PointXY point2;
    PointXY point3;
    long double a, b;

    a = this -> Beg.getX();
    b = this -> Beg.getY();

    point1.changeTo(this -> height(), this -> width());
    point2.changeTo(vect.height(), vect.width());
    point3 = point1 - point2;

    this -> setEnd(point3);
    this -> setBegin(0,0);

    this -> moveTo(a, b);

    return;
}

VectorXY VectorXY:: operator*( double k ) /***MNOZENIE PRZEZ SKALAR POSTACI (X,Y)*k ***/
{
    VectorXY tmp;

    PointXY point1;
    PointXY point2;

    point1 = this -> getBegin() * k;
    point2 = this -> getEnd() * k;

    tmp.setBegin(point1);
    tmp.setEnd(point2);

    return tmp;
}

void VectorXY:: operator*=( double k ) /***MNOZENIE PRZEZ SKALAR POSTACI (X,Y)*k i PRZYPISUJE WYNIK MNOZENIA***/
{
    VectorXY tmp;

    PointXY point1;
    PointXY point2;

    point1 = this -> getBegin() * k;
    point2 = this -> getEnd() * k;

    this -> setBegin(point1);
    this -> setEnd(point2);

    return;
}

bool VectorXY:: operator==(const VectorXY& vect) /***POROWNANIE DWOCH PUNKTOW == ***/
{
    if ((this -> getBegin() == vect.Beg) && (this -> getEnd() == vect.End))
        return true;
    else
        return false;
}

bool VectorXY:: operator!=(const VectorXY& vect) /***POROWNANIE DWOCH PUNKTOW != ***/
{
    if ((this -> getBegin() != vect.Beg) || (this -> getEnd() != vect.End))
        return true;
    else
        return false;
}


VectorXY operator*( const double & k, const VectorXY & vect ) /***MNOZENIE PRZEZ SKALAR POSTACI k*(X,Y) ***/
{
    VectorXY tmp;

    PointXY point1;
    PointXY point2;

    point1 = k*vect.Beg;
    point2 = k*vect.End;

    tmp.setBegin(point1);
    tmp.setEnd(point2);

    return tmp;
}

void VectorXY::show()
{
    cout << "Wektor [ (" << Beg.getX() << "," << Beg.getY() << ") , (" << End.getX() << "," << End.getY() << ") ]" << endl << endl;
}

