#pragma once
#include "Component.h"

#include "SharedLighting.h"

/**
 * @class	LightComponent - Supports all properties that are shared by all
 * 			types of light light sources. Super class for PointLightComponent,
 * 			DirectionalLightComponent, and SpotLightComponenet.
 *
 * @brief	A light component.
 */
class LightComponent : public Component
{
public:

    LightComponent(int controlKey);

    ~LightComponent();

    virtual void update(const float& deltaTime) override = 0;

    void setAmbientColor(vec4 ambColor);

    void setDiffuseColor(vec4 difColor);

    void setSpecularColor(vec4 specColor);

    void setEnable(bool onOff);

    bool getEnable();
    
    virtual void processInput() override;

    protected:

    /** @brief	Zero-based index of the specific light */
    int lightIndex;

    /** @brief	The key used to toggle the light on and off*/
    int toggleKey;

    /** @brief	Tracks state of the toggle key to detect each press once.*/
    bool toggleKeyDown = false;

};

