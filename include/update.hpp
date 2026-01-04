namespace MVE {
	bool wasEscapePressedLastFrame = false;
	bool inGame = true;
	mat4 transform(1.0f);
	bool fframeranForZis = false;
	bool btnClickedState;
	void Update() {
		if ((wasEscapePressedLastFrame && !instance->GetKey(GLFW_KEY_ESCAPE))) {
			if (inGame) {
				inGame = false;
				instance->showOverlay = false;
			}
			else {
				inGame = true;
				fframeranForZis = false;
				instance->showOverlay = true;
			}
		}
		if (!inGame) {
			instance->SetMouseVisibility(true);
			if (!fframeranForZis) {
				ImGui::SetNextWindowSize({ static_cast<float>(instance->windowSize.x), static_cast<float>(instance->windowSize.y) });
				ImGui::SetNextWindowPos({ 0, 0 });
				ImGui::SetNextWindowCollapsed(false);
				fframeranForZis = true;
			}
			ImGui::Begin("Game paused", 0, ImGuiWindowFlags_NoResize);
			if (!btnClickedState) {
				ImGui::Text("MVE build 1");
				btnClickedState = ImGui::Button("Click this");
			}
			if (btnClickedState) ImGui::Text("You clicked it! Why!");
			ImGui::End();
		}
		else instance->SetMouseVisibility(false);
		if (inGame) {
			if (instance->GetKey(GLFW_KEY_W)) {
				location.z += speed * (10.0f * instance->deltaTime) * cos(radians(rotation.y));
				location.x -= speed * (10.0f * instance->deltaTime) * sin(radians(rotation.y));
			}
			if (instance->GetKey(GLFW_KEY_S)) {
				location.z += -speed * (10.0f * instance->deltaTime) * cos(radians(rotation.y));
				location.x -= -speed * (10.0f * instance->deltaTime) * sin(radians(rotation.y));
			}
			if (instance->GetKey(GLFW_KEY_SPACE)) location.y -= speed * (10.0f * instance->deltaTime);
			if (instance->GetKey(GLFW_KEY_LEFT_SHIFT)) location.y -= -speed * (10.0f * instance->deltaTime);
			if (instance->fframeran) {
				rotation.x += instance->mousePosition.y - instance->oldMousePosition.y;
				rotation.y += instance->mousePosition.x - instance->oldMousePosition.x;
			}
			transform = Transform::Projection(instance->windowSize, 90.0f, 0.05f, 250.0f);
			transform = Transform::Rotate(transform, rotation);
			transform = Transform::Translate(transform, location);
		}
		gpu->SetTransform(transform, "transform");
		MVE::gpu->UseTexture2D(0, "objectTexture");
		glEnable(GL_DEPTH_TEST);
		MVE::chunk->ChunkUpdate();
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize({ instance->windowSize.x / 4.0f, (float)instance->windowSize.y });
		ImGui::Begin("Debug");
		ImGui::Text("Size of chunk data: %dB", chunk->mesh->vertices.size() * 4);
		ImGui::Text("Time taken to build the chunk: %f seconds", chunk->timeTakenToBuild.load());
		ImGui::End();
		wasEscapePressedLastFrame = instance->GetKey(GLFW_KEY_ESCAPE);
	}
}