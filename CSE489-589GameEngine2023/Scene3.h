#pragma once

#include "GameEngine.h"

class Scene3 : public Game
{

	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 3");

		// Set the clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Build shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// Build the shader program
		GLuint shaderProgram = BuildShaderProgram(shaders);

		// Make the shader program "current" Not needed if transformations are stored in uniform buffers
		//glUseProgram(shaderProgram);

		// Set up uniform blocks
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Point light *********

		GameObjectPtr pointLightGO = std::make_shared<GameObject>();
		addChildGameObject(pointLightGO);
		pointLightGO->setPosition(vec3(-10.0f, 10.0f, -3.0f), WORLD);

		std::shared_ptr<LightComponent> pointlight = std::make_shared<PositionalLightComponent>(GLFW_KEY_P);
		pointlight->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		pointlight->setDiffuseColor(vec4(0.6, 0.6, 0.6f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);
		pointLightGO->addComponent(pointlight);

		// ****** Directional light *********

		GameObjectPtr directionaLightGO = std::make_shared<GameObject>();;
		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		std::shared_ptr<DirectionalLightComponent> dlc = std::make_shared<DirectionalLightComponent>(GLFW_KEY_D);
		dlc->setAmbientColor(vec4(0.1f, 0.1f, 0.1f, 1.0f));
		dlc->setDiffuseColor(vec4(0.75f, 0.75f, 0.75f, 1.0f));
		dlc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setEnable(true);
		directionaLightGO->addComponent(dlc);

		// ****** Spotlight *********
		std::shared_ptr<GameObject> spotLightGO = std::make_shared<GameObject>();;
		this->addChildGameObject(spotLightGO);
		spotLightGO->setPosition(vec3(0.0f, 30.0f, -5.0f), WORLD);
		spotLightGO->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3), WORLD);

		std::shared_ptr<SpotLightComponent> splc = std::make_shared<SpotLightComponent>(GLFW_KEY_S);
		splc->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		splc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		splc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		splc->setEnable(true);
		splc->setCutoffAngleInDegrees(20.0f);
		spotLightGO->addComponent(splc);

		// ****** boxGameObject *********

		GameObjectPtr boxGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(boxGameObject);
		boxGameObject->setPosition(vec3(0.0f, -5.0f, 0.0f), WORLD);

		Material boxMat;
		//boxMat.setDiffuseMat(vec4(1.0f, 1.0f, 1.0f, 1.0f));

		boxMat.setDiffuseTexture(Texture::GetTexture("Textures/wood.png")->getTextureObject());
		std::shared_ptr<BoxMeshComponent> boxMesh = std::make_shared<BoxMeshComponent>(shaderProgram, boxMat,  100.0f, 1.0f, 100.0f);

		boxGameObject->addComponent(boxMesh);
		boxGameObject->addComponent(std::make_shared<RigidBodyComponent>(boxMesh, STATIONARY));
		boxGameObject->gameObjectName = "box - STATIONARY";
	
		//// ****** dinoGameObject *********

		std::shared_ptr<GameObject> dinoObject = std::make_shared<GameObject>();;
		this->addChildGameObject(dinoObject);
		std::shared_ptr<ModelMeshComponent> dino = std::make_shared<ModelMeshComponent>("Assets/Dinosaur/Trex.obj", shaderProgram);
		dinoObject->addComponent(dino);
		dinoObject->setPosition(vec3(4.0f, -3.0f, -2.0f), WORLD);
		dinoObject->addComponent(std::make_shared<RigidBodyComponent>(dino));

		GameObjectPtr dinoObject1 = std::make_shared<GameObject>();;
		this->addChildGameObject(dinoObject1);
		std::shared_ptr<ModelMeshComponent> dino1 = make_shared<ModelMeshComponent>("Assets/Dinosaur/Trex.obj", shaderProgram);
		dinoObject1->addComponent(dino1);
		dinoObject1->setPosition(vec3(-6.0f, 0.0f, 0.0f), WORLD);
		dinoObject1->setRotation(glm::rotate(PI_OVER_2, UNIT_Y_V3), WORLD);
		dinoObject1->addComponent(make_shared<RigidBodyComponent>(dino1));

		//// ****** Cameras *********

		GameObjectPtr cameraGameObject = std::make_shared<GameObject>();;
		this->addChildGameObject(cameraGameObject);
		cameraGameObject->setPosition(vec3(0.0f, 0.0f, 25.0f), WORLD);

		std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>();
		camera->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraGameObject->addComponent(camera);

		//SkyBoxComponent* skybox = new SkyBoxComponent();
		//cameraGameObject->addComponent(skybox);
		//camera->setSkyBox(skybox);

		std::shared_ptr<SoundListenerComponent> listen = std::make_shared<SoundListenerComponent>();
		cameraGameObject->addComponent(listen);

		GameObjectPtr cameraGameObject2 = std::make_shared<GameObject>();
		this->addChildGameObject(cameraGameObject2); 
		cameraGameObject2->setPosition(vec3(30.0f, 2.0f, 30.0f), WORLD);
		cameraGameObject2->setRotation(glm::rotate(PI/4.0f, UNIT_Y_V3));

		std::shared_ptr<CameraComponent> camera2 = std::make_shared<CameraComponent>(150);
		camera2->setViewPort(0.75f, 0.75f, 0.25f, 0.25f);
		cameraGameObject2->addComponent(camera2);

	} // end loadScene

};