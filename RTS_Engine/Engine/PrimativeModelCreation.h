#pragma once
#include "AssetManager.h"
#include "ServiceLocator.h"
#include "Vertex.h"

namespace Bennett
{
	static void CreateGenericModels();
}

namespace Bennett
{
    void CreateGenericModels()
    {
        AssetManager& am = ServiceLocator::GetAssetManager();

        std::vector<unsigned int> indices {};
        std::vector<Vertex> vertices {};

    #pragma region 1x1 Cube
        indices = {
            //Top
            2, 6, 7,
            2, 3, 7,
            //Bottom
            0, 4, 5,
            0, 1, 5,
            //Left
            0, 2, 6,
            0, 4, 6,
            //Right
            1, 3, 7,
            1, 5, 7,
            //Front
            0, 2, 3,
            0, 1, 3,
            //Back
            4, 6, 7,
            4, 5, 7
        };

        vertices = {
            Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),  //0
            Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),   //1
            Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),   //2
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),    //3
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)), //4
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),  //5
            Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),  //6
            Vertex(glm::vec3(0.5f, -0.5f, 0.5f))    //7
        };

        am.CreateModel("1x1_Cube", vertices, indices);
        vertices.clear();
        indices.clear();
#pragma endregion

    #pragma region 1x1 Quad
        indices = {
              2, 3, 0,
              0, 3, 1,
        };

        vertices = {
            Vertex(glm::vec3(-0.5f, 0.0f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f, 0.0f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
            Vertex(glm::vec3( 0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
        };

        am.CreateModel("1x1_Quad", vertices, indices);
        vertices.clear();
        indices.clear();
#pragma endregion

    }
}