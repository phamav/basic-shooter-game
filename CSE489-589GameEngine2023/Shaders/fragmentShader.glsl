// Targeting version 4.5 of GLSL. If the compiler does not support 4.5 it will cause an error.
#version 450 core

in vec3 worldPos;
in vec3 worldNorm;
in vec2 texCoord0;
in mat3 TBN;
out vec4 fragmentColor;

const float gamma = 2.2;

const int MaxLights = 16;

// Structure for holding general light properties
struct GeneralLight
{
	vec4 ambientColor;		// ambient color of the light
	vec4 diffuseColor;		// diffuse color of the light
	vec4 specularColor;		// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional and direction specified 
	// is the negative of the direction the light is shinning
	// if w = 1 then the light is positional
	vec4 positionOrDirection;

	// Spotlight attributes
	vec3 spotDirection;		// the direction the cone of light is shinning    
	bool isSpot;				// 1 if the light is a spotlight  
	float spotCutoffCos;	// Cosine of the spot cutoff angle
	float spotExponent;		// For gradual falloff near cone edge

	// Attenuation coefficients
	float constant;
	float linear;
	float quadratic;

	bool enabled;			// true if light is "on"

};

layout(shared) uniform LightBlock
{
	GeneralLight lights[MaxLights];
};

layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};


struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat;
	float specularExp;
	int textureMode;
	bool diffuseTextureEnabled;
	bool specularTextureEnabled;
	bool normalMapTextureEnabled;
};

layout(shared) uniform MaterialBlock
{
	Material object;
};

//layout(shared) uniform FogBlock
//{
//	vec4 fogColor;
//	float fogEnd;
//	float fogStart;
//	float fogDensity;
//	unsigned int fogMode; // 0 no fog, 1 linear, 2 exponential, 3 exponential two
//}

layout(binding = 0) uniform sampler2D diffuseSampler;
layout(binding = 1) uniform sampler2D specularSampler;
layout(binding = 2) uniform sampler2D normalMapSampler;

void main()
{
	vec4 totalColor = object.emmissiveMat;

	vec4 ambientColor = object.ambientMat;

	vec4 diffuseColor = object.diffuseMat;

	vec4 specularColor = object.specularMat;

	vec3 fragWorldNormal = normalize(worldNorm);

	if (object.normalMapTextureEnabled) {

		vec3 normal = texture(normalMapSampler, texCoord0).xyz;
		normal = normalize(normal * 2.0f - 1.0f);
		fragWorldNormal = normalize(TBN * normal);
	}

	if(object.textureMode != 0 && object.diffuseTextureEnabled) {

		diffuseColor = texture( diffuseSampler, texCoord0 );
		ambientColor = diffuseColor;
	 }

	 if(object.textureMode != 0 && object.specularTextureEnabled) {

		specularColor = texture( specularSampler, texCoord0 );
	 }

	 if(object.textureMode != 1) {

		 for(int i = 0; i < MaxLights ; i++) {

			 if(lights[i].enabled == true ) {

				vec3 lightVector;

				float attenuation;

				float spotCosine = 1.0;

				float fallOffFactor = 1.0f;

				if(lights[i].positionOrDirection.w < 0.98f) { // Directional
					
					// Normalize the light vector (points towards a light source that is an 
					// "infinite" distance away and has no potition
					lightVector = normalize(lights[i].positionOrDirection.xyz);

					// No attenuation for directional lights
					attenuation = 1.0f;
				}
				else { // Positional
					
					// Calculate the light vector					
					lightVector = normalize(lights[i].positionOrDirection.xyz - worldPos);

					// Calculate the distance to the light source
					float distanceToLight = distance(lights[i].positionOrDirection.xyz, worldPos);

					// Calculate the attenuation to weakend the light with distance
					attenuation = 1.0f / (lights[i].constant + lights[i].linear *distanceToLight + lights[i].quadratic * distanceToLight * distanceToLight);

					if (lights[i].isSpot == true) {
						
						// Calculate the spot cosine used to determine if in the spotlight cone
						spotCosine = dot(-lightVector, normalize(lights[i].spotDirection));

						// Caclate the fallOffFactor used to "blur" the edges of the spotlight cone
						fallOffFactor = pow(spotCosine, lights[i].spotExponent);  
						//fallOffFactor = clamp( 1.0f - (1.0f - spotCosine) / (1.0f - lights[i].spotCutoffCos), 0.0f, 1.0f);
					}
				}

				// Is it a spot light and are we in the cone?
				if ( lights[i].isSpot == false || (lights[i].isSpot == true && spotCosine >= lights[i].spotCutoffCos) ) {

					// Ambient reflection
					totalColor += attenuation * fallOffFactor * ambientColor * lights[i].ambientColor;

					// Diffuse reflection
					totalColor += attenuation * fallOffFactor * max(dot(fragWorldNormal, lightVector), 0.0f) * diffuseColor * lights[i].diffuseColor;

					// Specular reflection

					vec3 viewVector = normalize(worldEyePosition - worldPos);

					// Phong
					vec3 reflection = normalize(reflect(-lightVector, fragWorldNormal));
					totalColor += attenuation * fallOffFactor * pow(max(dot(reflection, viewVector),0.0f),object.specularExp) * specularColor * lights[i].specularColor;

					// Blinn - Phong
					//vec3 halfVector = normalize(lightVector + viewVector);
					//totalColor += attenuation * fallOffFactor * pow(max(dot(fragWorldNormal, halfVector),0.0f),object.specularExp) * specularColor * lights[i].specularColor;
				}
			}
		}
	}
	else {

		totalColor = diffuseColor;
	}
	fragmentColor = totalColor;

	// Gamma Correction 
	//fragmentColor.rgb = pow(fragmentColor.rgb, vec3(1.0f/ gamma));

}