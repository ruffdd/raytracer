#pragma GCC diagnostic ignored "-Wignored-attributes"

// TARGET OPENCL 1.2
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.hpp>
#include <EasyBMP.h>
#include <fstream>
#include <iostream>

RGBApixel RGBApixl(ebmpBYTE a, ebmpBYTE r, ebmpBYTE g, ebmpBYTE b);
void setColor(BMP &bmp, RGBApixel color);
void initOpenCL();

BMP output;
cl::Context context;
cl::Device device;
cl::CommandQueue queue;
cl::Kernel mainkernel;

int main(int argc, char const *argv[]) {
  std::cout << "Raytracer" << std::endl;
  output.SetSize(32, 32);
  output.SetBitDepth(32);
  setColor(output, RGBApixl(255, 0, 0, 0));
  initOpenCL();

  cl::Image2D image(context, CL_MEM_WRITE_ONLY,
                    cl::ImageFormat(CL_RGB, CL_UNSIGNED_INT8),
                    output.TellWidth(), output.TellHeight());
  //mainkernel.setArg(0, image);

  std::cout << queue.enqueueNDRangeKernel(mainkernel, 0, cl::NDRange(32, 32),
                                          cl::NDRange(32, 32), NULL, NULL)
            << std::endl;

  output.WriteToFile("raytracer-output.bmp");
  return 0;
}

void initOpenCL() {
  std::vector<cl::Platform> platforms;
  std::vector<cl::Device> devices;
  cl::Platform::get(&platforms);
  int platformnumber = 0;
  int devicenumber = 0;
  for (auto &plat : platforms) {
    std::cout << "platform " << platformnumber << ": "
              << plat.getInfo<CL_PLATFORM_NAME>() << std::endl;
    plat.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    devicenumber = 0;
    for (auto &device : devices) {
      std::cout << "  Device " << devicenumber << ": "
                << device.getInfo<CL_DEVICE_NAME>() << "\n";
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
  queue = (context, device, CL_QUEUE_PROFILING_ENABLE);
  std::string kernelcode;
  std::ifstream("mainkernel.cl") >> kernelcode;
  std::cout << kernelcode;
  cl::Program program = cl::Program(context, kernelcode);
  if (program.build(devices, "") != 0) {
    std::cerr << "\e[1;31mKernel Build Error:\e[0m\n";
    std::string buildstatus =
        program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
    std::cout << buildstatus << std::endl;
  }
  mainkernel = cl::Kernel(program, "kernel");
}

void setColor(BMP &bmp, RGBApixel color) {
  for (int x = 0; x < bmp.TellWidth(); x++)
    for (int y = 0; y < bmp.TellHeight(); y++) {
      bmp.SetPixel(x, y, color);
    }
}

RGBApixel RGBApixl(ebmpBYTE a, ebmpBYTE r, ebmpBYTE g, ebmpBYTE b) {
  RGBApixel pixel;
  pixel.Alpha = a;
  pixel.Red = r;
  pixel.Green = g;
  pixel.Blue = g;
  return pixel;
}