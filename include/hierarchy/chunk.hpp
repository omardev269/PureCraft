#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>
namespace CommunityCraft {
	class Chunk {
	public:
		bool dirty = true;
		BlockType blocks[32][32][32] = { airID };
		ivec3 indent;
		int amountOfVertices = 0;
		std::vector<float> internalBuffer;
		Chunk(ivec3 gindent) {
			indent = gindent;
			int height, rheight;
			for (int x = 0; x < 32; x++) {
				for (int z = 0; z < 32; z++) {
					rheight = 32+static_cast<int>((stb_perlin_noise3((x+indent.x)/50.0f, (z+indent.z)/50.0f, 0, 0, 0, 0))*32);
					height = clamp<int>((rheight - indent.y), 0, 32);
					for (int y = 0; y < height; y++) {
						if (y+indent.y != 0) {
							if (y+indent.y < 3)blocks[x][y][z] = bedrockID;
							else if ((y) == (rheight - 1 - indent.y)) {
								blocks[x][y][z] = grassID;
							}
							else {
								blocks[x][y][z] = stoneID;
							}
						}
						else {
							blocks[x][y][z] = bedrockID;
						}
					}
				}
			}
		}
		bool IsAir(ivec3 position) {
			if (indent.x <= position.x && indent.x + 32 > position.x && indent.y <= position.y && indent.y + 32 > position.y && indent.z <= position.z && indent.z + 32 > position.z)return blocks[position.x - indent.x][position.y - indent.y][position.z - indent.z] == airID;
			return true;
		}
		/*void InternalAddBlock(ivec3 blockLocation, BlockType btype) {
			std::vector <float> newVertices;
			std::vector <float> snewVertices;
			newVertices.resize(36 * 5);
			if (btype == grassID)snewVertices = grass;
			else if (btype == stoneID)snewVertices = stone;
			else if (btype == dirtID)snewVertices = dirt;
			else if (btype == bedrockID)snewVertices = bedrock;
			else if (btype == amethystID)snewVertices = amethyst;
			else return;
			for (GLuint i = 0; i < 36; i++) {
				newVertices[(i * 5)] = snewVertices[(i * 5)] + indent.x + blockLocation.x + 0.5;
				newVertices[(i * 5) + 1] = snewVertices[(i * 5) + 1] + indent.y + blockLocation.y + 0.5;
				newVertices[(i * 5) + 2] = snewVertices[(i * 5) + 2] + indent.z + blockLocation.z + 0.5;
				newVertices[(i * 5) + 3] = snewVertices[(i * 5) + 3];
				newVertices[(i * 5) + 4] = snewVertices[(i * 5) + 4];
				//newVertices[(i * 5) + 5] = 1.0f;
			}
			internalBuffer.insert(internalBuffer.end(), newVertices.begin(), newVertices.end());
			amountOfVertices += 36;
		}*/
		void InternalAddFace(vec3 blocation, std::vector<float> face) {
			blocation += indent;
			blocation += 0.5f;
			for (GLuint i = 0; i < 6; i++) {
				face[i * 5] += blocation.x;
				face[(i * 5) + 1] += blocation.y;
				face[(i * 5) + 2] += blocation.z;
			}
			internalBuffer.insert(internalBuffer.end(), face.begin(), face.end());
			amountOfVertices += 6;
		}
		void Build() {
			amountOfVertices = 0;
			internalBuffer.clear();
			ivec3 addedPos;
			for (int x = 0; x < 32; x++) {
				for (int z = 0; z < 32; z++) {
					for (int y = 0; y < 32; y++) {
						if (blocks[x][y][z] == airID) continue;
						addedPos = indent + ivec3(x, y, z);
						switch (blocks[x][y][z]) {
						case grassID:
							if (IsAir({ addedPos.x - 1 , addedPos.y, addedPos.z }))InternalAddFace(vec3(x, y, z), grassLeft);
							if (IsAir({ addedPos.x + 1 , addedPos.y, addedPos.z }))InternalAddFace(vec3(x, y, z), grassRight);
							if (IsAir({ addedPos.x , addedPos.y - 1, addedPos.z }))InternalAddFace(vec3(x, y, z), grassBottom);
							if (IsAir({ addedPos.x , addedPos.y + 1, addedPos.z }))InternalAddFace(vec3(x, y, z), grassTop);
							if (IsAir({ addedPos.x , addedPos.y, addedPos.z - 1 }))InternalAddFace(vec3(x, y, z), grassBack);
							if (IsAir({ addedPos.x , addedPos.y, addedPos.z + 1 }))InternalAddFace(vec3(x, y, z), grassFront);
						break;
						case stoneID:
							if (IsAir({ addedPos.x - 1 , addedPos.y, addedPos.z }))InternalAddFace(vec3(x, y, z), stoneLeft);
							if (IsAir({ addedPos.x + 1 , addedPos.y, addedPos.z }))InternalAddFace(vec3(x, y, z), stoneRight);
							if (IsAir({ addedPos.x , addedPos.y - 1, addedPos.z }))InternalAddFace(vec3(x, y, z), stoneBottom);
							if (IsAir({ addedPos.x , addedPos.y + 1, addedPos.z }))InternalAddFace(vec3(x, y, z), stoneTop);
							if (IsAir({ addedPos.x , addedPos.y, addedPos.z - 1 }))InternalAddFace(vec3(x, y, z), stoneBack);
							if (IsAir({ addedPos.x , addedPos.y, addedPos.z + 1 }))InternalAddFace(vec3(x, y, z), stoneFront);
						break;
						case bedrockID:
							if (IsAir({ addedPos.x - 1 , addedPos.y, addedPos.z }))InternalAddFace(vec3(x, y, z), bedrockLeft);
							if (IsAir({ addedPos.x + 1 , addedPos.y, addedPos.z }))InternalAddFace(vec3(x, y, z), bedrockRight);
							if (IsAir({ addedPos.x , addedPos.y - 1, addedPos.z }))InternalAddFace(vec3(x, y, z), bedrockBottom);
							if (IsAir({ addedPos.x , addedPos.y + 1, addedPos.z }))InternalAddFace(vec3(x, y, z), bedrockTop);
							if (IsAir({ addedPos.x , addedPos.y, addedPos.z - 1 }))InternalAddFace(vec3(x, y, z), bedrockBack);
							if (IsAir({ addedPos.x , addedPos.y, addedPos.z + 1 }))InternalAddFace(vec3(x, y, z), bedrockFront);
						break;
						}
					}
				}
			}
		}
		~Chunk() {

		}
	};
}