#include <iostream>
#include <string>

#include "./include/object.h"

#ifndef Grass_H
#define Grass_H

/**
 * @brief  The Grass Class is a special type of Object which is only a background in game.
 *
 *
 * @see Object
 */

class Grass: public Object
{
    private:
    public:
        static constexpr char* typeName=(char*)"Grass";

        Grass(std::string n = "Grass");
        virtual ~Grass ();
};

#endif // Grass_H

#ifndef TREE_H
#define TREE_H

/**
 * @brief  The Tree Class is a special type of Object which is static actor in map. Player will be interacting with this object.
 *
 *
 * @see Object
 */

class Tree: public Object
{
    private:

        static Array <PointXY*>* generateColMesh()  /**< Function which will be using to create a Collision Mesh*/
        {
          Array <PointXY*> *arr = new Array <PointXY*> (4);

            (*arr)[0]=new PointXY(2,2);
            (*arr)[1]=new PointXY(6,2);
            (*arr)[2]=new PointXY(2,6);
            (*arr)[3]=new PointXY(6,6);
            return arr;
        }

    public:
        static constexpr char* typeName=(char*)"Tree";

        Tree(std::string n = "Tree");
        virtual ~Tree ();
};

#endif // TREE_H
