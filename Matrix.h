#ifndef __cplusplus

#include <CL/cl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef enum {
    Null,
    operate,
    Multiple,
    eye,
    e,
    transpose
} choice;

#define TIME(str, code) ({                                                                                                       \
    float start = clock();                                                                                                       \
    code;                                                                                                                        \
    float end = clock();                                                                                                         \
    printf("This %s time is: %lf seconds,%lf ms\n", str, (end - start) / CLOCKS_PER_SEC, (end - start) * 1000 / CLOCKS_PER_SEC); \
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

#define CREATE(M, width, height)                                              \
    do {                                                                      \
        if (M != NULL)                                                        \
            freeMatrix(M);                                                    \
        M = (typeof(M))malloc(sizeof(*M));                                    \
        M->v = (typeof(M->v))calloc(width * height, sizeof(typeof(*(M->v)))); \
    } while (0);

#define SET(M)                                                                 \
    do {                                                                       \
        _Number i;                                                             \
        if (M != NULL) {                                                       \
            for (i = 0; i < M->x * M->y; i++) {                                \
                printf("the Matrix[%lld][%lld]=", i / M->y + 1, i % M->y + 1); \
                if (sizeof(M->v[i]) == sizeof(float))                          \
                    scanf("%f", &M->v[i]);                                     \
                else                                                           \
                    scanf("%Lf", &M->v[i]);                                    \
            }                                                                  \
        } else {                                                               \
            println("Creating Error!!!");                                      \
            freeMatrix(M);                                                     \
        }                                                                      \
    } while (0);

/*
#define operate(M,m,operating) ({\
    _Number i;\
    typeof(M) tmp=(typeof(M)) malloc(sizeof(typeof(*M)));\
    tmp->x=M->x;\
    tmp->y=M->y;\
    tmp->v=(typeof(M->v[0])*) calloc(sizeof(typeof(M->v[0])),tmp->x*tmp->y);\
    if((M)!=NULL&&(m)!=NULL&&(M->x)==(m->x)&&(M->y)==(m->y)&&sizeof(M->v[0])==sizeof(m->v[0])) {\
        for(i=0;i<M->x*M->y;i++) {\
            if (m->v[i]==0) tmp->v[i]=M->v[i];\
            else tmp->v[i]=operating>0?M->v[i]+m->v[i]:M->v[i]-m->v[i];\
            }\
        }\
    else {\
        freeMatrix(tmp);\
        println("Operating Error!!!");\
        tmp=NULL;\
    }
    ((M)!=NULL&&(m)!=NULL&&(M->x)==(m->x)&&(M->y)==(m->y)&&sizeof(M->v[0])==sizeof(m->v[0]))?tmp:NULL;\
})
*/

#define operate(M, m, operating) ({                                                                                 \
    _Number i;                                                                                                      \
    typeof(M) tmp = (typeof(M))malloc(sizeof(typeof(*M)));                                                          \
    tmp->x = M->x;                                                                                                  \
    tmp->y = M->y;                                                                                                  \
    tmp->v = (typeof(M->v[0])*)calloc(tmp->x * tmp->y, sizeof(typeof(M->v[0])));                                    \
    if ((M) != NULL && (m) != NULL && (M->x) == (m->x) && (M->y) == (m->y) && sizeof(M->v[0]) == sizeof(m->v[0])) { \
        for (i = 0; i < M->x * M->y; i++) {                                                                         \
            if (m->v[i] == 0)                                                                                       \
                tmp->v[i] = M->v[i];                                                                                \
            else                                                                                                    \
                tmp->v[i] = operating > 0 ? M->v[i] + m->v[i] : M->v[i] - m->v[i];                                  \
        }                                                                                                           \
    } else {                                                                                                        \
        freeMatrix(tmp);                                                                                            \
        println("Operating Error!!!");                                                                              \
        tmp = NULL;                                                                                                 \
    }                                                                                                               \
    tmp;                                                                                                            \
})

#define MULTIPLY(M0, M1) ({                                                                     \
    _Number n, k, h, w;                                                                         \
    typeof(M0->v[0]) tmp;                                                                       \
    typeof(M0) result = (typeof(M0))malloc(sizeof(typeof(*M0)));                                \
    if (M0 != NULL && M1 != NULL && M0->y == M1->x && sizeof(M0->v[0]) == sizeof(M1->v[0])) {   \
        result->x = M0->x;                                                                      \
        result->y = M1->y;                                                                      \
        result->v = (typeof(M0->v[0])*)calloc(result->x * result->y, sizeof(typeof(M0->v[0]))); \
        for (n = 0; n < result->x * result->y; n++) {                                           \
            tmp = 0;                                                                            \
            w = n / result->y;                                                                  \
            h = n - w * result->y;                                                              \
            for (k = 0; k < M0->y; k++)                                                         \
                tmp += M0->v[w * M0->y + k] * M1->v[k * M1->y + h];                             \
            result->v[w * result->y + h] = tmp;                                                 \
        }                                                                                       \
    } else {                                                                                    \
        freeMatrix(result);                                                                     \
        println("Generating Error!!!");                                                         \
        result = NULL;                                                                          \
    }                                                                                           \
    result;                                                                                     \
})

#define EYE(M, opt)                                   \
    ({                                                \
        _Number i;                                    \
        typeof(M) result = NULL;                      \
        if (M->x == M->y) {                           \
            if (opt == 0) {                           \
                for (i = 0; i < M->x * M->y; i++) {   \
                    if (i / M->y + 1 == i % M->y + 1) \
                        M->v[i] = 1;                  \
                    else                              \
                        M->v[i] = 0;                  \
                }                                     \
            } else {                                  \
                CREATE(result, M->x, M->y);           \
                for (i = 0; i < M->x * M->y; i++) {   \
                    if (i / M->y + 1 == i % M->y + 1) \
                        result->v[i] = 1;             \
                    else                              \
                        result->v[i] = 0;             \
                }                                     \
            }                                         \
        } else {                                      \
            println("Generating Error!!!");           \
            freeMatrix(M);                            \
        }                                             \
        opt == 0 ? NULL : result;                     \
    })

#define e(M, num, opt)                            \
    ({                                            \
        _Number i;                                \
        typeof(M) result = NULL;                  \
        if (M != NULL) {                          \
            if (opt == 0) {                       \
                for (i = 0; i < M->x * M->y; i++) \
                    M->v[i] = num;                \
            } else {                              \
                CREATE(result, M->x, M->y);       \
                for (i = 0; i < M->x * M->y; i++) \
                    M->v[i] = num;                \
            }                                     \
        } else {                                  \
            println("Generating Error!!!");       \
            freeMatrix(M);                        \
        }                                         \
        opt == 0 ? NULL : result;                 \
    })

#define t(M, opt)                                                                              \
    ({                                                                                         \
        _Number i, h, w;                                                                       \
        typeof(M) result = NULL;                                                               \
        typeof(M->v[0])* tmp = (typeof(M->v[0])*)calloc(M->x * M->y, sizeof(typeof(M->v[0]))); \
        if (M != NULL) {                                                                       \
            for (i = 0; i < M->x * M->y; i++) {                                                \
                w = i / M->y;                                                                  \
                h = i - M->y * w;                                                              \
                tmp[h * M->x + w] = M->v[w * M->y + h];                                        \
            }                                                                                  \
            if (opt == 0) {                                                                    \
                free(M->v);                                                                    \
                M->v = tmp;                                                                    \
                M->x = M->x ^ M->y;                                                            \
                M->y = M->y ^ M->x;                                                            \
                M->x = M->x ^ M->y;                                                            \
            } else {                                                                           \
                result = (typeof(M))malloc(sizeof(typeof(*M)));                                \
                result->x = M->y, result->y = M->x;                                            \
                result->v = tmp;                                                               \
            }                                                                                  \
        } else {                                                                               \
            println("Transforming Error!!!");                                                  \
            freeMatrix(M);                                                                     \
        }                                                                                      \
        opt == 0 ? NULL : result;                                                              \
    })

#define IMPORT(M, value, num)            \
    do {                                 \
        _Number i;                       \
        if (M->x * M->y == num) {        \
            for (i = 0; i < num; i++)    \
                M->v[i] = value[i];      \
        } else {                         \
            println("setting error!!!"); \
        }                                \
    } while (0);

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

void Set(Matrix* M)
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
}

Matrix*
Import(Matrix* M, const _M_value* value, const _Number num)
{
    _Number i;
    typeof(M) out = CREATE1(M->x, M->y);
    if (M->x * M->y == num) {
        for (i = 0; i < num; i++)
            out->v[i] = value[i];
    } else {
        println("setting error!!!");
        freeMatrix(out);
        out = NULL;
    }
    return out;
}

Matrix*
Eye(Matrix* M)
{
    _Number i;
    typeof(M) out = CREATE1(M->x, M->y);
    if (M->x == M->y) {
        for (i = 0; i < M->x * M->y; i++) {
            if (i / M->y + 1 == i % M->y + 1)
                out->v[i] = 1;
            else
                out->v[i] = 0;
        }
        return out;
    } else {
        println("Generating Error!!!");
        freeMatrix(out);
        return NULL;
    }
}

Matrix*
E(Matrix* M, const _M_value target)
{
    _Number i;
    typeof(M) out = CREATE1(M->x, M->y);
    if (M != NULL) {
        for (i = 0; i < M->x * M->y; i++)
            out->v[i] = target;
    } else {
        println("Generating Error!!!");
        freeMatrix(out);
        out = NULL;
    }
    return out;
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

Matrix*
T(Matrix* M)
{
    _Number i, h, w;
    typeof(M) out = CREATE1(M->x, M->y);
    if (M != NULL) {
        _M_value* tmp = (_M_value*)malloc(sizeof(_M_value) * M->x * M->y);
        for (i = 0; i < M->x * M->y; i++) {
            w = i / M->y;
            h = i - M->y * w;
            tmp[h * M->x + w] = M->v[w * M->y + h];
        }
        memcpy(out->v, tmp, sizeof(_M_value) * M->x * M->y);
        free(tmp);
        out->x = out->x ^ out->y;
        out->y = out->y ^ out->x;
        out->x = out->x ^ out->y;
    } else {
        println("Transforming Error!!!");
        freeMatrix(out);
        out = NULL;
    }
    return out;
}

Matrix* Multiple(const Matrix* M0, const Matrix* M1) {
    if((M0->x <=2 || M0->y <=2)&&(M1->x <=2 || M1->y <=2))
        return multiple(M0,M1);
    else {
        //Matrix* t0=Create(M0->x/2,M0->y/2);
        if(M0->x==1 || M0->y==1) {
            
        }
    }
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

void Set0(S_Matrix* M)
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

#define check(input, str)                                    \
    ({                                                       \
        if (input != CL_SUCCESS) {                           \
            printf("%s is err,the num is:%d\n", str, input); \
        }                                                    \
    })

typedef struct
{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue cmd_queue;
    cl_program program;
    cl_int* cl_err;
    cl_ulong* info[2];
    size_t *global_size, *local_size, N;
} cl_group;

cl_group*
format_group(const char* cl_filename)
{

    cl_group* c0 = (cl_group*)malloc(sizeof(cl_group));
    c0->cl_err = (cl_int*)calloc(1, sizeof(cl_int));

    *c0->cl_err = clGetPlatformIDs(1, &(c0->platform), NULL);
    *c0->cl_err = clGetDeviceIDs(c0->platform, CL_DEVICE_TYPE_ALL, 1, &c0->device, NULL);

    c0->info[0] = (cl_ulong*)malloc(sizeof(cl_ulong));
    c0->info[1] = (cl_ulong*)calloc(3, sizeof(cl_ulong));

    *c0->cl_err = clGetDeviceInfo(c0->device,
        CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
        sizeof(cl_ulong),
        c0->info[0],
        NULL);
    *c0->cl_err = clGetDeviceInfo(c0->device,
        CL_DEVICE_MAX_WORK_ITEM_SIZES,
        sizeof(cl_ulong),
        c0->info[1],
        NULL);

    if (c0->info[1][0] == 0) {
        println("your device dosen't support match CL_DEVICE_MAX_WORK_ITEM_SIZES");
        cl_ulong data;
        *c0->cl_err = clGetDeviceInfo(c0->device,
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            sizeof(cl_ulong),
            &data,
            NULL);
        for (int i = 0; i < 3; i++)
            c0->info[1][i] = data;
    } else
        printf("size: %ld,%ld,%ld\n", c0->info[1][0], c0->info[1][1], c0->info[1][2]);

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
    free(target->info[0]);
    free(target->info[1]);
    free(target);
    println("release object success!!!");
    return 0;
}

#define out(L, G, num)                                                       \
    do {                                                                     \
        short c = _Generic((L),                                              \
            S_Matrix * : 1,                                                  \
            default : 0);                                                    \
        if (c) {                                                             \
            if ((L)->x * (L)->y * sizeof(typeof((L)->v[0])) > *G->info[0]) { \
                println("buffer out");                                       \
                cl_clearmem(G);                                              \
                return NULL;                                                 \
            }                                                                \
        } else {                                                             \
            if (num * sizeof(typeof(L[0])) > *G->info[0]) {                  \
                println("buffer out");                                       \
                cl_clearmem(G);                                              \
                return NULL;                                                 \
            }                                                                \
        }                                                                    \
    } while (0);

const char* build_binary_kernel(const char* path, const char* filename)
{
    cl_group* g0 = format_group(path);
    size_t binsizes;
    clGetProgramInfo(g0->program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binsizes, NULL);
    unsigned char* binary = (unsigned char*)malloc(binsizes);
    clGetProgramInfo(g0->program, CL_PROGRAM_BINARIES, sizeof(unsigned char*), &binary, NULL);
    FILE* file = fopen(filename, "wb");
    fwrite(binary, 1, binsizes, file);
    fclose(file);
    return filename;
}

S_Matrix*
calculate(S_Matrix* a, S_Matrix* b, choice selection, char option, float tar)
{
    cl_group* g0 = format_group("M.cl");
    if (selection == Null) {
        println("please input the type of your selection:");
        scanf("%d", &selection);
    }

    while (selection != operate && selection != Multiple && selection != eye && selection != e && selection != transpose) {
        println("value is invaild,please input true type of your selection:");
        scanf("%d", &selection);
    }
    switch (selection) {
    case operate: {
        S_Matrix* result = Create0(a->x, b->y);
        free(result->v);

        if (option == -1) {
            println("please input the operate option:");
            scanf("%c", &option);
        }

        cl_kernel kernel = clCreateKernel(g0->program, "operate", g0->cl_err);

        cl_mem v0, v1, rlt;

        v0 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            a->x * a->y * sizeof(float),
            a->v,
            g0->cl_err);
        out(a, g0, 0);
        check(*g0->cl_err, "buffer create");

        v1 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            b->x * b->y * sizeof(float),
            b->v,
            g0->cl_err);
        out(b, g0, 0);

        rlt = clCreateBuffer(g0->context,
            CL_MEM_WRITE_ONLY,
            result->x * result->y * sizeof(float),
            NULL,
            g0->cl_err);
        out(result, g0, 0);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &v0);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &v1);
        *g0->cl_err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &rlt);
        *g0->cl_err = clSetKernelArg(kernel, 3, sizeof(char), &option);

        g0->global_size = (size_t*)malloc(1 * sizeof(size_t));
        g0->local_size = (size_t*)malloc(1 * sizeof(size_t));

        *g0->global_size = a->x * a->y;
        *g0->local_size = a->x;

        if (*g0->global_size > g0->info[1][0] * g0->info[1][1] * g0->info[1][2]) {
            println("kernel alloc location error");
            cl_clearmem(g0);
            return NULL;
        }

        result->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, rlt, CL_TRUE, CL_MAP_WRITE, 0, result->x * result->y * sizeof(float), 0, NULL, NULL, g0->cl_err);
        check(*g0->cl_err, "memory map");

        TIME("OpenCL vector operating", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 1, NULL, g0->global_size, g0->local_size, 0, NULL, NULL)));
        check(*g0->cl_err, "kernel execute");

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
        out(a, g0, 0);

        Matrix1 = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            b->x * b->y * sizeof(float),
            b->v,
            g0->cl_err);
        out(b, g0, 0);

        result = clCreateBuffer(g0->context,
            CL_MEM_WRITE_ONLY,
            output->x * output->y * sizeof(float),
            NULL,
            g0->cl_err);
        out(output, g0, 0);

        information = clCreateBuffer(
            g0->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 2 * sizeof(long), h, g0->cl_err);
        // out(h,g0,2);

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
        if (g0->global_size[0] * g0->global_size[1] > g0->info[1][0] * g0->info[1][1] * g0->info[1][2]) {
            println("kernel alloc location error");
            cl_clearmem(g0);
            return NULL;
        }

        TIME("OpenCL matrix multiply", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 2, NULL, g0->global_size, NULL, 0, NULL, NULL)));
        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, result, output->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return output;
        break;
    }
    case eye: {
        S_Matrix* output = Create0(a->x, a->y);
        free(output->v);
        unsigned long num = output->x;

        if (tar == 0) {
            println("please input the target value:");
            scanf("%f", &tar);
        }

        cl_kernel kernel = clCreateKernel(g0->program, "eye", g0->cl_err);

        cl_mem value;
        value = clCreateBuffer(g0->context,
            CL_MEM_READ_WRITE,
            output->x * output->y * sizeof(float),
            NULL,
            g0->cl_err);
        out(output, g0, 0);

        //*g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,v,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);
        output->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, value, CL_TRUE, CL_MAP_WRITE, 0, output->x * output->y * sizeof(float), 0, NULL, NULL, g0->cl_err);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &value);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(unsigned long), &num);
        *g0->cl_err = clSetKernelArg(kernel, 2, sizeof(float), &tar);

        g0->global_size = (size_t*)malloc(2 * sizeof(size_t));

        g0->global_size[0] = output->x, g0->global_size[1] = output->y;
        if (g0->global_size[0] * g0->global_size[1] > g0->info[1][0] * g0->info[1][1] * g0->info[1][2]) {
            println("kernel alloc location error");
            cl_clearmem(g0);
            return NULL;
        }

        TIME("OpenCL identitial matrix setting", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 2, NULL, g0->global_size, NULL, 0, NULL, NULL)));

        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, value, output->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return a->x == a->y ? output : NULL;

        break;
    }
    case e: {
        S_Matrix* out = Create0(a->x, a->y);
        free(out->v);

        if (tar == 0) {
            println("please input the target value:");
            scanf("%f", &tar);
        }

        cl_kernel kernel = clCreateKernel(g0->program, "e", g0->cl_err);

        cl_mem data;
        data = clCreateBuffer(g0->context,
            CL_MEM_READ_WRITE,
            out->x * out->y * sizeof(float),
            NULL,
            g0->cl_err);
        out(out, g0, 0);

        out->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, data, CL_TRUE, CL_MAP_WRITE, 0, out->x * out->y * sizeof(float), 0, NULL, NULL, g0->cl_err);

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &data);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(float), &tar);

        g0->global_size = (size_t*)malloc(sizeof(size_t));
        *g0->global_size = out->x * out->y;
        if (*g0->global_size > g0->info[1][0] * g0->info[1][1] * g0->info[1][2]) {
            println("kernel alloc location error");
            cl_clearmem(g0);
            return NULL;
        }

        TIME("OpenCL matrix setting", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 1, NULL, g0->global_size, NULL, 0, NULL, NULL)));

        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, data, out->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return out;

        break;
    }
    case transpose: {
        unsigned long info[2] = { a->x, a->y };

        S_Matrix* result = Create0(a->y, a->x);
        free(result->v);

        cl_kernel kernel = clCreateKernel(g0->program, "T", g0->cl_err);

        cl_mem A, B, C;

        A = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            a->x * a->y * sizeof(float),
            a->v,
            g0->cl_err);
        out(a, g0, 0);

        B = clCreateBuffer(g0->context,
            CL_MEM_READ_WRITE,
            result->x * result->y * sizeof(float),
            NULL,
            g0->cl_err);
        out(b, g0, 0);

        C = clCreateBuffer(g0->context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            2 * sizeof(unsigned long),
            info,
            g0->cl_err);

        result->v = (float*)clEnqueueMapBuffer(g0->cmd_queue, B, CL_TRUE, CL_MAP_WRITE, 0, result->x * result->y * sizeof(float), 0, NULL, NULL, g0->cl_err);
        check(*g0->cl_err, "memory map");

        *g0->cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &A);
        *g0->cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &B);
        *g0->cl_err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &C);

        g0->global_size = (size_t*)malloc(2 * sizeof(size_t));
        g0->global_size[0] = a->x, g0->global_size[1] = a->y;
        if (g0->global_size[0] * g0->global_size[1] > g0->info[1][0] * g0->info[1][1] * g0->info[1][2]) {
            println("kernel alloc location error");
            cl_clearmem(g0);
            return NULL;
        }

        TIME("OpenCL matrix setting", (*g0->cl_err = clEnqueueNDRangeKernel(g0->cmd_queue, kernel, 2, NULL, g0->global_size, NULL, 0, NULL, NULL)));
        check(*g0->cl_err, "kernel execute");

        *g0->cl_err = clEnqueueUnmapMemObject(g0->cmd_queue, B, result->v, 0, NULL, NULL);

        cl_clearmem(g0);

        return result;

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

#else

#include <CL/cl2.hpp>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <new>
#include <vector>

#define TIME(str, code) ({                                                                                  \
    auto start                                                                                              \
        = std::chrono::high_resolution_clock::now();                                                        \
    code;                                                                                                   \
    auto end = std::chrono::high_resolution_clock::now();                                                   \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);                     \
    auto d1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);                           \
    std::cout << "this " << str " Execution time is: " << duration.count() << " milliseconds" << std::endl; \
    std::cout << "this " << str " Execution time is: " << d1.count() << " microseconds" << std::endl;       \
    duration.count();                                                                                       \
})

typedef unsigned long long Integer;
typedef long double Double;

template <typename T>
void* operator new[](size_t size, T)
{
    return size == 0 ? nullptr : std::calloc(size, sizeof(T));
};

template <typename T>
void* operator new(size_t size, T)
{
    return size == 0 ? nullptr : std::calloc(size, sizeof(T));
};

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
        this->value = new Double[rows * column];
        std::memcpy(this->value, M.value, sizeof(Double) * rows * column);
    }

    ~Matrix()
    {
        rows = 0, column = 0;
        delete[] value;
    }

    template <class C>
    C size() const
    {
        return rows * column;
    }

    template <class Type>
    void set(const Type& num);

    template <typename U>
    void set(const U* data);

    template <class N>
    N info(bool b) const;

    template <class S>
    friend S* out(const Matrix& M);
    // friend decltype(S*) out(const Matrix& M);
    // friend auto out(const Matrix& M); c++14

    void set();

    void set(const Integer& x, const Integer& y);

    void info() const;

    Integer info(void* v) const;
    void* address() const;
    Matrix operator+(const Matrix& M);
    Matrix operator-(const Matrix& M);
    Matrix operator*(const Matrix& M);
    Matrix& operator=(const Matrix& M);
    bool operator==(const Matrix& M);
    Double& operator()(Integer x0, Integer y0) const;
    friend void display(const Matrix& M);
    friend void* address(const Matrix& M);
    friend Matrix T(const Matrix& M);
    friend Matrix& Eye(const Integer& num);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& M);
    friend std::istream& operator>>(std::istream& is, Matrix& M);
    friend Matrix kronecker(const Matrix& A, const Matrix& B);
    template <class O>
    friend O Value(const Matrix& M);
};

void Matrix::set()
{
    for (Integer i = 0; i < rows * column; i++) {
        std::cout << "please input the Matrix[" << i / column + 1 << "]"
                  << "[" << i - (i / column) * column + 1 << "]" << std::endl;
        std::cin >> value[i];
    }
}

template <class Type>
void Matrix::set(const Type& num)
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

template <typename U>
void Matrix::set(const U* data)
{
    for (Integer i = 0; i < rows * column; i++)
        value[i] = data[i];
}

void Matrix::info() const
{
    std::cout << "the row is:" << this->rows << "\n"
              << "the column is:" << this->column << std::endl;
}

Integer Matrix::info(void* v) const
{
    return v == nullptr ? rows : column;
}

template <class N>
N Matrix::info(bool b) const
{
    return N(b == false ? rows : column);
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

void* Matrix::address() const
{
    return (void*)this;
}

void* address(const Matrix& M)
{
    return M.value == nullptr ? nullptr : M.value;
}

Double&
Matrix::operator()(Integer x0, Integer y0) const
{
    if (x0 > rows || y0 > column) {
        std::cout << "error" << std::endl;
        Double tmp = 0;
        return tmp;
    } else {
        std::cout << "the Matrix[" << x0 << "]"
                  << "[" << y0 << "]is:" << value[(x0 - 1) * column + (y0 - 1)]
                  << std::endl;
        return value[(x0 - 1) * column + (y0 - 1)];
    }
}

template <class O>
O Value(const Matrix& M)
{
    if (M.rows != M.column)
        return 0;
    else {
        if (M.rows == 2)
            return M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1);
        else if (M.rows == 3)
            return M(1, 1) * M(2, 2) * M(3, 3) + M(1, 2) * M(2, 3) * M(3, 1) + M(1, 3) * M(2, 1) * M(3, 2) - M(1, 3) * M(2, 2) * M(3, 1) - M(1, 2) * M(2, 1) * M(3, 3) - M(1, 1) * M(2, 3) * M(3, 2);
        else
            return 0;
    }
}

Matrix&
Matrix::operator=(const Matrix& M)
{
    if (this != &M) {
        this->rows = M.rows;
        this->column = M.column;
        delete[] value; // Release previous memory
        this->value = new Double[rows * column];
        std::memcpy(this->value, M.value, sizeof(Double) * rows * column);
    }
    return *this;
}

bool Matrix::operator==(const Matrix& M)
{
    if (M.rows != rows || M.column != column)
        return false;
    else {
        for (int i = 0; i < rows * column; i++) {
            if (M.value[i] == value[i])
                continue;
            else
                return false;
        }
        return true;
    }
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

template <class S>
S* out(const Matrix& M)
{
    if (M.value != nullptr) {
        S* data = new S[M.rows * M.column];
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

// support OpenCL C++

inline std::string f_to_string(const std::string& path)
{
    std::ifstream input(path);
    std::string Content, tmp;
    if (input.is_open()) {
        while (std::getline(input, tmp)) {
            Content += tmp + "\n";
        }
        input.close();
        std::cout << "read success!!" << std::endl;
    } else {
        std::cout << "read err!!" << std::endl;
    }
    return Content;
}

inline bool checkExtensionSupport(const cl::Device& device, const std::string& extensionName)
{
    std::string extensions = device.getInfo<CL_DEVICE_EXTENSIONS>();
    return (extensions.find(extensionName) != std::string::npos);
}

inline cl::Program build_program(const cl::Context& context, const cl::Device& device, const std::string& sourceFile)
{
    cl::Program program = cl::Program(context, f_to_string(sourceFile));
    std::string options = "-cl-std=CL2.0";
    if (checkExtensionSupport(device, "cl_khr_fp64"))
        options += " -DENABLE_DOUBLE_PRECISION";
    cl_int err = program.build({ device }, options.c_str());
    if (err != CL_SUCCESS)
        std::cerr << "Failed to build the program: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;

    return program;
}

inline void check(int input, const std::string& info)
{
    if (input != CL_SUCCESS)
        std::cout << info << "error" << std::endl;
}

class cl_group {
public:
    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;
    cl::Platform platform;
    cl::Device device;
    cl::Context context;
    cl::CommandQueue cmdqueue;
    cl::Program program;
    cl_int* clerr;
    cl_group(const std::string& path)
    {
        clerr = new cl_int;
        cl::Platform::get(&platforms);
        platform = platforms[0];

        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        device = devices[0];

        context = cl::Context(device, nullptr, nullptr, nullptr, clerr);
        check(*clerr, "context make");

        cmdqueue = cl::CommandQueue(context, device, 0, clerr);
        check(*clerr, "cmdqueue build");

        program = build_program(context, device, path);
    }
    ~cl_group()
    {
        delete clerr;
    }
};

Matrix calculate(const Matrix& A = *new Matrix(0, 0), const Matrix& B = *new Matrix(0, 0), char choice = -1, char opt = -1, float target = 0)
{
    cl_group g0("M.cl");

    if (choice == -1) {
        std::cout << "please input your choices partener:" << std::endl;
        std::cin >> choice;
    }

    switch (choice) {
    case 48: {
        if (opt == -1) {
            std::cout << "please input your operation:" << std::endl;
            std::cin >> opt;
        }

        cl::Kernel kernel(g0.program, "operate", g0.clerr);
        check(*g0.clerr, "kernel make");

        cl::Buffer a, b, c;

        a = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * A.size<size_t>(), out<float>(A), g0.clerr);
        check(*g0.clerr, "buffer create");
        b = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * B.size<size_t>(), out<float>(B), g0.clerr);
        check(*g0.clerr, "buffer create");
        c = cl::Buffer(g0.context, CL_MEM_READ_WRITE, sizeof(float) * A.info<size_t>(0) * B.info<size_t>(1), nullptr, g0.clerr);
        check(*g0.clerr, "buffer create");
        // d = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char), &opt, g0.clerr);

        kernel.setArg(0, a);
        kernel.setArg(1, b);
        kernel.setArg(2, c);
        kernel.setArg(3, sizeof(char), &opt);

        // g0.cmdqueue.finish();

        TIME("kernel execute ", (*g0.clerr = g0.cmdqueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(A.size<size_t>()), cl::NDRange(1), nullptr, nullptr)));
        check(*g0.clerr, "kernel execute");

        float* data = (float*)g0.cmdqueue.enqueueMapBuffer(c, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(float) * A.size<size_t>(), nullptr, nullptr, g0.clerr);
        // g0.cmdqueue.enqueueReadBuffer(c,CL_TRUE,0,A.size<size_t>()*sizeof(float),data,nullptr,nullptr);

        g0.cmdqueue.enqueueUnmapMemObject(c, data, nullptr, nullptr);

        Matrix M(A.info<Integer>(0), A.info<Integer>(1));
        M.set<float>(data);

        return (A.info(nullptr) == B.info(nullptr) && A.info(address(M)) == B.info(address(M))) ? M : Matrix(0, 0);
        break;
    }
    case 49: {
        long H[2] = { A.info<long>(1), B.info<long>(1) };

        cl::Kernel kernel(g0.program, "multiple", g0.clerr);

        cl::Buffer X, Y, O, Z;

        X = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * A.size<size_t>(), out<float>(A), g0.clerr);
        check(*g0.clerr, "buffer create");
        Y = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * B.size<size_t>(), out<float>(B), g0.clerr);
        check(*g0.clerr, "buffer create");
        Z = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(long) * 2, H, g0.clerr);
        check(*g0.clerr, "buffer create");
        O = cl::Buffer(g0.context, CL_MEM_READ_WRITE, sizeof(float) * A.info<size_t>(0) * B.info<size_t>(1), nullptr, g0.clerr);
        check(*g0.clerr, "buffer create");

        kernel.setArg(0, X);
        kernel.setArg(1, Y);
        kernel.setArg(2, O);
        kernel.setArg(3, Z);

        TIME("kernel execute", (*g0.clerr = g0.cmdqueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(A.info<size_t>(0), B.info<size_t>(1)), cl::NDRange(1, B.info<size_t>(1)), nullptr, nullptr)));

        check(*g0.clerr, "kernel execute");

        float* data = (float*)g0.cmdqueue.enqueueMapBuffer(O, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(float) * A.info<size_t>(0) * B.info<size_t>(1), nullptr, nullptr, g0.clerr);
        check(*g0.clerr, "data Map");

        *g0.clerr = g0.cmdqueue.enqueueUnmapMemObject(O, data, nullptr, nullptr);
        check(*g0.clerr, "unmap data");

        Matrix M(A.info<Integer>(0), B.info<Integer>(1));
        M.set<float>(data);

        return (A.info(address(M)) == B.info(nullptr)) ? M : Matrix(0, 0);
        break;
    }
    case 50: {
        unsigned long info = A.info<unsigned long>(0);

        cl::Kernel kernel(g0.program, "eye", g0.clerr);

        cl::Buffer x;

        x = cl::Buffer(g0.context, CL_MEM_READ_WRITE, sizeof(float) * A.size<size_t>(), nullptr, g0.clerr);
        check(*g0.clerr, "buffer create");
        // y = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(unsigned long) * 2, info, g0.clerr);

        kernel.setArg(0, x);
        kernel.setArg(1, sizeof(unsigned long), &info);
        kernel.setArg(2, sizeof(float), &target);

        TIME("kernel execute", (*g0.clerr = g0.cmdqueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(A.info<size_t>(0), A.info<size_t>(1)), cl::NDRange(1, 1), nullptr, nullptr)));

        check(*g0.clerr, "kernel execute");

        float* data = (float*)g0.cmdqueue.enqueueMapBuffer(x, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(float) * A.info<size_t>(0) * B.info<size_t>(1), nullptr, nullptr, g0.clerr);
        check(*g0.clerr, "data Map");

        *g0.clerr = g0.cmdqueue.enqueueUnmapMemObject(x, data, nullptr, nullptr);
        check(*g0.clerr, "unmap data");

        Matrix M(A);
        M.set<float>(data);

        return (A.info(nullptr) == A.info(address(A))) ? M : Matrix(0, 0);
        break;
    }
    case 51: {
        cl::Kernel kernel(g0.program, "e", g0.clerr);

        cl::Buffer x;

        x = cl::Buffer(g0.context, CL_MEM_READ_WRITE, sizeof(float) * A.size<size_t>(), nullptr, g0.clerr);
        check(*g0.clerr, "buffer create");
        // y = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float), &target, g0.clerr);

        kernel.setArg(0, x);
        kernel.setArg(1, sizeof(float), &target);

        TIME("kernel execute", (*g0.clerr = g0.cmdqueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(A.size<size_t>()), cl::NDRange(1), nullptr, nullptr)));

        check(*g0.clerr, "kernel execute");

        float* data = (float*)g0.cmdqueue.enqueueMapBuffer(x, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(float) * A.info<size_t>(0) * A.info<size_t>(1), nullptr, nullptr, g0.clerr);
        check(*g0.clerr, "data Map");

        *g0.clerr = g0.cmdqueue.enqueueUnmapMemObject(x, data, nullptr, nullptr);
        check(*g0.clerr, "unmap data");

        Matrix M(A.info(nullptr), A.info(data));
        M.set<float>(data);
        return M;
        break;
    }
    case 52: {
        Matrix result(A.info(&g0), A.info(nullptr));

        unsigned long l[2] = { A.info<unsigned long>(0), A.info<unsigned long>(1) };

        cl::Kernel kernel(g0.program, "T", g0.clerr);

        cl::Buffer O, P, Q;

        O = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * A.size<size_t>(), out<float>(A), g0.clerr);
        P = cl::Buffer(g0.context, CL_MEM_READ_WRITE, sizeof(float) * result.size<size_t>(), nullptr, g0.clerr);
        Q = cl::Buffer(g0.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(unsigned long) * 2, l, g0.clerr);

        kernel.setArg(0, O);
        kernel.setArg(1, P);
        kernel.setArg(2, Q);

        TIME("kernel execute", (*g0.clerr = g0.cmdqueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(A.info<size_t>(0), A.info<size_t>(1)), cl::NDRange(1, 1), nullptr, nullptr)));

        float* data = (float*)g0.cmdqueue.enqueueMapBuffer(P, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(float) * result.info<size_t>(0) * result.info<size_t>(1), nullptr, nullptr, g0.clerr);

        *g0.clerr = g0.cmdqueue.enqueueUnmapMemObject(P, data, nullptr, nullptr);

        result.set<float>(data);
        return result;
        break;
    }
    default: {
        return Matrix(0, 0);
        break;
    }
    }
}

inline Matrix calculate(Matrix& M, char choices)
{
    return calculate(M, Matrix(0, 0), choices);
}

inline Matrix calculate(Matrix& M, char choices, float F)
{
    return calculate(M, Matrix(0, 0), choices, -1, F);
}

#ifdef OPERATE_RELOAD_OPENCL

Matrix Matrix::operator*(const Matrix& B)
{
    return calculate(*this, B, 49);
}

Matrix Matrix::operator+(const Matrix& B)
{
    return calculate(*this, B, 48, 48);
}

Matrix Matrix::operator-(const Matrix& B)
{
    return calculate(*this, B, 48, 49);
}

Matrix& Eye(const Integer I)
{
    Matrix A(I);
    A = calculate(A, 50);
    return A;
}

Matrix T(const Matrix& M)
{
    return calculate(M, 52, -1);
}

#else

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
Eye(const Integer& num)
{
    Matrix m0(num);
    for (Integer i = 0; i < num * num; i++)
        m0.value[i] = i / num == i - (i / num) * num ? 1 : 0;
    return m0;
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

#endif

#endif
