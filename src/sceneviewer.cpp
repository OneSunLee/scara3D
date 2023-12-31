#include "sceneviewer.h"

#include <string>
#include <qresource.h>
#include <qurl.h>
#include <qdir.h>

#include "illuminer.h"

#include <qopenglext.h>

SceneViewer::SceneViewer(QWidget* parent)
	: QOpenGLWidget(parent)
{
    // 开启鼠标追踪
    setMouseTracking(true);
    // 看得见按键追踪
    setFocusPolicy(Qt::StrongFocus);
    // 设置默认焦点
    setFocus();

    // OpenGL 初始化
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 3);
    setFormat(format);

    // 创建临时文件夹.temp
    QDir dir;
    if (!dir.exists("./temp"))
    {
        dir.mkdir("./temp");
    }
    if (!dir.exists("./temp/shaders"))
    {
        dir.mkdir("./temp/shaders");
    }
    
    // 将着色器复制到临时文件夹下
    extractShaderResource("vertexshader.glsl");
    extractShaderResource("fragmentshader.glsl");
    extractShaderResource("skyboxvertexshader.glsl");
    extractShaderResource("skyboxfragmentshader.glsl");
    extractShaderResource("terrainvertexshader.glsl");
    extractShaderResource("terrainfragmentshader.glsl");
    extractShaderResource("boundfragmentshader.glsl");
    extractShaderResource("boundvertexshader.glsl");
}

SceneViewer::~SceneViewer() {
    if (_dirLight) {
        delete _dirLight;
    }

    for (auto obj : _objects) {
        delete obj;
    }
}

void SceneViewer::extractShaderResource(const QString& shaderName) {
    QString shaderResourcePath = ":/shaders/" + shaderName;
    QString shaderTempPath = "./temp/shaders/" + shaderName;
    
    if (QFile::exists(shaderTempPath))
    {
        QFile::remove(shaderTempPath);
    }
    QFile::copy(shaderResourcePath, shaderTempPath);
    QFile::setPermissions(shaderTempPath, QFile::ReadOwner | QFile::WriteOwner);
}

Renderable* SceneViewer::hitTest(const Ray& ray) {
    HitRecord newRecord = HitRecord();
    Renderable* newObject = nullptr;
    // Object hit test
    for (int i = 0; i < _objects.size(); i++) {
        Logger::debug("Testing object " + std::to_string(i));
        Renderable* obj = _objects[i];
        if (obj == _operatingObject) {
            // 忽略当前操作对象
            continue;
        }
        HitRecord hitRecord = obj->hit(ray);
        if (hitRecord.hitted()) {
            Logger::debug("Hitted object " + std::to_string(i));
        }
        else {
            Logger::debug("Missed object " + std::to_string(i));
        }
        if (hitRecord.hitted() && hitRecord.t() < newRecord.t()) {
            newRecord = hitRecord;
            newObject = obj;
        }
    }
    // Terrain的光源光线测试
    if (_terrain != nullptr) {
        HitRecord hitRecord = _terrain->hit(ray);
        if (hitRecord.hitted()) {
            Logger::debug("Hitted terrain");
        }
        else {
            Logger::debug("Missed terrain");
        }
        if (hitRecord.hitted() && hitRecord.t() < newRecord.t()) {
            newRecord = hitRecord;
            newObject = nullptr;
        }
    }
    _hitRecord = newRecord;
    return newObject;
}

void SceneViewer::initializeGL() {
    initializeOpenGLFunctions();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
	
    Logger::info("Currently running on OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));

    _shaderProgram.ensureInitialized();
    Logger::info("Shader Program initialized");

    VertexShader vertexShader("./temp/shaders/vertexshader.glsl");
    FragmentShader fragmentShader("./temp/shaders/fragmentshader.glsl");
    _shaderProgram.attachShader(vertexShader);
    _shaderProgram.attachShader(fragmentShader);
    vertexShader.dispose();
    fragmentShader.dispose();

    _boundShader.ensureInitialized();
    Logger::info("Bound Shader initialized");
    VertexShader boundVertexShader("./temp/shaders/boundvertexshader.glsl");
    FragmentShader boundFragmentShader("./temp/shaders/boundfragmentshader.glsl");
    _boundShader.attachShader(boundVertexShader);
    _boundShader.attachShader(boundFragmentShader);
    boundVertexShader.dispose();
    boundFragmentShader.dispose();

    _skyShader.ensureInitialized();
    Logger::info("Sky Shader initialized");
    
    VertexShader skyVertexShader("./temp/shaders/skyboxvertexshader.glsl");
    FragmentShader skyFragmentShader("./temp/shaders/skyboxfragmentshader.glsl");
    _skyShader.attachShader(skyVertexShader);
    _skyShader.attachShader(skyFragmentShader);
    skyVertexShader.dispose();
    skyFragmentShader.dispose();
    
    _terrainShader.ensureInitialized();
    Logger::info("Terrain Shader initialized");
    
    VertexShader terrainVertexShader("./temp/shaders/terrainvertexshader.glsl");
    FragmentShader terrainFragmentShader("./temp/shaders/terrainfragmentshader.glsl");
    _terrainShader.attachShader(terrainVertexShader);
    _terrainShader.attachShader(terrainFragmentShader);
    terrainVertexShader.dispose();
    terrainFragmentShader.dispose();
    
    _dirLight = new DirLight();
    
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
}

void SceneViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SceneViewer::paintGL() {
    Logger::debug("Repainting");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 摄像机视角和投影矩阵
    glm::mat4 view = _camera.viewMatrix();
    glm::mat4 projection = _camera.projectionMatrix((float)width() / (float)height());

    // 天空盒子渲染 ---------------------------------------------------
    if (_sky != nullptr) {
        _skyShader.bind();
        _skyShader.setUniform("view", glm::mat4(glm::mat3(view)));
        _skyShader.setUniform("projection", projection);
        _sky->render();
        _skyShader.unbind();
    }
    // ------------------------------------------------------------------

    // 地形渲染 ---------------------------------------------------
    if (_terrain != nullptr) {
        _terrainShader.bind();
        _terrainShader.setUniform("view", view);
        _terrainShader.setUniform("projection", projection);
        _terrainShader.setUniform("model", _terrain->modelMatrix());
        _terrainShader.setUniform("texture1", 2);
        _terrain->render();
        _terrainShader.unbind();
    }
    // ------------------------------------------------------------------

    // 渲染着色器 ------------------------------------------------
    _shaderProgram.bind();

    _shaderProgram.setUniform("view", view);
    _shaderProgram.setUniform("projection", projection);
    _shaderProgram.setUniform("viewPos", _camera.position());

    int pointLights = 0;
    int spotLights = 0;

    // 更新光源
    for (auto object : _objects) {
        if (object->hasLight()) {
            ScopedLight light = object->transformedLight();
            if (light.isPointLight()) {
                light.updateShader(_shaderProgram, pointLights++);
            }
            else {
                light.updateShader(_shaderProgram, spotLights++);
            }
        }
    }
    
    _shaderProgram.setUniform("pointlightnr", pointLights);
    _shaderProgram.setUniform("spotlightnr", spotLights);

    if (_dirLight != nullptr && _dirLightOn) {
        _dirLight->updateShader(_shaderProgram, 0);
    }

    _shaderProgram.setUniform("dirlightnr", _dirLight != nullptr && _dirLightOn ? 1 : 0);

    // 渲染
    for (auto object : _objects) {
        if (object == _pressedObject) {
            _shaderProgram.setUniform("selColor", glm::vec3(0.22f));
        }
        else if (object == _operatingObject) {
            _shaderProgram.setUniform("selColor", glm::vec3(0.1f));
        }
        else if (object == _hoveredObject) {
            _shaderProgram.setUniform("selColor", glm::vec3(0.2f));
        }
        else {
            _shaderProgram.setUniform("selColor", glm::vec3(0.0f));
        }
        object->render(_shaderProgram);
    }

    _shaderProgram.unbind();
    // ------------------------------------------------------------------

    // 物体的bound盒渲染 -------------------------------------------------
    if (_selectedObject != nullptr && !_hideBound) {
        _boundShader.bind();
        _boundShader.setUniform("view", view);
        _boundShader.setUniform("projection", projection);
        _selectedObject->boundary().render();
        _boundShader.unbind();
    }
    if (_hoveredObject != nullptr && _hoveredObject != _selectedObject) {
        _boundShader.bind();
        _boundShader.setUniform("view", view);
        _boundShader.setUniform("projection", projection);
        _hoveredObject->boundary().render();
        _boundShader.unbind();
    }
    // ------------------------------------------------------------------
}

void SceneViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        _pressedObject = _hoveredObject;
    }
    else {
        _lastMousePosition = event->pos();
    }

    parentWidget()->update();
    setFocus();
}

void SceneViewer::mouseReleaseEvent(QMouseEvent* event) {
    // 状态转移
    bool startOperatingObject = false;
    if (_operatingObject != nullptr) {
        // 单击当使用一个操作物品时
        _operatingObject->updateBoundary();
        if (!_dragged) {
            // 当最后的鼠标按下，这是一个子任务单击
            _operatingObject = nullptr;
            _hideBound = false;
        }
        else {
            // 拖拽，保持
            _dragged = false;
            _hideBound = true;
            _operatingObject = _operatingObject;
        }
    }
    else if (_pressedObject != nullptr && _pressedObject == _selectedObject) {
        if (!_dragged) {
            // 选择然后进入操作模式
            _operatingObject = _selectedObject;
            _hideBound = true;
            startOperatingObject = true;
        }
        else {
            // 保持选择状态
            _dragged = false;
            _hideBound = false;
            _selectedObject->updateBoundary();
        }
    }
    else if (_dragged) {
        _dragged = false;
        _hideBound = false;
        if (_selectedObject != nullptr) {
            _selectedObject->updateBoundary();
        }
    }
    else {
        _selectedObject = _pressedObject;
        _hideBound = false;
        emit onSelect(_selectedObject);
    }
    
    // 预设按钮对象
    _pressedObject = nullptr;

    // 更新悬停对象
    float relX = (float)event->position().x() / (float)width();
    float relY = 1 - (float)event->y() / (float)height();
    Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
    _hoveredObject = hitTest(ray);
    
    if (startOperatingObject) {
        // 如果设置成操作模式，移动对象
        moveOperatingObject(ray);
    }

    // 更新视角
    parentWidget()->update();
}

void SceneViewer::mouseMoveEvent(QMouseEvent* event) {
    // 检查按钮事件的类
    switch (event->buttons()) {
        case Qt::LeftButton: {
            if (_selectedObject != nullptr) {
                // 选择拖拽
                _dragged = true;
                // 隐藏便捷
                _hideBound = true;
                // 旋转相机绕上轴
                glm::vec2 delta = glm::vec2(event->x() - _lastMousePosition.x(), event->y() - _lastMousePosition.y());
                _selectedObject->rotate(_camera.up(), delta.x * 1.0f);
                // 旋转相机绕右轴
                _selectedObject->rotate(_camera.right(), delta.y * 1.0f);
                emit onUpdate(_selectedObject);
            }
            break;
        }
        case Qt::RightButton: {
            // 设置拖拽
            _dragged = true;
            moveCamera(event);
            break;
        }
        case Qt::MiddleButton: {
            if (_controlPressed && _selectedObject != nullptr) {
                // 设置拖拽
                _dragged = true;
                // 隐藏边界
                _hideBound = true;
                // 放缩对象
                glm::vec2 delta = glm::vec2(event->x() - _lastMousePosition.x(), event->y() - _lastMousePosition.y());
                _selectedObject->scale(-delta.y * 0.01f);
                emit onUpdate(_selectedObject);
            }
            else {
                // 设置拖拽
                _dragged = true;
                rotateCamera(event);
            }
            break;
        }
        case Qt::NoButton: {
            float relX = (float)event->x() / (float)width();
            float relY = 1 - (float)event->y() / (float)height();
            Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
            if (_operatingObject == nullptr) {
                // 如果没有按钮按下，做拾取测试然后移动当前对象
                _hoveredObject = hitTest(ray);
                if (_hoveredObject != nullptr) {
                    setCursor(Qt::PointingHandCursor);
                }
                else {
                    setCursor(Qt::ArrowCursor);
                }
            }
            else {
                moveOperatingObject(ray);
                emit onUpdate(_selectedObject);
            }
            break;
        }
        default: {
            Logger::warning("Unknown mouse button input");
            Logger::warning("Mouse button: " + std::to_string(event->buttons()));
            break;
        }
    }
    // 更新鼠标落点位置
    _lastMousePosition = event->pos();
    // 视角更新
    parentWidget()->update();
}

void SceneViewer::wheelEvent(QWheelEvent* event) {
    // 放大缩小
    float wheelOffset = event->angleDelta().y();
    Logger::debug("Wheel offset: " + std::to_string(wheelOffset));
    _camera.push(wheelOffset * _cameraPushSpeed);
    glm::vec3 cameraFront = _camera.front();
    _rotateCenter += wheelOffset * _cameraPushSpeed * cameraFront;
    Logger::debug("New camera position: " + std::to_string(_camera.position().x) + ", " + std::to_string(_camera.position().y) + ", " + std::to_string(_camera.position().z));
    Logger::debug("New center position: " + std::to_string(_rotateCenter.x) + ", " + std::to_string(_rotateCenter.y) + ", " + std::to_string(_rotateCenter.z));
    // 更新视角
    parentWidget()->update();
}

void SceneViewer::moveCamera(QMouseEvent* event) {
    // 移动相机
    float xoffset = event->x() - _lastMousePosition.x();
    float yoffset = _lastMousePosition.y() - event->y();    // reversed since y-coordinates go from bottom to top
    float xmovement = xoffset * _cameraMovementSpeed;
    float ymovement = yoffset * _cameraMovementSpeed;
    glm::vec3 cameraPrevPos = _camera.position();
    _camera.move({ -xmovement, -ymovement });
    glm::vec3 cameraNewPos = _camera.position();
    _rotateCenter += cameraNewPos - cameraPrevPos;
    Logger::debug("Camera moved to: " + std::to_string(_camera.position().x) + ", " + std::to_string(_camera.position().y) + ", " + std::to_string(_camera.position().z));
    Logger::debug("New center: " + std::to_string(_rotateCenter.x) + ", " + std::to_string(_rotateCenter.y) + ", " + std::to_string(_rotateCenter.z));
    if (_operatingObject != nullptr) {
        float relX = (float)event->x() / (float)width();
        float relY = 1 - (float)event->y() / (float)height();
        Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
        moveOperatingObject(ray);
    }
}

void SceneViewer::rotateCamera(QMouseEvent* event) {
    // 旋转相机
    float xoffset = event->x() - _lastMousePosition.x();
    float yoffset = _lastMousePosition.y() - event->y();    // reversed since y-coordinates go from bottom to top
    // 计算 pitch 角度（pitch、yaw、roll）
    float pitch = yoffset * _cameraRotationSpeed;
    // Calculate yaw angle
    float yaw = xoffset * _cameraRotationSpeed;
    _camera.rotate(_rotateCenter, pitch, -yaw);
    Logger::debug("Camera rotated to: " + std::to_string(_camera.position().x) + ", " + std::to_string(_camera.position().y) + ", " + std::to_string(_camera.position().z));
    Logger::debug("Center at: " + std::to_string(_rotateCenter.x) + ", " + std::to_string(_rotateCenter.y) + ", " + std::to_string(_rotateCenter.z));
    if (_operatingObject != nullptr) {
        float relX = (float)event->x() / (float)width();
        float relY = 1 - (float)event->y() / (float)height();
        Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
        moveOperatingObject(ray);
    }
}

void SceneViewer::keyPressEvent(QKeyEvent* event) {
    Logger::debug("Detect keypress " + std::to_string(event->key()));
    // 如果ctrl按下
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        Logger::debug("Control pressed");
        _controlPressed = true;
    }
    //WSAD控制
    if (event->modifiers() == Qt::NoModifier && _selectedObject != nullptr) {
        switch (event->key()) {
            case Qt::Key_W: {
                // 向上移动
                glm::vec3 up = _camera.up();
                _selectedObject->move(up * 0.1f);
                _selectedObject->updateBoundary();
                break;
            }
            case Qt::Key_S: {
                // 向下移动
                glm::vec3 up = _camera.up();
                _selectedObject->move(-up * 0.1f);
                _selectedObject->updateBoundary();
                break;
            }
            case Qt::Key_A: {
                // 向左移动
                glm::vec3 right = _camera.right();
                _selectedObject->move(-right * 0.1f);
                _selectedObject->updateBoundary();
                break;
            }
            case Qt::Key_D: {
                // 向右移动
                glm::vec3 right = _camera.right();
                _selectedObject->move(right * 0.1f);
                _selectedObject->updateBoundary();
                break;
            }
        }
        parentWidget()->update();
    }
    else if (event->modifiers() == Qt::NoModifier) {
        // 移动相机
        switch (event->key()) {
            case Qt::Key_W: {
                // 向前移动
                glm::vec3 front = _camera.front();
                _camera.push(1.0f);
                _rotateCenter += front;
                break;
            }
            case Qt::Key_S: {
                // 向后移动
                glm::vec3 front = _camera.front();
                _camera.push(-1.0f);
                _rotateCenter -= front;
                break;
            }
            case Qt::Key_A: {
                // 向左移动
                glm::vec3 right = _camera.right();
                _camera.move(-right * 0.5f);
                _rotateCenter -= right * 0.5f;
                break;
            }
            case Qt::Key_D: {
                // 向右移动
                glm::vec3 right = _camera.right();
                _camera.move(right * 0.5f);
                _rotateCenter += right * 0.5f;
                break;
            }
        }
        parentWidget()->update();
    }
}

void SceneViewer::keyReleaseEvent(QKeyEvent* event) {
    // 如果按键释放
    if (!(event->modifiers().testFlag(Qt::ControlModifier))) {
        Logger::debug("Control released");
        _controlPressed = false;
    }
}

void SceneViewer::moveOperatingObject(const Ray& ray) {
    // 当前移动的物体
    hitTest(ray);
    if (!_hitRecord.hitted()) {
        // Move to the direction of current ray
        glm::vec3 target = _camera.position() + ray.direction() * 15.0f;
        _operatingObject->setPosition(target);
        _operatingObject->updateBoundary();
    }
    // 移动物品到hit point上（hit record算出的点上）
    else if (_stickToSurface) {
        // 模型的底面原点
        glm::vec3 bottomCenter = _operatingObject->modelMatrix() * glm::vec4(_operatingObject->model()->boundBox().bottomCenterPoint(), 1.0f);
        glm::vec3 modelCenter = _operatingObject->modelMatrix() * glm::vec4(glm::vec3(0.0f), 1.0f);   // model center in world space

        // 旋转模型对齐表面法线
        glm::vec3 normal = _hitRecord.normal();
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 axis = glm::normalize(glm::cross(up, normal));
        float deg = glm::degrees(glm::acos(glm::dot(up, normal)));
        _operatingObject->setRotation(axis, deg);

        // 移动模型到hit point点
        glm::vec3 target = _hitRecord.position();
        
        glm::vec3 newCenter = target + modelCenter - bottomCenter;
        _operatingObject->setPosition(newCenter);

        // Update the boundary
        _operatingObject->updateBoundary();
    }
    else {
        // 移动模型到hit point点
        Logger::debug("Hit point: " + std::to_string(_hitRecord.position().x) + ", " + std::to_string(_hitRecord.position().y) + ", " + std::to_string(_hitRecord.position().z));
        Logger::debug("Bottom center: " + std::to_string(_operatingObject->boundary().bottomCenterPoint().x) + ", " + std::to_string(_operatingObject->boundary().bottomCenterPoint().y) + ", " + std::to_string(_operatingObject->boundary().bottomCenterPoint().z));
        glm::vec3 target = _hitRecord.position();
        glm::vec3 modelCenter = _operatingObject->modelMatrix() * glm::vec4(glm::vec3(0.0f), 1.0f);   // model center in world space
        glm::vec3 bottomCenter = _operatingObject->modelMatrix() * glm::vec4(_operatingObject->model()->boundBox().bottomCenterPoint(), 1.0f);   // model center in world space
        glm::vec3 newCenter = target + modelCenter - bottomCenter;
        _operatingObject->setPosition(newCenter);

        // 更新边界
        _operatingObject->updateBoundary();
    }
}

void SceneViewer::addObject(Model* model) {
    makeCurrent();
    Model* newModel = model->copyToCurrentContext();
    Renderable* newObject = new Renderable(newModel);
    _selectedObject = newObject;
    _operatingObject = newObject;
    _objects.push_back(newObject);
    parentWidget()->update();
    emit onSelect(_selectedObject);
}

void SceneViewer::deleteObject() {
    if (_selectedObject == nullptr) {
        return;
    }
    makeCurrent();
    for (auto it = _objects.begin(); it != _objects.end(); ++it) {
        if (*it == _selectedObject) {
            _objects.erase(it);
            break;
        }
    }
    delete _selectedObject;
    if (_hoveredObject == _selectedObject) {
        _hoveredObject = nullptr;
    }
    if (_pressedObject == _selectedObject) {
        _pressedObject = nullptr;
    }
    _selectedObject = nullptr;
    _operatingObject = nullptr;
    emit onSelect(nullptr);
    parentWidget()->update();
}

void SceneViewer::updateSetting(QPair<QString, QString> setting) {
    makeCurrent();
    if (setting.first == "stickSurface") {
        if (setting.second == "true") {
            _stickToSurface = true;
        }
        else {
            _stickToSurface = false;
        }
    }
    else if (setting.first == "skybox") {
        if (_sky != nullptr) {
            delete _sky;
            _sky = nullptr;
        }
        if (!setting.second.isEmpty()) {
            _sky = new SkyBox(setting.second.toStdString());
        }
    }
    else if (setting.first == "terrain") {
        if (_terrain != nullptr) {
            delete _terrain;
            _terrain = nullptr;
        }
        if (!setting.second.isEmpty()) {
            _terrain = new Terrain(setting.second.toStdString());
        }
    }
    else if (setting.first == "dirLight") {
        if (setting.second == "true") {
            _dirLightOn = true;
        }
        else {
            _dirLightOn = false;
        }
    }
    else if (setting.first == "dirLightColorR") {
        _dirLight->setLightColor(glm::vec3(setting.second.toFloat(), _dirLight->lightColor().y, _dirLight->lightColor().z));
    }
    else if (setting.first == "dirLightColorG") {
        _dirLight->setLightColor(glm::vec3(_dirLight->lightColor().x, setting.second.toFloat(), _dirLight->lightColor().z));
    }
    else if (setting.first == "dirLightColorB") {
        _dirLight->setLightColor(glm::vec3(_dirLight->lightColor().x, _dirLight->lightColor().y, setting.second.toFloat()));
    }
    else if (setting.first == "dirLightIntensity") {
        _dirLight->setIntensity(setting.second.toFloat());
    }
    else if (setting.first == "dirLightDir") {
        // 解析 "x,y,z"
        QStringList list = setting.second.split(",");
        if (list.size() == 3) {
            _dirLight->setLightDirection(glm::vec3(list[0].toFloat(), list[1].toFloat(), list[2].toFloat()));
        }
        else {
            Logger::error("Invalid direction light direction setting");
        }
    }
    else {
        Logger::warning("Unknown setting input");
    }
    doneCurrent();
}
