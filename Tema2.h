#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "transform3D.h"
#include "create.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateDroneComponents();
        
        Mesh* GenerateTerrain(const std::string& name, int m, int n, float cellSize);
        void GenerateTree(const std::string& name, glm::vec3 position, float trunkHeight, float trunkRadius, float coneHeight, float coneRadius);
        bool GenerateRandomTrees(int treeCount, float areaSize);
        
        bool CheckCollisions(const glm::vec3& dronePosition, const glm::vec3& position, float x, float y, float z, float margin);
        void GenerateBuilding(const std::string& name, glm::vec3 position, glm::vec3 size, glm::vec3 color);
		void GenerateRandomBuildings(int buildingCount, float areaSize);
        bool CheckCollision(glm::vec3 pos1, glm::vec3 pos2);
        bool CheckCollisionWithGround(glm::vec3 dronePosition, float groundLevel);
        glm::vec3 GenerateRandomColor();

    protected:
        implemented::Camera2 *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        glm::vec3 position = glm::vec3(140, 6, 170);
        glm::vec3 forward = glm::vec3(0, 0, -1);
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = glm::cross(forward, up);

        float speed = 13.0f;
        float rotationSpeed = glm::radians(90.0f);
        float oxAngle = 0.0f;
        float oyAngle = 0.0f;

        std::vector<Mesh*> drone;
        std::vector<glm::vec3> bladeOffsets;
        float bladeRotationAngle = 0.0f;
        std::vector<Mesh*> trees;
        std::vector<glm::vec3> treesPositions;
        std::vector<Mesh*> buildings;
        std::vector<glm::vec3> buildingsPositions;

        struct Tree {
            glm::vec3 position;
            float trunkRadius;
            float trunkHeight;
            float coneHeight;
            float coneRadius;
        };

        struct Building {
            glm::vec3 position;
            float width;
            float height;
            float depth;
        };


        std::vector<Tree> treesData;
        std::vector<Building> buildingsData;

        
    };
}   // namespace m1
