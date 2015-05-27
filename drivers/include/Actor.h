#include <iostream>
#include <string>

#include "./include/object.h"

class Actor: public Object
{
    private:
        float MaxHealthPoint;
        float CurrentHealthPoint;
        int Level;
    public:
        static constexpr char* typeName=(char*)"Actor";

        Actor(std::string n = "Actor", float maxhp = 1, float curhp = 1, int lvl = 1);
        virtual ~Actor(){}

        void setMaxHealthPoint (float mhp);
        void setCurrentHealthPoint (float curhp);
        void setLevel (int lvl);

        inline float getMaxHealthPoint() const {return this->MaxHealthPoint;}
        inline float getCurrentHealthPoint() const {return this->CurrentHealthPoint;}
        inline int getLevel() const {return this->Level;}


};


class Player: public Actor
{
    private:
        float GainedExperience = 0;
        float ToNextLevelExperience = 100;
    public:
        static constexpr char* typeName=(char*)"Player";

        Player(std::string N = "Player", float maxhp = 1, float curhp = 1, int lvl = 1);

        virtual ~Player();

        inline float getGainedExperience() const {return this->GainedExperience;}
        inline float getToNextLevelExperience() const {return this->ToNextLevelExperience;}

        void AddExperience (float exp); /** DODAJE EXP I JAK PRZEKROCZY NA LVL ZWIEKSZY LVL I POTRZEBNY EXP**/
        void LevelUp ();



};
