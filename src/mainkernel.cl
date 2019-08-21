//const sampler_t =CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
/*
 ------>x(0)
|\
| \
|  \
V   _|
y(1)  z
*/
//#define DEBUG

__constant uint4 backgroundColor = (uint4)(10, 10, 10, 255);
__constant float camaraDistance = 5;
__constant float3 testTriangle[] = {(0, 0, 2), (10, 0, 5), (0, 10, 5)};

float surface(float3 a, float3 b, float3 c)
{
    float s=(length(a) + length(b) + length(c)) / 2;
    float ab = length(b-a);
    float bc = length(c-b);
    float ca = length(a-c);
    return sqrt(s*(s-ab)*(s-bc)*(s-ca));
}

float3 baryzentricCoordinates(float3 a, float3 b, float3 c, float3 q)
{
    float surfaceAll = surface(a,b,c);
    float l1 = surface(q,b,c)/surfaceAll;
    float l2 = surface(a,q,c)/surfaceAll;
    float l3 = surface(a,b,q) /surfaceAll;

    return (l1,l2,l3);
}

bool rayIntersects(float3 rayOrig, float3 rayDirect, __private float3 *triangle)
{

    return true;
}

void printv(float3 vec)
{
#ifdef DEBUG
    printf("(%f,%f,%f)", vec.x, vec.y, vec.z);
#endif
}

__kernel void mainkernel(__write_only image2d_t image)
{
    float3 cameraPosition = (float3)(0, 0, -camaraDistance);
    int2 imageCoords = (int2)(get_global_id(0), get_global_id(1));
    float3 targetOnImage = (float3)(get_global_id(0), get_global_id(1), 0) - ((float3)(get_global_size(0), get_global_size(1), 0)) / 2;
    float3 lightbeam = normalize(targetOnImage - cameraPosition);
    uint4 color = backgroundColor;
    //color = (uint4)(lightbeam.x * 255 + 128, lightbeam.y * 255 + 128, lightbeam.z * 255 + 128, 255);

    // printf("%f %f %f\n", bary.x, bary.y, bary.z);
    // if (rayIntersects(cameraPosition, lightbeam, testTriangle))
    // {
    //     color = (uint4)(255, 0, 0, 255);
    // }

    float3 plainePoint = testTriangle[0];
    float3 plaineA = normalize(testTriangle[1] - testTriangle[0]);
    float3 plaineB = testTriangle[2] - testTriangle[0];
    float3 plaineNormal = normalize(cross(plaineA, plaineB));
    plaineB = normalize(cross(plaineA, plaineNormal));
    float3 originOrigin = cameraPosition - testTriangle[0];
    float3 rayOriginPlaine = (float3)(dot(originOrigin, plaineA), dot(originOrigin, plaineB), dot(originOrigin, plaineNormal));
    float3 directionPlane = (float3)(dot(lightbeam, plaineA), dot(lightbeam, plaineB), dot(lightbeam, plaineNormal));
    float plaineT = copysign((rayOriginPlaine / directionPlane).x, 1);
    float3 plaineInter = cameraPosition + plaineT * lightbeam;
    
    float3 baryzentricCoords = baryzentricCoordinates(testTriangle[0],testTriangle[1],testTriangle[2],plaineInter);
    printv(baryzentricCoords);
    if(baryzentricCoords.x >= 0 && baryzentricCoords.y >= 0, baryzentricCoords.z >= 0){
        color=(uint4)(255,0,0,255);
    }else{
        color=backgroundColor;
    }
    write_imageui(image, imageCoords, color);
}