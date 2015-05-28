#include <iostream>
#include <string>

#include "./include/object.h"

#ifndef Actor_H
#define Actor_H

/**
 * @brief  The Actor class is a parent class for Player,Npc et cetera. This class help us with creating a Object Class with something new.
 * Class contain some basic attributes for actor characters.
 *
 * @see Object
 * @see Player
 */

class Actor: public Object
{
    private:
        float MaxHealthPoint;
        float CurrentHealthPoint;
        int Level;
    public:
        static constexpr char* typeName=(char*)"Actor"; /**< This help us to recognise later a type of Object*/

        Actor(std::string n = "Actor", float maxhp = 1, float curhp = 1, int lvl = 1);
        virtual ~Actor(){}

        void setMaxHealthPoint (float mhp); /**< thanks to that we can set a class variable MaxHealthPoint*/
        void setCurrentHealthPoint (float curhp); /**< thanks to that we can set a class variable CurrentHealthPoint*/
        void setLevel (int lvl);    /**< thanks to that we can set a class variable Level*/

        inline float getMaxHealthPoint() const {return this->MaxHealthPoint;} /**< That function help us to see what is in class variable MaxHealthPoint*/
        inline float getCurrentHealthPoint() const {return this->CurrentHealthPoint;} /**< That function help us to see what is in class variable CurrentHealthPoint*/
        inline int getLevel() const {return this->Level;} /**< That function help us to see what is in class variable CurrentHealthPoint*/


};

#endif // Actor_H


#ifndef Player_H
#define Player_H


/**
 * @brief  The Player Class is a expanded version of Actor class. Class contain some extra attributes which helped player in game;
 *
 * @see Object
 * @see Actor
 */


class Player: public Actor
{
    private:
        float GainedExperience = 0;
        float ToNextLevelExperience = 100;
    public:
        static constexpr char* typeName=(char*)"Player";

        Player(std::string N = "Player", float maxhp = 1, float curhp = 1, int lvl = 1);

        virtual ~Player();

        inline float getGainedExperience() const {return this->GainedExperience;}   /**< Thanks to that we can check what is in class variable GainedExperience*/
        inline float getToNextLevelExperience() const {return this->ToNextLevelExperience;}/**< Thanks to that we can check what is in class variable ToNextLevelExperience*/

        void AddExperience (float exp); /**< We can add experience to current experience*/
        void LevelUp ();    /**< Add one to class variable Level*/



};

#endif // Player_H
