#pragma OPENCL EXTENSION cl_khr_fp64:enable

kernel void operate(global const float* v0,global const  float* v1,global float* result,global const int* option) {
    int g=get_global_id(0);
    int l=get_local_id(0);
    //printf("the global position is :%d,the local position is : %d\n",g,l);
    result[g]=(*option>0)?(v0[g]+v1[g]):(v0[g]-v1[g]);
    //printf("input0  is:%f,input 1 is %f\n",M0[g],M1[g]);
}

kernel void multiple(global const float* a,global const float* b,global float* c,global const long* height) {
    private float tmp=0;
    unsigned long x,y,n;
    x=get_global_id(0);
    y=get_global_id(1);
    for(n=0;n<height[0];n++) tmp+=a[x*height[0]+n]*b[n*height[1]+y];
    c[x*height[1]+y]=tmp;
}

kernel void eye(global float* value,global const unsigned long* info) {
    unsigned long x=get_global_id(0);
    unsigned long y=get_global_id(1);
    //printf("the global position is :(%ld,%ld)\nthe length is:%ld\n",x,y,y*info[0]+x);
    value[y * info[0] + x] = (x == y) ? 1.0 : 0.0;
}


kernel void e(global float* value,global const float* target) {
    unsigned long n=get_global_id(0);
    value[n]=*target;
}
