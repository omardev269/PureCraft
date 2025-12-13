namespace OpenCraft {
	class World {
		std::vector<Chunk*> chunks;
	public:
		std::vector<float> vertices;
		unsigned amountOfVertices = 0;
		unsigned seed;
		World(unsigned gseed) {
			seed = gseed;
			chunks.push_back(new Chunk(seed, ivec3(0, 0, 0)));
			BuildMesh();
		}
	private:
		void AddBlock(BlockType btype, ivec3 blockLocation) {
			std::vector <float> newVertices;
			if (btype == grassID)newVertices = grass;
			else if (btype == stoneID)newVertices = stone;
			else if (btype == dirtID)newVertices = dirt;
			else if (btype == bedrockID)newVertices = bedrock;
			else if (btype == amethystID)newVertices = amethyst;
			else return;
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5)] += blockLocation.x + 0.5;
			}
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5) + 1] += blockLocation.y + 0.5;
			}
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5) + 2] += blockLocation.z + 0.5;
			}

			vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
			amountOfVertices += 36;
		}
	public:
		void BuildMesh() {
			amountOfVertices = 0;
			vertices.clear();
			ivec3 addedPos;
			for (Chunk* chunk : chunks) {
				chunk->Build();
				for (Block& block : chunk->blocks) {
					if (block.id != airID) {
						addedPos = block.position + chunk->indent;
						if (IsAir({ addedPos.x, addedPos.y + 1, addedPos.z }) || IsAir({ addedPos.x, addedPos.y - 1, addedPos.z })||IsAir({ addedPos.x-1, addedPos.y, addedPos.z })|| IsAir({ addedPos.x+1, addedPos.y, addedPos.z })|| IsAir({ addedPos.x, addedPos.y, addedPos.z - 1})|| IsAir({ addedPos.x, addedPos.y, addedPos.z+1 }))AddBlock(block.id, block.position + chunk->indent);
					}
				}
			}
		}
		bool ClearWorld(std::vector<ivec3> exceptions) {
			bool dontDelete = false;
			bool trivial = true;
			for (Chunk* chunk : chunks) {
				if (chunk->indent == ivec3(0, 0, 0))continue;
				else {
					for (ivec3 exc : exceptions) {
						if (chunk->indent == exc) dontDelete=true;
					}
				}
				if (!dontDelete) {
					DeleteChunk(chunk->indent);
					trivial = false;
				}
				dontDelete = false;
			}
			return trivial;
		}
		void AddChunk(ivec3 indent) {
			for (Chunk* chunk : chunks) {
				if (chunk->indent == indent) return;
			}
			chunks.push_back(new Chunk(seed, indent));
		}
		void DeleteChunk(ivec3 indent) {
			for (Chunk* chunk : chunks) {
				{
					if (chunk->indent != indent) continue;
					delete chunk;
					chunks.erase(std::remove(chunks.begin(), chunks.end(), chunk), chunks.end());
				}
			}
		}
		BlockType GetBlock(ivec3 position) {
			for (Chunk* chunk : chunks) {
				if (position.x >= chunk->indent.x && position.x < chunk->indent.x + 16 &&
					position.y >= chunk->indent.y && position.y < chunk->indent.y + 16 &&
					position.z >= chunk->indent.z && position.z < chunk->indent.z + 16) {
					ivec3 localPos = position - chunk->indent;
					return chunk->blockSet[localPos.x][localPos.y][localPos.z];
				}
			}
			return airID;
		}
		bool IsAir(ivec3 position) {
			return GetBlock(position) == airID;
		}
		void SetBlock(ivec3 position, BlockType btype) {
			for (Chunk* chunk : chunks) {
				if (position.x >= chunk->indent.x && position.x < chunk->indent.x + 16 &&
					position.y >= chunk->indent.y && position.y < chunk->indent.y + 16 &&
					position.z >= chunk->indent.z && position.z < chunk->indent.z + 16) {
					ivec3 localPos = position - chunk->indent;
					chunk->blockSet[localPos.x][localPos.y][localPos.z] = btype;
				}
			}
		}
		~World() {
			for (Chunk* chunk : chunks) {
				delete chunk;
			}
		}
	};
}