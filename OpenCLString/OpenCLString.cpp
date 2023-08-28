#define _CRT_SECURE_NO_WARNINGS

#include <CL/opencl.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PLATFORM 0

constexpr size_t STRLEN = 100;
constexpr size_t CASESZ = 100000;

cl::Device opencl_init() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty()) {
        std::cerr << "No platforms!" << std::endl;
        exit(-1);
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

    cl::Platform            platform = platforms[PLATFORM];
    std::vector<cl::Device> Devices;

    platform.getDevices(CL_DEVICE_TYPE_GPU, &Devices);
    if (Devices.empty()) {
        std::cerr << "No Devices!" << std::endl;
        exit(-1);
    }

    cl::Device device = Devices[0];
    std::cout << "Device : " << device.getInfo<CL_DEVICE_NAME>() << std::endl;

    return device;
}

std::string kernel_load(std::string filename) {
    std::ifstream kernel_file(filename);
    std::string   opencl_kernel(std::istreambuf_iterator<char>(kernel_file), (std::istreambuf_iterator<char>()));
    return opencl_kernel;
}

int main(int argc, char **argv) {
    cl::Device  device        = opencl_init();
    std::string opencl_kernel = kernel_load("str_op.cl");
    cl::Context context({device});
    cl::Program program(context, opencl_kernel);

    if (program.build() != CL_SUCCESS) {
        std::cerr << "Fail to build" << std::endl;
        std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        return -1;
    }

    std::vector<std::array<char, STRLEN>> A(CASESZ);
    std::vector<std::array<char, STRLEN>> B(CASESZ);

    cl_int kernel_size = STRLEN;

    for (auto &each : A) {
        const std::string example = "Long may our land be bright, with freedom's holy light.";
        std::copy(example.begin(), example.end(), each.data());
    }

    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(char) * STRLEN * CASESZ);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(char) * STRLEN * CASESZ);


    cl::CommandQueue queue(context, device);

    cl::Kernel str_op(program, "str_op");

    str_op.setArg(0, buffer_A);
    str_op.setArg(1, buffer_B);
    str_op.setArg(2, kernel_size);

    queue.enqueueWriteBuffer(buffer_A, CL_FALSE, 0, sizeof(char) * STRLEN * CASESZ, A.data());

    queue.enqueueNDRangeKernel(str_op, cl::NullRange, cl::NDRange(CASESZ), cl::NullRange);

    queue.enqueueReadBuffer(buffer_B, CL_FALSE, 0, sizeof(char) * STRLEN * CASESZ, B.data());

    queue.finish();

    int              indices[]{0, 1, 2, (static_cast<int>(A.size()) - 1)};
    constexpr size_t indices_size = sizeof(indices) / sizeof(int);

    auto print = [=](std::vector<std::array<char, STRLEN>> x) {
        for (size_t i = 0; i < indices_size; ++i) {
            int j = indices[i];
            if (i == indices_size - 1)
                std::cout << "...\n";
            std::cout << "[" << j + 1 << "]: " << x[j].data() << "\n";
        }
        std::cout << std::endl;
    };

    print(A);
    print(B);

    return 0;
}