#pragma once

#include "Ogre.h"

struct CollisionResult
{
	CollisionResult(bool collision, Ogre::Vector3 n) : collided(collision), normal(n) {}

	bool collided;
	Ogre::Vector3 normal;
};

class CollisionManager
{
public:
	static float clamp(float num, float min, float max)
	{
		return std::min(std::max(num,min), max);
	}

	static CollisionResult CircleAABBCollision(Ogre::Vector3 recPos, Ogre::Vector3 recSize, Ogre::Vector3 ballPos, float rad)
	{
		Ogre::Vector3 dif = ballPos - recPos;
		Ogre::Vector3 clamped = Ogre::Vector3(clamp(dif.x, -recSize.x / 2, recSize.x / 2), clamp(dif.y, -recSize.y / 2, recSize.y / 2), 0);
		Ogre::Vector3 closestPoint = recPos + clamped;
		bool collision = Ogre::Vector3(closestPoint - ballPos).squaredLength() < rad * rad;

		Ogre::Vector3 normal = Ogre::Vector3(0, 0, 0);
		if (collision)
		{
			Ogre::Vector3 minDif = Ogre::Vector3(recSize.x / 2 - abs(clamped.x), recSize.y / 2 - abs(clamped.y), 0);
			if (minDif.x < minDif.y)
			{
				normal = Ogre::Vector3((clamped.x < 0 ? -1 : 1), 0, 0);
			}
			else
			{
				normal = Ogre::Vector3(0, (clamped.y < 0 ? -1 : 1), 0);
			}
		}
		return CollisionResult(collision, normal);
	}

};