#include <iostream>
#include <string>

#include "./include/object.h"

class Grass: public Object
{
    private:
    public:
        static constexpr char* typeName=(char*)"Grass";

        Grass(std::string n = "Grass");
        virtual ~Grass ();
};

class Tree: public Object
{
    private:

        static Array <PointXY*>* generateColMesh()
        {
          Array <PointXY*> *arr = new Array <PointXY*> (4);

            (*arr)[0]=new PointXY(0.5,0.5);
            (*arr)[1]=new PointXY(7.5,0.5);
            (*arr)[2]=new PointXY(7.5,7.5);
            (*arr)[3]=new PointXY(0.5,7.5);
            return arr;
        }

    public:
        static constexpr char* typeName=(char*)"Tree";

        Tree(std::string n = "Tree");
        virtual ~Tree ();
};
