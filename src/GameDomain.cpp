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

void GameDomain:: remove (Interface& klocek)
{
    Chain <Team>* tmp;

    tmp = this -> chain;
    while (tmp -> data.driver == &klocek)
        tmp = tmp -> next();

    delete tmp -> data.link;
    delete tmp;
}

Chain <BaseEvent>* GameDomain:: reload ()
{
    Chain <BaseEvent>* tmp_BEvent = new Chain <BaseEvent> ({false, BaseEvent::Type::none});
    Chain <BaseEvent>* tmp_BEvent_single = new Chain <BaseEvent> ({false, BaseEvent::Type::none});
    Chain <Team>* tmp;

    tmp = this -> chain;

    while (tmp -> next() != nullptr)
    {
        if (tmp -> data.driver -> run().interrupt == true)
        {
            delete tmp_BEvent;
            tmp_BEvent_single -> insertAfter(tmp -> data.driver -> run());
            return tmp_BEvent_single;
        }
        else
        {
            tmp_BEvent -> insertAfter(tmp -> data.driver -> run());
        }

        tmp = tmp -> next();
    }
    delete tmp_BEvent_single;

    return tmp_BEvent;
}


/****************Interface*************/

Interface::Interface()
:GCRef()
{
}

/***************enum_Event*************/
