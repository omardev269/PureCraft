namespace OpenCraft {
	class World;
	class Chunk {
		siv::PerlinNoise* noise;
	public:
		BlockType blockSet[32][32][32] = { airID };
		ivec3 indent{ 0, 0, 0 };
		Chunk(siv::PerlinNoise* gnoise, ivec3 gindent) {
			noise = gnoise;
			indent = gindent;
			int rheight;
			for (int xptr = 0; xptr < 32; xptr++) {
				for (int zptr = 0; zptr < 32; zptr++) {
					rheight = 3+static_cast<unsigned>(noise->octave2D_01((xptr+indent.x)/35.0f, (zptr + indent.z) / 35.0f, 3) * (33 + static_cast<int>(noise->noise2D((indent.z+zptr)/75.0f,(indent.x+xptr)/75.0f)*32)));
					for (int yptr = 0; yptr < clamp<int>(rheight - indent.y, 0, 32); yptr++) {
						if (yptr + indent.y <= 1)blockSet[xptr][yptr][zptr] = bedrockID;
						else if (yptr + indent.y < rheight - 4)blockSet[xptr][yptr][zptr] = stoneID;
						else if (yptr + indent.y < rheight - 1)blockSet[xptr][yptr][zptr] = dirtID;
						else if (yptr + indent.y == rheight - 1)blockSet[xptr][yptr][zptr] = grassID;
					}
				}
			}
		}
	};
}