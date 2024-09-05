#pragma once

#include "GameEngine.h"

class Scene2 : public Game
{

	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 2");

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

		std::vector<string> faces
		{
			"Assets/Cube Map/right.jpg",
			"Assets/Cube Map/left.jpg",
			"Assets/Cube Map/top.jpg",
			"Assets/Cube Map/bottom.jpg",
			"Assets/Cube Map/back.jpg",
			"Assets/Cube Map/front.jpg"
		};

		// ****** Point light *********

		std::shared_ptr<class GameObject> pointLightGO = std::make_shared<GameObject>();
		addChildGameObject(pointLightGO);
		pointLightGO->setPosition(vec3(0.0f, 10.0f, 0.0f), WORLD);

		std::shared_ptr<LightComponent> pointlight = std::make_shared<PositionalLightComponent>(GLFW_KEY_P);
		pointlight->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		pointlight->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);
		pointLightGO->addComponent(pointlight);

		// ****** Directional light *********

		std::shared_ptr<class GameObject> directionaLightGO = std::make_shared<GameObject>();
		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		std::shared_ptr<DirectionalLightComponent> dlc = std::make_shared<DirectionalLightComponent>(GLFW_KEY_D);
		dlc->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		dlc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setEnable(true);
		directionaLightGO->addComponent(dlc);

		// ****** boxGameObject *********

		std::shared_ptr<class GameObject> boxGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(boxGameObject);
		boxGameObject->setPosition(vec3(100.0f, 0.0f, 0.0f), WORLD);

		Material boxMat;
		boxMat.setDiffuseMat(vec4(1.0f, 0.0f, 1.0f, 1.0f));

		std::shared_ptr<BoxMeshComponent> boxMesh = std::make_shared<BoxMeshComponent>(shaderProgram, boxMat, 10.0f, 10.0f, 10.0f);

		boxGameObject->addComponent(boxMesh);
		boxGameObject->addComponent(std::make_shared<RigidBodyComponent>(boxMesh, STATIONARY));

		boxGameObject->gameObjectName = "box - STATIONARY";
		boxGameObject->addComponent(std::make_shared<CollisionComponent>());

		// ****** sphereGameObject *********

		std::shared_ptr<class GameObject> sphereGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(sphereGameObject);
		sphereGameObject->setPosition(vec3(-100.0f, 0.0f, 0.0f), WORLD);

		Material sphereMat;
		sphereMat.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		std::shared_ptr<SphereMeshComponent> sphereMesh = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMat, 5.0f);
		sphereGameObject->addComponent(sphereMesh);

		std::shared_ptr <RigidBodyComponent> rigidBody = std::make_shared<RigidBodyComponent>(sphereMesh, STATIONARY);
		sphereGameObject->addComponent(rigidBody);

		sphereGameObject->gameObjectName = "earth - STATIONARY";
		sphereGameObject->addComponent(std::make_shared<CollisionComponent>());

		// ****** sphereGameObject2 *********

		std::shared_ptr<class GameObject> sphereGameObject2 = std::make_shared<GameObject>();
		this->addChildGameObject(sphereGameObject2);
		sphereGameObject2->setPosition(vec3(0.0f, 0.0f, -50.0f), WORLD);

		Material sphereMat2;
		sphereMat2.setDiffuseMat(vec4(0.0f, 0.0f, 1.0f, 1.0f));
		std::shared_ptr<SphereMeshComponent> sphereMesh2 = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMat2, 5.0f, 24, 32);
		sphereGameObject2->addComponent(sphereMesh2);

		std::shared_ptr<RigidBodyComponent> sgorg2 = std::make_shared<RigidBodyComponent>(sphereMesh2, STATIONARY, 1000.0f);
		sgorg2->setGravityOn(false);
		sphereGameObject2->addComponent(sgorg2);

		sphereGameObject2->gameObjectName = "blue sphere - DYNAMIC";
		sphereGameObject2->addComponent(std::make_shared<CollisionComponent>());

		// ****** cylinderGameObject *********

		std::shared_ptr<class GameObject> cylinderGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(cylinderGameObject);
		cylinderGameObject->setPosition(vec3(0.0f, 0.0f, 50.0f), WORLD);

		Material cylinderMat3;
		cylinderMat3.setDiffuseMat(vec4(1.0f, 1.0f, 0.0f, 1.0f));
		std::shared_ptr<CylinderMeshComponent> cylinder = std::make_shared<CylinderMeshComponent>(shaderProgram, cylinderMat3, 2.0f, 4.0f);
		cylinderGameObject->addComponent(cylinder);

		cylinderGameObject->gameObjectName = "cylinder without a rigid body";
		cylinderGameObject->addComponent(std::make_shared<CollisionComponent>());

		// ****** Cameras *********

		std::shared_ptr<class GameObject> cameraGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(cameraGameObject);
		cameraGameObject->setPosition(cylinderGameObject->getPosition(WORLD), WORLD);

		std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>();
		camera->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraGameObject->addComponent(camera);

		std::shared_ptr<SkyBoxComponent> skybox = std::make_shared<SkyBoxComponent>(faces);
		cameraGameObject->addComponent(skybox);
		camera->setSkyBox(skybox);

		std::shared_ptr<SoundListenerComponent> listen = std::make_shared<SoundListenerComponent>();
		cameraGameObject->addComponent(listen);


		std::shared_ptr<class GameObject> cameraGameObject2 = std::make_shared<GameObject>();
		this->addChildGameObject(cameraGameObject2);

		cameraGameObject2->setPosition(vec3(0.0f, 350.0f, 0.0f), WORLD);
		cameraGameObject2->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3));

		std::shared_ptr<CameraComponent> camera2 = std::make_shared<CameraComponent>(150);
		camera2->setViewPort(0.5f, 0.4f, 0.4f, 0.5f);
		cameraGameObject2->addComponent(camera2);

		// ****** Jet *********

		std::shared_ptr<class GameObject> jetGameObject = std::make_shared<GameObject>();

		this->addChildGameObject(jetGameObject);
		jetGameObject->setPosition(vec3(0.0f, 0.0f, 0.0f), WORLD);
		jetGameObject->setScale(vec3(0.25f, 0.25f, 0.25f), LOCAL);

		std::shared_ptr<ModelMeshComponent> jetMesh = std::make_shared<ModelMeshComponent>("Assets/jet_models/F-15C_Eagle.obj", shaderProgram);
		jetGameObject->addComponent(jetMesh);

		std::vector<std::weak_ptr<class GameObject>> waypoints = { std::weak_ptr(boxGameObject), std::weak_ptr(cylinderGameObject), std::weak_ptr(sphereGameObject), std::weak_ptr(sphereGameObject2) };

		std::shared_ptr<SteeringComponent> stComp = std::make_shared<SteeringComponent>(waypoints);
		jetGameObject->addComponent(stComp);

		std::shared_ptr<SoundSourceComponent> jetSound = std::make_shared<SoundSourceComponent>("Assets/airplane-interior-2.wav", true);
		jetSound->setLooping(true);
		jetSound->play();
		jetGameObject->addComponent(jetSound);

		jetGameObject->addComponent(std::make_shared<RigidBodyComponent>(jetMesh, KINEMATIC));
		jetGameObject->gameObjectName = "jet - KINEMATIC";
//		jetGameObject->addComponent(std::make_shared<CollisionComponent>());
		
		jetGameObject->addComponent(std::make_shared<ModelMakerComponent>(shaderProgram, "Assets/Dinosaur/Trex.obj"));

		// Add a spot light
		std::shared_ptr<class GameObject> spotLightGO = std::make_shared<GameObject>();
		jetGameObject->addChildGameObject(spotLightGO);
		spotLightGO->setPosition(vec3(0.0f, 0.0f, -8.0f), LOCAL);

		std::shared_ptr<SpotLightComponent> splc = std::make_shared<SpotLightComponent>(GLFW_KEY_S);
		splc->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		splc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		splc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		splc->setEnable(true);
		splc->setCutoffAngleInDegrees(15.0f);
		spotLightGO->addComponent(splc);

		// ****** Follow Camera *********

		std::shared_ptr<class GameObject> followCameraGameObject = std::make_shared<GameObject>();
		jetGameObject->addChildGameObject(followCameraGameObject);
		followCameraGameObject->setPosition(vec3(0.0f, 3.0f, 30.0f), LOCAL);

		std::shared_ptr<CameraComponent> followCamera = std::make_shared<CameraComponent>();
		followCamera->setViewPort(0.1f, 0.6f, 0.3f, 0.3f);
		followCameraGameObject->addComponent(followCamera);

		std::shared_ptr<SkyBoxComponent> skybox1 = std::make_shared<SkyBoxComponent>(faces);
		followCameraGameObject->addComponent(skybox1);
		followCamera->setSkyBox(skybox1);

	} // end loadScene

};