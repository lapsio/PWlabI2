#include "./include/GameDomain.h"

/************GameDomain******************/

GameDomain::GameDomain()
{
    this -> chain = new Chain <Team> ({nullptr,nullptr});
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

Chain <BaseEvent>* GameDomain:: reload ()
{
    Chain <BaseEvent>* tmp_BEvent = new Chain <BaseEvent> ({BaseEvent::Type::none, false});
    Chain <BaseEvent>* tmp_BEvent_single = new Chain <BaseEvent> ({BaseEvent::Type::none, false});
    Chain <Team>* tmp;

    tmp = this -> chain;

    while ((tmp = tmp -> next()))
    {
        BaseEvent& e = tmp -> data.driver -> run();
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
