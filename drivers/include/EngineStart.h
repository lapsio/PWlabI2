#include "include/session.h"

class EngineStart : public Interface
{
    private:

        bool IsObjectAdded = false;
        RenderEngine& renderEngine;

    public:

        EngineStart(RenderEngine& rE) : renderEngine (rE) {}

        ~EngineStart();

        BaseEvent& run(GameSession& session);

        void setIsObjectAdded(bool C = true);

};
