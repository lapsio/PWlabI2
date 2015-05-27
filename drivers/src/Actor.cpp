#include "./drivers/include/Actor.h"

/******************************************************************/
/*************************ACTOR************************************/
/******************************************************************/
Actor::Actor(std::string n, float maxhp, float curhp, int lvl)
:Object(n,Decal("misc/player.png"),
                    PhysicalBody(PhysicalBody::ObjectType::dynamic,PhysicalBody::CollisionType::solid,PointXY(2.5,2.5),10,1))
{
    this -> pushType(Actor::typeName);

    this->CurrentHealthPoint = curhp;
    this->MaxHealthPoint = maxhp;
    this->Level = lvl;
}

/**********************************METODY****************************/

void Actor::setMaxHealthPoint (float mhp)
{
    this->MaxHealthPoint = mhp;
}
void Actor::setCurrentHealthPoint (float curhp)
{
    this->CurrentHealthPoint = curhp;
}
void Actor::setLevel (int lvl)
{
    this->Level = lvl;
}

/******************************************************************/
/*************************PLAYER*********************************/
/******************************************************************/

Player::Player(std::string N, float maxhp, float curhp, int lvl)
:Actor(N , maxhp, curhp, lvl)
{
    this->pushType(Player::typeName);
}

Player::~Player() {}


/**********************************METODY****************************/

void Player::LevelUp()
{
    this->setLevel(this->getLevel() + 1);
}

void Player::AddExperience (float exp)
{
    this->GainedExperience = this->GainedExperience + exp;

    if (this->GainedExperience >= ToNextLevelExperience)
    {
        this->LevelUp();
        this -> ToNextLevelExperience = ToNextLevelExperience*(this->getLevel());
    }
}
