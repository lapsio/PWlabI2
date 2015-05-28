#include "drivers/include/Intro.h"


#ifndef EngineStart_H
#define EngineStart_H


/**
 * @brief The EngineStart class is using as a tool which will control current session and will change session when user press return
 *
 *
 * @see Interface
 */

class EngineStart: public Interface
{
    private:

        bool IsObjectAdded = false; /**< Variable which defend us from adding the same object dozens of times */
        RenderEngine& renderEngine;

    public:

        EngineStart(RenderEngine& rE) : renderEngine (rE) {}

        ~EngineStart();

        BaseEvent& run(GameSession& session); /**< Basic methods which give us a opportunity to control a session*/

        void setIsObjectAdded(bool C = true);   /**< Change a class variable IsObjectAdded */

};

#endif // EngineStart_H
