#pragma once

#include "Ogre.h"

class Paddle
{
public:
    Paddle(Ogre::SceneManager* sceneManager, float speed);

    void move(const Ogre::Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_PARENT);
	
    Ogre::SceneNode* GetPaddleNode() { return paddleNode; }

    void Update(Ogre::Vector2 ScreenBorders);

private:
    const Ogre::Vector2 paddleSize = Ogre::Vector2(4,1);

    Ogre::ManualObject* GeneratePaddleShape(Ogre::SceneManager* sceneManager);

    Ogre::SceneNode* paddleNode;
    float paddleSpeed;
};