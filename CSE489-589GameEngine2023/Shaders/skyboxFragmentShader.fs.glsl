#version 450 core

out vec4 fragmentColor;

in vec3 TexCoords;

layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

layout(binding = 0) uniform samplerCube skyboxSampler;

void main()
{    
	// Use the cube map sampler to obtain a texel value.
	fragmentColor = texture(skyboxSampler, TexCoords);
}