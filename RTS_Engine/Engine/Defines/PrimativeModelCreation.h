#pragma once
#include <System/Manager/AssetManager.h>
#include <System/ServiceLocator.h>
#include <Rendering/Vertex.h>
#include <corecrt_math_defines.h>

namespace Bennett
{
	static void CreateGenericModels();
}

namespace Bennett
{
    void CreateGenericModels()
    {
        AssetManager& am = ServiceLocator::GetAssetManager();

        std::vector<unsigned int> indices{};
        std::vector<Vertex> vertices{};

#pragma region 1x1 Cube
        indices = {
            //Left
            0, 1, 2,
            0, 2, 3,
            //Right
            6, 5, 4,
            7, 6, 4,
            //Back
            8, 9, 10,
            8, 10, 11,
            //Front
            14, 13, 12,
            15, 14, 12,
            //Top
            18, 17, 16,
            19, 18, 16,
            //Bottom
            20, 21, 22,
            20, 22, 23
        };  

        vertices =
        {
            //Face 1 - Right
            /*00*/ Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*01*/ Vertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*02*/ Vertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*03*/ Vertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            //Face 2 - Left                                        
            /*04*/ Vertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*05*/ Vertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*06*/ Vertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*07*/ Vertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            //Face 3 - Back
            /*08*/ Vertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*09*/ Vertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*10*/ Vertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*11*/ Vertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            //Face 4 - Front
            /*12*/ Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*13*/ Vertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*14*/ Vertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*15*/ Vertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            //Face 5 - Top
            /*16*/ Vertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*17*/ Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*18*/ Vertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*19*/ Vertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            //Face 6 - Bottom
            /*16*/ Vertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*17*/ Vertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*18*/ Vertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            /*19*/ Vertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),

        };

        am.CreateModel("1x1_Cube", vertices, indices);
        vertices.clear();
        indices.clear();
#pragma endregion


#pragma region 1x1 Cube Solid
        indices = {
            4, 2, 0,
            2, 7, 3,
            6, 5, 7,
            1, 7, 5,
            0, 3, 1,
            4, 1, 5,
            4, 6, 2,
            2, 6, 7,
            6, 4, 5,
            1, 3, 7,
            0, 2, 3,
            4, 0, 1
        };

        vertices = {                             
            Vertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //0 
            Vertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //1 
            Vertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //2
            Vertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //3
            Vertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //4  
            Vertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //5  
            Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),  //6 
            Vertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))   //7 
        };

        am.CreateModel("1x1_Cube_Solid", vertices, indices);
        vertices.clear();
        indices.clear();
#pragma endregion

#pragma region 1x1 Quad
        indices = {
              2, 3, 0,
              0, 3, 1,
        };

        vertices = {
            Vertex(glm::vec3(-0.5f, 0.0f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(0.5f, 0.0f,-0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
        };

        am.CreateModel("1x1_Quad", vertices, indices);
        vertices.clear();
        indices.clear();
#pragma endregion

#pragma region 1x1 Unit Sphere

        indices =
        {
             0,13,12,
             1,13,15,
             0,12,17,
             0,17,19,
             0,19,16,
             1,15,22,
             2,14,24,
             3,18,26,
             4,20,28,
             5,21,30,
             1,22,25,
             2,24,27,
             3,26,29,
             4,28,31,
             5,30,23,
             6,32,37,
             7,33,39,
             8,34,40,
             9,35,41,
             10,36,38,
             38,41,11,
             38,36,41,
             36,9,41,
             41,40,11,
             41,35,40,
             35,8,40,
             40,39,11,
             40,34,39,
             34,7,39,
             39,37,11,
             39,33,37,
             33,6,37,
             37,38,11,
             37,32,38,
             32,10,38,
             23,36,10,
             23,30,36,
             30,9,36,
             31,35,9,
             31,28,35,
             28,8,35,
             29,34,8,
             29,26,34,
             26,7,34,
             27,33,7,
             27,24,33,
             24,6,33,
             25,32,6,
             25,22,32,
             22,10,32,
             30,31,9,
             30,21,31,
             21,4,31,
             28,29,8,
             28,20,29,
             20,3,29,
             26,27,7,
             26,18,27,
             18,2,27,
             24,25,6,
             24,14,25,
             14,1,25,
             22,23,10,
             22,15,23,
             15,5,23,
             16,21,5,
             16,19,21,
             19,4,21,
             19,20,4,
             19,17,20,
             17,3,20,
             17,18,3,
             17,12,18,
             12,2,18,
             15,16,5,
             15,13,16,
             13,0,16,
             12,14,2,
             12,13,14,
             13,1,14,
        };

        vertices = {
            Vertex(glm::vec3( 0.000000,-0.500000, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.361804,-0.223610, 0.262863), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.138194,-0.223610, 0.425325), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.447213,-0.223608, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.138194,-0.223610,-0.425325), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.361804,-0.223610,-0.262863), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.138194, 0.223610, 0.425325), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.361804, 0.223610, 0.262863), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.361804, 0.223610,-0.262863), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.138194, 0.223610,-0.425325), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.447213, 0.223608, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.000000, 0.500000, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.081228,-0.425327, 0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.212661,-0.425327, 0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.131434,-0.262869, 0.404506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.425324,-0.262868, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.212661,-0.425327,-0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.262865,-0.425326, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.344095,-0.262868, 0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.081228,-0.425327,-0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.344095,-0.262868,-0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.131434,-0.262869,-0.404506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.475529, 0.000000, 0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.475529, 0.000000,-0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.000000, 0.000000, 0.500000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.293893, 0.000000, 0.404508), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.475529, 0.000000, 0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.293893, 0.000000, 0.404508), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.293893, 0.000000,-0.404508), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.475529, 0.000000,-0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.293893, 0.000000,-0.404508), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.000000, 0.000000,-0.500000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.344095, 0.262868, 0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.131434, 0.262869, 0.404506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.425324, 0.262868, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.131434, 0.262869,-0.404506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.344095, 0.262868,-0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.081228, 0.425327, 0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.262865, 0.425326, 0.000000), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.212661, 0.425327, 0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3(-0.212661, 0.425327,-0.154506), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            Vertex(glm::vec3( 0.081228, 0.425327,-0.249998), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
        };

        am.CreateModel("1x1_Sphere", vertices, indices);
        vertices.clear();
        indices.clear();

#pragma endregion
        

        /*
        float radius = 0.5f;
        int sectorCount = 20;
        int stackCount = 20;

        float x = 0.0f, y = 0.0f, z = 0.0f, xy = 0.0f;                       // vertex position
        float nx = 0.0f, ny = 0.0f, nz = 0.0f, lengthInv = 1.0f / radius;    // vertex normal
        float s = 0.0f, t = 0.0f;                                            // vertex texCoord

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                Vertex v;

                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                v.Position.x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                v.Position.y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                v.Position.z = z;

                // normalized vertex normal (nx, ny, nz)
                v.Normal.x = x * lengthInv;
                v.Normal.y = y * lengthInv;
                v.Normal.z = z * lengthInv;

                // vertex tex coord (s, t) range between [0, 1]
                v.UV.s = (float)j / sectorCount;
                v.UV.t = (float)i / stackCount;

                vertices.push_back(v);
            }
        }

        // generate CCW index list of sphere triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }


        am.CreateModel("1x1_Sphere", vertices, indices);
        vertices.clear();
        indices.clear();
        */
    }

}