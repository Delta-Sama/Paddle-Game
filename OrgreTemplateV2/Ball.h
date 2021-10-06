#pragma once

#include "Ogre.h"

class Ball
{
public:
    Ball(Ogre::SceneManager* sceneManager, float speed, float rad);

    Ogre::SceneNode* GetBallNode() { return ballNode; }
    float GetRadius() { return radius; }

    void Update(float deltaTime, Ogre::Vector2 ScreenBorders);

    void SetDirX(float dir) { xDirection = dir; }
    void SetDirY(float dir) { yDirection = dir; }

private:
    Ogre::ManualObject* GenerateBallShape(Ogre::SceneManager* sceneManager);

    Ogre::SceneNode* ballNode;
    float radius;
    float ballSpeed;

    float xDirection;
    float yDirection;
};