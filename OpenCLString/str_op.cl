#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

__kernel void str_op(__global char *A,
                     __global char *B,
                     const int      sz) {
    /*
    int global_size   = get_global_size(0);
    int global_idx    = get_global_id(0);
    int global_offset = get_global_offset(0);

    int group_size = get_num_groups(0);
    int group_idx  = get_group_id(0);

    int local_size = get_local_size(0);
    int local_idx  = get_local_id(0);

    printf("global_size=%d, global_idx=%d, global_offset=%d, group_size=%d, group_idx=%d, local_size=%d, local_idx=%d\n",
           global_size,
           global_idx,
           global_offset,
           group_size,
           group_idx,
           local_size,
           local_idx);
    */
    int id = get_global_id(0);
    for (int i = 0; i < sz; ++i) {
        if (likely(A[id * sz + i] != '\0')) {
            B[id * sz + i] = A[id * sz + i];
            if (unlikely(B[id * sz + i] == ',')) {
                B[id * sz + i]     = '.';
                B[id * sz + i + 1] = '\0';
                break;
            }
        }
    }
}
