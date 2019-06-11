__kernel void mainkernel(__write_only image2d_t image) {
    int x = get_global_id(0);
}