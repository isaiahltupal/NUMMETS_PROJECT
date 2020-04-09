#include<stdio.h>
#include<stdlib.h>


float *A, *L, *U, *B, *Z, *X;
int size;


void assign(float* arr , float value, int i){
    *(arr + i) = value;
}


void assign_2d(float* arr , float value, int r, int c){
    *(arr + (r * size) + c) = value;
}

float get_2d(float* arr, int r, int c){
    return  *(arr + r * size + c);
}

float get(float* arr, int i){
    return *(arr+i);
}

void inputMatrix(){

    printf("How Many Variables?: ");
    scanf("%i",&size);
    fflush(stdin);
    

    //Dynamically create array
    A = (float*)malloc(size*size*sizeof(float));
    L = (float*)malloc(size*size*sizeof(float));
    U = (float*)malloc(size*size*sizeof(float));
    B = (float*)malloc(size*sizeof(float));
    Z = (float*)malloc(size*sizeof(float));
    X = (float*)malloc(size*sizeof(float));
    


    char buff[100];
    char* buffPointer;
    int r,c,numBytes;
    float temp;
    for(r = 0; r < size; r++){
        fgets(buff, 100, stdin);
        buffPointer=buff;
        for(int c = 0 ;c < size; c++ ){
            sscanf(buffPointer,"%f%n",&temp,&numBytes);
            assign_2d(A,temp,r,c);
            buffPointer+=numBytes;
        }
        sscanf(buffPointer,"%f%n",&temp,&numBytes);
        assign(B,temp,r);
    }
   
}

void getLU(){
    //initialize LU matrices
    int r,c,cc;
    float multiplier, tempVal;  
    for(r = 0; r < size; r++){
        for(c = 0; c < size; c++){
            if(r==c){
                assign_2d(L,1,r,c);
            }
            else{
                assign_2d(L,0,r,c);
            }
            
            assign_2d(U, get_2d(A,r,c) ,r,c);
            
        }
    }

  
    // operations to get U:
    for(r = 1; r < size; r++){
        for(c = 0; c<r; c++){

            multiplier = get_2d(U,r,c)/get_2d(U,c,c);
            
            //assign_2d(L,multiplier,r,c); // add multiplier to L

            for(cc = 0; cc< size; cc++){
                tempVal = get_2d(U,r,cc)-get_2d(U,c,cc)*multiplier;
                //printf("%.3f",tempVal3);
                assign_2d(U,tempVal,r,cc); // assign to u
                assign_2d(L,multiplier,r,c); // asign to L
            }

        }
    }
   
}

void getX(){
    int i,j;
    float tempVal;
    for(i = size-1; i >= 0; i--){
        tempVal=get(Z,i);
        for(j = size-1; j > i; j--){
            tempVal-=get_2d(U,i,j)*get(X,j);
        }
        assign(X,tempVal/get_2d(U,i,i),i);
    }
}

void getZ(){
    int i,j;
    float tempVal;
    for(i = 0; i < size; i++){
        tempVal = get(B,i);
        for(j = 0; j < i; j++){
            tempVal-=get_2d(L,i,j)*get(Z,j);
        }
        assign(Z,tempVal/get_2d(L,i,i),i);
    }
}

void printArray_2d(float* arr){
    
    printf("\n\n\n");

    int r,j;
    for (r = 0; r <  size; r++){ 
        for (j = 0; j < size; j++) 
            printf("%f ", *(arr + r * size + j)); 
 
        printf("\n");
    }

}

void printArray(float* arr){
    printf("\n\n\n");
    int r;
    for(r=0;r<size;r++){
        printf("%f\n",get(arr,r));
    }

}

int main(){

    inputMatrix();
    printArray(B);
    getLU();
    getZ();
    getX();
    printArray_2d(A);
    printArray_2d(U);
    printArray_2d(L);
    printArray(Z);
    printArray(X);
    /*
    1. Get array 
    2. Get U while getting L
    3. Get Solution
    4. Get Inverse 
    */ 

    return 0;
}
