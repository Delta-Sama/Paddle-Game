#include "Paddle.h"

#include <iostream>
#include <algorithm>

Paddle::Paddle(Ogre::SceneManager* sceneManager, Ogre::Vector3 position, float speed, Ogre::Vector3 size)
{
    this->paddleSpeed = speed;
    this->paddleSize = size;

    this->defaultPosition = position;

    GeneratePaddleShape(sceneManager);

    paddleNode->setPosition(position);
}

void Paddle::move(const Ogre::Vector3& direction, Ogre::Node::TransformSpace relativeTo)
{
    paddleNode->translate(direction * paddleSpeed, relativeTo);
}

void Paddle::Update(float deltaTime, Ogre::Vector2 ScreenBorders)
{
    if (paddleNode->getPosition().x < -ScreenBorders.x + paddleSize.x / 2)
        paddleNode->setPosition(Ogre::Vector3(-ScreenBorders.x + paddleSize.x / 2, paddleNode->getPosition().y, 0));
    else if (paddleNode->getPosition().x > ScreenBorders.x - paddleSize.x / 2)
        paddleNode->setPosition(Ogre::Vector3(ScreenBorders.x - paddleSize.x / 2, paddleNode->getPosition().y, 0));
}

Ogre::ManualObject* Paddle::GeneratePaddleShape(Ogre::SceneManager* sceneManager)
{
    Ogre::ManualObject* ManualObject = NULL;
    ManualObject = sceneManager->createManualObject("Paddle");
    ManualObject->setDynamic(false);
    ManualObject->begin("BaseWhiteNoLighting",
        Ogre::RenderOperation::OT_TRIANGLE_LIST);
    ManualObject->position(-paddleSize.x / 2, -paddleSize.y / 2, 0);
    ManualObject->position(paddleSize.x / 2, -paddleSize.y / 2, 0);
    ManualObject->position(paddleSize.x / 2, paddleSize.y / 2, 0);
    ManualObject->position(-paddleSize.x / 2, paddleSize.y / 2, 0);
    ManualObject->triangle(0, 1, 2);
    ManualObject->triangle(0, 2, 3);
    ManualObject->end();

    paddleNode = sceneManager->getRootSceneNode()->
        createChildSceneNode("PaddleNode");
    paddleNode->attachObject(ManualObject);

    return ManualObject;
}