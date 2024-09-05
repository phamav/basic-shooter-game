#pragma once

#include "GameEngine.h"

class Scene1 : public Game
{
	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 1");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build and use the shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		GLuint shaderProgram = BuildShaderProgram(shaders);

		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Directional light *********

		std::shared_ptr<class GameObject> directionaLightGO = std::make_shared<GameObject>();

		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		std::shared_ptr<DirectionalLightComponent> dlc = std::make_shared<DirectionalLightComponent>(GLFW_KEY_D);
		dlc->setAmbientColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));
		dlc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setEnable(false);
		directionaLightGO->addComponent(dlc);

		// ****** Camera *********

		std::shared_ptr<class GameObject> cameraObject = std::make_shared<GameObject>();

		addChildGameObject(cameraObject);
		cameraObject->setPosition(vec3(0.0f, 0.0f, 0.0f), WORLD);

		std::shared_ptr<CameraComponent> cam = std::make_shared<CameraComponent>();
		cam->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraObject->addComponent(cam);

		cameraObject->addComponent(std::make_shared< ModelMakerComponent>(shaderProgram, "Assets/Dinosaur/Trex.obj"));

		// ****** Falling Dinosaurs  *********

		for (int i = 0; i < 10; i++) {

			std::shared_ptr<class GameObject> dinoObject = std::make_shared<GameObject>();

			this->addChildGameObject(dinoObject);
			std::shared_ptr<ModelMeshComponent> dino = std::make_shared<ModelMeshComponent>("Assets/Dinosaur/Trex.obj", shaderProgram);
			dinoObject->addComponent(dino);
			dinoObject->setPosition(vec3(rand() % 10 - 5.0f, 100.0f + rand() % 50 - 25.0f, rand() % 10 - 5.0f -85.0f), WORLD);
			dinoObject->addComponent(std::make_shared< RemoveComponent>(15.0f));
			dinoObject->addComponent(std::make_shared< RigidBodyComponent>(dino, DYNAMIC));
			//dinoObject->addComponent(std::make_shared< CollisionComponent());
		}

		// ****** Red Sphere  *********

		// Add a game object to hold a red sphere
		std::shared_ptr<class GameObject> sphereObject = std::make_shared<GameObject>();

		addChildGameObject(sphereObject);
		sphereObject->setPosition(vec3(1.0f, 0.0f, 0.0f), WORLD);

		// Create a red material for the sphere
		Material sphereMaterial;
		sphereMaterial.setAmbientAndDiffuseMat(vec4(1.0f, 0.0f, 0.0f, 1.0f));
		sphereMaterial.setSpecularMat(vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Create the sphere mesh
		std::shared_ptr<SphereMeshComponent> sphereMesh = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMaterial, 1.0f, 16, 32);
		sphereObject->addComponent(sphereMesh);

		std::shared_ptr<RigidBodyComponent> rg = std::make_shared<RigidBodyComponent>(sphereMesh, DYNAMIC, 1000.0f);
		sphereObject->addComponent(rg);
		rg->setVelocity(20.0f * vec3(0.0f, 1.0f, -1.0f));
		rg->setGravityOn(true);

		//sphereObject->addComponent(std::make_shared< CollisionComponent());
		sphereObject->addComponent(std::make_shared< RemoveComponent>());

		// Add a point light that will be inside the sphere
		std::shared_ptr<LightComponent> pointlight = std::make_shared< PositionalLightComponent>(GLFW_KEY_P);
		pointlight->setAmbientColor(vec4(0.1f, 0.1f, 0.1f, 1.0f));
		pointlight->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);

		sphereObject->addComponent(pointlight);
		
		// ****** Blue Sphere  *********
		std::shared_ptr<class GameObject> sphereObject2 = std::make_shared<GameObject>();

		addChildGameObject(sphereObject2);
		sphereObject2->setPosition(vec3(0.0f, 0.0f, -85.0f), WORLD);

		Material sphereMaterial2;
		sphereMaterial2.setAmbientAndDiffuseMat(vec4(0.0f, 0.0f, 1.0f, 1.0f));

		std::shared_ptr<SphereMeshComponent> sphereMesh2 = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMaterial2, 10.0f, 16, 32);
		sphereObject2->addComponent(sphereMesh2);

		std::shared_ptr<RigidBodyComponent> rg2 = std::make_shared<RigidBodyComponent>(sphereMesh2, STATIONARY);
		sphereObject2->addComponent(rg2);

	}
};
