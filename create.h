#pragma once

#include "utils/glm_utils.h"
#include "components/simple_scene.h"

namespace create
{
    inline Mesh* Parallelepiped(const std::string& name, glm::vec3 position, glm::vec3 size, glm::vec3 color) {
        std::vector<VertexFormat> vertices = {
            VertexFormat(position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), color),
            VertexFormat(position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), color),
            VertexFormat(position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color),
            VertexFormat(position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color),
            VertexFormat(position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), color),
            VertexFormat(position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), color),
            VertexFormat(position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color),
            VertexFormat(position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color)
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 1, 3, 2,
            4, 5, 6, 5, 7, 6,
            0, 2, 4, 2, 6, 4,
            1, 3, 5, 3, 7, 5,
            0, 1, 4, 1, 5, 4,
            2, 3, 6, 3, 7, 6
        };

        Mesh* parallelepiped = new Mesh(name);
        parallelepiped->InitFromData(vertices, indices);
        return parallelepiped;
    }

    inline Mesh* Cylinder(const std::string& name, glm::vec3 position, float radius, float height, glm::vec3 color) {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        int segments = 36;
        float angleStep = 2.0f * M_PI / segments;

        vertices.push_back(VertexFormat(position, color));
        for (int i = 0; i < segments; ++i) {
            float angle = i * angleStep;
            glm::vec3 point = position + glm::vec3(radius * cos(angle), 0, radius * sin(angle));
            vertices.push_back(VertexFormat(point, color));
        }

        glm::vec3 topCenter = position + glm::vec3(0, height, 0);
        vertices.push_back(VertexFormat(topCenter, color)); 
        for (int i = 0; i < segments; ++i) {
            float angle = i * angleStep;
            glm::vec3 point = topCenter + glm::vec3(radius * cos(angle), 0, radius * sin(angle));
            vertices.push_back(VertexFormat(point, color));
        }

        for (int i = 1; i <= segments; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i % segments + 1);
        }

        int topStartIndex = segments + 1;
        for (int i = 1; i <= segments; ++i) {
            indices.push_back(topStartIndex);
            indices.push_back(topStartIndex + i % segments + 1);
            indices.push_back(topStartIndex + i);
        }

        for (int i = 1; i <= segments; ++i) {
            int next = i % segments + 1;
            indices.push_back(i);
            indices.push_back(next);
            indices.push_back(topStartIndex + next);

            indices.push_back(i);
            indices.push_back(topStartIndex + next);
            indices.push_back(topStartIndex + i);
        }

        Mesh* cylinder = new Mesh(name);
        cylinder->InitFromData(vertices, indices);
        return cylinder;
    }

    inline Mesh* Cone(const std::string& name, glm::vec3 position, float radius, float height, glm::vec3 color) {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        int segments = 36;
        float angleStep = 2.0f * M_PI / segments;

        vertices.push_back(VertexFormat(position, color));
        for (int i = 0; i < segments; ++i) {
            float angle = i * angleStep;
            glm::vec3 point = position + glm::vec3(radius * cos(angle), 0, radius * sin(angle));
            vertices.push_back(VertexFormat(point, color));
        }

        glm::vec3 top = position + glm::vec3(0, height, 0);
        vertices.push_back(VertexFormat(top, color));

        for (int i = 1; i <= segments; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i % segments + 1);
        }

        int topIndex = segments + 1;
        for (int i = 1; i <= segments; ++i) {
            int next = i % segments + 1;
            indices.push_back(i);
            indices.push_back(next);
            indices.push_back(topIndex);
        }

        Mesh* cone = new Mesh(name);
        cone->InitFromData(vertices, indices);
        return cone;
    }
}  
