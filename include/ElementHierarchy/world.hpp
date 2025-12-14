namespace OpenCraft {

	class World {
	public:
		std::vector<Chunk*> chunks;
		unsigned seed;
		siv::PerlinNoise* noise;
		std::vector<float> internalPool;
		unsigned amountOfVerticesTaken = 0;
		World(unsigned gseed) {
			seed = gseed;
			noise = new siv::PerlinNoise(seed);
			chunks.push_back(new Chunk(noise, ivec3(0, 0, 0)));
			chunks.push_back(new Chunk(noise, ivec3(0, 32, 0)));
		}
		bool IsAir(ivec3 pos) {
			for (Chunk* chunk : chunks) {
				if (pos.x - chunk->indent.x >= 0 && pos.x - chunk->indent.x < 32 && pos.y - chunk->indent.y >= 0 && pos.y - chunk->indent.y < 32 && pos.z - chunk->indent.z >= 0 && pos.z - chunk->indent.z < 32)return chunk->blockSet[pos.x-chunk->indent.x][pos.y-chunk->indent.y][pos.z-chunk->indent.z]==airID;
			}
			return true;
		}
		void SetBlock(ivec3 pos, BlockType to) {
			for (Chunk* chunk in chunks) {
				if (chunk->indent.x <= pos.x && chunk->indent.x + 32 > pos.x && chunk->indent.z <= pos.z && chunk->indent.z + 32 > pos.z && chunk->indent.y <= pos.y && chunk->indent.y + 32 > pos.y) {
					chunk->blockSet[pos.x - chunk->indent.x][pos.y - chunk->indent.y][pos.z - chunk->indent.z] = to;
					return;
				}
			}
		}
		BlockType GetBlock(ivec3 pos) {
			for (Chunk* chunk in chunks) {
				if (chunk->indent.x <= pos.x && chunk->indent.x + 32 > pos.x && chunk->indent.z <= pos.z && chunk->indent.z + 32 > pos.z && chunk->indent.y <= pos.y && chunk->indent.y + 32 > pos.y) {
					return chunk->blockSet[pos.x - chunk->indent.x][pos.y - chunk->indent.y][pos.z - chunk->indent.z];
				}
			}
		}
		void AddChunk(ivec3 chunkIndent) {
			for (Chunk* chunk in chunks) {
				if (chunk->indent == chunkIndent)return;
			}
			chunks.push_back(new Chunk(noise, chunkIndent));
		}
		void DeleteChunk(ivec3 chunkIndent) {
			std::vector<Chunk*> toDelete;
			for (Chunk* chunk : chunks) {
				if (chunk->indent == chunkIndent) {
					toDelete.push_back(chunk);
				}
			}
			for (Chunk* chunk : toDelete) {
				chunks.erase(std::remove(chunks.begin(), chunks.end(), chunk), chunks.end());
				delete chunk;
			}
		}
		void ClearWorld(std::vector<ivec3> exceptions) {
			bool del = true;
			for (Chunk* chunk in chunks) {
				if (chunk->indent == ivec3(0, 0, 0)||chunk->indent==ivec3(0, 32, 0))del=false;
				for (ivec3 exception in exceptions) {
					if (chunk->indent == exception)del = false;
				}
				if (del) {
					DeleteChunk(chunk->indent);
				}
				del = true;
			}
		}
		void AddBlock(BlockType btype, ivec3 blockLocation) {
			std::vector <float> newVertices;
			std::vector <float> snewVertices;
			newVertices.resize(36 * 6);
			if (btype == grassID)snewVertices = grass;
			else if (btype == stoneID)snewVertices = stone;
			else if (btype == dirtID)snewVertices = dirt;
			else if (btype == bedrockID)snewVertices = bedrock;
			else if (btype == amethystID)snewVertices = amethyst;
			else return;
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 6)] = snewVertices[(i * 5) ] + blockLocation.x + 0.5;
				newVertices[(i * 6) + 1] = snewVertices[(i * 5) + 1] + blockLocation.y + 0.5;
				newVertices[(i * 6) + 2] = snewVertices[(i * 5) + 2]+blockLocation.z + 0.5;
				newVertices[(i * 6) + 3] = snewVertices[(i * 5) + 3];
				newVertices[(i * 6) + 4] = snewVertices[(i * 5) + 4];
				newVertices[(i * 6) + 5] = 1.0f;
			}
			internalPool.insert(internalPool.end(), newVertices.begin(), newVertices.end());
			amountOfVerticesTaken += 36;
		}
		void FlushAll() {
			internalPool.clear();
			amountOfVerticesTaken = 0;
		}
		void BuildChunk(ivec3 chunkIndent) {
			ivec3 addedPos;
			for (Chunk* chunk : chunks) {
				if (chunk->indent == chunkIndent) {
					for (int xptr = 0; xptr < 32; xptr++) {
						for (int yptr = 0; yptr < 32; yptr++) {
							for (int zptr = 0; zptr < 32; zptr++) {
								addedPos = ivec3(xptr, yptr, zptr) + chunkIndent;
								if (IsAir({ addedPos.x, addedPos.y + 1, addedPos.z }) || IsAir({ addedPos.x, addedPos.y - 1, addedPos.z }) || IsAir({ addedPos.x - 1, addedPos.y, addedPos.z }) || IsAir({ addedPos.x + 1, addedPos.y, addedPos.z }) || IsAir({ addedPos.x, addedPos.y, addedPos.z - 1 }) || IsAir({ addedPos.x, addedPos.y, addedPos.z + 1 }))AddBlock(chunk->blockSet[xptr][yptr][zptr], addedPos);
							}
						}
					}
				}
			}
		}
		~World() {
			for (Chunk* chunk : chunks) {
				delete chunk;
			}
			delete noise;
		}
	};
}