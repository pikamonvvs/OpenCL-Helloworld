// Add you host code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>

#include <fstream>
#include <string>
#include <sstream>

#define HEIGHT 8
#define WIDTH  8

int main(void)
{
	// 0. ����� ������ ����
	int platformNum = 0;
	int deviceNum = 0;
	const char* sourceFile = "helloworld.cl";
	const char* kernelName = "arrayAdd";

	int arrayA[HEIGHT][WIDTH] = {
		{ 1, 1, 1, 1, 2, 2, 2, 2, },
		{ 1, 1, 1, 1, 2, 2, 2, 2, },
		{ 1, 1, 1, 1, 2, 2, 2, 2, },
		{ 1, 1, 1, 1, 2, 2, 2, 2, },
		{ 3, 3, 3, 3, 4, 4, 4, 4, },
		{ 3, 3, 3, 3, 4, 4, 4, 4, },
		{ 3, 3, 3, 3, 4, 4, 4, 4, },
		{ 3, 3, 3, 3, 4, 4, 4, 4, },
	};
	int arrayB[HEIGHT][WIDTH] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, },
	};
	int arrayC[HEIGHT][WIDTH] = { 0, };

	// �Է� ������ ���
	printf("Array A:\n");
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%d ", arrayA[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("Array B:\n");
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%d ", arrayB[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	// 1. platform ��������
	cl_uint platformCount;
	clGetPlatformIDs(0, NULL, &platformCount);
	cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * platformCount);
	clGetPlatformIDs(platformCount, platforms, NULL);

	// 2. device ��������
	cl_uint deviceCount;
	clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
	cl_device_id* devices = (cl_device_id*)malloc(sizeof(cl_device_id) * deviceCount);
	clGetDeviceIDs(platforms[platformNum], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
	cl_device_id device = devices[deviceNum];

	// 3. context �����ϱ�
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

	// 4. command queue �����ϱ�
	cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

	// 5. source ��������
	FILE* fp = fopen(sourceFile, "rb");
	cl_int status = fseek(fp, 0, SEEK_END);
	long int size = ftell(fp);
	rewind(fp);
	char* source = (char*)malloc(sizeof(char) * (size + 1));
	fread(source, sizeof(char), size, fp);
	source[size] = '\0';

	// 6. program �����ϱ�
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
	cl_int build_status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

	// 7. kernel �����ϱ�
	cl_kernel kernel = clCreateKernel(program, kernelName, NULL);

	// 8. memory buffer �����ϱ�
	cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_WRITE, HEIGHT * WIDTH * sizeof(int), NULL, NULL);
	cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_WRITE, HEIGHT * WIDTH * sizeof(int), NULL, NULL);
	cl_mem bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE, HEIGHT * WIDTH * sizeof(int), NULL, NULL);

	// 9. command queue�� memory buffer �����ϱ�
	clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, HEIGHT * WIDTH * sizeof(int), arrayA, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, HEIGHT * WIDTH * sizeof(int), arrayB, 0, NULL, NULL);

	// 10. kernel argument �����ϱ�
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

	// 11. command queue�� kernel �����ϱ�
	size_t globalSize[2] = { HEIGHT, WIDTH };
	clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);

	// 12. ���� �Ϸ�� ������ ����ϱ�
	clFinish(queue);

	// 13. ��� ���ۿ� ��� ��ȯ�ϱ�
	clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, HEIGHT * WIDTH * sizeof(int), arrayC, 0, NULL, NULL);

	// ��� ������ ���
	printf("Array C:\n");
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%d ", arrayC[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	system("pause");

	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	free(source);
	fclose(fp);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(device);
	free(devices);
	free(platforms);

	return 0;
}
