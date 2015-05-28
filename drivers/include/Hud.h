#include "include/session.h"

#ifndef HealthBar_H
#define HealthBar_H

/**
 * @brief  Simply Class which help us to create a new type of Object class.
 *
 * @see Object
 */

class HealthBar: public Object
{
    private:
    public:
        static constexpr char* typeName=(char*)"HealthBar";

        HealthBar(std::string n = "HealthBar");
        virtual ~HealthBar();


};

#endif // HealthBar_H
