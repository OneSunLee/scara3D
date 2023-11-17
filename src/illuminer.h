#pragma once

#include "../include/GLM/glm.hpp"
#include <string>
#include <queue>

#include "shader.h"

class Illuminer {
protected:
    glm::vec3 _lightColor;

public:
    Illuminer(glm::vec3 color);
    ~Illuminer();
    
protected:
    virtual glm::vec3 ambientLightColor() const = 0;
    virtual glm::vec3 diffuseLightColor() const = 0;
    virtual glm::vec3 specularLightColor() const = 0;

    virtual void updateShader(ShaderProgram shader, int index) const = 0;
    
public:
    glm::vec3 lightColor() const { return _lightColor; }
    void setLightColor(glm::vec3 lightColor) { _lightColor = lightColor; }
};

// 平行光
class DirLight : public Illuminer{
protected:
    glm::vec3 _direction;   // 光的方向
    float _intensity = 50;
    
public:
    DirLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
    ~DirLight();
    
protected:
    virtual glm::vec3 ambientLightColor() const override { return glm::vec3(_intensity / 200.0f) * _lightColor; }
    virtual glm::vec3 diffuseLightColor() const override { return glm::vec3(_intensity / 100.0f) * _lightColor; }
    virtual glm::vec3 specularLightColor() const override { return glm::vec3(1.0f) * _lightColor; }
    
public:
    glm::vec3 lightDirection() const { return _direction; }  // 光是平行的
    void setLightDirection(glm::vec3 direction) { _direction = direction; }
    void setIntensity(float intensity) { _intensity = intensity; }
    
    // 渲染Render util function
    virtual void updateShader(ShaderProgram shader, int index) const override;
};

// 范围光是点光源和聚光灯的结合体，用phi值判断使用哪个着色器
// 小于162.5，聚光灯，反之它被考虑为点光源
class ScopedLight : public Illuminer{
protected:
    // Light source status
    glm::vec3 _position;
    glm::vec3 _direction;
    float _cutOffAngle = 180.0f;
    
    // 光源属性
    int _idealDistance = 32;     //理想举例
    const float _attConstant = 1.0f;   // 衰减常数项
    float _attLinear = 0.14f;   // 衰减一次项
    float _attQuadratic = 0.07f;    // 衰减二次项
    
public:
    ScopedLight(glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    ScopedLight(int distance, glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    ScopedLight(glm::vec3 position, glm::vec3 direction, float cutOffAngle, int idealDistance, float linear, float quadratic, glm::vec3 color);
    ~ScopedLight();

private:
    // 更新衰减系数
    inline void updateLinear();
    inline void updateQuadratic();
    inline float innerCutOffAngle() const;

protected:
    virtual glm::vec3 ambientLightColor() const override { return glm::vec3(0.2f) * _lightColor; }
    virtual glm::vec3 diffuseLightColor() const override { return glm::vec3(0.7f) * _lightColor; }
    virtual glm::vec3 specularLightColor() const override { return glm::vec3(1.0f) * _lightColor; }

public:
    // 属性的set和get
    int idealDistance() const { return _idealDistance; }
    void setIdealDistance(int distance);
    glm::vec3 lightDirection() const { return _direction; }
    void setLightDirection(glm::vec3 direction) { _direction = direction; }
    float cutOffAngle() const { return _cutOffAngle; }
    void setCutOffAngle(float angle);
    
    bool isPointLight() const { return abs(_cutOffAngle - 180.0f) < 1e-6; }

    ScopedLight toWorldSpace(glm::mat4 modelMatrix) const;

    // 更新着色器
    virtual void updateShader(ShaderProgram shader, int index) const override;
};
