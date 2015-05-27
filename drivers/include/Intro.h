#include "include/session.h"
#include "drivers/include/PlayerController.h"

class Intro: public Interface
{
    public:
        Intro (){}

        virtual ~Intro();

        BaseEvent& run(GameSession& session);
};