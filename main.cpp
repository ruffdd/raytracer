#include <iostream>
#include <EasyBMP.h>

RGBApixel RGBApixl(ebmpBYTE a,ebmpBYTE r, ebmpBYTE g, ebmpBYTE b);
void setColor(BMP& bmp,RGBApixel color);

BMP output;
int main(int argc, char const *argv[])
{
    std::cout <<"Raytracer"<< std::endl;
    output.SetSize(32,32);
    output.SetBitDepth(32);
    setColor(output,RGBApixl(255,0,0,0));
    output.WriteToFile("raytracer-output.bmp");
    return 0;
}

void setColor(BMP& bmp,RGBApixel color){
    for(int x = 0; x < bmp.TellWidth();x++)
        for(int y = 0; y<bmp.TellHeight();y++){
            bmp.SetPixel(x,y,color);
        }
}

RGBApixel RGBApixl(ebmpBYTE a,ebmpBYTE r, ebmpBYTE g, ebmpBYTE b){
    RGBApixel pixel;
    pixel.Alpha = a;
    pixel.Red = r;
    pixel.Green = g;
    pixel.Blue = g;
    return pixel;
}