//const sampler_t =CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
/*
 ------>x(0)
|\
| \
|  \
V   _|
y(1)  z
*/

__constant uint4 backgroundColor = (uint4)(10, 10, 10, 255);
__constant float camaraDistance = 5;
__constant float3 testTriangle[] = {(0, 0, 0), (10, 0, 0), (0, 10, 0)};

// float surface(float4 a, float3 b, float3 c)
// {
//     return (length(a) + lenght(b) + lenght(c)) / 2;
// }

// float3 baryzentricCoordinates(float3 a, float3 b, float3 c, float3 q)
// {

//     return (0.3f, 0.3f, 0.3f);
// }

// bool rayIntersects(float3 rayOrig, float3 rayDirect, __private float3 *triangle)
// {

//     return true;
// }

__kernel void mainkernel(__write_only image2d_t image)
{
    float3 cameraPosition = (float3)(0, 0, -camaraDistance);
    int2 imageCoords = (int2)(get_global_id(0), get_global_id(1));
    float3 targetOnImage = (float3)(get_global_id(0), get_global_id(1), 0) - ((float3)(get_global_size(0), get_global_size(1), 0)) / 2;
    float3 lightbeam = normalize(targetOnImage - cameraPosition);
    uint4 color = backgroundColor;
    color = (uint4)(lightbeam.x * 255 + 128, lightbeam.y * 255 + 128, lightbeam.z * 255 + 128, 255);

    //printf("%f %f %f\n", bary.x, bary.y, bary.z);
    // if (rayIntersects(cameraPosition, lightbeam, testTriangle))
    // {
    //     color = (uint4)(255, 0, 0, 255);
    // }

    // float3 plainePoint = testTriangle[0];
    // float3 plaineA =normalize(triangle[1] - testTriangle[0]);
    // float3 plaineB = testTriangle[2] - testTriangle[0];
    // float3 plaineNormal =normalize(cross(plaineA, plaineB));
    // float3 plaineB =normalize(cross(plaineA, plaineNormal));
    // float3 originOrigin = cameraPosition-testTriangle[0];
    //float3 originPlaine = 

    //colot = (uint4);

    write_imageui(image, imageCoords, color);
}