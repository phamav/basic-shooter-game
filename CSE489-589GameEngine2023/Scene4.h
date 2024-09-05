#pragma once

#include "GameEngine.h"

class Scene4 : public Game
{

	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 4");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// Build the shader program
		GLuint shaderProgram = BuildShaderProgram(shaders);

		glUseProgram(shaderProgram);

		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Directional light *********

		std::shared_ptr<class GameObject> directionaLightGO = std::make_shared<GameObject>();
		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		std::shared_ptr<DirectionalLightComponent> dlc = std::make_shared<DirectionalLightComponent>(GLFW_KEY_D);
		dlc->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		dlc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setEnable(false);
		directionaLightGO->addComponent(dlc);

		// ****** Sun *********

		std::shared_ptr<class GameObject> sphereGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(sphereGameObject);
		sphereGameObject->setPosition(vec3(0.0f, 0.0f, 0.0f), WORLD);

		Material sphereMat;
		sphereMat.setEmissiveMat(YELLOW_RGBA);
		std::shared_ptr<SphereMeshComponent> sphereMesh = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMat, 10.0f);
		sphereGameObject->addComponent(sphereMesh);
		//sphereGameObject->addComponent(std::make_shared<SpinComponent>());

		std::shared_ptr<LightComponent> pointlight = std::make_shared<PositionalLightComponent>(GLFW_KEY_P);
		pointlight->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		pointlight->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);
		sphereGameObject->addComponent(pointlight);

		// ****** Earth *********

		std::shared_ptr<class GameObject> earthGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(earthGameObject);
		earthGameObject->setPosition(vec3(-50.0f, 0.0f, 0.0f), WORLD);

		Material earthMat;
		earthMat.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		std::shared_ptr<SphereMeshComponent> earthMesh = std::make_shared<SphereMeshComponent>(shaderProgram, earthMat, 5.0f);
		earthGameObject->addComponent(earthMesh);
		//arthGameObject->addComponent(std::make_shared<SpinComponent>());


		// ****** Camera *********

		std::shared_ptr<class GameObject> cameraGameObject2 = std::make_shared<GameObject>();
		this->addChildGameObject(cameraGameObject2);

		cameraGameObject2->setPosition(vec3(0.0f, 300.0f, 0.0f), WORLD);
		cameraGameObject2->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3), WORLD);

		std::shared_ptr<CameraComponent> camera2 = std::make_shared<CameraComponent>(150);
		camera2->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraGameObject2->addComponent(camera2);

	} // end loadScene

};