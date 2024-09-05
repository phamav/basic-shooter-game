#include "WASDRotateComponent.h"
#include "SharedTransformations.h"

WASDRotateComponent::WASDRotateComponent(float rotRateDegreesPerSec, int updateOrder)
	:Component(updateOrder), radianRotRate(glm::radians(rotRateDegreesPerSec))
{

}

void WASDRotateComponent::update(const float& deltaTime)
{
	mat4 rot = owningGameObject->getRotation();

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A)) {

		rot *= glm::rotate(radianRotRate * deltaTime, UNIT_Y_V3);
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D)) {

		rot *= glm::rotate(-radianRotRate * deltaTime, UNIT_Y_V3);

	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S)) {

		rot *= glm::rotate(-radianRotRate * deltaTime, UNIT_X_V3);

	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W)) {

		rot *= glm::rotate(radianRotRate * deltaTime, UNIT_X_V3);

	}

	this->owningGameObject->setRotation(rot);
}

void WASDRotateComponent::processInput() {

}