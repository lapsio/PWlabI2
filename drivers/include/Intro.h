#include "include/session.h"
#include "drivers/include/PlayerController.h"

class Intro: public Interface
{
    private:
        RenderEngine& renderEngine;
    public:
        Intro (RenderEngine& rE): renderEngine (rE) {}

        virtual ~Intro();

        BaseEvent& run(GameSession& session);
};
