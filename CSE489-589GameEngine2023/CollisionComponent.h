#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include "Scoring.h"

class CollisionComponent :
    public Component
{
public:
	CollisionComponent() : Component(100) {}

	virtual void collisionEnter(const class RigidBodyComponent* collisionData)
	{
		if (collisionData->owningGameObject->gameObjectName == "projectile") {

			if (VERBOSE) cout << "HIT BY PROJECTILE - " << owningGameObject->gameObjectName << endl;

			if (owningGameObject->gameObjectName == "cat") {
				Scoring::addPoints(10);
			}
			else if (owningGameObject->gameObjectName == "penguin") {
				Scoring::addPoints(20);
			}
			else if (owningGameObject->gameObjectName == "jet") {
				Scoring::addPoints(50);
			}

			owningGameObject->removeAndDelete();
			collisionData->owningGameObject->removeAndDelete();

		}
	
		if(VERBOSE) cout << "collision enter " << owningGameObject->gameObjectName << endl;

	};

	virtual void collisionStay(const class RigidBodyComponent* collisionData)
	{ 
		if (VERBOSE) cout << "collision stay " << owningGameObject->gameObjectName << endl;
	};

	virtual void collisionExit(const class RigidBodyComponent* collisionData)
	{ 
		if (VERBOSE) cout << "collision exit " << owningGameObject->gameObjectName << endl;
		//owningGameObject->removeAndDelete();
	};


protected:

	bool VERBOSE = false;

};

