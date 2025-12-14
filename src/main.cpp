#include <MCGL.hpp>
#include <perlin.hpp>
#include <time.h>
#include <thread>
using namespace MCGL;
namespace OpenCraft{
	Instance* instance;
	GPU* gpu;
	GameObject* mesh;
	GameObject* crosshair;
	mat4 transform(1.0f);
	ivec3 coordinates;
	vec3 location = { -8, -16, -8 };
	vec3 rotation = { 0,0,0 };
	bool fframeran = false;
	std::vector<std::string> toPrint;
	std::vector<float> crosshairVertices = {
		-0.125, 0.125, 0, 0,0,
		0.125, 0.125,  0, 1,0,
		0.125, -0.125, 0, 1,1,
		0.125, -0.125, 0, 1,1,
		-0.125, -0.125,0, 0,1,
		-0.125, 0.125, 0, 0,0
	};
	std::vector<float> dumb;
}
typedef unsigned char BlockType;
#include <defaultCrosshair.hpp>
#include <blocks.hpp>
#include <Models.hpp>
#include <ElementHierarchy/chunk.hpp>
#include <ElementHierarchy/world.hpp>
namespace OpenCraft {
	World* world;
}
#include <raycast.hpp>
const unsigned renderDistance = 3;
std::vector<ivec3> exceptionLista;
std::vector<ivec3> exceptionList;
#include <ElementHierarchy/gen.hpp>
bool freefly = true;
void Update(){

	OpenCraft::gpu->Flush();
	OpenCraft::gpu->UseTexture2D(0, "objectTexture");
	glEnable(GL_DEPTH_TEST);
	OpenCraft::instance->running = !OpenCraft::instance->GetKey(GLFW_KEY_ESCAPE);
	if (OpenCraft::instance->GetKey(GLFW_KEY_W)) { 
		OpenCraft::location.z += (10.0f * OpenCraft::instance->deltaTime)*cos(radians(OpenCraft::rotation.y));
		OpenCraft::location.x -= (10.0f * OpenCraft::instance->deltaTime)*sin(radians(OpenCraft::rotation.y));
		if (!freefly) {
			if (!OpenCraft::world->IsAir(-OpenCraft::location) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 1, -OpenCraft::location.z }) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 2, -OpenCraft::location.z })) {
				OpenCraft::location.z -= (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
				OpenCraft::location.x += (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
			}
		}
	}
	if (OpenCraft::instance->GetKey(GLFW_KEY_S)) {
		OpenCraft::location.z -= (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
		OpenCraft::location.x += (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
		if (!freefly) {
			if (!OpenCraft::world->IsAir(-OpenCraft::location) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 1, -OpenCraft::location.z }) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 2, -OpenCraft::location.z })) {
				OpenCraft::location.z += (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
				OpenCraft::location.x -= (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
			}
		}
	}
	if (OpenCraft::instance->GetKey(GLFW_KEY_A)) {
		OpenCraft::location.x += (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
		OpenCraft::location.z += (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
		if (!freefly) {
			if (!OpenCraft::world->IsAir(-OpenCraft::location) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 1, -OpenCraft::location.z }) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 2, -OpenCraft::location.z })) {
				OpenCraft::location.x -= (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
				OpenCraft::location.z -= (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
			}
		}
	}
	if (OpenCraft::instance->GetKey(GLFW_KEY_D)) {
		OpenCraft::location.x -= (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
		OpenCraft::location.z -= (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
		if (!freefly) {
			if (!OpenCraft::world->IsAir(-OpenCraft::location) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 1, -OpenCraft::location.z }) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 2, -OpenCraft::location.z })) {
				OpenCraft::location.x += (10.0f * OpenCraft::instance->deltaTime) * cos(radians(OpenCraft::rotation.y));
				OpenCraft::location.z += (10.0f * OpenCraft::instance->deltaTime) * sin(radians(OpenCraft::rotation.y));
			}
		}
	}
	if (OpenCraft::instance->GetKey(GLFW_KEY_SPACE)) {
		OpenCraft::location.y -= (10.0f * OpenCraft::instance->deltaTime);
		if (!freefly) {
			if (!OpenCraft::world->IsAir(-OpenCraft::location) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 1, -OpenCraft::location.z }) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 2, -OpenCraft::location.z })) {
				OpenCraft::location.y += (10.0f * OpenCraft::instance->deltaTime);
			}
		}
	}
	if (OpenCraft::instance->GetKey(GLFW_KEY_LEFT_SHIFT)) {
		OpenCraft::location.y += (10.0f * OpenCraft::instance->deltaTime);
		if (!freefly) {
			if (!OpenCraft::world->IsAir(-OpenCraft::location) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 1, -OpenCraft::location.z }) || !OpenCraft::world->IsAir({ -OpenCraft::location.x, -OpenCraft::location.y - 2, -OpenCraft::location.z })) {
				OpenCraft::location.y -= (10.0f * OpenCraft::instance->deltaTime);
			}
		}
	}
	if (OpenCraft::fframeran) {
		OpenCraft::rotation.y += (OpenCraft::instance->mousePosition.x - OpenCraft::instance->oldMousePosition.x);
		OpenCraft::rotation.x += (OpenCraft::instance->mousePosition.y - OpenCraft::instance->oldMousePosition.y);
	}
	OpenCraft::rotation.x = clamp(OpenCraft::rotation.x, -90.0f, 90.0f);
	OpenCraft::transform = Transform::Projection(OpenCraft::instance->windowSize, 55.0f, 0.05f, 100.0f);
	OpenCraft::transform = Transform::Rotate(OpenCraft::transform, OpenCraft::rotation);
	OpenCraft::transform = Transform::Translate(OpenCraft::transform, OpenCraft::location);
	OpenCraft::gpu->SetTransform(OpenCraft::transform, "transform");
	OpenCraft::mesh->Render();
	OpenCraft::transform = Transform::Projection(OpenCraft::instance->windowSize, 55.0f, 0.05f, 100.0f);
	OpenCraft::transform = Transform::Translate(OpenCraft::transform, vec3(0,0,-7.5f));
	OpenCraft::gpu->SetTransform(OpenCraft::transform, "transform");
	OpenCraft::gpu->UseTexture2D(2, "objectTexture");
	glDisable(GL_DEPTH_TEST);
	OpenCraft::crosshair->Render();
	OpenCraft::RaycastUpdate();
	OpenCraft::GenUpdate();
	OpenCraft::gpu->UseTexture2D(1, "objectTexture");
	OpenCraft::gpu->SetTransform2D(OpenCraft::instance->windowSize, "transform");
	MCGL::UI::Print("PureCraft Build 1");
	MCGL::UI::Print(std::format("XYZ: {} {} {}", OpenCraft::coordinates.x, OpenCraft::coordinates.y, OpenCraft::coordinates.z));
	for (auto& str : OpenCraft::toPrint) {
		MCGL::UI::Print(str);
	}
	if (!OpenCraft::fframeran)OpenCraft::fframeran = true;
	OpenCraft::coordinates = -OpenCraft::location;

}
int main() {
	OpenCraft::instance = new Instance({ 800,600 }, "PureCraft Build 1", false, true);
	OpenCraft::instance->SetBackgroundColor(vec3(0.4f, 0.6f, 1.0f));
	OpenCraft::instance->SetMouseVisibility(false);
	OpenCraft::gpu = new GPU;
	OpenCraft::gpu->RegisterTexture2D_S(0, blocksData, blocksSize);
	OpenCraft::gpu->RegisterTexture2D_S(1, UI::bitmap, UI::bitmapSize);
	OpenCraft::gpu->RegisterTexture2D_S(2, defaultCrosshairData, defaultCrosshairSize);
	OpenCraft::mesh = new GameObject(OpenCraft::dumb, 36 * 32*32*32*32*32, 5);
	OpenCraft::mesh->amountOfVertices = 0;
	OpenCraft::mesh->SetUsage(1, 3, 2);
	OpenCraft::crosshair = new GameObject(OpenCraft::crosshairVertices, 6, 5);
	OpenCraft::crosshair->SetUsage(1, 3, 2);
	OpenCraft::world = new OpenCraft::World(static_cast<unsigned>(clock()));
	OpenCraft::mesh->vertices = OpenCraft::world->vertices;
	OpenCraft::mesh->amountOfVertices = OpenCraft::world->amountOfVertices;
	OpenCraft::mesh->UpdateData();
	OpenCraft::instance->Start(Update);
	OpenCraft::OnEnd();
	delete OpenCraft::world;
	delete OpenCraft::mesh;
	delete OpenCraft::gpu;
	delete OpenCraft::instance;
	return 2;

}
