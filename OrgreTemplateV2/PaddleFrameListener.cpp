#include "PaddleFrameListener.h"
#include "PaddleGame.h"

bool PaddleFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    Game->paddle->move(Game->direction * evt.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_WORLD);

    Game->update(evt.timeSinceLastFrame);

    Game->checkCollisions();

    Game->updateHUD(evt.timeSinceLastFrame);

    Game->getRenderWindow()->resize(640, 480);

    Game->frame += 1;

    return true;
}
