#include "include/session.h"
#include "drivers/include/PlayerController.h"
#include "drivers/include/Static.h"

#ifndef Intro_H
#define Intro_H

/**
 * @brief  The Intro Class is a tool which help us to control a session. This Class will change a current session to new one created in function.
 *
 *
 * @see Interface
 */

class Intro: public Interface
{
    public:
        Intro (){}

        virtual ~Intro();

        BaseEvent& run(GameSession& session);   /**< Basic funtion which control a session*/
};

#endif // Intro_H
