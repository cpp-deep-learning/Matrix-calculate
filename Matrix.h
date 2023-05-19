#ifdef __cplusplus
extern "C" {
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<CL/cl.h>
#include<time.h>

#ifndef MATRIX
	typedef long double _M_value;
	typedef unsigned long long _Number;
	#define MATRIX
	//#define println(str) free((void *)printf("%s\n", str))
	#define println(str) do{ printf("%s\n", str); } while(0)
#endif

	
//this struct Matrix is used CPU Calculate
typedef struct {
	_Number x,y;
	_M_value* v;
} Matrix;

	
//this struct S_Matrix is used GPU(OpenCL) Calculate
typedef struct {
	_Number x,y;
	float* v;
} S_Matrix;

//there are some macro,you can use it like function
	
//this macro freeMatrix can Release the Matrix's memory
#define freeMatrix(M) do {\
  free(M->v);\
  free(M);\
} while(0)

	
//this macro length can return char*(string) length
#define length(s) ({\
	printf("the string  ");\
	int i=printf("%s",s);\
	printf("  's length is:%d\n",i);\
	i;\
})
	
//this macro can return the end of file's location
#define end(file) ({\
	int locate=0;\
	while(getc(file)!=EOF) {\
		locate++;\
		fseek(file,1,locate);\
	}\
	locate;\
})

//this macro can create an new Matrix 
#define CREATE0() ({\
	Matrix* tmp=(Matrix*) malloc(sizeof(Matrix));\
	printf("please input the Matrix's row:");\
	scanf("%lld",&tmp->x);\
	printf("please input the Matrix's column:");\
	scanf("%lld",&tmp->y);\
	tmp->v=(_M_value*) malloc(sizeof(_M_value)*tmp->x*tmp->y);\
	tmp;\
})

//this macro can create an new Matrix
#define CREATE1(width,height) ({\
	Matrix* target=(Matrix*) malloc(sizeof(Matrix));\
	target->x=width,target->y=height;\
	target->v=(_M_value*) malloc(sizeof(_M_value)*width*height);\
	target;\
})

	
//this macro can setting the new Matrix
#define SET(M) ({\
	_Number i;\
	if(M!=NULL) {\
		for(i=0;i<M->x*M->y;i++) {\
			printf("the Matrix[%lld][%lld]=",i/M->y+1,i%M->y+1);\
			scanf("%Lf",&M->v[i]);\
		}\
	}\
	else {\
		println("Creating Error!!!");\
		freeMatrix(M);\
	}\
	M;\
})

//the macro has bug,please keep ceriosity to use it
#define EYE(M) ({\
_Number i;\
	if(M->x==M->y) {\
		for(i=0;i<M->x*M->y;i++) {\
			if(i/M->y+1==i%M->y+1) M->v[i]=1;\
			else M->v[i]=0;\
		}\
		M;\
	}\
	else {\
		println("Generating Error!!!");\
		freeMatrix(M);\
		NULL;\
	}\
})

	
//this macro can change all the Matrix's value to num
#define e(M,num) ({\
	_Number i;\
	if(M!=NULL) {\
		for(i=0;i<M->x*M->y;i++)\
			M->v[i]=num;\
	}\
	else {\
		println("Generating Error!!!");\
		freeMatrix(M);\
	}\
	M;\
})

	
//this macro can transpose the Matrix
#define t(M) ({\
	_Number i,h,w;\
	if(M!=NULL) {\
		if(sizeof(M->v[0])==sizeof(float)) float* tmp=(float*) malloc(sizeof(float)*M->x*M->y);\
		else _M_value* tmp=(_M_value*) malloc(sizeof(_M_value)*M->x*M->y);\
		for(i=0;i<M->x*M->y;i++) {\
			w=i/M->y;\
			h=i-M->y*w;\
			tmp[h*M->x+w]=M->v[w*M->y+h];\
		}\
		M->v=tmp;\
		M->x=M->x^M->y;\
		M->y=M->y^M->x;\
		M->x=M->x^M->y;\
	}\
	else {\
		println("Transforming Error!!!");\
		freeMatrix(M);\
	}\
	M;\
})

	
//this macro can import data from value to Matrix M
#define IMPORT(M,value,num) ({\
	_Number i;\
	if(M->x*M->y==num) {\
		for(i=0;i<num;i++) M->v[i]=value[i];\
	}\
	else {\
		println("setting error!!!");\
	}\
	M;\
})

	
//this macro can display the Matrix
#define  DISPLAY(M) do {\
	_Number i;\
	if(M!=NULL) {\
		println("");\
		for(i=0;i<M->x*M->y;i++) {\
			if(sizeof(M->v[0])==sizeof(float)) printf("%24.12f	",M->v[i]);\
			else printf("%24.12Lf	",M->v[i]);\
			if((i+1)%M->y==0) printf("\n");\
		}\
		println("");\
	}\
	else {\
		println("print Matrix Error!!!");\
		freeMatrix(M);\
	}\
} while(0)

	
//this function can create an new Matrix
Matrix* create() {
	Matrix* tmp=(Matrix*) malloc(sizeof(Matrix));
	printf("please input the Matrix's row:");
	scanf("%lld",&tmp->x);
	printf("please input the Matrix's column:");
	scanf("%lld",&tmp->y);
	tmp->v=(_M_value*) malloc(sizeof(_M_value)*tmp->x*tmp->y);
	return tmp;
}

//this function can create an new Matrix
Matrix* Create(_Number width,_Number height) {
	Matrix* target=(Matrix*) malloc(sizeof(Matrix));
	target->x=width,target->y=height;
	target->v=(_M_value*) malloc(sizeof(_M_value)*width*height);
	return target;
}

	
//this function can Set the new Matrix's value
Matrix* Set(Matrix* M) {
	_Number i;
	if(M!=NULL) {
		for(i=0;i<M->x*M->y;i++) {
			printf("the Matrix[%lld][%lld]=",i/M->y+1,i%M->y+1);
			scanf("%Lf",&M->v[i]);
		}
	}
	else {
		println("Creating Error!!!");
		freeMatrix(M);
	}
	return M;
}

//this function can import data from value to Matrix M
Matrix* Import(Matrix* M,const _M_value* value,const _Number num) {
	_Number i;
	if(M->x*M->y==num) {
		for(i=0;i<num;i++) M->v[i]=value[i];
	}
	else {
		println("setting error!!!");
	}
	return M;
}

//this function can format identity Matrix
Matrix* Eye(Matrix* M) {
	_Number i;
	if(M->x==M->y) {
		for(i=0;i<M->x*M->y;i++) {
			if(i/M->y+1==i%M->y+1) M->v[i]=1;
			else M->v[i]=0;
		}
		return M;
	}
	else {
		println("Generating Error!!!");
		freeMatrix(M);
		return NULL;
	}
}

	
//this function can change all the Matrix's value to target
Matrix* E(Matrix* M,const _M_value target) {
	_Number i;
	if(M!=NULL) {
		for(i=0;i<M->x*M->y;i++)
			M->v[i]=target;
	}
	else {
		println("Generating Error!!!");
		freeMatrix(M);
	}
	return M;
}

	
//this function can add two Matrix
Matrix* add(const Matrix* M,const Matrix* m) {
	_Number i;
	if(M!=NULL&&m!=NULL&&(M->x)==(m->x)&&(M->y)==(m->y)) {
		Matrix* tmp=CREATE1(M->x,M->y);
		for(i=0;i<M->x*M->y;i++) {
			if (m->v[i]==0) tmp->v[i]=M->v[i];
			else tmp->v[i]=M->v[i]+m->v[i];
		}
		return tmp;
	}
	else return NULL;
}

	
//this function can return two Matrix subtractioned result
Matrix* subtraction(const Matrix* M0,const Matrix* M1) {
	_Number i;
	if(M0!=NULL&&M1!=NULL&&M0->x==M1->x&&M0->y==M1->y) {
		Matrix* tmp=CREATE1(M0->x,M0->y);
		for(i=0;i<M0->x*M0->y;i++) {
			if (M1->v[i]==0) tmp->v[i]=M0->v[i];
			else tmp->v[i]=M0->v[i]-M1->v[i];
		}
		return tmp;
	}
	else return NULL;
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
			else tmp->v[i]=operating>0?M->v[i]+m->v[i]:M->v[i]-m->v[i];\
		}\
		tmp;\
	}\
	else {\
		NULL;\
	}\
})
*/

//this function can transpose the Matrix
Matrix* T(Matrix* M) {
	_Number i,h,w;
	if(M!=NULL) {
		_M_value* tmp=(_M_value*) malloc(sizeof(_M_value)*M->x*M->y);
		for(i=0;i<M->x*M->y;i++) {
			w=i/M->y;
			h=i-M->y*w;
			tmp[h*M->x+w]=M->v[w*M->y+h];
		}
		M->v=tmp;
		M->x=M->x^M->y;
		M->y=M->y^M->x;
		M->x=M->x^M->y;
	}
	else {
		println("Transforming Error!!!");
		freeMatrix(M);
	}
	return M;
}

	
//this function can multiply two Matrix
Matrix* multiple(const Matrix* M0,const Matrix* M1) {
	_Number n,k,h,w;
	_M_value tmp;
	if(M0!=NULL&&M1!=NULL&&M0->y==M1->x) {
		Matrix* result=Create(M0->x,M1->y);
		for(n=0;n<result->x*result->y;n++) {
			tmp=0;
			w=n/result->y;
			h=n-w*result->y;
			for(k=0;k<M0->y;k++) tmp+=M0->v[w*M0->y+k]*M1->v[k*M1->y+h];
			result->v[w*result->y+h]=tmp;
		} 
		return result;
	}
	else {
		println("Generating Error!!!");
		return NULL;
	}
}

/*
#define MULTIPLE(M0,M1) ({\
	_Number n,k,h,w;\
	_M_value tmp;\
	if(M0!=NULL&&M1!=NULL&&M0->y==M1->x) {\
		Matrix* result=(Matrix*) malloc(sizeof(Matrix));\
		result->x=M0->x;\
		result->y=M1->y;\
		result->v=(_M_value*) malloc(sizeof(_M_value)*result->x*result->y);\
		for(n=0;n<result->x*result->y;n++) {\
			tmp=0;\
			w=n/result->y;\
			h=n-w*result->y;\
			for(k=0;k<M0->y;k++) tmp+=M0->v[w*M0->y+k]*M1->v[k*M1->y+h];\
			result->v[w*result->y+h]=tmp;\
		}\
		(Matrix*)result;\
	}\
	else {\
		println("Generating Error!!!");\
		(Matrix*)NULL;\
	}\
})
*/

	
//the function can export data to the return value
_M_value* Export(const Matrix* M) {
	_Number i;
	if(M!=NULL&&M->v!=NULL) {
		_M_value* value=(_M_value*)malloc(sizeof(_M_value)*M->x*M->y);
		for(i=0;i<M->x*M->y;i++) value[i]=M->v[i];
		return value;
	}
	else {
		println("output error!!!");
		return NULL;
	}
}

/*
void display(Matrix* M) {
	_Number i;
	if(M!=NULL) {
		println("");
		for(i=0;i<M->x*M->y;i++) {
			printf("%24.12Lf	",M->v[i]);
			if((i+1)%M->y==0) printf("\n");
		}
		println("");
	}
	else {
		println("print Matrix Error!!!");
		freeMatrix(M);
	}
}
*/

	
//this function can get a file's String and save it in return value
char* filestring(const char* filename) {
  FILE* f0=fopen(filename,"r+");
  long location=0;
  fseek(f0,0,SEEK_END);
  char* tmp=(char*)calloc(ftell(f0)+1,sizeof(char));
  fseek(f0,0,SEEK_SET);
  while(fgetc(f0)!=EOF) {
    fseek(f0,location,SEEK_SET);
    tmp[location]=fgetc(f0);
    location++;
  }
  printf("%s\n",tmp);
  fclose(f0);
  return tmp;
}

	
//this function can create new Matrix to use OpenCL calculate
S_Matrix* create0() {
	S_Matrix* tmp=(S_Matrix*) malloc(sizeof(S_Matrix));
	printf("please input the Matrix's row:");
	scanf("%lld",&tmp->x);
	printf("please input the Matrix's column:");
	scanf("%lld",&tmp->y);
	tmp->v=(float*) malloc(sizeof(float)*tmp->x*tmp->y);
	return tmp;
}

//this function can create new Matrix to use OpenCL calculate
S_Matrix* Create0(_Number width,_Number height) {
	S_Matrix* target=(S_Matrix*) malloc(sizeof(S_Matrix));
	target->x=width,target->y=height;
	target->v=(float*) malloc(sizeof(float)*width*height);
	return target;
}

	
//this function can set the Matrix value to use OpenCL calculate
S_Matrix* Set0(S_Matrix* M) {
	_Number i;
	if(M!=NULL) {
		for(i=0;i<M->x*M->y;i++) {
			printf("the Matrix[%lld][%lld]=",i/M->y+1,i%M->y+1);
			scanf("%f",&M->v[i]);
		}
	}
	else {
		println("Creating Error!!!");
		freeMatrix(M);
	}
	return M;
}

//this function can make kronecker product with Matrix A and B
Matrix* kronecker(Matrix* A,Matrix* B) {
	_Number i,j,k,l;
	if(A == NULL || B == NULL || A->v == NULL || B->v == NULL || A->x <= 0 || A->y <= 0 || B->x <= 0 || B->y <= 0) {
		println("Generating Error!!!");
		return NULL;
	}
	else {
		Matrix* result=Create(A->x*B->x,A->y*B->y);
		for(l=0;l<A->x;l++) {
			for(k=0;k<B->x;k++) {
				for(j=0;j<A->y;j++) {
					for(i=0;i<B->y;i++) result->v[(l*B->y+k)*A->y*B->y+j*B->y+i/*(j*B->y+i)*A->y*B->y+l*B->y+k*/]=A->v[l*A->y+j]*B->v[k*B->y+i];
				}
			}
		}
		return result;
	}
}

//support OpenCL

	
//this macro can check OpenCL API execute result
#define check(input,str) ({\
    if(input!=CL_SUCCESS) {\
        printf("%s is err,the num is:%d",str,input);\
        return 1;\
    }\
})

typedef enum {operate,Multiple,eye,e} choice;

	
//this struct have the base of OpenCL executing environment
typedef struct {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue cmd_queue;
    cl_program program;
    cl_int* cl_err;
    cl_ulong info[2];
    size_t *global_size,*local_size;
} cl_group;

	
//this function start the OpenCL environment
cl_group* format_group(const char* cl_filename) {

    cl_group* c0=(cl_group*)malloc(sizeof(cl_group));
    c0->cl_err=(cl_int*)calloc(1,sizeof(cl_int));

    *c0->cl_err=clGetPlatformIDs(1,&(c0->platform),NULL);
    *c0->cl_err=clGetDeviceIDs(c0->platform,CL_DEVICE_TYPE_ALL,1,&c0->device,NULL);

    *c0->cl_err = clGetDeviceInfo(c0->device, CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(cl_ulong), &c0->info[0], NULL);
    c0->info[0]*=1048576;
    *c0->cl_err = clGetDeviceInfo(c0->device, CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(cl_ulong), &c0->info[1], NULL);
    c0->info[1]*=c0->info[1]*c0->info[1];

    c0->context=clCreateContext(NULL,1,&c0->device,NULL,NULL,c0->cl_err);
    c0->cmd_queue=clCreateCommandQueueWithProperties(c0->context,c0->device,NULL,c0->cl_err);

    char* s=filestring(cl_filename);
    c0->program=clCreateProgramWithSource(c0->context,1,(const char **)&s,NULL,c0->cl_err);
    *c0->cl_err=clBuildProgram(c0->program,1,&c0->device,NULL,NULL,NULL);

    return c0;
}

	
//this function delete the environment of OpenCL
int cl_clearmem(cl_group* target) {
    *target->cl_err = clFlush(target->cmd_queue);
    check(*target->cl_err,"release memory");
    *target->cl_err = clFinish(target->cmd_queue);
    check(*target->cl_err,"release memory");
    *target->cl_err = clReleaseCommandQueue(target->cmd_queue);
    check(*target->cl_err,"release memory");
    *target->cl_err = clReleaseContext(target->context);
    check(*target->cl_err,"release memory");
    *target->cl_err = clReleaseProgram(target->program);
    check(*target->cl_err,"release memory");
    free(target->global_size);
    free(target->local_size);
    free(target->cl_err);
    free(target);
    println("release object success!!!");
    return 0;
}

	
//this function using OpenCL to calculate Matrix
int calculate(S_Matrix* a,S_Matrix* b) {
    cl_group* g0=format_group("M.cl");
    choice selection;
    println("please input the type of your selection:");
    scanf("%d",&selection);
    while(selection!=operate&&selection!=Multiple&&selection!=eye&&selection!=e) {
        println("value is invaild,please input true type of your selection:");
        scanf("%d",&selection);
    }
    switch(selection) {
        case operate: {
            S_Matrix* result=Create0(a->x,b->y);
            int option;
            println("please input the operate option:");
            scanf("%d",&option);
		
	//make OpenCL kernel Object and mem Object
            cl_kernel kernel=clCreateKernel(g0->program, "operate", g0->cl_err);
            cl_mem a0,a1,rlt,set;

            a0=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,a->x*a->y*sizeof(float),NULL,g0->cl_err);
            a1=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,b->x*b->y*sizeof(float),NULL,g0->cl_err);
            rlt=clCreateBuffer(g0->context,CL_MEM_WRITE_ONLY,result->x*result->y*sizeof(float),NULL,g0->cl_err);
            set=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,sizeof(int),NULL,g0->cl_err);
		
	//copy the data to buffer
            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,a0,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);
            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,a1,CL_TRUE,0,b->x*b->y*sizeof(float),b->v,0,NULL,NULL);
            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,set,CL_TRUE,0,sizeof(int),&option,0,NULL,NULL);
		
	//set kernel arguments
            *g0->cl_err=clSetKernelArg(kernel,0,sizeof(cl_mem),&a0);
            *g0->cl_err=clSetKernelArg(kernel,1,sizeof(cl_mem),&a1);
            *g0->cl_err=clSetKernelArg(kernel,2,sizeof(cl_mem),&rlt);
            *g0->cl_err=clSetKernelArg(kernel,3,sizeof(cl_mem),&set);

            g0->global_size=(size_t*)malloc(1*sizeof(size_t));
            g0->local_size=(size_t*)malloc(1*sizeof(size_t));

            *g0->global_size=a->x*a->y;
            *g0->local_size=a->x;
	//execute kernel
            *g0->cl_err=clEnqueueNDRangeKernel(g0->cmd_queue,kernel,1,NULL,g0->global_size,g0->local_size,0,NULL,NULL);

	//copy buffer data to global memory
            *g0->cl_err=clEnqueueReadBuffer(g0->cmd_queue,rlt,CL_TRUE,0,result->x*result->y*sizeof(float),result->v,0,NULL,NULL);

            DISPLAY(result);
            break;
        }
        case Multiple:{
            S_Matrix* result=Create0(a->x,b->y);
            long h[2]={(long)a->y,(long)b->y};

            cl_kernel kernel=clCreateKernel(g0->program, "multiple", g0->cl_err);
            cl_mem a0,a1,rlt,set;

            a0=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,a->x*a->y*sizeof(float),NULL,g0->cl_err);
            a1=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,b->x*b->y*sizeof(float),NULL,g0->cl_err);
            rlt=clCreateBuffer(g0->context,CL_MEM_WRITE_ONLY,result->x*result->y*sizeof(float),NULL,g0->cl_err);
            set=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,2*sizeof(long),NULL,g0->cl_err);

            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,a0,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);
            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,a1,CL_TRUE,0,b->x*b->y*sizeof(float),b->v,0,NULL,NULL);
            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,set,CL_TRUE,0,2*sizeof(long),h,0,NULL,NULL);

            *g0->cl_err=clSetKernelArg(kernel,0,sizeof(cl_mem),&a0);
            *g0->cl_err=clSetKernelArg(kernel,1,sizeof(cl_mem),&a1);
            *g0->cl_err=clSetKernelArg(kernel,2,sizeof(cl_mem),&rlt);
            *g0->cl_err=clSetKernelArg(kernel,3,sizeof(cl_mem),&set);

            g0->global_size=(size_t*)malloc(2*sizeof(size_t));
            //g0->local_size=(size_t*)malloc(2*sizeof(size_t));

            g0->global_size[0]=result->x;
            g0->global_size[1]=result->y;
            //*g0->local_size=a->x;

            *g0->cl_err=clEnqueueNDRangeKernel(g0->cmd_queue,kernel,2,NULL,g0->global_size,NULL,0,NULL,NULL);

            *g0->cl_err=clEnqueueReadBuffer(g0->cmd_queue,rlt,CL_TRUE,0,result->x*result->y*sizeof(float),result->v,0,NULL,NULL);

            DISPLAY(result);
            break;
        }
        case eye:{
            unsigned long* l0=(unsigned long*)malloc(sizeof(unsigned long)*2);
            l0[0]=a->x,l0[1]=a->y;

            cl_kernel kl=clCreateKernel(g0->program, "eye", g0->cl_err);

            cl_mem v,I;
            v=clCreateBuffer(g0->context,CL_MEM_READ_WRITE,a->x*a->y*sizeof(float),NULL,g0->cl_err);
            I=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,2*sizeof(unsigned long),NULL,g0->cl_err);

            //*g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,v,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);
            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,I,CL_TRUE,0,2*sizeof(unsigned long),l0,0,NULL,NULL);

            *g0->cl_err=clSetKernelArg(kl,0,sizeof(cl_mem),&v);
            *g0->cl_err=clSetKernelArg(kl,1,sizeof(cl_mem),&I);

            g0->global_size=(size_t*)malloc(2*sizeof(size_t));
            g0->local_size=(size_t*)malloc(2*sizeof(size_t));

            g0->global_size[0]=a->x,g0->global_size[1]=a->y;
            g0->local_size[0]=a->x/2,g0->local_size[1]=a->y/2;

            *g0->cl_err=clEnqueueNDRangeKernel(g0->cmd_queue,kl,2,NULL,g0->global_size,g0->local_size,0,NULL,NULL);

            *g0->cl_err=clEnqueueReadBuffer(g0->cmd_queue,v,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);

            DISPLAY(a);

            break;
        }
        case e:{
            float target;
            println("please input the target value:");
            scanf("%f",&target);

            cl_kernel kl0=clCreateKernel(g0->program, "e", g0->cl_err);

            cl_mem v,I;
            v=clCreateBuffer(g0->context,CL_MEM_READ_WRITE,a->x*a->y*sizeof(float),NULL,g0->cl_err);
            I=clCreateBuffer(g0->context,CL_MEM_READ_ONLY,sizeof(float),NULL,g0->cl_err);

            *g0->cl_err=clEnqueueWriteBuffer(g0->cmd_queue,I,CL_TRUE,0,sizeof(float),&target,0,NULL,NULL);

            *g0->cl_err=clSetKernelArg(kl0,0,sizeof(cl_mem),&v);
            *g0->cl_err=clSetKernelArg(kl0,1,sizeof(cl_mem),&I);

            g0->global_size=(size_t*)malloc(sizeof(size_t));
            *g0->global_size=a->x*a->y;

            *g0->cl_err=clEnqueueNDRangeKernel(g0->cmd_queue,kl0,1,NULL,g0->global_size,NULL,0,NULL,NULL);

            *g0->cl_err=clEnqueueReadBuffer(g0->cmd_queue,v,CL_TRUE,0,a->x*a->y*sizeof(float),a->v,0,NULL,NULL);

            DISPLAY(a);

            break;
        }
        default:{
            println("the program error!!!");
            return 1;
        }//goto exit;
    }
    /*exit: {
        println("the program error!!!");
        return 1;
    }*/
    return 0;
}

#define M_ADD 1
#define M_SUBSTRACTION -1

#undef MATRIX

#ifdef __cplusplus
}
#endif
