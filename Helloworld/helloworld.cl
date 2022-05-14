// TODO: Add OpenCL kernel code here.

__kernel void arrayAdd(__global int* pA, __global int* pB, __global int* pC)
{
    const int x     = get_global_id(0);
    const int y     = get_global_id(1);
    const int width = get_global_size(0);

    const int id = y * width + x;

    pC[id] = pA[id] + pB[id];
}
