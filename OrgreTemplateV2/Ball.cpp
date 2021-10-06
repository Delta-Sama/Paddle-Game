#include "Ball.h"

Ball::Ball(Ogre::SceneManager* sceneManager, float speed, float rad)
{
    this->ballSpeed = speed;
    this->radius = rad;

    this->xVelocity = 1.0f;
    this->yVelocity = 1.0f;

    GenerateBallShape(sceneManager);
}

void Ball::Update(float deltaTime, Ogre::Vector2 ScreenBorders)
{
    
}

Ogre::ManualObject* Ball::GenerateBallShape(Ogre::SceneManager* sceneManager)
{
    Ogre::ManualObject* ManualObject = NULL;
    ManualObject = sceneManager->createManualObject("Ball");
    ManualObject->setDynamic(false);
    ManualObject->begin("BaseWhiteNoLighting",
        Ogre::RenderOperation::OT_TRIANGLE_LIST);
    ManualObject->position(-radius / 2, -radius / 2, 0);
    ManualObject->position(radius / 2, -radius / 2, 0);
    ManualObject->position(radius / 2, radius / 2, 0);
    ManualObject->position(-radius / 2, radius / 2, 0);
    ManualObject->triangle(0, 1, 2);
    ManualObject->triangle(0, 2, 3);
    ManualObject->end();

    ballNode = sceneManager->getRootSceneNode()->
        createChildSceneNode("BallNode");
    ballNode->attachObject(ManualObject);

    return ManualObject;
}