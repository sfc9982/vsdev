__kernel void vec_add(__global int *A, __global int *B, __global int *C) {
    int id = get_global_id(0);
    C[id]  = A[id] + B[id];
}
