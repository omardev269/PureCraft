//BismIllahirRahmanirRaheem
#include <MCGL.hpp>
using namespace MCGL;
typedef unsigned char BlockType;
#include <vertices.hpp>
#include <thread>
#include <atomic>
namespace CommunityCraft {
	std::atomic<unsigned> tamountOfVertices;
	unsigned worldSeed = 0;
	bool IsAirW(ivec3 position);
	Instance* instance;
	GameObject* mesh;
	GameObject* crosshair;
	GameObject* collider;
	GPU* gpu;
	vec3 location(0,-64,0);
	vec3 coords(0,0,0);
	mat4 transform(1.0f);
	vec3 rotation(0,0,0);
	mat4 crosshairTransform(1.0f);
	std::vector<float> crosshairVertices = {
			-0.125f, 0.125f, 0.0f, 0.0f, 0.0f,
			0.125f, 0.125f, 0.0f, 1.0f, 0.0f,
			0.125f,-0.125f, 0.0f, 1.0f, 1.0f,
			0.125f,-0.125f, 0.0f, 1.0f, 1.0f,
			-0.125f,-0.125f, 0.0f, 0.0f, 1.0f,
			-0.125f,0.125f, 0.0f, 0.0f, 0.0f,
	};
	bool fframeran = false;
	float speed = 1.0f;
	std::vector<std::string> toPrint;
	bool inGame = false;
	bool wasEscape = false;
	unsigned char currentScreen = 5; //0 = main menu, 1 = settings, 2 = debug data, 3 = statistics, 4 = about
	bool showDemoWindow = false;
	bool drag = false;
	unsigned amountOfBlockActions = 0;
	int maxBlocks;
}
#include <hierarchy/chunk.hpp>
#include <hierarchy/world.hpp>
#include <raycast.hpp>
bool rf = true;
double timeBeforeIG = 0;
double timeAfterIG = 0;
double otimeBeforeIG = 0;
double otimeAfterIG = 0;
double timeBeforeCP = 0;
double timeAfterCP = 0;
double timeBeforeGP = 0;
double timeAfterGP = 0;
double timeBeforeRP = 0;
double timeAfterRP = 0;
//uint fshaders, nshaders;
u16 sisterShark;
void Update() {
	//CommunityCraft::gpu->UseShaders(fshaders);
	//glUniform3f(glGetUniformLocation(CommunityCraft::gpu->programAddress, "playerLocation"), CommunityCraft::coords.x, CommunityCraft::coords.y, CommunityCraft::coords.z);
	if (CommunityCraft::currentScreen == 5) CommunityCraft::instance->SetMouseVisibility(true);
	if (!CommunityCraft::wasEscape && CommunityCraft::instance->GetKey(GLFW_KEY_ESCAPE)) {
		if (CommunityCraft::inGame) {
			CommunityCraft::inGame = false;
			CommunityCraft::instance->SetMouseVisibility(true);
		}
		else {
			if (CommunityCraft::currentScreen != 5) {
				CommunityCraft::inGame = true;
				CommunityCraft::instance->SetMouseVisibility(false);
			}
		}
	}
	glEnable(GL_DEPTH_TEST);
	if (CommunityCraft::inGame) {
		if (CommunityCraft::instance->GetKey(GLFW_KEY_W)) {
			CommunityCraft::location.z += CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * cos(radians(CommunityCraft::rotation.y));
			CommunityCraft::location.x -= CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * sin(radians(CommunityCraft::rotation.y));

		}
		if (CommunityCraft::instance->GetKey(GLFW_KEY_S)) {
			CommunityCraft::location.z += -CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * cos(radians(CommunityCraft::rotation.y));
			CommunityCraft::location.x -= -CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * sin(radians(CommunityCraft::rotation.y));

		}
		if (CommunityCraft::instance->GetKey(GLFW_KEY_A)) {
			CommunityCraft::location.z += CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * sin(radians(CommunityCraft::rotation.y));
			CommunityCraft::location.x += CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * cos(radians(CommunityCraft::rotation.y));

		}
		if (CommunityCraft::instance->GetKey(GLFW_KEY_D)) {
			CommunityCraft::location.z += -CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * sin(radians(CommunityCraft::rotation.y));
			CommunityCraft::location.x += -CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime) * cos(radians(CommunityCraft::rotation.y));

		}
		if (CommunityCraft::instance->GetKey(GLFW_KEY_SPACE)) {
			CommunityCraft::location.y -= CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime);

		}
		if (CommunityCraft::instance->GetKey(GLFW_KEY_LEFT_SHIFT)) {
			CommunityCraft::location.y -= -CommunityCraft::speed * (10.0f * CommunityCraft::instance->deltaTime);

		}
		if (CommunityCraft::fframeran) {
			CommunityCraft::rotation.y += CommunityCraft::instance->mousePosition.x - CommunityCraft::instance->oldMousePosition.x;
			CommunityCraft::rotation.x += CommunityCraft::instance->mousePosition.y - CommunityCraft::instance->oldMousePosition.y;
		}

		CommunityCraft::transform = Transform::Projection(CommunityCraft::instance->windowSize, 90.0f, 0.1f, 100.0f);
		CommunityCraft::transform = Transform::Rotate(CommunityCraft::transform, CommunityCraft::rotation);
		CommunityCraft::transform = Transform::Translate(CommunityCraft::transform, CommunityCraft::location);
	}
	CommunityCraft::gpu->UseTexture2D(0, "objectTexture");
	CommunityCraft::gpu->SetTransform(CommunityCraft::transform, "transform");
	CommunityCraft::RaycastUpdate();
	timeBeforeGP = glfwGetTime();
	CommunityCraft::WorldUpdate();
	timeAfterGP = glfwGetTime();
	timeBeforeCP = glfwGetTime();
	glDisable(GL_DEPTH_TEST);
	CommunityCraft::gpu->UseTexture2D(2, "objectTexture");
	CommunityCraft::gpu->SetTransform(CommunityCraft::crosshairTransform, "transform");
	CommunityCraft::crosshair->Render();
	timeAfterCP = glfwGetTime();
	timeBeforeIG = glfwGetTime();
	CommunityCraft::coords = -CommunityCraft::location;
	if (!CommunityCraft::inGame) {
		switch (CommunityCraft::currentScreen) {
		case 0:
			ImGui::SetNextWindowCollapsed(false);
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ CommunityCraft::instance->windowSize.x / 1.0f, CommunityCraft::instance->windowSize.y / 1.0f });
			ImGui::Begin("Game paused");
			ImGui::SetCursorPos({ CommunityCraft::instance->windowSize.x / 4.0f - (CommunityCraft::instance->windowSize.x / 4.0f) / 2.0f, CommunityCraft::instance->windowSize.y / 4.5f - (CommunityCraft::instance->windowSize.y / 20.0f) / 2.0f });
			if (ImGui::Button("About", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 4;
			ImGui::SetCursorPosX(CommunityCraft::instance->windowSize.x / 4.0f - (CommunityCraft::instance->windowSize.x / 4.0f) / 2.0f);
			if (ImGui::Button("Debug data", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 2;
			ImGui::SetCursorPosX(CommunityCraft::instance->windowSize.x / 4.0f - (CommunityCraft::instance->windowSize.x / 4.0f) / 2.0f);
			if (ImGui::Button("Statistics", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 3;
			ImGui::SetCursorPosX(CommunityCraft::instance->windowSize.x / 4.0f - (CommunityCraft::instance->windowSize.x / 4.0f) / 2.0f);
			if (ImGui::Button("Settings", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 1;
			ImGui::SetCursorPosX(CommunityCraft::instance->windowSize.x / 4.0f - (CommunityCraft::instance->windowSize.x / 4.0f) / 2.0f);
			CommunityCraft::instance->running = !ImGui::Button("Exit Game", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f });
			ImGui::End();
			break;
		case 1:
			ImGui::SetNextWindowCollapsed(false);
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ CommunityCraft::instance->windowSize.x / 1.0f, CommunityCraft::instance->windowSize.y / 1.0f });
			ImGui::Begin("Settings");
			if (ImGui::Button("Exit to escape menu", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 0;
			ImGui::SliderFloat("Movement speed", &CommunityCraft::speed, 0.1f, 10.0f);
			ImGui::Checkbox("Free mouse movement", &CommunityCraft::fframeran);
			ImGui::Checkbox("Show ImGui demo window", &CommunityCraft::showDemoWindow);
			if (CommunityCraft::showDemoWindow) ImGui::ShowDemoWindow();
			ImGui::End();
			break;
		case 2:
			ImGui::SetNextWindowCollapsed(false);
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ CommunityCraft::instance->windowSize.x / 1.0f, CommunityCraft::instance->windowSize.y / 1.0f });
			ImGui::Begin("Debug data");
			if (ImGui::Button("Exit to escape menu", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 0;
			ImGui::Text("FPS: %d", (int)(1.0f / CommunityCraft::instance->deltaTime));
			ImGui::Text("GPU: %s", CommunityCraft::gpu->name.c_str());
			ImGui::Text("Driver version: OpenGL %s", CommunityCraft::gpu->glversion.c_str());
			ImGui::Text("GLFW version: %d.%d.%d", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
			ImGui::Text("MCGL version: %s", MCGL_VERSION_STRING);
			ImGui::Text("ImGui version: %s", ImGui::GetVersion());
			ImGui::End();
			break;
		case 3:
			ImGui::SetNextWindowCollapsed(false);
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ CommunityCraft::instance->windowSize.x / 1.0f, CommunityCraft::instance->windowSize.y / 1.0f });
			ImGui::Begin("Statistics");
			if (ImGui::Button("Exit to escape menu", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 0;
			ImGui::Text("Amount of block actions: %d", CommunityCraft::amountOfBlockActions);
			ImGui::End();
			break;
		case 4:
			ImGui::SetNextWindowCollapsed(false);
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ CommunityCraft::instance->windowSize.x / 1.0f, CommunityCraft::instance->windowSize.y / 1.0f });
			ImGui::Begin("About");
			if (ImGui::Button("Exit to escape menu", { CommunityCraft::instance->windowSize.x / 4.0f, CommunityCraft::instance->windowSize.y / 20.0f })) CommunityCraft::currentScreen = 0;
			ImGui::Text("CommunityCraft is a open-source voxel game engine made with MCGL.");
			ImGui::Text("PureCraft is a CommunityCraft-based voxel game made for pure mining and crafting, no distractions.");
			ImGui::Text("(C) omardev269 2026");
			ImGui::End();
			break;
		case 5:
			ImGui::SetNextWindowCollapsed(false);
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ CommunityCraft::instance->windowSize.x / 1.0f, CommunityCraft::instance->windowSize.y / 1.0f });
			ImGui::Begin("PureCraft Build 4");
			ImGui::Text("Welcome to PureCraft");
			ImGui::InputInt("Maximum amount of blocks", &CommunityCraft::maxBlocks);
			if (CommunityCraft::maxBlocks < 0) CommunityCraft::maxBlocks = 0;
			if (ImGui::Button("Continue")) CommunityCraft::currentScreen = 0;
			ImGui::End();
			break;
		}
	}
	ImGui::SetNextWindowPos({ 20,20 });
	ImGui::Begin("Debug Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Location: X:%d Y:%d Z:%d", (int)CommunityCraft::coords.x, (int)CommunityCraft::coords.y, (int)CommunityCraft::coords.z);
	ImGui::Text("Amount Of Vertices: %d", CommunityCraft::mesh->amountOfVertices);
	ImGui::Text("TAmount Of Vertices: %d", CommunityCraft::tamountOfVertices.load());
	ImGui::Text("Profiler found %f seconds for ImGui", otimeAfterIG - otimeBeforeIG);
	ImGui::Text("Profiler found %f seconds for GenProc", timeAfterGP - timeBeforeGP);
	ImGui::Text("Profiler found %f seconds for CrosshairProc", timeAfterCP - timeBeforeCP);
	sisterShark = 0;
	for (auto& babyShark : CommunityCraft::chunksInIvec3) {
		//ImGui::Text("Chunk %d at X %d Y %d Z %d", sisterShark, babyShark.x, babyShark.y, babyShark.z);
		sisterShark++;
	}
	ImGui::End();
	timeAfterIG = glfwGetTime();
	if (!CommunityCraft::fframeran&&rf) {
		CommunityCraft::fframeran = true;
		rf = false;
	}
	if (1.0f / CommunityCraft::instance->deltaTime < 60) {
		std::println("Low FPS detected: {}", (int)(1.0f / CommunityCraft::instance->deltaTime));
	}
	CommunityCraft::wasEscape = CommunityCraft::instance->GetKey(GLFW_KEY_ESCAPE);
	otimeAfterIG = timeAfterIG;
	otimeBeforeIG = timeBeforeIG;
	if (CommunityCraft::maxBlocks > 0) {
		if (CommunityCraft::amountOfBlockActions > CommunityCraft::maxBlocks) CommunityCraft::instance->running = false;
	}
}
int main() {
	CommunityCraft::instance = new Instance(ivec2(800, 600), "PureCraft Build 4", false, true);
	CommunityCraft::instance->SetMouseVisibility(false);
	CommunityCraft::instance->SetBackgroundColor(vec3(0.4, 0.6, 1.0));
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	CommunityCraft::instance->showOverlay = true;
	CommunityCraft::instance->showCopyright = true;
	CommunityCraft::gpu = new GPU;
	//fshaders = CommunityCraft::gpu->programAddress;
	//CommunityCraft::gpu->fragmentShader = CommunityCraft::gpu->nonFogShader;
	//CommunityCraft::gpu->CompileShaders();
	//nshaders = CommunityCraft::gpu->programAddress;
	CommunityCraft::crosshairTransform = Transform::Projection(CommunityCraft::instance->windowSize, 55.0f, 0.1f, 100.0f);
	CommunityCraft::crosshairTransform = Transform::Translate(CommunityCraft::crosshairTransform, vec3(0.0f, 0.0f, -5.5f));
	CommunityCraft::gpu->RegisterTexture2D(0, RESOURCES_PATH "blocks.bmp");
	CommunityCraft::gpu->RegisterTexture2D(1, RESOURCES_PATH "bmp.png");
	CommunityCraft::gpu->RegisterTexture2D(2, RESOURCES_PATH "defaultCrosshair.png");
	std::vector<float> dumb;
	CommunityCraft::mesh = new GameObject(dumb, 0x800000, 5);
	CommunityCraft::mesh->SetUsage(1, 3, 2);
	CommunityCraft::mesh->amountOfVertices = 0;
	CommunityCraft::crosshair = new GameObject(CommunityCraft::crosshairVertices, 6, 5);
	CommunityCraft::crosshair->SetUsage(1, 3, 2);
	CommunityCraft::collider = new GameObject(dumb, 36, 5);
	CommunityCraft::collider->SetUsage(1, 3, 2);
	CommunityCraft::WorldInit();
	CommunityCraft::instance->Start(Update);
	CommunityCraft::WorldEnd();
	delete CommunityCraft::mesh;
	delete CommunityCraft::collider;
	delete CommunityCraft::crosshair;
	delete CommunityCraft::gpu;
	delete CommunityCraft::instance;
	return 0;
}