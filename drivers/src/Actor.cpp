#include "./drivers/include/Actor.h"

/******************************************************************/
/*************************ACTOR************************************/
/******************************************************************/
Actor::Actor(std::string n, int maxhp, int curhp, int lvl)
:Object(n,Decal("misc/player.png"),
                    PhysicalBody(PhysicalBody::ObjectType::dynamic,PhysicalBody::CollisionType::solid,PointXY(2.5,2.5),30,1))
{
    this -> pushType(Actor::typeName);

    this->CurrentHealthPoint = curhp;
    this->MaxHealthPoint = maxhp;
    this->Level = lvl;
}

/**********************************METODY****************************/

void Actor::setMaxHealthPoint (int mhp)
{
    this->MaxHealthPoint = mhp;
}
void Actor::setCurrentHealthPoint (int curhp)
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

Player::Player(std::string N, int maxhp, int curhp, int lvl)
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
