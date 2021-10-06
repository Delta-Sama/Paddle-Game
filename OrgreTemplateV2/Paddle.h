#pragma once

#include "Ogre.h"

class Paddle
{
public:
	Ogre::SceneNode* paddleNode;
	float paddleSpeed;

    void move(const Ogre::Vector3& v, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_PARENT)
    {
        paddleNode->translate(v, relativeTo);
    }
	
	Paddle(Ogre::SceneManager* sceneManager, float speed)
	{
		this->paddleSpeed = speed;

        GeneratePaddleShape(sceneManager);
	}

private:
    Ogre::ManualObject* GeneratePaddleShape(Ogre::SceneManager* sceneManager);

};

Ogre::ManualObject* Paddle::GeneratePaddleShape(Ogre::SceneManager* sceneManager)
{
    Ogre::ManualObject* ManualObject = NULL;
    ManualObject = sceneManager->createManualObject("Paddle");
    ManualObject->setDynamic(false);
    ManualObject->begin("BaseWhiteNoLighting",
        Ogre::RenderOperation::OT_TRIANGLE_LIST);
    ManualObject->position(0, 0, 0);
    ManualObject->position(4, 0, 0);
    ManualObject->position(4, 1, 0);
    ManualObject->position(0, 1, 0);
    ManualObject->triangle(0, 1, 2);
    ManualObject->triangle(0, 2, 3);
    ManualObject->end();

    paddleNode = sceneManager->getRootSceneNode()->
        createChildSceneNode("PaddleNode");
    paddleNode->attachObject(ManualObject);

    return ManualObject;
}