#pragma once
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"

#include "CollisionManager.h"
#include "Paddle.h"
#include "Ball.h"

using namespace Ogre;
using namespace OgreBites;

class PaddleGame
    : public ApplicationContext
    , public InputListener
{
private:
    float paddleSpeed = 11.0f;
    float ballSpeed = 8.0f;

    SceneManager* scnMgr;
    Root* root;

public:
    Paddle* paddle;
    Ball* ball;

    Ogre::Vector3 direction;

    Ogre::Vector2 ScreenBorders;
    Viewport* viewport;

    OgreBites::Label* mScoreTextLabel;
    OgreBites::Label* mScoreLabel;
    OgreBites::Label* mLivesTextLabel;
    OgreBites::Label* mLivesLabel;

    OgreBites::Label* mFPSTextLabel;
    OgreBites::Label* mFPSLabel;
    OgreBites::Label* mDeltaTimeTextLabel;
    OgreBites::Label* mDeltaTimeLabel;

    int score;
    const int SCORE_FOR_TOUCH = 10;

    int lives;
    const int MAX_LIVES = 5;

    long int frame = 0;

public:
    PaddleGame();
    virtual ~PaddleGame() {}

    void setup();
    void createScene();
    void createCamera();
    bool keyPressed(const KeyboardEvent& evt);
    bool keyReleased(const KeyboardEvent& evt);
    void createFrameListener();
    void clear();

    void checkCollisions();
    void update(float deltaTime);
    void updateHUD(float deltaTime);
    void reset();
};