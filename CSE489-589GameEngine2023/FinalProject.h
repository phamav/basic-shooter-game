#pragma once

#include "GameEngine.h"
#include <chrono>
#include <random>

class FinalProject : public Game
{
protected:

	virtual void processGameInput()
	{
		Game::processGameInput(); // call to superclass

		// LEFT click to shoot
		if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !shootingKeyDown)
		{
				shootingKeyDown = true;

				// create a new projectile
				auto btGO = std::make_shared<GameObject>();
				btGO->gameObjectName = "projectile";
				addChildGameObject(btGO);
				btGO->setPosition(vec3(0.0f, 2.0f, 0.0f));

				// material for bullet
				Material btMat;
				btMat.setAmbientAndDiffuseMat(vec4(0.831f, 0.686f, 0.216f, 1.0f));
				btMat.setSpecularMat(vec4(1.0f, 0.843f, 0.0f, 1.0f));
				btMat.setEmissiveMat(vec4(0.0f, 0.0f, 0.0f, 1.0f));
				btMat.setSpecularExponentMat(50.0f);

				// cylinder mesh for bullet
				auto btSphereShape = std::make_shared<SphereMeshComponent>
					(shaderProgram, btMat, 0.5f, 12, 16);
				
				// bullet dynamic rigid body
				auto btRigidBody = std::make_shared<RigidBodyComponent>(
					btSphereShape, DYNAMIC, 5.0f);
				btRigidBody->setVelocity(100.0f * gun->getFowardDirection());
				btGO->addComponent(btSphereShape);
				btGO->addComponent(btRigidBody);
				btGO->addComponent(std::make_shared<CollisionComponent>());

				auto btRemove = std::make_shared<RemoveComponent>(5.0f);
				btGO->addComponent(btRemove);

				// add gunshot sound
				auto gunSound = std::make_shared<SoundSourceComponent>("Assets/gunshot.wav");
				gunSound->play();
				gun->addComponent(gunSound);

		} 
		else if (!(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
		{
			shootingKeyDown = false;
		}
	}

	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "CSE 489/589 GameEngine Fall 2023 Final Project");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build and use the shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		shaderProgram = BuildShaderProgram(shaders);

		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);
		
		std::vector<string> faces
		{
			"Assets/Tenerife/posx.jpg",
			"Assets/Tenerife/negx.jpg",
			"Assets/Tenerife/posy.jpg",
			"Assets/Tenerife/negy.jpg",
			"Assets/Tenerife/posz.jpg",
			"Assets/Tenerife/negz.jpg"
		};

		// TUTORIAL
		// Use WASD for controling the gun
		// Use arrow keys to control main camera
		// To shoot, left click
		
		// ****** Point light *********
		auto posLight = std::make_shared<GameObject>();
		addChildGameObject(posLight);
		posLight->setPosition(glm::vec3(10.0f, 10.0f, 10.0f), WORLD);

		auto posLightComp = std::make_shared<PositionalLightComponent>(GLFW_KEY_P); // 'P' key to toggle
		posLightComp->setEnable(true);
		posLight->addComponent(posLightComp);

		// ****** Directional light *********
		auto dirLight = std::make_shared<GameObject>();
		addChildGameObject(dirLight);
		dirLight->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		auto dirLightComp = std::make_shared<DirectionalLightComponent>(GLFW_KEY_L); // 'L' key
		dirLightComp->setEnable(true);
		dirLight->addComponent(dirLightComp);

		// ****** Cameras *********
		// Add the main (front) camera
		// Camera attached to the player
		auto mainCam = std::make_shared<GameObject>();
		addChildGameObject(mainCam);
		auto mainCamComp = std::make_shared<CameraComponent>();
		mainCam->addComponent(mainCamComp);

		// Add a second camera for an overall view
		auto auxCam = std::make_shared<GameObject>();
		addChildGameObject(auxCam);

		// set camera position
		auxCam->setPosition(vec3(0.0f, 100.0f, -20.0f)); // high above the scene
		auxCam->rotateTo(vec3(0.0f, -1.0f, 0.0f)); // point downward

		auto auxCamComp = std::make_shared<CameraComponent>();
		auxCamComp->setViewPort(0.6, 0.7, 0.4f, 0.3f);
		auxCam->addComponent(auxCamComp);

		std::shared_ptr<SkyBoxComponent> skybox1 = std::make_shared<SkyBoxComponent>(faces);
		auxCam->addComponent(skybox1);
		auxCamComp->setSkyBox(skybox1);

		// ****** Player *********
		auto player = std::make_shared<GameObject>();
		addChildGameObject(player);
		player->addChildGameObject(mainCam);
		player->setPosition(vec3(0.0f, 0.0f, 0.0f));
		player->addComponent(std::make_shared<ArrowRotateComponent>());

		// set camera position
		mainCam->setPosition(vec3(0.0f, 10.0f, 60.0f), LOCAL);
		mainCamComp->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		mainCam->rotateTo(vec3(0.0f, 0.0f, 1.0f)); // Point towards the player or the scene

		std::shared_ptr<SkyBoxComponent> skybox = std::make_shared<SkyBoxComponent>(faces);
		mainCam->addComponent(skybox);
		mainCamComp->setSkyBox(skybox);

		// add player's gun mesh
		// CONTROL THE GUN WITH WASD KEYS
		gun = std::make_shared<GameObject>();
		gun->addComponent(std::make_shared<WASDRotateComponent>());
		player->addChildGameObject(gun);

		auto gunMesh = std::make_shared<ModelMeshComponent>("Assets/0815_01_OBJ/0815_01.obj", shaderProgram);
		gun->addComponent(gunMesh);
		gun->setPosition(vec3(0.0f, 0.0f, 0.0f));
		gun->setScale(vec3(1.5f, 1.5f, 1.5f));
		gun->rotateTo(vec3(0.0f, 0.0f, 1.0f));
		
		// add a spotlight to the gun
		auto gunLight = std::make_shared<GameObject>();
		gun->addChildGameObject(gunLight);
		addChildGameObject(gunLight);
		auto spotlight = std::make_shared<SpotLightComponent>(GLFW_KEY_G);
		spotlight->setCutoffAngleInDegrees(90);
		gunLight->addComponent(spotlight);

		// ****** Targets *********
			
		// ***** Penguin ******
		auto penguinGO = std::make_shared<GameObject>();
		addChildGameObject(penguinGO);
		penguinGO->setPosition(vec3(5.0f, 10.0f, -50.0f), WORLD);
		penguinGO->setScale(vec3(6.0f, 6.0f, 6.0f));
		penguinGO->gameObjectName = "penguin";

		// mesh
		auto penguinMesh = std::make_shared<ModelMeshComponent>("Assets/02_TEXTURE_MAP_penguin/01_OBJ_penguin/penguin.obj", shaderProgram);
		penguinGO->addComponent(penguinMesh);
		penguinGO->addComponent(std::make_shared<SpinComponent>(UNIT_Y_V3, 20.0f));
		
		// physics
		auto penguinrb = std::make_shared<RigidBodyComponent>(penguinMesh, KINEMATIC);
		penguinrb->setGravityOn(false);
		penguinGO->addComponent(penguinrb);
		penguinGO->addComponent(std::make_shared<CollisionComponent>());

		// penguin has a positional light source at its location
		std::shared_ptr<GameObject> penguinLight = std::make_shared<GameObject>();
		addChildGameObject(penguinLight);
		penguinLight->setPosition(penguinGO->getPosition());
		penguinLight->addComponent(std::make_shared<PositionalLightComponent>(GLFW_KEY_1));
		penguinGO->addChildGameObject(penguinLight);

		// ***** Cat ******
		auto catOrbitPenguin = std::make_shared<GameObject>();
		addChildGameObject(catOrbitPenguin);

		auto catGO = std::make_shared<GameObject>();
		catOrbitPenguin->addChildGameObject(catGO);

		auto cat = std::make_shared<ModelMeshComponent>("Assets/Cat/cat.obj", shaderProgram);
		catGO->addComponent(cat);
		catGO->setScale(vec3(0.2f, 0.2f, 0.2f));
		catGO->setPosition(vec3(0.0f, 0.0f, -20.0f), LOCAL);
		catGO->gameObjectName = "cat";

		catOrbitPenguin->setPosition(vec3(5.0f, 10.0f, -50.0f), WORLD);
		catOrbitPenguin->addComponent(std::make_shared<SpinComponent>(UNIT_Y_V3, 10.0f));

		// physics
		auto catRb = std::make_shared<RigidBodyComponent>(cat, KINEMATIC);
		catGO->addComponent(catRb);
		catGO->addComponent(std::make_shared<CollisionComponent>());

		//***** Spaceship travels to the penguin, the cat, and towards me *****
		auto spaceshipObj = std::make_shared<GameObject>();
		addChildGameObject(spaceshipObj);

		std::shared_ptr<ModelMeshComponent> spaceshipMesh = std::make_shared<ModelMeshComponent>("Assets/jet_models/F-15C_Eagle.obj", shaderProgram);
		spaceshipObj->addComponent(spaceshipMesh);

		spaceshipObj->setPosition(vec3(-5.0f, 20.0f, -100.0f), WORLD);
		spaceshipObj->setScale(vec3(0.5f, 0.5f, 0.5f));

		std::vector<std::weak_ptr<GameObject>> waypoints = { std::weak_ptr(penguinGO), std::weak_ptr(catGO), std::weak_ptr(player) };

		std::shared_ptr<SteeringComponent> stComp = std::make_shared<SteeringComponent>(waypoints);
		spaceshipObj->addComponent(stComp);

		spaceshipObj->addComponent(std::make_shared<RigidBodyComponent>(spaceshipMesh, KINEMATIC));
		spaceshipObj->gameObjectName = "jet";
		spaceshipObj->addComponent(std::make_shared<CollisionComponent>());

		// Background sound
		auto background = std::make_shared<GameObject>();
		addChildGameObject(background);

		auto backgroundSound = std::make_shared<SoundSourceComponent>("Assets/seagull.wav");
		backgroundSound->setLooping(true);
		backgroundSound->play();
		background->addComponent(backgroundSound);

	} // end loadScene

private:
	GLuint shaderProgram;
	bool shootingKeyDown;
	std::shared_ptr<GameObject> gun;
};
