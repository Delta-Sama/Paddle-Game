
//Animation(Rotation) of a simple Quad
// Rendering operation using vertex buffers
//  OT_POINT_LIST = 1, OT_LINE_LIST = 2, OT_LINE_STRIP = 3, OT_TRIANGLE_LIST = 4,
//  OT_TRIANGLE_STRIP = 5, OT_TRIANGLE_FAN = 6,...
//Hooman Salamat

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "Paddle.h"
#include <iostream>

using namespace Ogre;
using namespace OgreBites;

Ogre::Vector3 translate(0, 0, 0);

class ExampleFrameListener : public Ogre::FrameListener
{
private:
    Paddle* paddle;
public:

    ExampleFrameListener(Paddle* p)
    {
        paddle = p;
    }

    bool frameStarted(const Ogre::FrameEvent& evt)
    {
        paddle->move(translate * evt.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_WORLD);

        return true;
    }
};

class OgreTutorial
    : public ApplicationContext
    , public InputListener
{
private:
    float paddleSpeed = 10.0f;

    SceneManager* scnMgr;
    Root* root;

    Paddle* paddle;

public:
    OgreTutorial();
    virtual ~OgreTutorial() {}

    void setup();
    void createScene();
    void createCamera();
    bool keyPressed(const KeyboardEvent& evt);
    bool keyReleased(const KeyboardEvent& evt);
    void createFrameListener();
    Ogre::SceneNode* TriangleNode;
};


OgreTutorial::OgreTutorial()
    : ApplicationContext("Paddle Game Dobrivskiy")
{
}


void OgreTutorial::setup()
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

void OgreTutorial::createScene()
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

    paddle = new Paddle(scnMgr,paddleSpeed);


    // -- tutorial section end --
}

void OgreTutorial::createCamera()
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
    getRenderWindow()->addViewport(cam);

    //! [camera]
}

bool OgreTutorial::keyPressed(const KeyboardEvent& evt)
{
    switch (evt.keysym.sym)
    {
    case SDLK_ESCAPE:
        getRoot()->queueEndRendering();
        break;
    case 'a':
        translate = Ogre::Vector3(-10, 0, 0);
        std::cout << "A" << std::endl;
        break;
    case 'd':
        translate = Ogre::Vector3(10, 0, 0);
        std::cout << "D" << std::endl;
        break;
    default:
        break;
    }
    return true;
}

bool OgreTutorial::keyReleased(const KeyboardEvent& evt)
{
    switch (evt.keysym.sym)
    {
    case 'a':
        if (translate.x < 0)
            translate = Ogre::Vector3(0, 0, 0);
        break;
    case 'd':
        if (translate.x > 0)
            translate = Ogre::Vector3(0, 0, 0);
        break;
    default:
        break;
    }
    return true;
}

void OgreTutorial::createFrameListener()
{
    Ogre::FrameListener* FrameListener = new ExampleFrameListener(paddle);
    mRoot->addFrameListener(FrameListener);
}


int main(int argc, char** argv)
{
    try
    {
        OgreTutorial app;
        app.initApp();
        app.getRoot()->startRendering();
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
