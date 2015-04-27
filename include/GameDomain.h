#include "./include/utils.h"
#include "./include/misc.h"

#ifndef BaseEvent_H
#define BaseEvent_H

class BaseEvent
{

    public:
        enum class Type
        {
            swapSessions,
            none = 0
        };
        const Type type;
        const bool interrupt;

    public:

        BaseEvent(const Type t = Type::none, const bool interrupt = false) : type(t), interrupt(interrupt){}
        virtual ~BaseEvent(){}

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

        Event (const D data, const Type t = Type::none, const bool interrupt = false) : BaseEvent (t,interrupt), data(data){}
        virtual ~Event(){}
};


#endif // Event_H

#ifndef INTERFACE_H
#define INTERFACE_H


class Interface: public GCRef
{
    public:

        Interface() : GCRef(){}
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
        ~GameDomain();

        void add (Interface&);
        void remove (Interface*);

        Chain <BaseEvent>* reload ();

};

#endif // GAMEDOMAIN_H


