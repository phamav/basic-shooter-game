#include "PositionalLightComponent.h"


void PositionalLightComponent::update(const float& deltaTime)
{
    vec3 position = owningGameObject->getPosition(WORLD);

    SharedLighting::setPositionOrDirection(lightIndex, vec4(position, 1.0f));

} // end update

void PositionalLightComponent::setAttenuationFactors(float constant, float linear, float quadratic)
{
	SharedLighting::setConstantAttenuation(lightIndex, constant);

	SharedLighting::setLinearAttenuation(lightIndex, linear);

	SharedLighting::setQuadraticAttenuation(lightIndex, quadratic);

}