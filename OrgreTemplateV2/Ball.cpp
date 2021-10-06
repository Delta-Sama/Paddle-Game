#include "Ball.h"

Ball::Ball(Ogre::SceneManager* sceneManager, float speed, float rad)
{
    this->ballSpeed = speed;
    this->radius = rad;

    this->xDirection = -1;
    this->yDirection = -1;

    GenerateBallShape(sceneManager);
}

void Ball::Update(float deltaTime, Ogre::Vector2 ScreenBorders)
{
    ballNode->translate(Ogre::Vector3(xDirection * ballSpeed * deltaTime, yDirection * ballSpeed * deltaTime, 0),
        Ogre::Node::TransformSpace::TS_WORLD);

    if (ballNode->getPosition().x - radius < -ScreenBorders.x)
    {
        ballNode->setPosition(Ogre::Vector3(-ScreenBorders.x + radius, ballNode->getPosition().y, 0));
        xDirection = 1;
    }
    else if (ballNode->getPosition().x + radius > ScreenBorders.x)
    {
        ballNode->setPosition(Ogre::Vector3(ScreenBorders.x - radius, ballNode->getPosition().y, 0));
        xDirection = -1;
    }

    if (ballNode->getPosition().y - radius < -ScreenBorders.y)
    {
        ballNode->setPosition(Ogre::Vector3(ballNode->getPosition().x, -ScreenBorders.y + radius, 0));
        yDirection = 1;
    }
    else if (ballNode->getPosition().y + radius > ScreenBorders.y)
    {
        ballNode->setPosition(Ogre::Vector3(ballNode->getPosition().x, ScreenBorders.y - radius, 0));
        yDirection = -1;
    }
}

Ogre::ManualObject* Ball::GenerateBallShape(Ogre::SceneManager* sceneManager)
{
    Ogre::ManualObject* ManualObject = NULL;
    ManualObject = sceneManager->createManualObject("Ball");
    ManualObject->setDynamic(false);
    ManualObject->begin("BaseWhiteNoLighting",
        Ogre::RenderOperation::OT_TRIANGLE_LIST);
    ManualObject->position(-radius, -radius, 0);
    ManualObject->position(radius, -radius, 0);
    ManualObject->position(radius, radius, 0);
    ManualObject->position(-radius, radius, 0);
    ManualObject->triangle(0, 1, 2);
    ManualObject->triangle(0, 2, 3);
    ManualObject->end();

    ballNode = sceneManager->getRootSceneNode()->
        createChildSceneNode("BallNode");
    ballNode->attachObject(ManualObject);

    return ManualObject;
}