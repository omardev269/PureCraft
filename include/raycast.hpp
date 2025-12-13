namespace OpenCraft {
	glm::ivec3 lcurrentPos;
	bool ppress = false;
	bool ppressr = false;
	float rotate0, rotate1;
	bool allowed;
	float srx, sry, srz;
	glm::ivec3 currentPos;
	glm::vec3 scurrentPos;
	vec3 vdir;
	void RaycastUpdate() {
		rotate0 = glm::radians(rotation.x);
		rotate1 = glm::radians(rotation.y);
		allowed = false;
		srx = cos(rotate0) * sin(rotate1);
		sry = sin(rotate0);
		srz = (cos(rotate0) * cos(rotate1));
		vdir = vec3(srx, -sry, -srz);
		for (GLuint aadvanced = 1; aadvanced <= 5; aadvanced++) {
			scurrentPos = glm::normalize(vdir);
			scurrentPos.x *= aadvanced;
			scurrentPos.y *= aadvanced;
			scurrentPos.z *= aadvanced;
			scurrentPos.x -= location[0];
			scurrentPos.y -= location[1];
			scurrentPos.z -= location[2];

			currentPos = glm::floor(scurrentPos);
			if (!world->IsAir(currentPos)) {
				allowed = true;
				break;
			}
		}/*
		if (allowed) {
			Textures::Use2D(8, "objectTexture", CommunityCraft::programAddress);
			std::vector <float> newVertices = stone;
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5)] += currentPos.x + 0.5;
			}
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5) + 1] += currentPos.y + 0.5;
			}
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5) + 2] += currentPos.z + 0.5;
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Rendering::SetData(&BlockPointer, newVertices.data(), 0, 36 * 5 * sizeof(float));
			Rendering::Render(&BlockPointer, 0, 36);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}*/
		if (instance->GetButton(GLFW_MOUSE_BUTTON_LEFT) && allowed && !ppress) {
			world->SetBlock(currentPos, airID);
		}
		if (instance->GetButton(GLFW_MOUSE_BUTTON_RIGHT) && allowed && !ppressr) {
			if (coordinates.y > currentPos.y) {
				if (world->IsAir({ currentPos.x, currentPos.y + 1, currentPos.z })) {
					world->SetBlock({ currentPos.x, currentPos.y + 1, currentPos.z }, stoneID);
				}
			}
			else if (coordinates.y < currentPos.y) {
				if (world->IsAir({ currentPos.x, currentPos.y - 1, currentPos.z })) {
					world->SetBlock({ currentPos.x, currentPos.y - 1, currentPos.z }, stoneID);
				}
			}
		}
		ppress = instance->GetButton(GLFW_MOUSE_BUTTON_LEFT);
		ppressr = instance->GetButton(GLFW_MOUSE_BUTTON_RIGHT);

	}

}