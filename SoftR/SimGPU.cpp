#include "SimGPU.h"

SrSimGPU* g_gpu = nullptr;// new SrSimGPU();

void func()
{
	while(true)
	g_logger->debug_print("sssssssssss\n");
}
//const int thread_num = 2;
std::mutex SrSimGPU::mu;
std::condition_variable SrSimGPU::g_pass_done;