#include "include/session.h"
#include "./drivers/include/Actor.h"

class PlayerController: public Interface
{
    private:
        ObjectPhysicsMeta* PlayerHolder = nullptr;
    public:
        PlayerController()
        :Interface() {}

        virtual ~PlayerController();

        void setPlayerHolder (ObjectMapMeta* MM);

        BaseEvent& run(GameSession& session);

};
