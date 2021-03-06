#pragma once

#include "Ogre.h"

class Paddle
{
public:
    Paddle(Ogre::SceneManager* sceneManager, Ogre::Vector3 position, float speed, Ogre::Vector3 size);

    void move(const Ogre::Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_PARENT);
	
    Ogre::SceneNode* GetPaddleNode() { return paddleNode; }
    Ogre::Vector3 GetPaddleSize() { return paddleSize; }

    void Update(float deltaTime, Ogre::Vector2 ScreenBorders);

    void Reset();

private:
    Ogre::ManualObject* GeneratePaddleShape(Ogre::SceneManager* sceneManager);

    Ogre::SceneNode* paddleNode;
    Ogre::Vector3 paddleSize;
    float paddleSpeed;

    Ogre::Vector3 defaultPosition;
};