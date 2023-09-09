#include <CL/opencl.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty()) {
        std::cerr << "No platforms!" << std::endl;
        return -1;
    }


    if (!platforms.empty()) {
        std::cout << "Available Devices :" << std::endl;
        int idx = 1;
        for (const auto &plat : platforms) {
            std::vector<cl::Device> gpus;
            plat.getDevices(CL_DEVICE_TYPE_GPU, &gpus);
            if (!gpus.empty()) {
                cl::Device gpu = gpus[0];
                std::cout << idx++ << ". " << gpu.getInfo<CL_DEVICE_NAME>() << std::endl;
            }
        }
        std::cout << std::endl;
    }

    cl::Platform            platform = platforms[0];
    std::vector<cl::Device> Devices;

    platform.getDevices(CL_DEVICE_TYPE_GPU, &Devices);
    if (Devices.empty()) {
        std::cerr << "No Devices!" << std::endl;
        return -1;
    }

    cl::Device device = Devices[0];
    std::cout << "Device : " << device.getInfo<CL_DEVICE_NAME>() << std::endl;


    std::ifstream kernel_file("vec_add.cl");
    std::string   opencl_kernel(std::istreambuf_iterator<char>(kernel_file), (std::istreambuf_iterator<char>()));
    cl::Context   context({device});
    cl::Program   program(context, opencl_kernel);

    if (program.build() != CL_SUCCESS) {
        std::cerr << "Fail to build" << std::endl;
        std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        return -1;
    }

    std::vector<int> A(100, 1);
    std::vector<int> B(100, 2);
    std::vector<int> C(100, 0);

    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 100);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 100);
    cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 100);

    cl::CommandQueue queue(context, device);

    cl::Kernel vec_add(program, "vec_add");

    vec_add.setArg(0, buffer_A);
    vec_add.setArg(1, buffer_B);
    vec_add.setArg(2, buffer_C);

    queue.enqueueWriteBuffer(buffer_A, CL_FALSE, 0, sizeof(int) * 100, A.data());
    queue.enqueueWriteBuffer(buffer_B, CL_FALSE, 0, sizeof(int) * 100, B.data());

    queue.enqueueNDRangeKernel(vec_add, cl::NullRange, cl::NDRange(100), cl::NullRange);

    queue.enqueueReadBuffer(buffer_C, CL_FALSE, 0, sizeof(int) * 100, C.data());

    queue.finish();

    for (const auto &v : A) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    for (const auto &v : B) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    for (const auto &v : C) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}