#pragma once
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"

using namespace Ogre;
using namespace OgreBites;

class PaddleGame;

class PaddleFrameListener : public Ogre::FrameListener
{
private:
    PaddleGame* Game;

public:
    PaddleFrameListener(PaddleGame* mainGame) : Game(mainGame) {}

    bool frameStarted(const Ogre::FrameEvent& evt);
};