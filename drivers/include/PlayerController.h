#include "include/session.h"
#include "./drivers/include/Actor.h"

#ifndef PlayerController_H
#define PlayerController_H

/**
 * @brief  The PlayerController Class is a tool which we add to session and this allow us to control a Player Object in game.
 *
 *
 * @see Interface
 */

class PlayerController: public Interface
{
    private:
        ObjectPhysicsMeta* PlayerHolder = nullptr;  /**< Handler for a player object added to PhysicEngine.*/
    public:
        PlayerController()
        :Interface() {}

        virtual ~PlayerController();

        void setPlayerHolder (ObjectMapMeta* MM); /**< Setting a class variable setPlayerHolder.*/

        BaseEvent& run(GameSession& session);   /**< Basic funtion which control a session*/

};

#endif // PlayerController
