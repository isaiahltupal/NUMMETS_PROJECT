#include<stdio.h>
#include<stdlib.h>


float *A, *L, *U, *B, *Z, *X,*I;
int size;


void free_ptr(float* ptr){
    if(ptr!=NULL)
        free(ptr);
}

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



void backwardSub(float* constantArr, float* solutionArr){
    int i,j;
    float tempVal;
    for(i = size-1; i >= 0; i--){
        tempVal=get(constantArr,i);
        for(j = size-1; j > i; j--){
            tempVal-=get_2d(U,i,j)*get(solutionArr,j);
        }
        assign(solutionArr,tempVal/get_2d(U,i,i),i);

    }

}


void inputMatrix(int command){

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
        if (command==1){
            sscanf(buffPointer,"%f%n",&temp,&numBytes);
            assign(B,temp,r);
        }
    }
   
}

void forwardSub(float* constantArr, float* solutionArr){
    int i,j;
    float tempVal;
    for(i = 0; i < size; i++){
        tempVal = get(constantArr,i);
        for(j = 0; j < i; j++){
            tempVal-=get_2d(L,i,j)*get(solutionArr,j);
        }
        assign(solutionArr,tempVal/get_2d(L,i,i),i);
    }
}


void getX(){
    backwardSub(Z,X);
}


void getZ(){
    forwardSub(B,Z);
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

void printSolutionArray(float* arr){
    printf("\nThe solution to the linear equation is\n");
    int r;
    
    for(r=0;r<size;r++){
            printf("x%i %f\n",r+1,get(arr,r));
    }

}



void printArray(float* arr){
    int r;
    for(r=0;r<size;r++){
            printf("%f\n",get(arr,r));
    }
    printf("\n");

}

void solveInverse(){
    inputMatrix(2);

    I = (float*)malloc(size*size*sizeof(float));
    B = (float*)malloc(size*sizeof(float));
    int i,j,k;
    for(j = 0; j < size; j++){
        /*
            initialize constants for first column
        */
       for(i=0;i<size;i++){
            if(i==j)
                assign(B,1,i);
            else
                assign(B,0,i);
            
       }


        getLU();
        forwardSub(B,Z);
        backwardSub(Z,X);

       // place answers to Inverse matrix
       for(k=0;k<size;k++){
            assign_2d(I,get(X,k),k,j); 
        }   


        // get solution coloumn

    }
    printf("\nThe Inverse of the Square matrix is: \n");
    printArray_2d(I);

    

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


void solveX(){
    inputMatrix(1);
    getLU();
    getZ();
    getX();
    printSolutionArray(X);
}

int main(){
    int loop=1;
    int command=0;
    while(loop==1){
        printf("\nWELCOME TO LU DECOMPOSITION PROGRAM\n1 - Solve a simulatneous linear equation\n2 - Find inverse of square matrix\npress any key to exit");
        printf("\nEnter Command: ");
        scanf("%d",&command);
        switch(command){
            case 1:
                solveX();
                break;
            case 2:
                solveInverse();
                break;
            default:
                loop == 0;
                break;

        }
        if(loop != 0){
            printf("Do you want to exit? \n1-No \n2-Yes");
            scanf("%i",&loop);
        }


    }
 
    /*
    1. Get array 
    2. Get U while getting L
    3. Get Solution
    4. Get Inverse 
    */ 

   // *A, *L, *U, *B, *Z, *X,*I
    free_ptr(A);
    free_ptr(L);
    free_ptr(U);
    free_ptr(B);
    free_ptr(Z);
    free_ptr(X);
    free_ptr(I);
    return 0;
}
