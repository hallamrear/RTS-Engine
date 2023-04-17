#define TINYOBJLOADER_IMPLEMENTATION  
#include <tiny_obj_loader.h>
#include <istream>

#include "BennettPCH.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Renderer.h"

namespace Bennett
{
    Model* ModelLoader::Load(const Renderer& renderer, const std::string& filepath)
    {
        return Load(renderer, filepath.c_str());
    }

    Model* ModelLoader::Load(const Renderer& renderer, const char* filepath)
    {
        std::string error, warning;
        tinyobj::ObjReaderConfig reader_config;
        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filepath, reader_config)) 
        {
            Log("Failed to parse obj file correctly.", LOG_SERIOUS);
            if (!reader.Error().empty())
            {
                Log(reader.Error(), LOG_SERIOUS);
            }
            return nullptr;
        }

        if (!reader.Warning().empty()) 
        {
            Log(reader.Warning(), LOG_SERIOUS);
            return nullptr;
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        Model* model = new Model();

        // Loop over shapes
        for (const auto& shape : shapes)
        {
            std::vector<Vertex> verts = std::vector<Vertex>();
            std::vector<unsigned int> indices = std::vector<unsigned int>();

            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex;

                vertex.Position = glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                    );

                vertex.Normal = glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                );

                vertex.UV = glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                );

                vertex.Colour = glm::vec4(
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                );
                verts.push_back(vertex);
                indices.push_back(indices.size());
            }

            model->m_Meshes.push_back(new Mesh(renderer, verts, indices));
        }

        return model;
    }
}