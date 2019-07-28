//const sampler_t =CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
__kernel void mainkernel(__write_only image2d_t image) {
    int2 coords = (int2)(get_global_id(0),get_global_id(1));
    uint colorval=coords.x%33<16^coords.y%33<16;
    colorval*=255;
    uint4 color = (uint4)(255,0,0,0);
    write_imageui(image,coords,color);

}