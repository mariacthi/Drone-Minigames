#include "Tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;
using namespace m1;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
    for (auto part : drone) {
        delete part;
    }
	for (auto tree : trees) {
		delete tree;
	}
	for (auto building : buildings) {
		delete building;
	}
}

void Tema2::CreateDroneComponents() 
{
    glm::vec3 droneColors[2] = { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f) };

    std::vector<std::string> componentNames = { "p1", "p2" };
    std::vector<glm::vec3> componentSizes = { glm::vec3(1.2f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 1.2f) };

    for (size_t i = 0; i < componentNames.size(); ++i) {
        Mesh* component = create::Parallelepiped(componentNames[i],glm::vec3(0), componentSizes[i], droneColors[0]);
        drone.push_back(component);
    }

    float offset = 0.55f;
    glm::vec3 bladeSize = glm::vec3(0.5f, 0.025f, 0.1f);

    for (int i = 0; i < 4; ++i) {
        glm::vec3 cubePos;
        switch (i) {
        case 0:
            cubePos = glm::vec3(offset, 0.1f, 0); // față-stânga
            break;
        case 1:
            cubePos = glm::vec3(-offset, 0.1f, 0); // față-dreapta
            break;
        case 2:
            cubePos = glm::vec3(0, 0.1f, offset); // spate-stânga
            break;
        case 3:
            cubePos = glm::vec3(0, 0.1f, -offset); // spate-dreapta
            break;
        default:
            cubePos = glm::vec3(0);
            break;
        }

        Mesh* cube = create::Parallelepiped("cube" + std::to_string(i), cubePos, glm::vec3(0.1f), droneColors[0]);
        drone.push_back(cube);

        glm::vec3 bladePos = cubePos + glm::vec3(0, 0.07f, 0);
        Mesh* blade = create::Parallelepiped("blade" + std::to_string(i),bladePos, bladeSize, droneColors[1]);
        drone.push_back(blade);

        bladeOffsets.emplace_back(bladePos);
    }
}


Mesh* Tema2::GenerateTerrain(const std::string& name, int m, int n, float cellSize)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            glm::vec3 position = glm::vec3(j * cellSize, 0, i * cellSize);
            vertices.emplace_back(position);
        }
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            unsigned int topLeft = i * (n + 1) + j;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (i + 1) * (n + 1) + j;
            unsigned int bottomRight = bottomLeft + 1;

            // Primul triunghi
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Al doilea triunghi
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh* terrain = new Mesh(name);
    terrain->InitFromData(vertices, indices);
    return terrain;
}

bool Tema2::CheckCollision(glm::vec3 pos1, glm::vec3 pos2) 
{
    float distanceSquared = pow(pos1.x - pos2.x, 2) + pow(pos1.z - pos2.z, 2);
    float minDistance = 25.0f;
    return distanceSquared < minDistance * minDistance;
}

void Tema2::GenerateTree(const std::string& name, glm::vec3 position, float trunkHeight, float trunkRadius, float coneHeight, float coneRadius) 
{
    Mesh* trunk = create::Cylinder(name + "_trunk", position, trunkRadius, trunkHeight, glm::vec3(0.7f, 0.4f, 0.1f));
    trees.push_back(trunk);

    glm::vec3 coneBase = position + glm::vec3(0, trunkHeight, 0);
    Mesh* leavesBottom = create::Cone(name + "_leaves_bottom", coneBase, coneRadius, coneHeight, glm::vec3(0.0f, 0.7f, 0.1f));
    trees.push_back(leavesBottom);

    glm::vec3 coneTopBase = coneBase + glm::vec3(0, coneHeight * 0.6f, 0);
    Mesh* leavesTop = create::Cone(name + "_leaves_top", coneTopBase, coneRadius * 0.8f, coneHeight * 0.8f, glm::vec3(0.0f, 0.7f, 0.1f));
    trees.push_back(leavesTop);
}

bool Tema2::GenerateRandomTrees(int treeCount, float areaSize) 
{   
    int valid = 0;
    for (int i = 0; i < treeCount; i++) {
        float x = static_cast<float>(rand()) / RAND_MAX * areaSize;
        float z = static_cast<float>(rand()) / RAND_MAX * areaSize;

        if (x < 0) {
            x = -x;
        } 
		if (z < 0) {
			z = -z;
		}

        glm::vec3 treePosition = glm::vec3(x, 0, z);
        bool validPosition = true;
        
        for (const auto& tree : treesPositions) {
            if (CheckCollision(treePosition, tree)) {
                validPosition = false;
                break;
            }
        }

        for (const auto& building : buildingsPositions) {
            if (CheckCollision(treePosition, building)) {
                validPosition = false;
                break;
            }
        }

        if (validPosition) {
            treesPositions.push_back(treePosition); 
            float trunkHeight = static_cast<float>(rand()) / RAND_MAX * 5.0f + 2.0f;
            float trunkRadius = static_cast<float>(rand()) / RAND_MAX * 1.0f + 0.5f;
            float coneHeight = trunkHeight + static_cast<float>(rand()) / RAND_MAX * 5.0f + 1.0f;
            float coneRadius = trunkRadius + static_cast<float>(rand()) / RAND_MAX * 3.0f + 0.5f;

            Tree tree = { treePosition, trunkRadius, trunkHeight, coneHeight, coneRadius };
            treesData.push_back(tree);

            GenerateTree("tree_" + std::to_string(i), treePosition, trunkHeight, trunkRadius, coneHeight, coneRadius);
            valid++;
        }
    }

    if (valid >= 5) {
        return true;
    }
    return false;
}

glm::vec3 Tema2::GenerateRandomColor() 
{
    return glm::vec3(static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX);
}

void Tema2::GenerateBuilding(const std::string& name, glm::vec3 position, glm::vec3 size, glm::vec3 color) 
{
    Mesh* building = create::Parallelepiped(name, position, size, color);
    buildings.push_back(building);
}

void Tema2::GenerateRandomBuildings(int buildingCount, float areaSize) 
{
    for (int i = 0; i < buildingCount; i++) {
        float x = static_cast<float>(rand()) / RAND_MAX * areaSize;
        float z = static_cast<float>(rand()) / RAND_MAX * areaSize;
       
        if (x < 0) {
            x = -x;
        }
        if (z < 0) {
            z = -z;
        }
        glm::vec3 position = glm::vec3(x, 0, z);
        bool validPosition = true;

        for (const auto& tree : treesPositions) {
            if (CheckCollision(position, tree)) {
                validPosition = false;
                break;
            }
        }

        for (const auto& building : buildingsPositions) {
            if (CheckCollision(position, building)) {
                validPosition = false;
                break;
            }
        }

        if (validPosition) {
            buildingsPositions.push_back(position);
            float width = static_cast<float>(rand()) / RAND_MAX * 10.0f + 5.0f;
            float height = static_cast<float>(rand()) / RAND_MAX * 20.0f + 10.0f;
            float depth = static_cast<float>(rand()) / RAND_MAX * 20.0f + 2.0f; 
            
            Building building = { position, width, height, depth };
            buildingsData.push_back(building);
            GenerateBuilding("building_" + std::to_string(i), position, glm::vec3(width, height, depth), GenerateRandomColor());
        }  
    }
}

void Tema2::Init()
{
    renderCameraTarget = false;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    CreateDroneComponents();
    
    camera = new implemented::Camera2();

    glm::vec3 cameraOffset = glm::vec3(0, 2, 5);
    glm::vec3 cameraPosition = position - forward * cameraOffset.z + up * cameraOffset.y;
    glm::vec3 cameraTarget = position;

    camera->Set(cameraPosition, cameraTarget, up);
    
    Mesh* terrain = GenerateTerrain("terrain", 50, 50, 5.0f);
    meshes[terrain->GetMeshID()] = terrain;
   
    Shader* shader = new Shader("TerrainShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

	if (!GenerateRandomTrees(30, 250.0f)) {
		throw std::runtime_error("Failed to generate 5 trees");
	}
    GenerateRandomBuildings(30, 250.0f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds) 
{   
    RenderMesh(meshes["terrain"], shaders["TerrainShader"], glm::mat4(1));
    
    for (size_t i = 0; i < trees.size(); i++) {
        Mesh* treePart = trees[i];
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(treePart, shaders["VertexColor"], modelMatrix);
    }

    for (size_t i = 0; i < buildings.size(); i++) {
        Mesh* building = buildings[i];
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(building, shaders["VertexColor"], modelMatrix);
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(position.x, position.y, position.z);
    modelMatrix *= transform3D::RotateOY(oyAngle);
    modelMatrix *= transform3D::RotateOX(oxAngle);

    bladeRotationAngle += glm::radians(720.0f) * deltaTimeSeconds;
    bladeRotationAngle = fmod(bladeRotationAngle, glm::radians(360.0f));
    for (size_t i = 0; i < drone.size(); ++i) {
        Mesh* part = drone[i];
        glm::mat4 partModelMatrix = modelMatrix;

        if (i % 2 == 1 && i > 2) {
            glm::vec3 offset = bladeOffsets[i / 2 - 1];
            partModelMatrix *= transform3D::Translate(offset.x, offset.y, offset.z);
            partModelMatrix *= transform3D::RotateOY(bladeRotationAngle);
            partModelMatrix *= transform3D::Translate(-offset.x, -offset.y, -offset.z);
        }

        RenderMesh(part, shaders["VertexColor"], partModelMatrix);
    }
  
}

bool Tema2::CheckCollisionWithGround(glm::vec3 dronePosition, float groundLevel)
{
    return dronePosition.y <= groundLevel + 1.5f;
}

bool Tema2::CheckCollisions(const glm::vec3& dronePosition, const glm::vec3& position, float x, float y, float z, float margin)
{
    bool withinX = dronePosition.x >= (position.x - x - margin) &&
        dronePosition.x <= (position.x + x + margin);

    bool withinY = dronePosition.y >= (position.y - margin) &&
        dronePosition.y <= (position.y + y + margin);

    bool withinZ = dronePosition.z >= (position.z - z - margin) &&
        dronePosition.z <= (position.z + z + margin);

    return withinX && withinY && withinZ;
}

void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec3 camForward = glm::vec3(camera->forward.x, 0, camera->forward.z);
    glm::vec3 camRight = glm::vec3(camera->right.x, 0, camera->right.z);

    glm::vec3 lastPosition = position;
    bool dontMove = false;

    if (window->KeyHold(GLFW_KEY_W)) {
        position += camForward * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        position -= camForward * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        position -= camRight * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        position += camRight * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        position += up * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        position -= up * speed * deltaTime;
    }
    
    if (window->KeyHold(GLFW_KEY_Z)) {
        oyAngle += deltaTime;
		dontMove = true;
    }

    if (window->KeyHold(GLFW_KEY_X)) {
        oyAngle -= deltaTime;
        dontMove = true;
    }

    if (window->KeyHold(GLFW_KEY_C)) {
        glm::mat4 rotationMatrix = transform3D::RotateOY(-deltaTime);
        forward = glm::vec3(rotationMatrix * glm::vec4(forward, 0));
        right = glm::cross(forward, up);
        oyAngle -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_V)) {
        glm::mat4 rotationMatrix = transform3D::RotateOY(deltaTime);
        forward = glm::vec3(rotationMatrix * glm::vec4(forward, 0));
        right = glm::cross(forward, up);
        oyAngle = deltaTime;
    }

    if (CheckCollisionWithGround(position, 0.0f)) {
        position = lastPosition; 
    }

    for (const Tree& tree : treesData) {
        if (CheckCollisions(position, tree.position, tree.trunkRadius, tree.trunkHeight + tree.coneHeight, tree.trunkRadius,2.0f)) {
            position = lastPosition;
            break;
        }
    }

    for (const Building& building : buildingsData) {
        if (CheckCollisions(position, building.position, building.width / 2, building.height, building.depth / 2, 0.5f)) {
            position = lastPosition;
            break;
        }
    }

    if (!dontMove) {
        glm::vec3 cameraOffset = glm::vec3(0, 2, 5);
        glm::vec3 cameraPosition = position - forward * cameraOffset.z + up * cameraOffset.y;
        glm::vec3 cameraTarget = position;

        camera->Set(cameraPosition, cameraTarget, up);
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_P)
    {
        projectionMatrix = glm::perspective(RADIANS(45), window->props.aspectRatio, 0.01f, 200.0f);
    }

    if (key == GLFW_KEY_O)
    {
        projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 200.0f);
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}