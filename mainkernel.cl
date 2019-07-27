//const sampler_t =CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
__kernel void mainkernel(__write_only image2d_t image) {
    int2 coords = (int2)(get_global_id(0),get_global_id(1));
    uint4 color = (uint4)(0xff,0xff,0xff,0x80);
    write_imageui(image,coords,color);
}