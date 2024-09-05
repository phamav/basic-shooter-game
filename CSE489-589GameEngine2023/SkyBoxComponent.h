#pragma once
#include "Component.h"

// https://www.humus.name/index.php?page=Textures

class SkyBoxComponent : public Component
{
public:

	/**
	 * @fn	SkyBoxComponent::SkyBoxComponent(std::vector<string> faces, int updateOrder = 100);
	 *
	 * @brief	Constructor
	 *
	 * @param 	faces	   	The faces.
	 * @param 	updateOrder	(Optional) The update order.
	 */
	SkyBoxComponent(std::vector<string> faces, int updateOrder = 100);

	~SkyBoxComponent();

	virtual void initialize() override;

	void renderSkyBox();

protected:

	bool loadCubeMap();

	void buildSkyboxShaderProgram();

	bool buildSkyBoxMesh();

	std::vector<string> faces;

	GLuint skyboxTexture = 0;

	unsigned int VAO = 0, VBO = 0;

	static GLuint skyboxShaderProgram;
	
	static bool areSkyboxShadersBuilt;

};

