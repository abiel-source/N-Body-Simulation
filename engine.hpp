#pragma once
#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "settings.hpp"
#include "utility.hpp"

#include <math.h>
#include <vector>

#pragma pack(push,1)
struct StarDescriptions {
    // physics
    glm::vec3 v;
    glm::vec3 fnet;
    GLfloat m;

    // transformations
    glm::mat4 model;

    StarDescriptions()
    {
        // physics
        v = glm::vec3(0.0f, 0.0f, 0.0f);
        fnet = glm::vec3(0.0f, 0.0f, 0.0f);
        m = avgM;

        // transformations
        //glm::vec3 randomVector = glm::vec3(generateRandomNormal(), generateRandomNormal(), generateRandomNormal());
        //float angle = 100.0f * generateRandomNormal();
        model = glm::mat4(1.0f);
        //model = glm::translate(model, randomVector);
        //model = glm::rotate(model, glm::radians(angle), randomVector);
    }
};
#pragma pack(pop)

#pragma pack(push,1)
struct StarVertices {
    std::vector<float> sphereVertices;

    StarVertices()
    {
        for (int y = 0;y <= Y_SEGMENTS;y++)
        {
            for (int x = 0;x <= X_SEGMENTS;x++)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = 0.01f * std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * 1.0f * PI);
                float yPos = 0.01f * std::cos(ySegment * 1.0f * PI);
                float zPos = 0.01f * std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * 1.0f * PI);
                sphereVertices.push_back(xPos);
                sphereVertices.push_back(yPos);
                sphereVertices.push_back(zPos);
            }
        }
    }
};
#pragma pack(pop)

#pragma pack(push,1)
struct StarIndices {
    std::vector<int> sphereIndices;

    StarIndices()
    {
        for (int i = 0;i < Y_SEGMENTS;i++)
        {
            for (int j = 0;j < X_SEGMENTS;j++)
            {
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
            }
        }
    }
};
#pragma pack(pop)

class Galaxy {
private:
    std::vector<StarDescriptions> s_desc;
    std::vector<StarVertices> s_vert;
    StarIndices s_indc;
public:
    void generateStar()
    {
        StarDescriptions s_d;
        s_desc.push_back(s_d);

        StarVertices s_v;
        s_vert.push_back(s_v);
        glm::vec3 randomVector = glm::vec3(generateRandomNormal(), generateRandomNormal(), generateRandomNormal());
        stepUTIL(s_vert.size() - 1, randomVector);
    }

    // POST: return a reference to i'th model matrix,
    // so glm::value_ptr will get get the address of the
    // referent matrix.
    glm::mat4& getIthModelMat4(unsigned int i)
    {
        return s_desc[i].model;
    }

    // POST: returns a pointer to the i'th vertex data struct;
    // to be used when we re-buffer each vertex data, for each step;
    float* getIthVertexData(unsigned int i)
    {
        return &s_vert[i].sphereVertices[0];
    }

    int* getIndexData()
    {
        return &s_indc.sphereIndices[0];
    }

    GLsizeiptr vertSize()
    {
        return s_vert[0].sphereVertices.size() * sizeof(float);
        //return Y_SEGMENTS * X_SEGMENTS * 3 * sizeof(float);
    }

    GLsizeiptr indcSize()
    {
        return s_indc.sphereIndices.size() * sizeof(int);
        //return Y_SEGMENTS * X_SEGMENTS * 6 * sizeof(int);
    }

    void step(float dt)
    {
        // 1) calculate Fnet for each star
        for (unsigned int i = 0; i < N; i++)
        {
            for (unsigned int j = 0; j < N; j++)
            {
                if (i == j) continue;

                float m_1 = s_desc[i].m;
                float m_2 = s_desc[j].m;

                glm::vec3 r_ij = glm::vec3(0.0f);
                r_ij.x = s_vert[j].sphereVertices[0] - s_vert[i].sphereVertices[0];
                r_ij.y = s_vert[j].sphereVertices[1] - s_vert[i].sphereVertices[1];
                r_ij.z = s_vert[j].sphereVertices[2] - s_vert[i].sphereVertices[2];

                float r_ij_norm = sqrt(pow(r_ij.x, 2) + pow(r_ij.y, 2) + pow(r_ij.z, 2));

                // softening factor eliminates radical slingshot effects; we do not account for star collisions
                glm::vec3 f_ij = (G * m_1 * m_2 * r_ij) / (pow(r_ij_norm, 3) + pow(e_soft, 3));

                s_desc[i].fnet += f_ij;
            }
        }

        // 2) calculate velocity for each star
        for (unsigned int i = 0; i < N; i++)
        {
            s_desc[i].v += s_desc[i].fnet / s_desc[i].m * dt;
        }

        // 3) calculate final position for each star
        for (unsigned int i = 0; i < N; i++)
        {
            glm::vec3 dlta_s = s_desc[i].v * dt;
            stepUTIL(i, dlta_s);
        }

        // clean up
        for (unsigned int i = 0; i < N; i++)
        {
            s_desc[i].fnet = glm::vec3(0.0f);
        }
    }

    void stepUTIL(unsigned int i, glm::vec3 vec)
    {
        //s_desc[i].model = glm::translate(s_desc[i].model, vec);

        
        for (unsigned int ii = 0; ii < s_vert[i].sphereVertices.size(); ii += 3)
        {
            s_vert[i].sphereVertices[ii] += vec.x;
            s_vert[i].sphereVertices[ii + 1] += vec.y;
            s_vert[i].sphereVertices[ii + 2] += vec.z;
        }
    }
};