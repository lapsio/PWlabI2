#include <iostream>
#include <string>

#include "./include/object.h"

class Actor: public Object
{
    private:
        int MaxHealthPoint;
        int CurrentHealthPoint;
        int Level;
    public:
        static constexpr char* typeName=(char*)"Actor";

        Actor(std::string n = "Actor", int maxhp = 1, int curhp = 1, int lvl = 1);
        virtual ~Actor(){}

        void setMaxHealthPoint (int mhp);
        void setCurrentHealthPoint (int curhp);
        void setLevel (int lvl);

        inline int getMaxHealthPoint() const {return this->MaxHealthPoint;}
        inline int getCurrentHealthPoint() const {return this->CurrentHealthPoint;}
        inline int getLevel() const {return this->Level;}


};


class Player: public Actor
{
    private:
        float GainedExperience = 0;
        float ToNextLevelExperience = 100;
    public:
        static constexpr char* typeName=(char*)"Player";

        Player(std::string N = "Player", int maxhp = 1, int curhp = 1, int lvl = 1);

        virtual ~Player();

        inline float getGainedExperience() const {return this->GainedExperience;}
        inline float getToNextLevelExperience() const {return this->ToNextLevelExperience;}

        void AddExperience (float exp); /** DODAJE EXP I JAK PRZEKROCZY NA LVL ZWIEKSZY LVL I POTRZEBNY EXP**/
        void LevelUp ();



};
