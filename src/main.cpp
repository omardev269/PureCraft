#include <MCGL.hpp>
#include <FastNoiseSIMD.h>
#include <atomic>
typedef unsigned char BlockType;
#include <block.hpp>
using namespace MCGL;
namespace MVE {
	FastNoiseSIMD* noise;
}
#include <chunk.hpp>
namespace MVE {
	Chunk* chunk;
	Instance* instance;
	GPU* gpu;
	vec3 rotation(0, 0, 0);
	vec3 location(0, 0, 0);
	vec3 place(0, 0, 0);
	float speed = 1.0f;
}
#include <update.hpp>
int main(){
	MVE::instance = new Instance({ 800, 600 }, "MVE", false, true);
	MVE::instance->SetMouseVisibility(false);
	MVE::instance->SetBackgroundColor({ 0.5, 0.7, 1.0 });
	MVE::gpu = new GPU;
	u8 green[4] = { 0, 255, 0, 255 };
	MVE::gpu->RegisterTexture2D(0, RESOURCES_PATH "blocks.bmp");
	MVE::noise = FastNoiseSIMD::NewFastNoiseSIMD();
	MVE::noise->SetNoiseType(FastNoiseSIMD::PerlinFractal);
	MVE::chunk = new MVE::Chunk({ 0,0,0 });
	MVE::chunk->ChunkBuild();
	MVE::instance->Start(MVE::Update);
	delete MVE::gpu;
	delete MVE::instance;
	return 0;
}
