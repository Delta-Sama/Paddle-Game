#include "PaddleGame.h"
#include "PaddleFrameListener.h"

#include <iostream>
#include <string>

PaddleGame::PaddleGame() : ApplicationContext("Paddle Game Dobrivskiy") {}

void PaddleGame::setup()
{
    // do not forget to call the base first
    ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    root = getRoot();
    scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    createScene();
    createCamera();
    createFrameListener();
}

void PaddleGame::createScene()
{

    // -- tutorial section start --
    //! [turnlights]
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    //! [turnlights]

    //! [newlight]
    //
    Light* light1 = scnMgr->createLight("Light1");
    light1->setType(Ogre::Light::LT_POINT);
    // Set Light Color
    light1->setDiffuseColour(1.0f, 1.0f, 1.0f);
    // Set Light Reflective Color
    light1->setSpecularColour(1.0f, 0.0f, 0.0f);
    // Set Light (Range, Brightness, Fade Speed, Rapid Fade Speed)
    light1->setAttenuation(10, 0.5, 0.045, 0.0);

    //
    Entity* lightEnt = scnMgr->createEntity("LightEntity", "sphere.mesh");
    SceneNode* lightNode = scnMgr->createSceneNode("LightNode");
    lightNode->attachObject(lightEnt);
    lightNode->attachObject(light1);
    lightNode->setScale(0.01f, 0.01f, 0.01f);

    scnMgr->getRootSceneNode()->addChild(lightNode);
    //! [newlight]

    //! [lightpos]
    lightNode->setPosition(0, 4, 10);
    //! [lightpos]

    paddle = new Paddle(scnMgr, Ogre::Vector3(0, -5.25f, 0), paddleSpeed, Ogre::Vector3(2.5f, 0.5f, 0));
    ball = new Ball(scnMgr, Ogre::Vector3(0, 5.0f, 0), ballSpeed, 0.3f);

    OgreBites::TrayManager* mTrayMgr = new OgreBites::TrayManager("PaddleGameHUD", getRenderWindow());
    scnMgr->addRenderQueueListener(mOverlaySystem);

    addInputListener(mTrayMgr);

    mScoreTextLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Score", "Score", 120);
    mScoreLabel = mTrayMgr->createLabel(TL_TOPLEFT, "score", "0", 120);
    mLivesTextLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Lives", "Lives", 120);
    mLivesLabel = mTrayMgr->createLabel(TL_TOPLEFT, "lives", "0", 120);

    mFPSTextLabel = mTrayMgr->createLabel(TL_TOPRIGHT, "FPS", "FPS", 120);
    mFPSLabel = mTrayMgr->createLabel(TL_TOPRIGHT, "fps", "0", 120);
    mDeltaTimeTextLabel = mTrayMgr->createLabel(TL_TOPRIGHT, "DeltaTime", "Delta Time", 120);
    mDeltaTimeLabel = mTrayMgr->createLabel(TL_TOPRIGHT, "deltaTime", "0", 120);

    score = 0;
    lives = MAX_LIVES;

    // -- tutorial section end --
}

void PaddleGame::createCamera()
{

    //! [camera]
    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    // create the camera
    Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Node::TS_WORLD);

    // and tell it to render into the main window
    viewport = getRenderWindow()->addViewport(cam);

    ScreenBorders = Ogre::Vector2(8.5f, 6.2f);

    //! [camera]
}

void PaddleGame::checkCollisions()
{
    CollisionResult result = CollisionManager::CircleAABBCollision(paddle->GetPaddleNode()->getPosition(), paddle->GetPaddleSize(),
        ball->GetBallNode()->getPosition(), ball->GetRadius());
    if (result.collided)
    {
        if (result.normal.y != 0)
        {
            score += SCORE_FOR_TOUCH;
            ball->SetDirY(result.normal.y);
        }
        else if (result.normal.x != 0)
            ball->SetDirX(result.normal.x);

        ball->GetBallNode()->translate(result.resolve, Ogre::Node::TS_WORLD);
    }

    if (ball->GetBallNode()->getPosition().y - ball->GetRadius() < -ScreenBorders.y)
    {
        ball->Reset(ScreenBorders);

        lives -= 1;
    }

}

void PaddleGame::update(float deltaTime)
{
    paddle->Update(deltaTime, ScreenBorders);
    ball->Update(deltaTime, ScreenBorders);

    if (lives < 1)
    {
        reset();
    }
}

void PaddleGame::updateHUD(float deltaTime)
{
    mScoreLabel->setCaption(std::to_string(score));
    mLivesLabel->setCaption(std::to_string(lives));

    if (frame % 10 == 0)
    {
        std::stringstream deltaTimeStream;
        deltaTimeStream << std::fixed << std::setprecision(3) << deltaTime;

        mFPSLabel->setCaption(std::to_string(static_cast<int>(1 / deltaTime)));
        mDeltaTimeLabel->setCaption(deltaTimeStream.str());
    }
}

void PaddleGame::reset()
{
    ball->Reset(ScreenBorders);
    paddle->Reset();

    lives = MAX_LIVES;
    score = 0;
}

bool PaddleGame::keyPressed(const KeyboardEvent& evt)
{
    switch (evt.keysym.sym)
    {
    case SDLK_ESCAPE:
        getRoot()->queueEndRendering();
        break;
    case 'a':
        direction = Ogre::Vector3(-1, 0, 0);
        break;
    case 'd':
        direction = Ogre::Vector3(1, 0, 0);
        break;
    default:
        break;
    }
    return true;
}

bool PaddleGame::keyReleased(const KeyboardEvent& evt)
{
    switch (evt.keysym.sym)
    {
    case 'a':
        if (direction.x < 0)
            direction = Ogre::Vector3(0, 0, 0);
        break;
    case 'd':
        if (direction.x > 0)
            direction = Ogre::Vector3(0, 0, 0);
        break;
    default:
        break;
    }
    return true;
}

void PaddleGame::createFrameListener()
{
    direction = Ogre::Vector3(0, 0, 0);

    Ogre::FrameListener* FrameListener = new PaddleFrameListener(this);
    mRoot->addFrameListener(FrameListener);
}

void PaddleGame::clear()
{
    if (paddle) delete paddle;
    if (ball) delete ball;
}