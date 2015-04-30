#include "./include/GameDomain.h"

/************GameDomain******************/

GameDomain::GameDomain() :
  chain(new Chain<Team> ({nullptr,nullptr})),
  session(nullptr)
{

}

void GameDomain:: add (Interface& klocek)
{
    this -> chain -> insertAfter({&klocek,klocek.link()});
}

void GameDomain:: remove (Interface* klocek)
{
    Chain <Team>* tmp;

    tmp = this -> chain;
    while ((tmp = tmp->next()))
        if (tmp->data.driver == klocek){
          delete tmp->data.link;
          delete tmp;
        }

    throw "Could not find driver";
}

Chain <BaseEvent&>* GameDomain:: reload ()
{
    if (!this->session)
      throw "Session uninitialized";

    Chain <BaseEvent&>* tmp_BEvent = new Chain <BaseEvent&> (*(new BaseEvent()));
    Chain <BaseEvent&>* tmp_BEvent_single = new Chain <BaseEvent&> (*(new BaseEvent()));
    Chain <Team>* tmp;

    tmp = this -> chain;

    while ((tmp = tmp -> next()))
    {
        BaseEvent& e = tmp -> data.driver -> run(*this->session);
        if (e.interrupt == true)
        {
            delete tmp_BEvent;
            tmp_BEvent_single -> insertAfter(e);
            return tmp_BEvent_single;
        }
        else if (e.type!=BaseEvent::Type::none)
        {
            tmp_BEvent -> insertAfter(e);
        }
        else
          delete &e;
    }
    delete tmp_BEvent_single;

    return tmp_BEvent;
}


/****************Interface*************/



/***************enum_Event*************/
