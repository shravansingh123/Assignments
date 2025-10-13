#include<stdio.h>
#include<string.h>
#include<stdlib.h>

float **EnterArray(int ,int );
float **CreateFArray(int ,int );
struct fMatrix *fEnterMatrix(int,int,float** );
int PrintArray(float **,int ,int );
float getDeterminant(struct fMatrix *);
struct fMatrix *stripMatrix(struct fMatrix *,int );

struct fMatrix
{
	float **farray;	
	int row;
	int col;
	int determinantstatus;
	float determinant;	
};
struct fMatrix *EnterMatrix(int row,int col,float **farray)
{
	struct fMatrix *matrix=(struct fMatrix *)calloc(1,sizeof(struct fMatrix));
	matrix->farray=farray;
	matrix->col =col;
	matrix->row =row;
	matrix->determinant =0;
	matrix->determinantstatus =0;	
	return matrix;
}

int main()
{
	int matRow,matCol;
	printf("Please enter the first array row:");
	scanf("%d",&matRow);
	printf("Please enter the first array column:");
	scanf("%d",&matCol);
	float **farray=EnterArray( matRow,matCol);
	struct fMatrix *testmat=EnterMatrix(matRow,matCol,farray);
	
	PrintArray(testmat->farray, matRow, matCol);
	getDeterminant(testmat);
	printf("%f",testmat->determinant );
	getchar();
	free(testmat);
}
// step by step code write and execute!!
float getDeterminant(struct fMatrix *matrix)
{
	if(matrix->determinantstatus ==1)
		return matrix->determinant ;
	else
	{
		for(int matCol=0;matCol < matrix->col ;matCol++)
		{
			if(matrix->row ==2)
			{
				matrix->determinant= matrix->farray[0][0]*matrix->farray[1][1]-matrix->farray[0][1]*matrix->farray[1][0];				
				break;
			}
			else
			{
				struct fMatrix *tempmat=stripMatrix(matrix,matCol);
				getDeterminant(tempmat);
				matrix->determinant =matrix->determinant + matrix->farray[0][matCol]*tempmat->determinant *(matCol%2==0? 1: -1);
				free(tempmat);
			}		  
		}	
	matrix->determinantstatus =1;
	}
	return matrix->determinant;
}

struct fMatrix *stripMatrix(struct fMatrix *matrix,int matCol)
{
	float **farray=CreateFArray(matrix->row-1 ,matrix->col-1 );
	struct fMatrix *tempmatrix=EnterMatrix(matrix->row-1 ,matrix->col-1,farray);
	for(int matRowtemp=1;matRowtemp<matrix->row ;matRowtemp++)
	{
		for(int matColtemp=0, temp=0;matColtemp<matrix->col;matColtemp++)
		{
			if(matCol==matColtemp)
				continue;
			else
			{
				tempmatrix->farray[matRowtemp-1][temp]=matrix->farray[matRowtemp][matColtemp];
				temp++;
			}		
		}
	}
	return tempmatrix;
}

int PrintArray(float **Array,int ArrRow,int ArrCol)
{
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		{
		printf("%f ",Array[ArrRowTemp][ArrColTemp]);
		}
	  printf("\n");
	}
printf("\n");
}

float **EnterArray(int ArrRow,int ArrCol)
{
	float **Array=CreateFArray(ArrRow,ArrCol);
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		scanf("%f",&Array[ArrRowTemp][ArrColTemp]);
	}
	return Array;
}
float **CreateFArray(int ArrRow,int ArrCol)
{
	float **Array=(float **)calloc(sizeof(float*),ArrRow);
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
		Array[ArrRowTemp]=(float *)calloc(sizeof(float),ArrCol);
	}
	return Array;
}
