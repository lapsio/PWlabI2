#include "./include/utils.h"
#include "./include/misc.h"

#ifndef BaseEvent_H
#define BaseEvent_H

class BaseEvent
{

    public:
        const bool interrupt;
        enum class Type
        {
            swapSessions,
            none = 0
        };

    private:

        const Type type = Type::none;

    public:

        BaseEvent(bool interrupt, const Type t) :interrupt(false),  type(t){};
        virtual ~BaseEvent(){};

};

#endif // BaseEvent_H

#ifndef Event_H
#define Event_H

template <typename D>
class Event: public BaseEvent
{
    private:

        const D data;

    public:

        Event (const D data, const Type t)
        :BaseEvent (t)
        {
            this -> data = data;
        }
        virtual ~Event(){};
};


#endif // Event_H

#ifndef INTERFACE_H
#define INTERFACE_H


class Interface: public GCRef
{
    public:

        Interface();
        virtual ~Interface() = 0;

        virtual BaseEvent& run() = 0;

};

#endif // INTERFACE_H

#ifndef GAMEDOMAIN_H
#define GAMEDOMAIN_H


class GameDomain
{
    private:
        struct Team
        {
            Interface* driver;
            GCRefLink* link;
        };
        Chain <Team> * chain;
    public:
        GameDomain();
        virtual ~GameDomain() = 0;

        void add (Interface&);
        void remove (Interface&);

        Chain <BaseEvent>* reload ();

};

#endif // GAMEDOMAIN_H


