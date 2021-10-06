#include "Ball.h"

#include <iostream>

Ball::Ball(Ogre::SceneManager* sceneManager, Ogre::Vector3 position, float speed, float rad)
{
    this->ballSpeed = speed;
    this->radius = rad;

    this->xDirection = -1;
    this->yDirection = -1;

    this->defaultPosition = position;

    GenerateBallShape(sceneManager);

    ballNode->setPosition(position);
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

    if (ballNode->getPosition().y + radius > ScreenBorders.y)
    {
        ballNode->setPosition(Ogre::Vector3(ballNode->getPosition().x, ScreenBorders.y - radius, 0));
        yDirection = -1;
    }
}

void Ball::Reset(Ogre::Vector2 ScreenBorders)
{
    this->xDirection = 1 - 2 * (rand() % 2);
    this->yDirection = -1;

    ballNode->setPosition(Ogre::Vector3(-ScreenBorders.x + ScreenBorders.x * 2 * (rand() % 100) / 100.0f, defaultPosition.y, 0));
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