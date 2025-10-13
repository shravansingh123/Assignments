#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int **EnterArray(int ,int );
int **CreateFArray(int ,int );
int PrintArray(int **,int ,int );
int FindMinArray(int *,int );
int FindMaxArray(int *,int );
int ReducetoZero(int **);
int CheckOptimality(int **);
int Doiterations(int **);
int matRow,matCol, **tempcopy1,**orgMatrix;

int main()
{
	
	printf("Please enter the number of jobs:");
	scanf("%d",&matRow);
	printf("Please enter the number of persons:");
	scanf("%d",&matCol);
	int **iarray=EnterArray( matRow,matCol);
	orgMatrix=CreateFArray(matRow,matCol);
	for(int i=0;i<matRow;i++)
	{
		for(int j=0;j<matCol;j++)
		{						
		orgMatrix[i][j]=iarray[i][j];
		}
	}	
	printf("Assignment Matrix:\n");
	PrintArray(iarray,matRow,matCol);	
	ReducetoZero(iarray);
	Doiterations(iarray);
	
	free(iarray);
	free(tempcopy1);
}

int Doiterations(int **iarray)
{
int iteration=0;
	while(iteration<10)
	{
		if(!CheckOptimality(iarray))
		{		
			PrintArray(iarray,matRow,matCol);	
			//allocation of jobs
			int p,q,job[4];
			//allocation row wise
			for(int i=0;i<matRow;i++)
			{
				p=0;
				q=0;
				int j;
				for(j=0;j<matCol;j++)
				 {
					if(iarray[i][j]==0)
					{
						p++;
						q=j;
					}
						
						if(p==2)
						{
							break;
						}
				}
				 
				 if(p==1)
				 {
				 	job[i]=q+1;
					for(int temprow=i+1;temprow<matRow;temprow++)
						iarray[temprow][q]==0?iarray[temprow][q]=-1:1;			 	
					//iarray[i][j]=1;			 	
				 }		 
		    }		    
		    //allocations column wise		    
		    for(int j=0;j<matCol;j++)
			{
				p=0;
				q=0;
				for(int i=0;i<matRow;i++)
				 {
					if(iarray[i][j]==0)
					{
						p++;
						q=i;
				    	}
						if(p==2)
						{
							break;
						}
				 }
				 
				 if(p==1)
				 {
				 	job[q]=j+1;
				 	for(int tempcol=j+1;tempcol<matCol;tempcol++)
						iarray[q][tempcol]==0?iarray[q][tempcol]=-1:1;			 	
				 	//iarray[j][q]=1;
				 }				 
		    }
	    	printf("optimality condition reached, with assignments:\n"); 
			int costsum=0;
	    	for(int i=0;i<matRow;i++)
	    	{
				printf("Job %d to M%d\t",i+1,job[i]);
				costsum=costsum+orgMatrix[i][job[i]-1];
			}
		printf("\nFinal cost %d\n",costsum);
		return 1;//successful
		}
		else
		{
			//find minimum
			//PrintArray(tempcopy1,matRow,matCol);	
			int minelement=100000; // some large value
			int i;
			for(i=0;i<matRow;i++)
			{
				for(int j=0;j<matCol;j++)
				{	
					//if(i==0 && j==0) minelement=iarray[0][0];
					if(tempcopy1[i][j]!=1 && tempcopy1[i][j]!=2 && iarray[i][j]<minelement)
					 minelement=iarray[i][j];	
				}
			}
			// for subtracting minelement from array and adding where two lines crosses
			for(i=0;i<matRow;i++)
			{
				for(int j=0;j<matCol;j++)
				{						
					if(tempcopy1[i][j]==0 )
						iarray[i][j]=iarray[i][j]-minelement;
					else if(tempcopy1[i][j]==2 )
						iarray[i][j]=iarray[i][j]+minelement;
					else;
				}
			}
			//PrintArray(iarray,matRow,matCol);	
		}
		iteration++;
	}

}

int CheckOptimality(int **iarray)
{
	int rowzeros[matRow],colzeros[matCol],counter=0,itercounter=0;
	int **tempcopy=CreateFArray(matRow,matCol);
	tempcopy1=CreateFArray(matRow,matCol);
	//memmove(tempcopy,iarray,sizeof(int)*matRow*matCol); // this function is giving problem of copy by reference tempcopy is also changing the iarray??
	for(int i=0;i<matRow;i++)
			{
				for(int j=0;j<matCol;j++)
				{						
				tempcopy[i][j]=iarray[i][j];
				}
			}
	// for making row zeros line
	while(itercounter<matCol)
	{
	
		for(int i=0;i<matRow;i++)
		{
			counter=0;
			for(int j=0;j<matCol;j++)
			{
				if(tempcopy[i][j]==0)counter++;
			}
			rowzeros[i]=counter;	
		}
		
		int maxzerosrow=FindMaxArray(rowzeros,matRow );
		// if no zeros found in the matrix
		if(rowzeros[maxzerosrow]==0)
		{
			return 1;
		}
				
		// for making col zeros line		
		for(int i=0;i<matCol;i++)
		{
			counter=0;
			for(int j=0;j<matRow;j++)
			{
				if(tempcopy[j][i]==0)counter++;
			}
			colzeros[i]=counter;	
		}
		
		int maxzeroscol=FindMaxArray(colzeros,matCol );
		if(colzeros[maxzeroscol]==0)// if no more zeros left in matrix then out of loop
		{
			return 1;
		}
		
		if(maxzerosrow>maxzeroscol)
		{
		
			for(int j=0;j<matCol;j++)
			{
				if(tempcopy[maxzerosrow][j]==0)
					tempcopy[maxzerosrow][j]=-1;
				tempcopy1[maxzerosrow][j]=tempcopy1[maxzerosrow][j]+1;// saving 1 where zero is found for leaving these values for next iteration if optimality is not reached
			}
		}
		else
		{
	
			for(int j=0;j<matRow;j++)
			{
				if(tempcopy[j][maxzeroscol]==0)
					tempcopy[j][maxzeroscol]=-1;
				tempcopy1[j][maxzeroscol]=tempcopy1[j][maxzeroscol]+1;// saving 1 where zero is found for leaving these values for next iteration if optimality is not reached
				
			}
		}
		itercounter++;		
		//PrintArray(tempcopy,matRow,matCol);
		//PrintArray(iarray,matRow,matCol);
		//printf("%d\n",itercounter);
	}
	//PrintArray(tempcopy1,matRow,matCol);
	if(itercounter==matCol)
	{	
		
		return 0; //successful
	}
	
}

int ReducetoZero(int **iarray)
{
	for(int i=0;i<matRow;i++)
	{
		int index=FindMinArray(*(iarray+i),matCol);
		int tempmin=iarray[i][index];
		for(int j=0;j<matCol;j++)
		{
			//printf("%d\t%d\t%d \n",iarray[i][j],tempmin,index);
			iarray[i][j]=iarray[i][j]-tempmin;
			
		}
	}
	//PrintArray(iarray,matRow,matCol);
	for(int i=0;i<matCol;i++)
	{
		// for finding any zero in the col
		int j;	
		for(j=0;j<matRow;j++)	
		if(iarray[j][i]	==0)break;
		// if no zero in the column then substract the min from the cols
		if(j==matRow)
		{
			int minelement=0;
			// for finding min among the cols		
			for(int icol=0;icol<matCol;icol++)
			{			
			if(icol==0)continue;
			if(iarray[minelement][i]>iarray[icol][i])
			 minelement=icol;	
			}
			int tempmin=iarray[minelement][i];
			for(int j=0;j<matRow;j++)	
			iarray[j][i]=iarray[j][i]-tempmin;
		}
	}
}
// returns the index of mininum element
int FindMinArray(int *array,int totalelement)
{
int minelement=0;
	int i;
	for(i=0;i<totalelement;i++)
	{
		if(i==0)continue;
		if(array[minelement]>array[i])
		 minelement=i;	
	}
	return minelement;
}
int FindMaxArray(int *array,int totalelement)
{
int maxelement=0;
	int i;
	for(i=0;i<totalelement;i++)
	{
		if(i==0)continue;
		if(array[maxelement]<array[i])
		 maxelement=i;	
	}
	return maxelement;
}


int PrintArray(int **Array,int ArrRow,int ArrCol)
{
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		{
		printf("%d ",Array[ArrRowTemp][ArrColTemp]);
		}
	  printf("\n");
	}
printf("\n");
}

int **EnterArray(int ArrRow,int ArrCol)
{
	int **Array=CreateFArray(ArrRow,ArrCol);
	/*Array[0][0]=10;
	Array[0][1]=5;
	Array[0][2]=3;
	Array[0][3]=8;
	Array[1][0]=5;
	Array[1][1]=3;
	Array[1][2]=8;
	Array[1][3]=7;
	Array[2][0]=6;
	Array[2][1]=11;
	Array[2][2]=13;
	Array[2][3]=7;
	Array[3][0]=9;
	Array[3][1]=5;
	Array[3][2]=6;
	Array[3][3]=7;	
	PrintArray(Array,ArrRow,ArrCol); */
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		scanf("%d",&Array[ArrRowTemp][ArrColTemp]);
	}
	return Array;
}

int **CreateFArray(int ArrRow,int ArrCol)
{
	int **Array=(int **)calloc(sizeof(int*),ArrRow);
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
		Array[ArrRowTemp]=(int *)calloc(sizeof(int),ArrCol);
	}
	return Array;
}

