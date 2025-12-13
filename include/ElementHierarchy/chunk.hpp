namespace OpenCraft {
	class Chunk {
	public:
		BlockType blockSet[16][16][16] = { airID };
		std::vector<Block> blocks;
		ivec3 indent;
		siv::PerlinNoise* noise;
		Chunk(unsigned seed, ivec3 gindent) {
			noise = new siv::PerlinNoise(seed);
			indent = gindent;
			int height;
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					if (indent.y > 3) height = (clamp<int>(static_cast<int>(noise->octave2D_01((indent.x + x)/22.5f, (indent.z + z)/22.5f, 3)*14), indent.y, indent.y+14))-indent.y;
					else height = (clamp<int>(static_cast<int>(noise->octave2D_01((indent.x + x) / 22.5f, (indent.z + z) / 22.5f, 3) * 14), indent.y+3, indent.y + 14))-indent.y;
					for (int y = 0; y < height; y++) {
						if (y == height - 1) blockSet[x][y][z] = grassID;
						else if (y < 2) blockSet[x][y][z] = bedrockID;
						else if (y > height - (noise->noise3D_01((indent.x + x) / 5.0f, (indent.y + y) / 5.0f, (indent.z + z) / 5.0f)*3)) blockSet[x][y][z] = dirtID;
						else blockSet[x][y][z] = stoneID;
					}
				}
			}
			Build();
		}
		void Build(){
			blocks.clear();
			Block tempBlock;
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					for (int y = 0; y < 16; y++) {
						if (blockSet[x][y][z] == airID) continue;
						tempBlock.id = blockSet[x][y][z];
						tempBlock.position = ivec3(x, y, z);
						blocks.push_back(tempBlock);
					}
				}
			}
		}
		~Chunk() {
			delete noise;
		}
	};
}