#pragma OPENCL EXTENSION cl_khr_fp64:enable

kernel void operate(global const float* v0,global const float* v1,global float* result,char option) {
    int g=get_global_id(0);
    int l=get_local_id(0);
    switch(option) {
        case '0': {
            result[g]=v0[g]+v1[g];
            break;
        }
        case '1': {
            result[g]=v0[g]-v1[g];
            break;
        }
        case '2': {
            result[g]=v0[g]*v1[g];
            break;
        }
        default :{
            result[g]=0;
            break;
        }
    }
}

kernel void multiple(global const float* a,global const float* b,global float* c,global const long* height) {
    private float tmp=0;
    unsigned long x,y,n;
    x=get_global_id(0);
    y=get_global_id(1);
    for(n=0;n<height[0];n++) tmp+=a[x*height[0]+n]*b[n*height[1]+y];
    c[x*height[1]+y]=tmp;
}

kernel void eye(global float* value,unsigned long info,float target) {
    unsigned long x=get_global_id(0);
    unsigned long y=get_global_id(1);
    value[y * info + x] = (x == y) ? target : 0;
}

kernel void e(global float* value,float target) {
    unsigned long n=get_global_id(0);
    value[n]=target;
}

kernel void T(global const float* value,global float* result,global const unsigned long* size) {
    unsigned long x=get_global_id(0);
    unsigned long y=get_global_id(1);
    result[y*size[0]+x]=value[x*size[1]+y];
}
