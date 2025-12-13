namespace OpenCraft {
	bool running = false;
	bool hasRan = false;
	std::thread* t;
	bool htrivial=false;
	void InternalGenUpdate() {
		running = true;
		exceptionList.clear();
		exceptionLista.clear();
		exceptionList.push_back(ivec3(floor(-OpenCraft::location.x / 16) * 16, 0, floor(-OpenCraft::location.z / 16) * 16));
		for (int rp = 0; rp < (renderDistance - 1); rp++) {
			for (ivec3 item : exceptionList) {
				exceptionLista.push_back({ item.x + 16,item.y,item.z });
				exceptionLista.push_back({ item.x - 16,item.y,item.z });
				exceptionLista.push_back({ item.x, item.y,item.z + 16 });
				exceptionLista.push_back({ item.x, item.y,item.z - 16 });
			}
			exceptionList.insert(exceptionList.end(), exceptionLista.begin(), exceptionLista.end());
			exceptionLista.clear();
		}
		for (ivec3 ind : exceptionList) {
			OpenCraft::world->AddChunk(ind);
		}
		htrivial = OpenCraft::world->ClearWorld(exceptionList);
		world->BuildMesh();
		running = false;
	}
	void GenUpdate() {
		if (!running) {
			if (hasRan) {
				t->join();
				delete t;
			}
			OpenCraft::mesh->vertices = OpenCraft::world->vertices;
			OpenCraft::mesh->amountOfVertices = OpenCraft::world->amountOfVertices;
			OpenCraft::mesh->UpdateData();
			t = new std::thread(InternalGenUpdate);
			hasRan = true;
		}
	}
	void OnEnd() {
		if (hasRan) {
			t->join();
			delete t;
		}
	}
}