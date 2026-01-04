namespace MVE {
	class Chunk {
	public:
		ivec3 indent;
		BlockType blockSet[16][16][16] = { airID };
		std::atomic<int> amountOfVertices;
		GameObject* mesh;
		std::atomic<bool> signalToUpdateData;
		std::atomic<bool> dirty;
		std::atomic<double> timeTakenToBuild;
		Chunk(ivec3 gindent) {
			indent = gindent;
			signalToUpdateData.store(false);
			dirty.store(true);
			mesh = new GameObject({}, 36 * 16 * 16 * 16, 5);
			mesh->SetUsage(1, 3, 2);
			amountOfVertices.store(0);
			mesh->amountOfVertices = 0;
			timeTakenToBuild.store(0);
			for (u8 x = 0; x < 16; x++) {
				for (u8 z = 0; z < 16; z++) {
					for (u8 y = 0; y < 8+(noise->GetNoiseSet(x+indent.x, z+indent.z, 0, 1, 1, 1)[0]*8); y++) {
						blockSet[x][y][z] = grassID;
					}
				}
			}
		}
		void AddBlockFace(std::vector<float> face, vec3 flocation) {
			std::vector<float> workVector = face;
			flocation+=indent;
			for (u8 i = 0; i < 6; i++) {
				workVector[i * 5] += flocation.x;
				workVector[(i * 5) + 1] += flocation.y;
				workVector[(i * 5) + 2] += flocation.z;
			}
			mesh->vertices.insert(mesh->vertices.end(), workVector.begin(), workVector.end());
			amountOfVertices.fetch_add(6);
		}
		bool IsAir(ivec3 blocation){
			if (blocation.x >= 0 && blocation.x < 16 && blocation.y >= 0 && blocation.y < 16 && blocation.z >= 0 && blocation.z < 16) return blockSet[blocation.x][blocation.y][blocation.z] == airID;
			else return true;
		}
		void ChunkBuild() {
			double t1 = glfwGetTime();
			amountOfVertices.store(0);
			for (u8 x = 0; x < 16; x++) {
				for (u8 y = 0; y < 16; y++) {
					for (u8 z = 0; z < 16; z++) {
						if (!IsAir({ x, y, z })) {
							if (IsAir({ x - 1, y, z })) AddBlockFace(grassLeft, { x, y, z });
							if (IsAir({ x + 1, y, z })) AddBlockFace(grassRight, { x, y, z });
							if (IsAir({ x, y - 1, z })) AddBlockFace(grassBottom, { x, y, z });
							if (IsAir({ x, y + 1, z })) AddBlockFace(grassTop, { x, y, z });
							if (IsAir({ x, y, z - 1 })) AddBlockFace(grassBack, { x, y, z });
							if (IsAir({ x, y, z + 1 })) AddBlockFace(grassFront, { x, y, z });
						}
					}
				}
			}
			timeTakenToBuild.store(glfwGetTime() - t1);
			dirty.store(false);
			signalToUpdateData.store(true);
		}
		void ChunkUpdate() {
			if (signalToUpdateData.load()) {
				mesh->amountOfVertices = amountOfVertices.load();
				mesh->UpdateData();
				signalToUpdateData.store(false);
			}
			mesh->Render();
		}
		~Chunk() {
			delete mesh;
		}
	};
}