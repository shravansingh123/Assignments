#include<stdio.h>
main()
{
	int a[50][50],b[50],c,d,m,n,i,j,max,min;
	printf("Enter the total no of activities\n");
	scanf("%d",&m);
	printf("Enter the source node number\n");
	scanf("%d",&n);
	printf("\n");
	for(i=0;i<m;i++)
	{
		printf("ACTIVITY %d\n",i+1);
		a[i][0]=i+1;
		printf("Starting Node=\t");
		scanf("%d",&a[i][1]);
		printf("Finishing Node=\t");
		scanf("%d",&a[i][2]);
		printf("Activity Duration= ");
		scanf("%d",&a[i][3]);
		printf("\n\n");
		b[i]=0;
	}
	
	/*NOTATIONS
	  a[i][0]=starting node
	  a[i][1]=finishing node
	  a[i][2]=time duration
	  a[i][3]=est
	  a[i][4]=eft
	  a[i][5]=lst
	  a[i][6]=lft*/
	  
	  
	for(i=0;i<m;i++)
	{  
	
		if(a[i][1]==n)
		{
		 a[i][4]=0;
		 a[i][5]=a[i][3]; 
		}
		
		else if(a[i][0]!=n)
		{
		//identifying the set of est for the activity which has more than one tail activity 
		for(j=0;j<i;j++)
		{
			if(a[j][2]==a[i][1])
			{
				b[j]=a[j][5];
		    }
		}
		
		//obtaining the est for the activity which has more than one tail activity
		max=b[0];
		for(j=0;j<i;j++)
		{			
			if(max<b[j])
			{
				max=b[j];
			}
		}
	
			a[i][4]=max;
			a[i][5]=max+a[i][3];
		}
		for(j=0;j<m;j++)
		b[j]=0;
    }
    
    //max=maximum completion time and its address
    max=0;
	for(i=0;i<m;i++)
	{
		if(max<a[i][5])
		{
		max=a[i][5];
		d=i;
	    }
	} 
	
	//assigning a value of 1000 to all elements of matrix b in order to find minimum element
	for(j=0;j<m;j++)
	b[j]=1000;
	
	
	
	//determiming lst & lft
	//a[i][5]=lst;a[i][6]=lft
	for(i=m-1;i>=0;i--)
	{
		if(a[i][2]==a[d][2])
		{
			a[i][7]=max;
			a[i][6]=max-a[i][3];
		}
		
		else if(a[i][7]!=a[d][7])
		{
		  //identifying the set of lst for the activity which has more than one forward activity 
		  for(j=m-1;j>i;j--)
		  {
		  	if(a[j][1]==a[i][2])
		  	{
			  b[j]=a[j][6];
		  	}
		  }
		  
		  
		  for(n=0;n<m;n++)
		  //obtaining the lft for the activity which has more than one forward activity
		  min=1000;
		  for(j=m-1;j>i;j--)
		  {
		    if(min>b[j])
		    {
			min=b[j];
	    	}
		  }
		  a[i][7]=min;
		  a[i][6]=min-a[i][3];		  
		  }
		for(j=0;j<m;j++)
		b[j]=1000;
    }
    
    //determining slack of each activity 
	for(i=0;i<m;i++)
	{
		a[i][8]=a[i][7]-a[i][5];
	} 
	
	//printing the table
	printf("Act.\t SN\t FN\t Time\t EST\t EFT\t LST\t LFT\t Slack\t\n");
	for(i=0;i<m;i++)
	{
	for(j=0;j<9;j++)
	printf("%d\t",a[i][j]);
	printf("\n");
    }
    printf("\n\n");
    
    //printing minimum project duration
    printf("Minimum project completion duration is %d\n\n",max);
	
	//determining critical activities
	printf("The Critical Activities are\n");
	for(i=0;i<m;i++)
	{
		if(a[i][8]==0)
		printf("Activity %d\n",i+1);
	}  
	
}


