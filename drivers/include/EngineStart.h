#include "include/session.h"

class EngineStart : public Interface
{

    public:

        RenderEngine& renderEngine;

        EngineStart(RenderEngine& rE) : renderEngine (rE) {}

        ~EngineStart();

        BaseEvent& run(GameSession& session);

};
