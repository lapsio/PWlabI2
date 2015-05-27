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
