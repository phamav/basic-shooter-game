#include "DirectionalLightComponent.h"

void DirectionalLightComponent::update(const float& deltaTime)
{
    vec3 direction = owningGameObject->getFowardDirection(WORLD);

    SharedLighting::setPositionOrDirection(lightIndex, vec4(-normalize(direction), 0.0f));

} // end update
