#include "logger.h"
#include "model.h"

Model::Model(std::string path) {
    // 从路径中加载模型数据
    // TODO: 实现同步
    loadModel(path);
}

Model::~Model() {
    // TODO: 对资源的回收，目前有个指针返回没写好，回头改成传引用
}

Model::Model(std::vector<Mesh>&& meshes, std::vector<Texture>&& textures, std::string directory, Boundary boundBox) {
    _meshes = std::move(meshes);
    _texturesLoaded = std::move(textures);
    _directory = directory;
    _status = LOADED;
    _boundBox = boundBox;
}

void Model::loadModel(std::string path) {
    Logger::info("Loading model from path: " + path);

    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::error( "Failed to open file: " + path );
        _status = ERR;
        return;
    }

    // Convert all '\' to '/'
    std::replace(path.begin(), path.end(), '\\', '/');
    _directory = path.substr(0, path.find_last_of('/'));

    Logger::info("Model read successfully");
    Logger::info("Processing model nodes");
//-------------------------
    std::string line;
    std::vector<glm::vec3> vertices_postions{{0,0,0}};
    std::vector<glm::vec3> vertices_normals{{0,0,0}};
    std::vector<glm::vec3> vertices_texCoords{{0,0,0}};
    int mtl_num;//一个模型上有若干纹理号
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "g ")
        {
            std::istringstream s(line.substr(2));
            s >> mtl_num;
            //TODO对不同纹理进行处理;
            //初步的想法是

        }
        else if (line.substr(0, 2) == "v ")
        {
            std::istringstream s(line.substr(2));
            glm::vec3 vertex;
            s >> vertex.x >> vertex.y >> vertex.z;
            vertices_positions.push_back(vertex);
        }
        else if (line.substr(0, 2) == "vn")
        {
            std::istringstream s(line.substr(2));
            glm::vec3 vertex;
            s >> vertex.x >> vertex.y >> vertex.z;
            vertices_normals.push_back(vertex);
        }
        else if (line.substr(0, 2) == "vt")
        {
            std::istringstream s(line.substr(2));
            glm::vec3 vertex;
            s >> vertex.x >> vertex.y >> vertex.z;
            vertices_normals.push_back(vertex);
        }
        else if (line.substr(0, 2) == "f ") {
            std::istringstream s(line.substr(2));
            std::string splitted;
            std::vector<unsigned int> indices;
            while (std::getline(s, splitted, ' ')) {
                unsigned int index = 1;
                std::istringstream(splitted) >> index;
                indices.push_back(index - 1);
            }
            for (size_t i = 2; i < indices.size(); i++) {
                glm::uvec3 face = glm::uvec3(indices[0], indices[i - 1], indices[i]);
                faces.push_back(face);
            }
        }
    }
//---------------------------

    file.close();
    _status = LOADED;
    Logger::info("Model loaded: "+_meshes.size+ " meshes, " + _texturesLoaded.size() + " textures.");
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Logger::debug("Processing mesh with " + std::to_string(mesh->mNumVertices) + " vertices");
    // 处理顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // 用来作为我这边自用buffer的中间矩阵
        glm::vec3 vertexPosition = glm::vec3(0.0f);
        glm::vec3 vertexNormal = glm::vec3(0.0f);
        glm::vec2 vertexTextureCoordinate = glm::vec2(0.0f);
        glm::vec3 vertexTangent = glm::vec3(0.0f);
        glm::vec3 vertexBitangent = glm::vec3(0.0f);
        
        // 顶点位置更新
        vertexPosition.x = mesh->mVertices[i].x;
        vertexPosition.y = mesh->mVertices[i].y;
        vertexPosition.z = mesh->mVertices[i].z;

        // 更新boundary box
        _boundBox.updateControlPoints(vertexPosition);

        // 处理顶点法向量
        if (mesh->mNormals) {
            vertexNormal.x = mesh->mNormals[i].x;
            vertexNormal.y = mesh->mNormals[i].y;
            vertexNormal.z = mesh->mNormals[i].z;
        }

        // 处理顶点纹理坐标
        if (mesh->mTextureCoords[0]) {
            // Process texture coordinates
            vertexTextureCoordinate.x = mesh->mTextureCoords[0][i].x;
            vertexTextureCoordinate.y = mesh->mTextureCoords[0][i].y;
            
            //// 顶点的切线
            //if (mesh->mTangents) {
            //    vertexTangent.x = mesh->mTangents[i].x;
            //    vertexTangent.y = mesh->mTangents[i].y;
            //    vertexTangent.z = mesh->mTangents[i].z;
            //}
            //
            //// 顶点的双切
            //if (mesh->mBitangents) {
            //    vertexBitangent.x = mesh->mBitangents[i].x;
            //    vertexBitangent.y = mesh->mBitangents[i].y;
            //    vertexBitangent.z = mesh->mBitangents[i].z;
            //}
        }
        else {
            vertexTextureCoordinate = glm::vec2(0.0f, 0.0f);
        }

        // 创建新顶点
        Vertex newVertex = {
            vertexPosition,
            vertexNormal,
            vertexTextureCoordinate,
            //vertexTangent,
            //vertexBitangent
        };

        // 添加定点
        vertices.push_back(newVertex);
    }
    Logger::debug("Vertices vector memory usage: " + std::to_string(vertices.size() * sizeof(Vertex) / 1024) + " KB");

    Logger::debug("Processing mesh with " + std::to_string(mesh->mNumFaces) + " faces");
    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    Logger::debug("Indices vector memory usage: " + std::to_string(indices.size() * sizeof(unsigned int) / 1024) + " KB");

    Logger::debug("Processing mesh materials");
    // 材质属性
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 散射光
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 高亮反射光
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    Logger::debug("Textures vector memory usage: " + std::to_string(textures.size() * sizeof(Texture) / 1024) + " KB");

    Logger::debug("Mesh processed");
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < _texturesLoaded.size(); j++) {
            if (std::strcmp(_texturesLoaded[j].path().data(), str.C_Str()) == 0) {
                textures.push_back(_texturesLoaded[j]);
                skip = true;
                Logger::debug("Texture already loaded, skipped");
                break;
            }
        }
        if (!skip) {
            Texture newTexture(textureType, _directory + '/' + str.C_Str());
            textures.push_back(newTexture);
            _texturesLoaded.push_back(newTexture);
        }
    }
    return textures;
}

void Model::render(const ShaderProgram& shader) const {
    // 模型状态
    if (_status != LOADED) {
        Logger::error("Trying to render unloaded model");
        return;
    }
    // 渲染模型
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].render(shader);
    }
}

HitRecord Model::hit(const Ray& ray, const glm::mat4& modelMatrix) const {
    HitRecord record = HitRecord();
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        HitRecord hitRecord = _meshes[i].hit(ray, modelMatrix);
        if (hitRecord.hitted() && hitRecord.t() < record.t()) {
            record = hitRecord;
        }
    }
    return record;
}

Model* Model::copyToCurrentContext() const {
    // 重加载纹理
    std::vector<Texture> newTextures;
    for (unsigned int i = 0; i < _texturesLoaded.size(); i++) {
        // 加载纹理
        Texture newTexture = Texture(_texturesLoaded[i].type(), _directory + '/' + _texturesLoaded[i].path());
        newTextures.push_back(newTexture);
    }
    
    // 拷贝meshes
    std::vector<Mesh> newMeshes;
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        Mesh newMesh = Mesh(_meshes[i].vertices(), _meshes[i].indices(), newTextures);
        newMeshes.push_back(newMesh);
    }
    
    // 新模型（TODO:这里回头看还有没有优化空间）
    Model* newModel = new Model(std::move(newMeshes), std::move(newTextures), _directory, _boundBox);
    return newModel;
}
