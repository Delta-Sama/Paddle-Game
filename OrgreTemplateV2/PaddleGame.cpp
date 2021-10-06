
//Animation(Rotation) of a simple Quad
// Rendering operation using vertex buffers
//  OT_POINT_LIST = 1, OT_LINE_LIST = 2, OT_LINE_STRIP = 3, OT_TRIANGLE_LIST = 4,
//  OT_TRIANGLE_STRIP = 5, OT_TRIANGLE_FAN = 6,...
//Hooman Salamat

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"
#include "CollisionManager.h"
#include "Paddle.h"
#include "Ball.h"

#include <iostream>
#include <string>

using namespace Ogre;
using namespace OgreBites;

Ogre::Vector3 direction(0, 0, 0);

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

    Ogre::Vector2 ScreenBorders;
    Viewport* viewport;

    OgreBites::Label* mScoreTextLabel;
    OgreBites::Label* mScoreLabel;
    OgreBites::Label* mLivesTextLabel;
    OgreBites::Label* mLivesLabel;

    int score;
    const int SCORE_FOR_TOUCH = 10;

    int lives;
    const int MAX_LIVES = 5;

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
    void updateHUD();
    void reset();
};

class ExampleFrameListener : public Ogre::FrameListener
{
private:
    PaddleGame* Game;

public:

    ExampleFrameListener(PaddleGame* mainGame) : Game(mainGame) {}

    bool frameStarted(const Ogre::FrameEvent& evt)
    {
        Game->paddle->move(direction * evt.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_WORLD);

        Game->update(evt.timeSinceLastFrame);

        Game->checkCollisions();

        Game->updateHUD();

        Game->getRenderWindow()->resize(640, 480);

        return true;
    }
};


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
    ball = new Ball(scnMgr, Ogre::Vector3(0, 5.0f, 0), ballSpeed, 0.2f);

    OgreBites::TrayManager* mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow());
    scnMgr->addRenderQueueListener(mOverlaySystem);

    addInputListener(mTrayMgr);

    mScoreTextLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Score", "Score", 120);
    mScoreLabel = mTrayMgr->createLabel(TL_TOPLEFT, "score", "0", 120);
    mLivesTextLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Lives", "Lives", 120);
    mLivesLabel = mTrayMgr->createLabel(TL_TOPLEFT, "lives", "0", 120);

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

void PaddleGame::updateHUD()
{
    mScoreLabel->setCaption(std::to_string(score));
    mLivesLabel->setCaption(std::to_string(lives));
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
    Ogre::FrameListener* FrameListener = new ExampleFrameListener(this);
    mRoot->addFrameListener(FrameListener);
}

void PaddleGame::clear()
{
    if (paddle) delete paddle;
    if (ball) delete ball;
}

int main(int argc, char** argv)
{
    try
    {
        PaddleGame app;
        app.initApp();
        app.getRoot()->startRendering();
        app.clear();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

//! [fullsource]
