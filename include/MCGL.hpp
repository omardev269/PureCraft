#ifndef MCGLIncluded
#define MCGLIncluded
#include <time.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <cstring>
#include <vector>
#include <format>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <print>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>
#include <filesystem>
namespace MCGL {
	using namespace glm;
	namespace UI {
		unsigned cyptr = 0;
	}

	class GPU {
	public:
		const char* defaultVertexShader = R"(
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
out vec2 fragUV;
out vec3 fragPos;
uniform mat4 transform;
void main() {
	fragUV = uv;
	fragPos = pos;
	gl_Position = transform*vec4(pos, 1.0);
}
		)";
		const char* defaultFragmentShader = R"(
#version 330 core
in vec2 fragUV;
in vec3 fragPos;
out vec4 color;
uniform sampler2D objectTexture;
uniform vec3 playerLocation;
void main() {
	color = texture(objectTexture, fragUV);
}
		
		)";
		std::string vertexShader = defaultVertexShader;
		std::string fragmentShader = defaultFragmentShader;
		std::string name, vendor, glversion;
		GLuint programAddress;
		bool DepthAllowed = false;
		GPU() {
			name = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
			glversion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
			CompileShaders();

		}
		void UseShaders(uint gprogramAddress) {
			programAddress = gprogramAddress;
			glUseProgram(programAddress);
		}
		void CompileShaders() {
			GLuint vert, frag;
			const char* fs = fragmentShader.c_str();
			const char* vs = vertexShader.c_str();
			vert = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vert, 1, &vs, NULL);
			glCompileShader(vert);
			frag = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(frag, 1, &fs, NULL);
			glCompileShader(frag);
			programAddress = glCreateProgram();
			glAttachShader(programAddress, vert);
			glAttachShader(programAddress, frag);
			glLinkProgram(programAddress);
			glUseProgram(programAddress);
		}
		void Flush() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (DepthAllowed) { glEnable(GL_DEPTH_TEST); }
			else { glDisable(GL_DEPTH_TEST); }
			MCGL::UI::cyptr = 0;

		}
		uint RegisterTexture2D(uint slot, std::string filename) {
			const char* cfn = filename.c_str();
			uint id;
			glGenTextures(1, &id);
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			int width, height;
			unsigned char* image = stbi_load(cfn, &width, &height, nullptr, STBI_rgb_alpha);
			/*std::vector<unsigned char> vimage;
			for (int s = 0; s < width * height * 4; s++)vimage.push_back(image[s]);
			std::println("{}", vimage);*/
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
			return id;
		}
		uint RegisterTexture2D_AA(uint slot, std::string filename) {
			const char* cfn = filename.c_str();
			uint id;
			glGenTextures(1, &id);
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			int width, height;
			unsigned char* image = stbi_load(cfn, &width, &height, nullptr, STBI_rgb_alpha);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
			return id;
		}
		// note: data given should be in 0 - 255 format.
		uint RegisterTexture2D_S(uint slot, unsigned char* image, uvec2 size) {
			uint id;
			glGenTextures(1, &id);
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			return id;
		}
		void DeleteTexture2D(uint id) {
			glDeleteTextures(1, &id);
		}
		void UseTexture2D(uint slot, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform1i(place, slot);
		}
		void SetTransform2D(glm::vec2 screenSize, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glm::mat4 transform = glm::ortho(0.0f, (float)((uint)screenSize[0]), (float)((uint)screenSize[1]), 0.0f);
			glUniformMatrix4fv(place, 1, GL_FALSE, value_ptr(transform));
		}
		void SetTransform(glm::mat4 transformMatrix, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniformMatrix4fv(place, 1, GL_FALSE, value_ptr(transformMatrix));
		}
		void Set(vec3 data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform3f(place, data.x, data.y, data.z);
		}
		void Set(float data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform1f(place, data);
		}
		void Set(int data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform1i(place, data);
		}
		void Set(bool data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform1i(place, data);
		}
		void Set(uint data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform1ui(place, data);
		}
		void Set(vec2 data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform2f(place, data.x, data.y);
		}
		void Set(vec4 data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniform4f(place, data.x, data.y, data.z, data.w);
		}
		void Set(mat4 data, std::string nameInShader) {
			uint place = glGetUniformLocation(programAddress, nameInShader.c_str());
			glUniformMatrix4fv(place, 1, GL_FALSE, value_ptr(data));
		}
	};
	namespace Transform {
		glm::mat4 Projection(glm::vec2 screenSize, float fov, float near, float far) {
			return glm::perspective(radians(fov), (float)screenSize[0] / (float)screenSize[1], near, far);
		}
		glm::mat4 Translate(glm::mat4 formerMatrix, glm::vec3 translateVec3) {
			return translate(formerMatrix, translateVec3);
		}
		glm::mat4 Rotate(glm::mat4 formerMatrix, glm::vec3 rotation) {
			glm::mat4 tr = glm::rotate(formerMatrix, radians(rotation[0]), glm::vec3(1, 0, 0));
			tr = glm::rotate(tr, radians(rotation[1]), glm::vec3(0, 1, 0));
			tr = glm::rotate(tr, radians(rotation[2]), glm::vec3(0, 0, 1));
			return tr;
		}
	}
	class GameObject {
	private:
		GLuint VBO, VAO;
	public:
		std::vector <float>vertices;
		long long size;
		short vertexLength;
		long long amountOfVertices;
		GameObject(std::vector<float> givenVertices, uint gamountOfVertices, uint gvertexLength) {
			size = gamountOfVertices*gvertexLength;
			amountOfVertices = gamountOfVertices;
			vertexLength = gvertexLength;

			
			vertices = givenVertices;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float),vertices.data(), GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexLength * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}
		void SetUsage(GLuint index, GLuint start, GLuint size) {
			glBindVertexArray(VAO);
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, vertexLength * sizeof(float), (void*)(start * sizeof(float)));
			glEnableVertexAttribArray(index);
		}
		void UpdateData() {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			
			size = vertices.size();
			glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), vertices.data());
		}
		void UpdateData(float* data, unsigned start, unsigned end) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, start, end - start, data);
		}
		void Render() {
			glBindVertexArray(VAO);
			glDrawArrays(4, 0, amountOfVertices);
		}
		void LRender() {
			glBindVertexArray(VAO);
			glDrawArrays(1, 0, amountOfVertices);
		}
		void Disable() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			amountOfVertices = 0;
			vertexLength = 0;
			size = 0;
			vertices.clear();
		}
	};
	class Instance {
	public:
		GLFWwindow* window;
		glm::ivec2 windowSize = { 800,600 };
		glm::vec2 mousePosition = { 0,0 };
		glm::vec2 oldMousePosition = { 0,0 };
		bool showOverlay = true;
		bool showCopyright = true;
		float deltaTime = 1;
		double timeLastFrame;
		double timeCurrentFrame;
		bool running = true;
		const GLFWvidmode* mvm;
		ImGuiIO io;
		bool fframeran=false;
		Instance(ivec2 gsize, std::string title, bool native, bool decorated) {
			glfwInit();
			mvm = glfwGetVideoMode(glfwGetPrimaryMonitor());
			windowSize = gsize;
			if (gsize.x == 0 || gsize.y == 0) {
				native = true;
				windowSize.x = mvm->width;
				windowSize.y = mvm->height;
			}
			if (!native) {
				window = glfwCreateWindow(windowSize.x, windowSize.y, "", NULL, NULL);
			}
			else window = glfwCreateWindow(windowSize.x, windowSize.y, "", glfwGetPrimaryMonitor(), NULL);
			glfwMakeContextCurrent(window);
			glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
			if (title != "")glfwSetWindowTitle(window, title.c_str());
			if (!decorated)glfwSetWindowAttrib(window, GLFW_DECORATED, false);
			glfwSetWindowPos(window, (mvm->width / 2) - (windowSize.x / 2), (mvm->height / 2) - (windowSize.y / 2));
			glfwSwapInterval(0.0);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			glViewport(0, 0, windowSize.x, windowSize.y);
			glClearColor(0.15, 0.15, 0.15, 1);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			io = ImGui::GetIO();
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init();
			if (std::filesystem::exists(std::filesystem::path(RESOURCES_PATH "PublicSans-Light.ttf"))) { ImGui::GetIO().Fonts->AddFontFromFileTTF(RESOURCES_PATH "PublicSans-Light.ttf"); };
		}
		void SetBackgroundColor(glm::vec3 color) {
			glClearColor(color[0], color[1], color[2], 1.0);
		}
		void SetWindowSize(glm::vec2 size) {
			glfwSetWindowSize(window, size[0], size[1]);
		}
		void SetWindowTitle(std::string title) {
			glfwSetWindowTitle(window, title.c_str());
		}
		void SetVSYNC(bool value) {
			glfwSwapInterval(value);
		}
		void SetMouseVisibility(bool value) {
			if (value) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		bool GetButton(int button) {
			return (glfwGetMouseButton(window, button) == GLFW_PRESS);
		}
		bool GetKey(int key) {
			return (glfwGetKey(window, key) == GLFW_PRESS);
		}
		void Start(void(*Update)()) {
			ImGuiWindowFlags wfl = ImGuiWindowFlags_NoBringToFrontOnFocus;
			timeLastFrame = glfwGetTime();
			while ((!glfwWindowShouldClose(window)) && running) {
				glfwPollEvents();
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				oldMousePosition = mousePosition;
				mousePosition = { (int)xpos,(int)ypos };
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Update();
				if (showOverlay) {
					if (!fframeran) {
						ImGui::SetNextWindowPos({ static_cast<float>(windowSize.x - 300), 0 });
						ImGui::SetNextWindowCollapsed(false);
					}
					ImGui::Begin("MCGL Overlay", NULL, wfl);
					ImGui::Text("MCGL 0.2a");
					if (showCopyright)ImGui::Text("(C) omardev269");
					ImGui::Text(std::format("FPS: {}", 1.0f / deltaTime).c_str());
					ImGui::Text(std::format("MOUSE POSITION: X{} Y{}", mousePosition[0], mousePosition[1]).c_str());
					ImGui::Text(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
					ImGui::End();
				}
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				glfwSwapBuffers(window);
				timeLastFrame = timeCurrentFrame;
				timeCurrentFrame = glfwGetTime();
				deltaTime = (timeCurrentFrame - timeLastFrame);
				if (!fframeran) fframeran = true;
			}
		}
		~Instance() {
			glfwTerminate();
		}
	};
}
#endif 
