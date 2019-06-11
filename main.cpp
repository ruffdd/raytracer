#pragma GCC diagnostic ignored "-Wignored-attributes"

// TARGET OPENCL 1.2
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.hpp>
#include <CL/opencl.h>
#include <EasyBMP.h>
#include <fstream>
#include <iostream>
#include "clerrorhelper.hpp"

RGBApixel RGBApixl(ebmpBYTE a, ebmpBYTE r, ebmpBYTE g, ebmpBYTE b);
void setColor(BMP &bmp, RGBApixel color);
void initOpenCL();

static BMP output;
static cl::Context context;
static cl::Device device;
static cl::CommandQueue queue;
static cl::Kernel mainkernel;
static cl::Program program;

int main(int argc, char const *argv[])
{
    std::cout << "Raytracer" << std::endl;
    output.SetSize(32, 32);
    output.SetBitDepth(32);
    setColor(output, RGBApixl(255, 0, 0, 0));
    initOpenCL();

    cl::Image2D kernel_out = cl::Image2D(context,CL_MEM_WRITE_ONLY,cl::ImageFormat(CL_RGB,CL_UNSIGNED_INT8),output.TellWidth(),output.TellHeight());
    cl::size_t<3> origin;
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;
    cl::size_t<3> region;
    region[0] = output.TellWidth();
    region[1] = output.TellHeight();
    region[2] = 1;
    //queue.enqueueReadImage(image, true, origin, region, output.TellWidth() * output.TellBitDepth() / 8, 0, &image);
    mainkernel = cl::Kernel(program, "mainkernel");
    mainkernel.setArg(0, kernel_out);
    cl_int kernelerror = queue.enqueueNDRangeKernel(mainkernel, 0, cl::NDRange(32, 32), cl::NDRange(32, 32));
    if (kernelerror != 0)
    {
        std::cerr << "\e[1;31mKernel Run Error:\e[0m " << kernelerror << " " << error_to_string(kernelerror) << std::endl;
    }else{
        std::cout << "\e[1;32mKernel run Succsesfully\e[0m" << std::endl;
    }

    // output.WriteToFile("raytracer-output.bmp");
    return 0;
}

void initOpenCL()
{
    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;
    cl::Platform::get(&platforms);
    int platformnumber = 0;
    int devicenumber = 0;
    for (auto &plat : platforms)
    {
        std::cout << "platform " << platformnumber << ": "
                  << plat.getInfo<CL_PLATFORM_NAME>() << std::endl;
        plat.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        devicenumber = 0;
        for (auto &device : devices)
        {
            std::cout << "\t\tDevice " << devicenumber << ": "
                      << device.getInfo<CL_DEVICE_NAME>() << "\n"
                      << "\t\t\tMax Workgroup size: "
                      << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "\n"
                      << "\t\t\tMax Workitem size: "
                      << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[0];
            std::cout << std::endl;
            devicenumber++;
        }
        platformnumber++;
        devices.clear();
    }
    platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
    context = cl::Context(
        CL_DEVICE_TYPE_GPU); // (CL_DEVICE_TYPE_GPU, { CL_CONTEXT_PLATFORM,
                             // (cl_context_properties) platforms[0] (), 0, 0 });
    device = context.getInfo<CL_CONTEXT_DEVICES>()[0];
    queue = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE);

    std::ifstream file("mainkernel.cl");
    std::string kernelcode = std::string(std::istreambuf_iterator<char>(file),
                                         std::istreambuf_iterator<char>());
    program = cl::Program(context, kernelcode.c_str(),CL_TRUE);
    cl_int builderr = program.build(devices, "");
    if (builderr != 0)
    {
        std::cerr << "\e[1;31mKernel Build Error:\e[0m\n";
        std::cerr << builderr << " " << error_to_string(builderr);
        std::string buildstatus =
            program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        std::cout << buildstatus << std::endl;
    }
    mainkernel = cl::Kernel(program, "mainkernel");
}

void setColor(BMP &bmp, RGBApixel color)
{
    for (int x = 0; x < bmp.TellWidth(); x++)
        for (int y = 0; y < bmp.TellHeight(); y++)
        {
            bmp.SetPixel(x, y, color);
        }
}

RGBApixel RGBApixl(ebmpBYTE a, ebmpBYTE r, ebmpBYTE g, ebmpBYTE b)
{
    RGBApixel pixel;
    pixel.Alpha = a;
    pixel.Red = r;
    pixel.Green = g;
    pixel.Blue = g;
    return pixel;
}