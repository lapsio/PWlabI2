#include "./include/utils.h"
#include "./include/misc.h"
//#include "./include/session.h"

#ifndef BaseEvent_H
#define BaseEvent_H

class GameSession;

/**
 * @brief The BaseEvent class class providing events functionality designed for session management. Drivers can affect session flow by returning special Events.
 * Base event doesn't contain data, however drivers can create extended Event providing for example new session to be loaded.
 *
 * @see Event
 */

class BaseEvent
{

    public:
        enum class Type
        {
            none = 0,
            swapSessions
        };
        const Type type; /**< event type */
        const bool interrupt; /**< whether to interrupt drivers execution queue and jump to session loop in order to instantly parse Event */

    public:

        BaseEvent(const Type t = Type::none, const bool interrupt = false) : type(t), interrupt(interrupt){}
        virtual ~BaseEvent(){}

};

#endif // BaseEvent_H

#ifndef Event_H
#define Event_H

template <typename D>
/**
 * @brief The Event class extension to BaseEvent class allowing drivers to pass custom data in event.
 * @see BaseEvent
 */
class Event: public BaseEvent
{

    public:
        const D data; /**< custom data */

        Event (const D data, const Type t = Type::none, const bool interrupt = false) : BaseEvent (t,interrupt), data(data){}
        virtual ~Event(){}
};


#endif // Event_H

#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * @brief The Interface class Interface to be implemented by all drivers. Each driver needs to implement certain functionality in order to work with
 * game domain manager and session properly. Interface by default inherits from GCRef so it's deleted automatically once all sessions using driver will be destroyed
 */

class Interface: public GCRef
{
    protected:
        Interface() : GCRef(){}
    public:

        virtual ~Interface();

        /**
         * @brief run method called before each screen redraw
         * @param session current session passed to driver by game domain manager
         * @return
         */
        virtual BaseEvent& run(GameSession& session) = 0;

};

#endif // INTERFACE_H

#ifndef GAMEDOMAIN_H
#define GAMEDOMAIN_H

/**
 * @brief The GameDomain class Game domain manager class, providing game domain code layer for game logic. Game is created using multiple drivers, handling
 * various tasks.
 */

class GameDomain
{
    private:
        struct Team
        {
            Interface* driver;
            GCRefLink* link;
        };
        Chain <Team> * chain;
        GameSession* session;
    public:
        GameDomain();
        ~GameDomain();

        /**
         * @brief add add new driver to execution queue of this manager
         * @param interface interface to be added
         */
        void add (Interface& interface);
        /**
         * @brief remove remove driver from manager
         * @param interface to be removed
         */
        void remove (Interface* interface);
        /**
         * @brief bindSession bind session to this manager. THIS IS REQUIRED. Each manager HAS TO have some session bound to it.
         * @param s session to be bound
         */
        inline void bindSession(GameSession& s){this->session=&s;}

        /**
         * @brief reload reload drivers queue
         * @return
         */
        Chain <BaseEvent&>* reload ();

};

#endif // GAMEDOMAIN_H


