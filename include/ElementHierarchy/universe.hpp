namespace OpenCraft {
	class Universe {
	public:
		World* world;
		std::thread* t;
		bool hasRan=false;
		bool running = false;
		bool waiting = false;
		std::vector<ivec3> exceptionList;
		std::vector<ivec3> exceptionLista;
		std::vector<Chunk*> chunksThatWereBuilt;
		std::vector<Chunk*> tchunksThatWereBuilt;
		bool alive = false;
		bool skip = false;
		Universe(unsigned seed){
			world = new World(seed);
		}
		void InternalUpdate() {
			tchunksThatWereBuilt.clear();
			world->FlushAll();
			running = true;
			exceptionList.clear();
			exceptionList.push_back(ivec3(floor(-OpenCraft::location.x / 32) * 32, 0, floor(-OpenCraft::location.z / 32) * 32));
			for (int rp = 0; rp < (renderDistance - 1); rp++) {
				for (ivec3 item : exceptionList) {
					exceptionLista.push_back({ item.x + 32,item.y,item.z });
					exceptionLista.push_back({ item.x - 32,item.y,item.z });
					exceptionLista.push_back({ item.x, item.y,item.z + 32 });
					exceptionLista.push_back({ item.x, item.y,item.z - 32 });
					exceptionLista.push_back({ item.x + 32,item.y+32,item.z });
					exceptionLista.push_back({ item.x - 32,item.y+32,item.z });
					exceptionLista.push_back({ item.x, item.y+32,item.z + 32 });
					exceptionLista.push_back({ item.x, item.y+32,item.z - 32 });
				}
				exceptionList.insert(exceptionList.end(), exceptionLista.begin(), exceptionLista.end());
				exceptionLista.clear();
			}
			world->ClearWorld(exceptionList);
			for (ivec3 ind : exceptionList) {
				world->AddChunk(ind);
			}
			for (Chunk* chunk : world->chunks) {
				world->BuildChunk(chunk->indent);
				tchunksThatWereBuilt.push_back(chunk);
				for (Chunk* builtChunk : chunksThatWereBuilt) {
					if (builtChunk == chunk) {
						skip = true;
					}
				}
				if (!skip) {
					waiting = true;
					while (true) {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						if (waiting == false)break;
					}
				}
				skip = false;
			}
			toPrint.clear();
			toPrint.push_back("Total Chunks: " + std::to_string(world->chunks.size()));
			chunksThatWereBuilt = tchunksThatWereBuilt;
			running = false;
		}
		void UniverseUpdate() {
			if (!running) {
				if (hasRan) {
					t->join();
					delete t;
				}
				OpenCraft::mesh->vertices = world->internalPool;
				OpenCraft::mesh->amountOfVertices = world->amountOfVerticesTaken;
				OpenCraft::mesh->UpdateData();
				t = new std::thread(&Universe::InternalUpdate, this);
				hasRan = true;
			}
			if (waiting) {
				OpenCraft::mesh->vertices = world->internalPool;
				OpenCraft::mesh->amountOfVertices = world->amountOfVerticesTaken;
				OpenCraft::mesh->UpdateData();
				waiting = false;
			}
			OpenCraft::mesh->Render();
		}
		bool IsAir(ivec3 pos) {
			return world->IsAir(pos);
		}
		BlockType GetBlock(ivec3 pos) {
			return world->GetBlock(pos);
		}
		void SetBlock(ivec3 pos, BlockType to) {
			world->SetBlock(pos, to);
		}
		~Universe() {
			t->join();
			delete t;
			delete world;

		}
	};
}