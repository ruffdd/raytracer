//const sampler_t =CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
/*
 ------>x(0)
|\
| \
|  \
V   _|
y(1)  z
*/


__constant uint4 backgroundColor =(uint4)(10,10,10,255);
__constant float camaraDistance = 5;
__constant float3 testTrinagle[] = {(0,0,0),(10,0,0),(0,10,0)};

__kernel void mainkernel(__write_only image2d_t image) {
    float3 cameraPosition = (float3)(0,0,-camaraDistance);
    int2 imageCoords = (int2)(get_global_id(0),get_global_id(1));
    float3 targetOnImage= (float3)(get_global_id(0),get_global_id(1),0)-((float3)(get_global_size(0),get_global_size(1),0))/2;
    float3 lightbeam = normalize(targetOnImage-cameraPosition);
    uint4 color = backgroundColor;
    color = (uint4)(lightbeam.x*255+128,lightbeam.y*255+128,lightbeam.z*255+128,255);



    write_imageui(image,imageCoords,color);

}