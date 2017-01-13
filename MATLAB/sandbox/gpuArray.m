g = gpuDevice(1);
M = gpuArray(magic(4));
M_exists = existsOnGPU(M);