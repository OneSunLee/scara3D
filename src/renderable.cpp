#include "renderable.h"

Renderable::Renderable(Model* model) : _model(model) {
    _boundary = model->boundBox();
}

Renderable::Renderable(Model* model, glm::vec3 position) : _model(model), _position(position) {
    _boundary = model->boundBox();
}

Renderable::~Renderable() {
    if (_light != nullptr) {
        delete _light;
        _light = nullptr;
    }
}

void Renderable::setModel(Model* model) {
    _model = model;
    _boundary = model->boundBox();
}

void Renderable::move(glm::vec3 deltaVec) {
    _position += deltaVec;
}

void Renderable::setPosition(glm::vec3 position) {
    _position = position;
}

void Renderable::rotate(glm::vec3 axis, float deltaAngle) {
    _rotation = glm::rotate(_rotation, glm::radians(deltaAngle), axis);
}

void Renderable::setRotation(glm::vec3 axis, float angle) {
    _rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
}

void Renderable::scale(float deltaScale) {
    _scale += deltaScale;
}

void Renderable::setScale(float scale) {
    _scale = glm::vec3(scale);
}

ScopedLight Renderable::transformedLight() const {
    // 光源位置转换到世界坐标
    return _light->toWorldSpace(modelMatrix());
}

ScopedLight* Renderable::originalLight() const {
    return _light;
}

void Renderable::makeLight() {
    _light = new ScopedLight(glm::vec3(0.0f));
}

void Renderable::disposeLight() {
    if (_light != nullptr) {
        delete _light;
        _light = nullptr;
    }
}

void Renderable::render(ShaderProgram shader) {
    // 变量检查
    if (_model == nullptr) {
        Logger::error("Tries to render an uninitialized renderable object");
        return;
    }
    // 设置model矩阵
    shader.setUniform("model", modelMatrix());

    // 渲染
    _model->render(shader);
}

void Renderable::updateBoundary() {
    // 遍历索引来测试每个三角形...
    Boundary newBoundary;
    glm::mat4 model = modelMatrix();
    for (auto& mesh : _model->meshes()) {
        for (auto& vertex : mesh.vertices()) {
            glm::vec4 transformedVertex = model * glm::vec4(vertex._position, 1.0f);
            newBoundary.updateControlPoints(glm::vec3(transformedVertex));
        }
    }
    _boundary = newBoundary;
    Logger::debug("Boundary updated");
    Logger::debug("[+] Bottom control point: " + std::to_string(newBoundary.bottomControlPoint().x) + ", " + std::to_string(newBoundary.bottomControlPoint().y) + ", " + std::to_string(newBoundary.bottomControlPoint().z));
    Logger::debug("[+] Top control point: " + std::to_string(newBoundary.topControlPoint().x) + ", " + std::to_string(newBoundary.topControlPoint().y) + ", " + std::to_string(newBoundary.topControlPoint().z));
}

HitRecord Renderable::hit(const Ray& ray) const {
    // 先判断是否光线打到boundary box
    if (!_boundary.hit(ray)) {
        return HitRecord();
    }
    else {
        // 再判断是否拾取模型
        return _model->hit(ray, modelMatrix());
    }
}
