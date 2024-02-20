#include <BennettPCH.h>
#include <istream>
#include <System/ServiceLocator.h>
#include <System/Loader/ModelLoader.h>
#include <Rendering/Model.h>
#include <System/Logger.h>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <tiny_gltf.h>

#define GLTF_COMPONENT_TYPE_USHORT 5123
#define GLTF_COMPONENT_TYPE_FLOAT 5126

namespace Bennett
{
    Model* ModelLoader::Load(const std::string& filepath, const bool& isBinary)
    {
        return Load(filepath.c_str(), isBinary);
    }

    Model* ModelLoader::Load(const char* filepath, const bool& isBinary)
    {
        tinygltf::Model gltfModel;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = false;

        if(isBinary)
            ret = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, filepath);
        else
            ret = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, filepath);

        if (!warn.empty()) {
            printf("Warn: %s\n", warn.c_str());
            return nullptr;
        }

        if (!err.empty()) {
            printf("Err: %s\n", err.c_str());
            return nullptr;
        }

        if (!ret) {
            printf("Failed to parse glTF\n");
            return nullptr;
        }

        Model* model = new Model();

        printf("accessors: %u\n", gltfModel.accessors.size());
        printf("bufferViews: %u\n", gltfModel.bufferViews.size());
        printf("meshes: %u\n", gltfModel.meshes.size());
        printf("buffers: %u\n", gltfModel.buffers.size());
        for (size_t i = 0; i < gltfModel.buffers.size(); i++)
        {
            printf("\tbuffer[%u] byteCount: %u\n", i, gltfModel.buffers[i].data.size());
        }

        bool loadedMeshData = LoadMeshData(*model, gltfModel);

        //CreateHitboxFromModel();

        if (model->m_Meshes.size() == 0 || loadedMeshData != true)
        {
            delete model;
            model = nullptr;
        }

        return model;
    }

    bool LoadIndices(const tinygltf::Model& gltf, const ushort& indexAccessor, std::vector<VertexIndex>& indices)
    {
        const tinygltf::Accessor& iAccessor = gltf.accessors[indexAccessor];
        if (iAccessor.componentType != GLTF_COMPONENT_TYPE_USHORT)
        {
            Log(LOG_SERIOUS, "Index accessor component type is not unsigned short.\n.");
            return false;
        }
        
        //Reserve space for all our indices.
        indices.resize(iAccessor.count);

        const tinygltf::BufferView& iBufferView = gltf.bufferViews[iAccessor.bufferView];

        if (iBufferView.byteLength == 0)
        {
            Log(LOG_SERIOUS, "Indices bufferview bytelength was invalid.\n.");
            return false;
        }

        const tinygltf::Buffer& binaryBuffer = gltf.buffers[iBufferView.buffer];

        const uchar* indicesData = binaryBuffer.data.data() + iBufferView.byteOffset;
        memcpy(indices.data(), indicesData, iBufferView.byteLength);

        return true;
    }

    template<class T>
    bool LoadVertexDetailFromModel(const tinygltf::Model& gltf, const std::string& identifier, std::vector<T>& vertexData)
    {
        int accessorIndex = -1;
        const std::map<std::string, int>& attribs = gltf.meshes[0].primitives[0].attributes;
        std::map<std::string, int>::const_iterator search = attribs.find(identifier);

        if (search == attribs.end())
        {
            Log(LOG_SERIOUS, "Failed to find vertex details attributes \"%s\" in model.\n", identifier.c_str());
            return false;
        }

        accessorIndex = search->second;

        const tinygltf::Accessor& accessor = gltf.accessors[accessorIndex];
        const tinygltf::BufferView& bufferView = gltf.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = gltf.buffers[bufferView.buffer];

        if (bufferView.byteLength == 0)
        {
            Log(LOG_SERIOUS, "Vertex detail \"%s\" bufferview bytelength was invalid.\n.", identifier.c_str());
            return false;
        }

        vertexData.clear();
        vertexData.resize(accessor.count);

        const uchar* dataLocation = buffer.data.data() + bufferView.byteOffset;
        memcpy(vertexData.data(), dataLocation, bufferView.byteLength);

        return true;
    }

    bool LoadVertexData(const tinygltf::Model& gltf, std::vector<Vertex>& modelData)
    {     
        std::vector<glm::vec3> positions, normals;
        std::vector<glm::vec4> tangents;
        std::vector<glm::vec2> texCoords;

        bool positionsLoaded = LoadVertexDetailFromModel(gltf, "POSITION", positions);
        bool normalsLoaded = LoadVertexDetailFromModel(gltf, "NORMAL", normals);
        bool tangentsLoaded = LoadVertexDetailFromModel(gltf, "TANGENT", tangents);
        bool texCoordsLoaded = LoadVertexDetailFromModel(gltf, "TEXCOORD_0", texCoords);

        if ((positionsLoaded) &&
            (normalsLoaded) &&
            (tangentsLoaded) &&
            (texCoordsLoaded) == false)
        {
            positions.clear();
            normals.clear();
            tangents.clear();
            texCoords.clear();

            Log(LOG_SERIOUS, "Model does not contain a required accessor for either position, normal, tangents, or texcoords.\n");
            return false;
        }

        //if ((positions.size() == normals.size() == tangents.size() == texCoords.size()) == false)

        //This should be guarenteed to exist as it would be checked above.
        int posIndex = gltf.meshes[0].primitives[0].attributes.find("POSITION")->second;
        const tinygltf::Accessor& posAcc = gltf.accessors[posIndex];
        glm::vec3 maxPos = { (float)posAcc.maxValues[0], (float)posAcc.maxValues[1], (float)posAcc.maxValues[2] };
        glm::vec3 minPos = { (float)posAcc.minValues[0], (float)posAcc.minValues[1], (float)posAcc.minValues[2] };

        Vertex vertex;
        for (size_t i = 0; i < positions.size(); i++)
        {
            vertex.Position = positions[i];
            vertex.Normal = normals[i];
            vertex.Colour = tangents[i];
            vertex.UV = texCoords[i];
            modelData.push_back(vertex);
        }

        return true;
    }

    bool ModelLoader::LoadMeshData(Model& model, const tinygltf::Model& gltf)
    {
        for (size_t i = 0; i < gltf.meshes.size(); i++)
        {
            printf("Loading mesh [%i]\n", i);

            int mode = gltf.meshes[i].primitives[0].mode;
            printf("Primitive mode [%u]\n", mode);
            
            std::vector<Vertex> modelData = std::vector<Vertex>();
            bool loadedVertices = LoadVertexData(gltf, modelData);
            if (loadedVertices == false)
            {
                Log(LOG_SERIOUS, "Failed to load vertex data.\n.");
                modelData.clear();
                return false;
            }

            std::vector<VertexIndex> indices = std::vector<VertexIndex>();
            size_t indexAccessor = gltf.meshes[i].primitives[0].indices;
            bool loadedIndices = LoadIndices(gltf, indexAccessor, indices);

            if (loadedIndices == false)
            {
                Log(LOG_SERIOUS, "Failed to load indices.\n.");
                indices.clear();
                return false;
            }      


            size_t triCount = indices.size() / 3;
            for (size_t i = 0; i < triCount; i++)
            {

            }




            //Should have full mesh data loaded.
            Mesh* mesh = new Mesh(modelData, indices);
            if (mesh)
            {
                model.m_Meshes.push_back(mesh);
            }

        }

        return true;
    }

    Model* ModelLoader::Create(const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices)
    {
        Model* model = new Model();
        model->m_Meshes.push_back(new Mesh(vertices, indices));
        return model;
    }
}