namespace CommunityCraft {
	std::vector<Chunk*> chunks;
	std::atomic<bool> running = false;
	bool hasRan = false;
	std::thread* t;
	bool haveTo = false;
	std::vector<ivec3> exceptionList;
	std::vector<ivec3> exceptionListt;
	std::vector<Chunk*> toDelete;
	const int renderDistance = 6;
	std::vector<ivec3> chunksInIvec3;
	void Server() {
		running.store(true);
		exceptionList.clear();
		mesh->vertices.clear();
		tamountOfVertices.store(0);
		toDelete.clear();
		exceptionList.push_back(ivec3(floor(-location.x / 32) * 32, 0, floor(-location.z / 32) * 32));
		exceptionList.push_back(ivec3(floor(-location.x / 32) * 32, 32, floor(-location.z / 32) * 32));
		for (int rp = 0; rp < (renderDistance - 1); rp++) {
			for (ivec3 item : exceptionList) {
				exceptionListt.push_back({ item.x + 32,0,item.z });
				exceptionListt.push_back({ item.x - 32,0,item.z });
				exceptionListt.push_back({ item.x, 0,item.z + 32 });
				exceptionListt.push_back({ item.x, 0,item.z - 32 });
				exceptionListt.push_back({ item.x + 32, 32, item.z });
				exceptionListt.push_back({ item.x - 32, 32, item.z });
				exceptionListt.push_back({ item.x, 32, item.z + 32 });
				exceptionListt.push_back({ item.x, 32, item.z - 32 });
			}
			exceptionList.insert(exceptionList.end(), exceptionListt.begin(), exceptionListt.end());
			exceptionListt.clear();
		}
		bool found;
		for (Chunk* chunk : chunks) {
			found = false;
			for (ivec3 ind : exceptionList) {
				if (chunk->indent == ind) {
					found = true;
					break;
				}
			}
			if (!found) {
				toDelete.push_back(chunk);
			}
		}
		for (Chunk* chunk : toDelete) {
			delete chunk;
			chunks.erase(std::remove(chunks.begin(), chunks.end(), chunk), chunks.end());
		}
		for (ivec3 ind : exceptionList) {
			found = false;
			for (Chunk* chunk : chunks) {
				if (chunk->indent == ind) {
					found = true;
					break;
				}
			}
			if (found)continue;
			chunks.push_back(new Chunk(ind));

		}
		for (Chunk* chunk : chunks) {
			if (chunk->dirty) {
				haveTo = true;
				chunk->Build();
				chunk->dirty = false;
			}
			mesh->vertices.insert(mesh->vertices.end(), chunk->internalBuffer.begin(), chunk->internalBuffer.end());
			tamountOfVertices.fetch_add(chunk->amountOfVertices);
		}
		//printf("%d\n", chunks.size());
		running.store(false);
	}
	bool IsAirW(ivec3 position) {
		for (Chunk* chunk : chunks) {
			if (chunk->indent.x <= position.x && chunk->indent.x + 32 > position.x && chunk->indent.y <= position.y && chunk->indent.y + 32 > position.y && chunk->indent.z <= position.z && chunk->indent.z + 32 > position.z)return chunk->blocks[position.x - chunk->indent.x][position.y - chunk->indent.y][position.z - chunk->indent.z] == airID;
		}
		return true;
	}
	void SetBlock(ivec3 position, BlockType btype) {
		amountOfBlockActions++;
		for (Chunk* chunk : chunks) {
			if (chunk->indent.x <= position.x && chunk->indent.x + 32 > position.x && chunk->indent.y <= position.y && chunk->indent.y + 32 > position.y && chunk->indent.z <= position.z && chunk->indent.z + 32 > position.z) {
				chunk->blocks[position.x - chunk->indent.x][position.y - chunk->indent.y][position.z - chunk->indent.z] = btype;
				chunk->dirty = true;
			}
		}
	}
	void WorldInit() {
		chunks.push_back(new Chunk(ivec3(0, 0, 0)));
		chunks.push_back(new Chunk(ivec3(0, 32, 0)));
	}
	void WorldUpdate() {
		if (!running.load()) {
			if (hasRan) {
				t->join();
				delete t;
				if (haveTo) {
					mesh->amountOfVertices = tamountOfVertices.load();
					mesh->UpdateData();
					haveTo = false;
				}
			}
			chunksInIvec3.clear();
			for (Chunk* chunk : chunks) chunksInIvec3.push_back(chunk->indent);
			t = new std::thread(Server);
		}
		mesh->Render();
		hasRan = true;
	}
	void WorldEnd() {
		if (hasRan) {
			t->join();
		}
		for (Chunk* chunk : chunks) {
			delete chunk;
		}
		chunks.clear();
		delete t;
	}
}