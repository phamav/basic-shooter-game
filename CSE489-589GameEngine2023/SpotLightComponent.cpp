#include "SpotLightComponent.h"

void SpotLightComponent::initialize()
{
    SharedLighting::setIsSpot(lightIndex, true);

} // end initialize

void SpotLightComponent::update(const float& deltaTime)
{
    PositionalLightComponent::update(deltaTime); // Set the position of the spot light

    vec3 direction = owningGameObject->getFowardDirection(WORLD);

    SharedLighting::setSpotDirection(lightIndex, direction);

}  // end update

void SpotLightComponent::setCutoffAngleInDegrees(float cutOffInDegrees)
{
    SharedLighting::setSpotCutoffCos(lightIndex, glm::cos(glm::radians(cutOffInDegrees)));

}  // end setCutoffAngleInDegrees