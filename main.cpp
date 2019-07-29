#pragma GCC diagnostic ignored "-Wignored-attributes"

// TARGET OPENCL 1.2
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.hpp>
#include <CL/opencl.h>
#include <EasyBMP.h>
#include <fstream>
#include <iostream>
#include <string>
#include "helper.hpp"
#include "iomanip"

void setColor(BMP &bmp, RGBApixel color);
void initOpenCL();

static BMP output;
static cl::Context context;
static cl::Device device;
static cl::CommandQueue queue;
static cl::Kernel mainkernel;
static cl::Program program;

static char *outPath = "raytracer-out.bmp";

int main(int argc, char const *argv[])
{
    std::cout << "Raytracer" << std::endl;
    output.SetSize(1920, 1080);
    output.SetBitDepth(32);
    setColor(output, rgbapixel(255, 255, 0, 0));
    initOpenCL();
    char *imageHostPtr;
    cl_int imageError;
    cl::Image2D kernel_out = cl::Image2D(context, CL_MEM_WRITE_ONLY, cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT8), output.TellWidth(), output.TellHeight(), 0, NULL, &imageError);
    runPrintError(imageError, "Image build error");
    std::cout << "picture Size: " << kernel_out.getInfo<CL_MEM_SIZE>() << std::endl;
    cl::size_t<3> origin;
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;
    cl::size_t<3> region;
    region[0] = output.TellWidth();
    region[1] = output.TellHeight();
    region[2] = 1;
    mainkernel = cl::Kernel(program, "mainkernel");
    runPrintError(mainkernel.setArg(0, kernel_out), "Set Arg Error");
    queue.enqueueNDRangeKernel(mainkernel, 0, cl::NDRange(output.TellWidth(), output.TellHeight()), cl::NDRange(1, 1));
    uint32_t *imageData = (uint32_t *)malloc(output.TellWidth() * output.TellHeight() * output.TellBitDepth()/4);
    runPrintError(queue.enqueueReadImage(kernel_out, CL_TRUE, origin, region, 0, 0, imageData, NULL, NULL), "Read Image error");

    for (int x = 0; x < output.TellWidth(); x++)
        for (int y = 0; y < output.TellHeight(); y++)
        {
            uint8_t* pixel = (uint8_t*)(((uint32_t*) imageData)+x + y * output.TellWidth());
            
            output.SetPixel(x,y,rgbapixel(pixel[0], pixel[1], pixel[2],pixel[3]));
        }

#ifdef LOG
    std::cout << "image memdump:\n";
    for (int i = 0; i < 32; i++)
    {
        uint32_t data = ((uint32_t *)imageData)[i];
        std::cout << std::hex << (int)data<<" ";
    }
    std::cout << std::endl;
#endif

    output.WriteToFile(outPath);
    //system(("xdg-open "+std::string(outPath)+"&").c_str());
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
            std::cout << "\tDevice " << devicenumber << ": " << device.getInfo<CL_DEVICE_NAME>() << "\n"
                      << "\t\tMax Workgroup size: " << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "\n"
                      << "\t\tMax Workitem size: " << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[0] << "\n"
                      << "\t\tMemory size: " << bytestohuman(device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>());
            std::cout << std::endl;
            devicenumber++;
        }
        platformnumber++;
        devices.clear();
    }
    platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
    context = cl::Context(CL_DEVICE_TYPE_GPU, NULL, errorCallback);
    device = context.getInfo<CL_CONTEXT_DEVICES>()[0];
    cl_int queueError;
    queue = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &queueError);
    runPrintError(queueError, "Create Commandqueue error");
    std::ifstream file("mainkernel.cl");
    std::string kernelcode = std::string(std::istreambuf_iterator<char>(file),
                                         std::istreambuf_iterator<char>());
    program = cl::Program(context, kernelcode.c_str(), CL_TRUE);
    runPrintError(program.build(devices, ""), "Kernel build Error", "\n" + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device));
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
