#include <algorithm>
#include <vector>

#include "boundary.h"
#include "vao.h"

void Boundary::updateControlPoints(const glm::vec3 point) {
    _bottomControlPoint.x = std::min(_bottomControlPoint.x, point.x);
    _bottomControlPoint.y = std::min(_bottomControlPoint.y, point.y);
    _bottomControlPoint.z = std::min(_bottomControlPoint.z, point.z);
    _topControlPoint.x = std::max(_topControlPoint.x, point.x);
    _topControlPoint.y = std::max(_topControlPoint.y, point.y);
    _topControlPoint.z = std::max(_topControlPoint.z, point.z);
}

bool Boundary::hit(const Ray& ray) const {
    //测试光线是否穿过我们的boundary box世界范围
    //boundary box用两个点定义 前顶右 和 后底左。
    // 光线已经被转移。
    
    // 测试x轴
    float tmin = (_bottomControlPoint.x - ray.origin().x) / ray.direction().x;
    float tmax = (_topControlPoint.x - ray.origin().x) / ray.direction().x;
    if (tmin > tmax) std::swap(tmin, tmax);
    
    // 测试y轴
    float tymin = (_bottomControlPoint.y - ray.origin().y) / ray.direction().y;
    float tymax = (_topControlPoint.y - ray.origin().y) / ray.direction().y;
    if (tymin > tymax) std::swap(tymin, tymax);
    
    // 测试z轴
    float tzmin = (_bottomControlPoint.z - ray.origin().z) / ray.direction().z;
    float tzmax = (_topControlPoint.z - ray.origin().z) / ray.direction().z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    // 误差管理，查看是否在误差的间隔内。
    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    
    // 测试t的z的范围
    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    
    return true;
}

void Boundary::render() const {
    // 创建定点
    std::vector<Vertex> vertices;
    vertices.push_back(Vertex(glm::vec3(_bottomControlPoint.x, _bottomControlPoint.y, _bottomControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_topControlPoint.x, _bottomControlPoint.y, _bottomControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_topControlPoint.x, _topControlPoint.y, _bottomControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_bottomControlPoint.x, _topControlPoint.y, _bottomControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_bottomControlPoint.x, _bottomControlPoint.y, _topControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_topControlPoint.x, _bottomControlPoint.y, _topControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_topControlPoint.x, _topControlPoint.y, _topControlPoint.z)));
    vertices.push_back(Vertex(glm::vec3(_bottomControlPoint.x, _topControlPoint.y, _topControlPoint.z)));
    // 创建顶点索引
    std::vector<unsigned int> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(3); // bottom
    indices.push_back(4); indices.push_back(5); indices.push_back(6); indices.push_back(7); // top
    indices.push_back(0); indices.push_back(1); indices.push_back(5); indices.push_back(4); // left
    indices.push_back(3); indices.push_back(2); indices.push_back(6); indices.push_back(7); // right
    indices.push_back(0); indices.push_back(3); indices.push_back(7); indices.push_back(4); // back
    indices.push_back(1); indices.push_back(2); indices.push_back(6); indices.push_back(5); // front
    // 创建VBO
    VertexBufferObject vbo(vertices);
    // 创建EBO
    ElementBufferObject ebo(indices);
    // 创建VAO
    VertexArrayObject vao(vbo, ebo);
    vao.setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 渲染
    vao.bind();//绑定VAO和VBO
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(4 * sizeof(unsigned int)));
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(8 * sizeof(unsigned int)));
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
    vao.unbind();
    //回收VBO 和 VAO
    vbo.dispose();
    vao.dispose();
}
