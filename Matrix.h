#ifndef __cplusplus
// extern "C" {
// #endif

#include <CL/cl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MATRIX
typedef long double _M_value;
typedef unsigned long long _Number;
#define MATRIX
#define println(str)         \
    do {                     \
        printf("%s\n", str); \
    } while (0)
#endif

#define TIME(str, code) ({                                                         \
    float start = clock();                                                         \
    code;                                                                          \
    float end = clock();                                                           \
    printf("This %s time is: %lf seconds\n", str, (end - start) / CLOCKS_PER_SEC); \
})

typedef struct
{
    _Number x, y;
    _M_value* v;
} Matrix;

typedef struct
{
    _Number x, y;
    float* v;
} S_Matrix;

// there are some macro,you can use it like function
#define freeMatrix(M) \
    do {              \
        free(M->v);   \
        free(M);      \
    } while (0)

#define length(s)                         \
    ({                                    \
        printf("the string  ");           \
        int i = printf("%s", s);          \
        printf("  's length is:%d\n", i); \
        i;                                \
    })

#define end(file)                   \
    ({                              \
        int locate = 0;             \
        while (getc(file) != EOF) { \
            locate++;               \
            fseek(file, 1, locate); \
        }                           \
        locate;                     \
    })

#define CREATE0()                                                       \
    ({                                                                  \
        Matrix* tmp = (Matrix*)malloc(sizeof(Matrix));                  \
        printf("please input the Matrix's row:");                       \
        scanf("%lld", &tmp->x);                                         \
        printf("please input the Matrix's column:");                    \
        scanf("%lld", &tmp->y);                                         \
        tmp->v = (_M_value*)malloc(sizeof(_M_value) * tmp->x * tmp->y); \
        tmp;                                                            \
    })

#define CREATE1(width, height)                                            \
    ({                                                                    \
        Matrix* target = (Matrix*)malloc(sizeof(Matrix));                 \
        target->x = width, target->y = height;                            \
        target->v = (_M_value*)malloc(sizeof(_M_value) * width * height); \
        target;                                                           \
    })

#define SET(M)                                                                 \
    ({                                                                         \
        _Number i;                                                             \
        if (M != NULL) {                                                       \
            for (i = 0; i < M->x * M->y; i++) {                                \
                printf("the Matrix[%lld][%lld]=", i / M->y + 1, i % M->y + 1); \
                scanf("%Lf", &M->v[i]);                                        \
            }                                                                  \
        } else {                                                               \
            println("Creating Error!!!");                                      \
            freeMatrix(M);                                                     \
        }                                                                      \
        M;                                                                     \
    })

// the macro has bug,please keep ceriosity to use it
#define EYE(M)                                    \
    ({                                            \
        _Number i;                                \
        if (M->x == M->y) {                       \
            for (i = 0; i < M->x * M->y; i++) {   \
                if (i / M->y + 1 == i % M->y + 1) \
                    M->v[i] = 1;                  \
                else                              \
                    M->v[i] = 0;                  \
            }                                     \
            M;                                    \
        } else {                                  \
            println("Generating Error!!!");       \
            freeMatrix(M);                        \
            NULL;                                 \
        }                                         \
    })

#define e(M, num)                             \
    ({                                        \
        _Number i;                            \
        if (M != NULL) {                      \
            for (i = 0; i < M->x * M->y; i++) \
                M->v[i] = num;                \
        } else {                              \
            println("Generating Error!!!");   \
            freeMatrix(M);                    \
        }                                     \
        M;                                    \
    })

#define t(M)                                                                       \
    ({                                                                             \
        _Number i, h, w;                                                           \
        if (M != NULL) {                                                           \
            if (sizeof(M->v[0]) == sizeof(float))                                  \
                float* tmp = (float*)malloc(sizeof(float) * M->x * M->y);          \
            else                                                                   \
                _M_value* tmp = (_M_value*)malloc(sizeof(_M_value) * M->x * M->y); \
            for (i = 0; i < M->x * M->y; i++) {                                    \
                w = i / M->y;                                                      \
                h = i - M->y * w;                                                  \
                tmp[h * M->x + w] = M->v[w * M->y + h];                            \
            }                                                                      \
            M->v = tmp;                                                            \
            M->x = M->x ^ M->y;                                                    \
            M->y = M->y ^ M->x;                                                    \
            M->x = M->x ^ M->y;                                                    \
        } else {                                                                   \
            println("Transforming Error!!!");                                      \
            freeMatrix(M);                                                         \
        }                                                                          \
        M;                                                                         \
    })

#define IMPORT(M, value, num)            \
    ({                                   \
        _Number i;                       \
        if (M->x * M->y == num) {        \
            for (i = 0; i < num; i++)    \
                M->v[i] = value[i];      \
        } else {                         \
            println("setting error!!!"); \
        }                                \
        M;                               \
    })

#define DISPLAY(M)                                    \
    do {                                              \
        _Number i;                                    \
        if (M != NULL) {                              \
            println("");                              \
            for (i = 0; i < M->x * M->y; i++) {       \
                if (sizeof(M->v[0]) == sizeof(float)) \
                    printf("%24.12f	", M->v[i]);  \
                else                                  \
                    printf("%24.12LF	", M->v[i]);  \
                if ((i + 1) % M->y == 0)              \
                    printf("\n");                     \
            }                                         \
            println("");                              \
        } else {                                      \
            println("print Matrix Error!!!");         \
            freeMatrix(M);                            \
        }                                             \
    } while (0)

Matrix*
create()
{
    Matrix* tmp = (Matrix*)malloc(sizeof(Matrix));
    printf("please input the Matrix's row:");
    scanf("%lld", &tmp->x);
    printf("please input the Matrix's column:");
    scanf("%lld", &tmp->y);
    tmp->v = (_M_value*)malloc(sizeof(_M_value) * tmp->x * tmp->y);
    return tmp;
}

Matrix*
Create(_Number width, _Number height)
{
    Matrix* target = (Matrix*)malloc(sizeof(Matrix));
    target->x = width, target->y = height;
    target->v = (_M_value*)malloc(sizeof(_M_value) * width * height);
    return target;
}

Matrix*
Set(Matrix* M)
{
    _Number i;
    if (M != NULL) {
        for (i = 0; i < M->x * M->y; i++) {
            printf("the Matrix[%lld][%lld]=", i / M->y + 1, i % M->y + 1);
            scanf("%Lf", &M->v[i]);
        }
    } else {
        println("Creating Error!!!");
        freeMatrix(M);
    }
    return M;
}

Matrix*
Import(Matrix* M, const _M_value* value, const _Number num)
{
    _Number i;
    if (M->x * M->y == num) {
        for (i = 0; i < num; i++)
            M->v[i] = value[i];
    } else {
        println("setting error!!!");
    }
    return M;
}

Matrix*
Eye(Matrix* M)
{
    _Number i;
    if (M->x == M->y) {
        for (i = 0; i < M->x * M->y; i++) {
            if (i / M->y + 1 == i % M->y + 1)
                M->v[i] = 1;
            else
                M->v[i] = 0;
        }
        return M;
    } else {
        println("Generating Error!!!");
        freeMatrix(M);
        return NULL;
    }
}

Matrix*
E(Matrix* M, const _M_value target)
{
    _Number i;
    if (M != NULL) {
        for (i = 0; i < M->x * M->y; i++)
            M->v[i] = target;
    } else {
        println("Generating Error!!!");
        freeMatrix(M);
    }
    return M;
}

Matrix*
add(const Matrix* M, const Matrix* m)
{
    _Number i;
    if (M != NULL && m != NULL && (M->x) == (m->x) && (M->y) == (m->y)) {
        Matrix* tmp = CREATE1(M->x, M->y);
        for (i = 0; i < M->x * M->y; i++) {
            if (m->v[i] == 0)
                tmp->v[i] = M->v[i];
            else
                tmp->v[i] = M->v[i] + m->v[i];
        }
        return tmp;
    } else
        return NULL;
}

Matrix*
subtraction(const Matrix* M0, const Matrix* M1)
{
    _Number i;
    if (M0 != NULL && M1 != NULL && M0->x == M1->x && M0->y == M1->y) {
        Matrix* tmp = CREATE1(M0->x, M0->y);
        for (i = 0; i < M0->x * M0->y; i++) {
            if (M1->v[i] == 0)
                tmp->v[i] = M0->v[i];
            else
                tmp->v[i] = M0->v[i] - M1->v[i];
        }
        return tmp;
    } else
        return NULL;
}

/*
#define operate(M,m,operating) ({\
        _Number i;\
        if((M)!=NULL&&(m)!=NULL&&(M->x)==(m->x)&&(M->y)==(m->y)) {\
                Matrix* tmp=(Matrix*) malloc(sizeof(Matrix));\
                tmp->x=M->x;\
                tmp->y=M->y;\
                tmp->v=(_M_value*) malloc(sizeof(_M_value)*tmp->x*tmp->y);\
                for(i=0;i<M->x*M->y;i++) {\
                        if (m->v[i]==0) tmp->v[i]=M->v[i];\
                        else
tmp->v[i]=operating>0?M->v[i]+m->v[i]:M->v[i]-m->v[i];\
                }\
                tmp;\
        }\
        else {\
                NULL;\
        }\
})
*/

Matrix*
T(Matrix* M)
{
    _Number i, h, w;
    if (M != NULL) {
        _M_value* tmp = (_M_value*)malloc(sizeof(_M_value) * M->x * M->y);
        for (i = 0; i < M->x * M->y; i++) {
            w = i / M->y;
            h = i - M->y * w;
            tmp[h * M->x + w] = M->v[w * M->y + h];
        }
        M->v = tmp;
        M->x = M->x ^ M->y;
        M->y = M->y ^ M->x;
        M->x = M->x ^ M->y;
    } else {
        println("Transforming Error!!!");
        freeMatrix(M);
    }
    return M;
}

Matrix*
multiple(const Matrix* M0, const Matrix* M1)
{
    _Number n, k, h, w;
    _M_value tmp;
    if (M0 != NULL && M1 != NULL && M0->y == M1->x) {
        Matrix* result = Create(M0->x, M1->y);
        for (n = 0; n < result->x * result->y; n++) {
            tmp = 0;
            w = n / result->y;
            h = n - w * result->y;
            for (k = 0; k < M0->y; k++)
                tmp += M0->v[w * M0->y + k] * M1->v[k * M1->y + h];
            result->v[w * result->y + h] = tmp;
        }
        return result;
    } else {
        println("Generating Error!!!");
        return NULL;
    }
}

_M_value*
Export(const Matrix* M)
{
    _Number i;
    if (M != NULL && M->v != NULL) {
        _M_value* value = (_M_value*)malloc(sizeof(_M_value) * M->x * M->y);
        for (i = 0; i < M->x * M->y; i++)
            value[i] = M->v[i];
        return value;
    } else {
        println("output error!!!");
        return NULL;
    }
}

void display(Matrix* M)
{
    _Number i;
    if (M != NULL) {
        println("");
        for (i = 0; i < M->x * M->y; i++) {
            printf("%24.12Lf	", M->v[i]);
            if ((i + 1) % M->y == 0)
                printf("\n");
        }
        println("");
    } else {
        println("print Matrix Error!!!");
        freeMatrix(M);
    }
}

char* filestring(const char* filename)
{
    FILE* f0 = fopen(filename, "r+");
    long location = 0;
    fseek(f0, 0, SEEK_END);
    char* tmp = (char*)calloc(ftell(f0) + 1, sizeof(char));
    fseek(f0, 0, SEEK_SET);
    while (fgetc(f0) != EOF) {
        fseek(f0, location, SEEK_SET);
        tmp[location] = fgetc(f0);
        location++;
    }
    // printf("%s\n",tmp);
    fclose(f0);
    return tmp;
}

S_Matrix*
create0()
{
    S_Matrix* tmp = (S_Matrix*)malloc(sizeof(S_Matrix));
    printf("please input the Matrix's row:");
    scanf("%lld", &tmp->x);
    printf("please input the Matrix's column:");
    scanf("%lld", &tmp->y);
    tmp->v = (float*)malloc(sizeof(float) * tmp->x * tmp->y);
    return tmp;
}

S_Matrix*
Create0(_Number width, _Number height)
{
    S_Matrix* target = (S_Matrix*)malloc(sizeof(S_Matrix));
    target->x = width, target->y = height;
    target->v = (float*)malloc(sizeof(float) * width * height);
    return target;
}

S_Matrix*
Set0(S_Matrix* M)
{
    _Number i;
    if (M != NULL) {
        for (i = 0; i < M->x * M->y; i++) {
            printf("the Matrix[%lld][%lld]=", i / M->y + 1, i % M->y + 1);
            scanf("%f", &M->v[i]);
        }
    } else {
        println("Creating Error!!!");
        freeMatrix(M);
    }
    return M;
}

Matrix*
kronecker(Matrix* A, Matrix* B)
{
    _Number i, j, k, l;
    if (A == NULL || B == NULL || A->v == NULL || B->v == NULL || A->x <= 0 || A->y <= 0 || B->x <= 0 || B->y <= 0) {
        println("Generating Error!!!");
        return NULL;
    } else {
        Matrix* result = Create(A->x * B->x, A->y * B->y);
        for (l = 0; l < A->x; l++) {
            for (k = 0; k < B->x; k++) {
                for (j = 0; j < A->y; j++) {
                    for (i = 0; i < B->y; i++)
                        result->v[(l * B->y + k) * A->y * B->y + j * B->y + i /*(j*B->y+i)*A->y*B->y+l*B->y+k*/] = A->v[l * A->y + j] * B->v[k * B->y + i];
                }
            }
        }
        return result;
    }
}

// support OpenCL

#define check(input, str)                                  \
    ({                                                     \
        if (input != CL_SUCCESS) {                         \
            printf("%s is err,the num is:%d", str, input); \
            return 1;                                      \
        }                                                  \
    })

typedef enum {
    operate,
    Multiple,
    eye,
    e
} choice;

typedef struct
{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue cmd_queue;
    cl_program program;
    cl_int* cl_err;
    cl_ulong info[2];
    size_t *global_size, *local_size, N;
} cl_group;

cl_group*
format_group(const char* cl_filename)
{

    cl_group* c0 = (cl_group*)malloc(sizeof(cl_group));
    c0->cl_err = (cl_int*)calloc(1, sizeof(cl_int));

    *c0->cl_err = clGetPlatformIDs(1, &(c0->platform), NULL);
    *c0->cl_err = clGetDeviceIDs(c0->platform, CL_DEVICE_TYPE_ALL, 1, &c0->device, NULL);

    *c0->cl_err = clGetDeviceInfo(c0->device,
        CL_DEVICE_GLOBAL_MEM_SIZE,
        sizeof(cl_ulong),
        &c0->info[0],
        NULL);
    *c0->cl_err = clGetDeviceInfo(c0->device,
        CL_DEVICE_MAX_WORK_ITEM_SIZES,
        sizeof(cl_ulong),
        &c0->info[1],
        NULL);
    c0->info[1] *= c0->info[1] * c0->info[1];

    c0->context = clCreateContext(NULL, 1, &c0->device, NULL, NULL, c0->cl_err);
    c0->cmd_queue = clCreateCommandQueueWithProperties(
        c0->context, c0->device, NULL, c0->cl_err);

    char* s = filestring(cl_filename);
    c0->program = clCreateProgramWithSource(
        c0->context, 1, (const char**)&s, NULL, c0->cl_err);
    *c0->cl_err = clBuildProgram(c0->program, 1, &c0->device, NULL, NULL, NULL);

    return c0;
}

int cl_clearmem(cl_group* target)
{
    *target->cl_err = clFlush(target->cmd_queue);
    check(*target->cl_err, "release memory");
    *target->cl_err = clFinish(target->cmd_queue);
    check(*target->cl_err, "release memory");
    *target->cl_err = clReleaseCommandQueue(target->cmd_queue);
    check(*target->cl_err, "release memory");
    *target->cl_err = clReleaseContext(target->context);
    check(*target->cl_err, "release memory");
    *target->cl_err = clReleaseProgram(target->program);
    check(*target->cl_err, "release memory");
    free(target->global_size);
    free(target->local_size);
    free(target->cl_err);
    free(target);
    println("release object success!!!");
    return 0;
}

S_Matrix*
calculate(S_Matrix* a, S_Matrix* b)
{
    cl_group* g0 = format_group("M.cl");
    choice selection;
    println("please input the type of your selection:");
    scanf("%d", &selection);
    while (selection != operate && selection != Multiple && selection != eye && selection != e) {
        println("value is invaild,please input true type of your selection:");
        scanf("%d", &selection);
    }
    switch (selection) {
    case operate: {
        S_Matrix* result = Create0(a->x, b->y);
        free(result->v);
        int option;
        println("please input the operate option:");
        scanf("%d", &option);

        cl_kernel kernel = clCreateKernel(g0->program, "operate", g0->cl_err);
        cl_mem v0, v1, rlt, set;

        v0 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            a->x * a->y * sizeof(float),
            a->v,
            g0->cl_err);
        v1 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            b->x * b->y * sizeof(float),
            b->v,
            g0->cl_err);
        rlt = clCreateBuffer(g0->context,
            CL_MEM_WRITE_ONLY,
            result->x * result->y * sizeof(float),
            NULL,
            g0->cl_err);
        set = clCreateBuffer(
            g0->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int), &option, g0->cl_err);

        result->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, rlt, CL_TRUE, CL_MAP_WRITE, 0, result->x * result->y * sizeof(float), 0, NULL, NULL, g0->cl_err);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &v0);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &v1);
        *g0->cl_err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &rlt);
        *g0->cl_err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &set);

        g0->global_size = (size_t*)malloc(1 * sizeof(size_t));
        g0->local_size = (size_t*)malloc(1 * sizeof(size_t));

        *g0->global_size = a->x * a->y;
        *g0->local_size = a->x;

        TIME("OpenCL vector operating", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 1, NULL, g0->global_size, g0->local_size, 0, NULL, NULL)));

        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, rlt, result->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return result;
        break;
    }
    case Multiple: {
        S_Matrix* output = Create0(a->x, b->y);
        free(output->v);
        long h[2] = { (long)a->y, (long)b->y };

        cl_kernel kernel = clCreateKernel(g0->program, "multiple", g0->cl_err);
        cl_mem Matrix0, Matrix1, result, information;

        Matrix0 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            a->x * a->y * sizeof(float),
            a->v,
            g0->cl_err);
        Matrix1 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            b->x * b->y * sizeof(float),
            b->v,
            g0->cl_err);
        result = clCreateBuffer(g0->context,
            CL_MEM_WRITE_ONLY,
            output->x * output->y * sizeof(float),
            NULL,
            g0->cl_err);
        information = clCreateBuffer(
            g0->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 2 * sizeof(long), h, g0->cl_err);

        output->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, result, CL_TRUE, CL_MAP_WRITE, 0, output->x * output->y * sizeof(float), 0, NULL, NULL, g0->cl_err);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &Matrix0);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &Matrix1);
        *g0->cl_err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &result);
        *g0->cl_err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &information);

        g0->global_size = (size_t*)malloc(2 * sizeof(size_t));
        // g0->local_size=(size_t*)malloc(2*sizeof(size_t));

        g0->global_size[0] = output->x;
        g0->global_size[1] = output->y;
        //*g0->local_size=a->x;

        TIME("OpenCL matrix multiply", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 2, NULL, g0->global_size, NULL, 0, NULL, NULL)));
        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, result, output->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return output;
        break;
    }
    case eye: {
        free(a->v);
        unsigned long* size = (unsigned long*)malloc(sizeof(unsigned long) * 2);
        size[0] = a->x, size[1] = a->y;

        cl_kernel kernel = clCreateKernel(g0->program, "eye", g0->cl_err);

        cl_mem value, Info;
        value = clCreateBuffer(g0->context,
            CL_MEM_READ_WRITE,
            a->x * a->y * sizeof(float),
            NULL,
            g0->cl_err);
        Info = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            2 * sizeof(unsigned long),
            size,
            g0->cl_err);

        //*g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,v,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);

        a->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, value, CL_TRUE, CL_MAP_WRITE, 0, a->x * a->y * sizeof(float), 0, NULL, NULL, g0->cl_err);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &value);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &Info);

        g0->global_size = (size_t*)malloc(2 * sizeof(size_t));
        g0->local_size = (size_t*)malloc(2 * sizeof(size_t));

        g0->global_size[0] = a->x, g0->global_size[1] = a->y;
        g0->local_size[0] = a->x / 2, g0->local_size[1] = a->y / 2;

        TIME("OpenCL identitial matrix setting", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 2, NULL, g0->global_size, g0->local_size, 0, NULL, NULL)));

        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, value, a->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return size[0] == size[1] ? a : NULL;

        break;
    }
    case e: {
        free(a->v);
        float target;
        println("please input the target value:");
        scanf("%f", &target);

        cl_kernel kernel = clCreateKernel(g0->program, "e", g0->cl_err);

        cl_mem data, content;
        data = clCreateBuffer(g0->context,
            CL_MEM_READ_WRITE,
            a->x * a->y * sizeof(float),
            NULL,
            g0->cl_err);
        content = clCreateBuffer(
            g0->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float), &target, g0->cl_err);

        a->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, data, CL_TRUE, CL_MAP_WRITE, 0, a->x * a->y * sizeof(float), 0, NULL, NULL, g0->cl_err);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &data);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &content);

        g0->global_size = (size_t*)malloc(sizeof(size_t));
        *g0->global_size = a->x * a->y;

        TIME("OpenCL matrix setting", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 1, NULL, g0->global_size, NULL, 0, NULL, NULL)));

        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, data, a->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return a;

        break;
    }
    default: {
        println("the program error!!!");
        return NULL;
    } // goto exit;
    }
    /*exit: {
        println("the program error!!!");
        return 1;
    }*/
}

#undef MATRIX

// #ifndef __cplusplus
// }
#endif

#ifdef __cplusplus

#include <iostream>
#include <new>

typedef unsigned long long Integer;
typedef long double Double;

class Matrix {
private:
    Integer rows, column;
    Double* value;

public:
    Matrix()
    {
        std::cout << "please input the column and rows:" << std::endl;
        std::cin >> rows;
        std::cin >> column;
        value = new Double[rows * column];
    }
    Matrix(const Integer& x, const Integer& y)
    {
        rows = x, column = y;
        value = new Double[rows * column];
    }
    Matrix(const Integer& size)
    {
        rows = column = size;
        value = new Double[size * size];
    }
    Matrix(const Matrix& M)
    {
        this->rows = M.rows;
        this->column = M.column;
        this->value = M.value;
    }

    ~Matrix()
    {
        rows = 0, column = 0;
        delete[] value;
    }

    void set();
    void set(const Integer& x, const Integer& y);
    void set(const Integer& num);
    void set(const Double* data);
    void info() const;
    friend void display(const Matrix& M);
    Matrix operator+(const Matrix& M);
    Matrix operator-(const Matrix& M);
    Matrix operator*(const Matrix& M);
    Matrix& operator=(const Matrix& M);
    Double& operator()(Integer x0, Integer y0);
    friend Matrix T(const Matrix& M);
    friend Matrix& Eye(const Integer& num);
    friend Double* M_out(const Matrix& M);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& M);
    friend std::istream& operator>>(std::istream& is, Matrix& M);
    friend Matrix kronecker(const Matrix& A, const Matrix& B);
};

void Matrix::set()
{
    for (Integer i = 0; i < rows * column; i++) {
        std::cout << "please input the Matrix[" << i / column + 1 << "]"
                  << "[" << i - (i / column) * column + 1 << "]" << std::endl;
        std::cin >> value[i];
    }
}

void Matrix::set(const Integer& num)
{
    for (Integer i = 0; i < rows * column; i++)
        value[i] = num;
}

void Matrix::set(const Integer& x, const Integer& y)
{
    rows = x, column = y;
    delete[] value;
    value = new Double[x * y];
}

void Matrix::info() const
{
    std::cout << "the row is:" << this->rows << "\n"
              << "the column is:" << this->column << std::endl;
}

void Matrix::set(const Double* data)
{
    for (Integer i = 0; i < rows * column; i++)
        value[i] = data[i];
}

void display(const Matrix& M)
{
    for (Integer i = 0; i < M.rows * M.column; i++) {
        std::cout << M.value[i] << "    ";
        if ((i + 1) % M.column == 0)
            std::cout << "\n"
                      << std::endl;
    }
}

Matrix
Matrix::operator+(const Matrix& M)
{
    if (M.rows != 0 && M.column != 0 && rows != 0 && column != 0 && (M.rows) == (rows) && (M.column) == (column)) {
        Matrix* tmp = new Matrix(rows, column);
        for (Integer i = 0; i < rows * column; i++) {
            if (M.value[i] == 0)
                tmp->value[i] = value[i];
            else
                tmp->value[i] = value[i] + M.value[i];
        }
        return *tmp;
    } else
        return Matrix(0, 0);
}

Matrix
Matrix::operator-(const Matrix& M)
{
    if (M.rows != 0 && M.column != 0 && rows != 0 && column != 0 && (M.rows) == (rows) && (M.column) == (column)) {
        Matrix* tmp = new Matrix(rows, column);
        for (Integer i = 0; i < rows * column; i++) {
            if (M.value[i] == 0)
                tmp->value[i] = value[i];
            else
                tmp->value[i] = value[i] - M.value[i];
        }
        return *tmp;
    } else
        return Matrix(0, 0);
}

Matrix
T(const Matrix& M)
{
    Integer x, y, i;
    if (M.rows != 0 && M.column != 0) {
        Matrix* target = new Matrix(M.column, M.rows);
        for (i = 0; i < M.rows * M.column; i++) {
            x = i / M.column;
            y = i - M.column * x;
            target->value[y * M.rows + x] = M.value[x * M.column + y];
        }
        return *target;
    } else {
        return Matrix(0, 0);
    }
}

Double&
Matrix::operator()(Integer x0, Integer y0)
{
    if (x0 > rows || y0 > column)
        std::cout << "error" << std::endl;
    else
        std::cout << "the Matrix[" << x0 << "]"
                  << "[" << y0 << "]is:" << value[(x0 - 1) * column + (y0 - 1)]
                  << std::endl;
    return value[(x0 - 1) * column + (y0 - 1)];
}

Matrix
Matrix::operator*(const Matrix& M)
{
    Integer i, j, k, n;
    Double tmp;
    if (M.rows != 0 && M.column != 0 && rows != 0 && column != 0 && column == M.rows) {
        Matrix* result = new Matrix(rows, M.column);
        for (n = 0; n < rows * M.column; n++) {
            tmp = 0;
            i = n / M.column;
            j = n - i * M.column;
            for (k = 0; k < column; k++)
                tmp += value[i * column + k] * M.value[k * M.column + j];
            result->value[n] = tmp;
        }
        return *result;
    } else {
        // println("Generating Error!!!");
        return Matrix(0, 0);
    }
}

Matrix&
Matrix::operator=(const Matrix& M)
{
    this->rows = M.rows;
    this->column = M.column;
    this->value = M.value;
    return *this;
}

Matrix&
Eye(const Integer& num)
{
    Matrix* m0 = new Matrix(num);
    for (Integer i = 0; i < num * num; i++)
        m0->value[i] = i / num == i - (i / num) * num ? 1 : 0;
    return *m0;
}

std::ostream&
operator<<(std::ostream& os, const Matrix& M)
{
    for (Integer i = 0; i < M.rows * M.column; i++) {
        os << M.value[i] << "    ";
        if ((i + 1) % M.column == 0)
            os << "\n"
               << std::endl;
    }
    return os;
}

std::istream&
operator>>(std::istream& is, Matrix& M)
{
    for (Integer i = 0; i < M.rows * M.column; i++) {
        std::cout << "please input the Matrix[" << i / M.column + 1 << "]"
                  << "[" << i - (i / M.column) * M.column + 1 << "]" << std::endl;
        std::cin >> M.value[i];
    }
    return is;
}

Double*
M_out(const Matrix& M)
{
    if (M.value != nullptr) {
        Double* data = new Double[M.rows * M.column];
        for (Integer i = 0; i < M.rows * M.column; i++)
            data[i] = M.value[i];
        return data;
    } else {
        std::cout << "copy failed!" << std::endl;
        return nullptr;
    }
}

Matrix
kronecker(const Matrix& A, const Matrix& B)
{
    Integer i, j, k, l;
    if (A.rows <= 0 || A.column <= 0 || B.rows <= 0 || B.column <= 0 || A.value == nullptr || B.value == nullptr) {
        std::cout << "Generating Error!!!" << std::endl;
        return Matrix(0, 0);
    } else {
        Matrix* result = new Matrix(A.rows * B.rows, A.column * B.column);
        for (l = 0; l < A.rows; l++) {
            for (k = 0; k < B.rows; k++) {
                for (j = 0; j < A.column; j++) {
                    for (i = 0; i < B.column; i++)
                        result->value[(l * B.column + k) * A.column * B.column + j * B.column + i] = A.value[l * A.column + j] * B.value[k * B.column + i];
                }
            }
        }
        return *result;
    }
}

#endif
