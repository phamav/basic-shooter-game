#version 450 core

layout (location = 0) in vec4 vertexPosition;

out vec3 TexCoords;

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 normalModelMatrix;
};

void main()
{
	// Use the local position as the texture coordinate for (interpolated) 
	// use in the fragment shader.
    TexCoords = vec3(vertexPosition.x, vertexPosition.y, vertexPosition.z);

	vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;

	// Set the z component of the output position equal to its w component 
	// which will result in a z component that is always equal to 1.0.The 
	// resulting normalized device coordinates will then always have a z 
	// value equal to 1.0: the maximum depth value. The skybox will as a 
	// result only be rendered wherever there are no objects visible
	gl_Position = pos.xyww;

}  