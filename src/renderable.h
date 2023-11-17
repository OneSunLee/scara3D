#pragma once

#include "../include/GLM/glm.hpp"
#include "../include/GLM/ext/matrix_transform.hpp"
#include "../include/GLM/gtx/quaternion.hpp"

#include "model.h"
#include "illuminer.h"
#include "shader.h"
#include "boundary.h"
#include "hitrecord.h"

class Renderable {
public:
    static Renderable empty() {
        return Renderable();
    }

private:
    Renderable() {}

private:
    Model* _model = nullptr;
    ScopedLight* _light = nullptr;
    glm::vec3 _position = glm::vec3(0.0f);
    glm::mat4 _rotation = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f);
    Boundary _boundary; // 渲染物品的边界盒子，在矩阵转换完成后应该被更新

public:
    Renderable(Model* model);
    Renderable(Model* model, glm::vec3 position);
    ~Renderable();

public:
    glm::vec3 position() const { return _position; }
    glm::mat4 rotation() const { return _rotation; }
    glm::vec3 scaleVal() const { return _scale; }
    Model* model() const { return _model; }
    
    void setModel(Model* model);
    void move(glm::vec3 deltaVec);
    void setPosition(glm::vec3 position);
    void rotate(glm::vec3 axis, float deltaAngle);
    void setRotation(glm::vec3 axis, float angle);
    void scale(float deltaScale);
    void setScale(float scale);

    ScopedLight transformedLight() const; // 将灯光对象传给场景管理器求和所有光源
    ScopedLight* originalLight() const; // 将灯光对象传给场景管理器获得光源属性
    bool hasLight() const { return _light != nullptr; }
    void makeLight();   // 创建对象中的光源
    void disposeLight();    // 移除对象中的光源

    const Boundary& boundary() const { return _boundary; }
    
    inline glm::mat4 modelMatrix() const;

public:
    void updateBoundary();  // 更新渲染的边界在变形完成后
    HitRecord hit(const Ray& ray) const;    // 测试输入矩阵的光线拾取记录
    void render(ShaderProgram shader);
};

inline glm::mat4 Renderable::modelMatrix() const {
    //计算旋转方向和角度
    glm::vec3 axis;
    float angle;
    glm::quat quat(_rotation);
    glm::vec3 axisAngle = glm::axis(quat);
    axis = glm::normalize(axisAngle);
    angle = glm::angle(quat);
    //float rotateX = glm::degrees(glm::asin(-_rotation[1][2]));
    //float rotateY = glm::degrees(glm::atan(_rotation[0][2] / _rotation[2][2]));
    //float rotateZ = glm::degrees(glm::atan(_rotation[1][0] / _rotation[1][1]));
    // 计算model矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _position);
    model = glm::rotate(model, angle, axis);
    //model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, _scale);
    return model;
}
