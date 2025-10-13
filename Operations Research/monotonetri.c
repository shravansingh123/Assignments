#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define X 0
#define Y 1
typedef enum{FALSE, TRUE} bool;
#define DIM 2
typedef int tpointi[DIM];

typedef struct tvertexstructure tsvertex;
typedef tsvertex * tvertex;
tvertex vertices=NULL; //head of circular list

//vertex structure
struct tvertexstructure{
    int vnum;
    tpointi v;
    bool ear;
    tvertex next,prev;
};

#define exitfailure 1

#define NEW(p,type) if((p=(type *) malloc(sizeof(type)))==NULL) {printf("New() out of memeory\n"); exit(exitfailure);}

#define ADD(head,p) if(head){ p->next=head; p->prev=head->prev;head->prev=p;p->prev->next=p;} else {head=p;\
                             head->next=head->prev=p;} 
#define FREE(p) if(p) {free((char*)p); p=NULL}
#define MAXSIZE 100
struct stack
{
    int list[MAXSIZE][4];
    int top;
};
typedef struct stack STACK;
STACK s;
 
void push(int *);
int*  pop(void);
void entervertices(void );
void triangulate(void);
void mergechain();
bool checkmonotone(tvertex );
void hardcodevertices();
void triangulateMonotone();

bool diagonalise(tvertex,tvertex);
bool incone(tvertex ,tvertex);
void printtruth(bool );
bool diagonal(tvertex ,tvertex);

int nvertices;
tvertex max,min;
int main()
{
    s.top=-1;
    entervertices();
    //hardcodevertices();
    int mergelist[MAXSIZE][4];
    memset(mergelist,'\0',300);
    if(!checkmonotone(vertices))
        printf("polygon in not monotone");
    mergechain(mergelist);
    int j;
    for( j=0;j<nvertices ;j++)
        printf("%d\n", mergelist[j][1]);
    triangulateMonotone(mergelist);
}
void triangulateMonotone(int mergelist[][4])
{
    push(mergelist[0]);
    push(mergelist[1]);
    int j;
    int *tele,*tele1;
  
    printf("triangle are:\n");
    for(j=2;j<nvertices-1;j++)
    {
        if(mergelist[j][2]!=s.list[s.top][2]) //if uj and the top of stack on different chains
        {
            tele=pop(); // u(j-1) to insert back into stack at last
            push(tele); ///pushing it back to restore state
            while(s.top!=0)
            {
                tele1=pop(); 
                int *temp=pop(); // temparary poping to get previous uj for creating triangle
                push(temp); ///pushing it back to restore state
                printf("triangle no:%d coordinates are:%d %d, %d %d, %d %d\n",j-1,mergelist[j][0],mergelist[j][1],tele1[0],tele1[1],temp[0],temp[1]);                    
            }
            pop(); // remove last element that we pushed in
            push(tele);
            push(mergelist[j]);
        }
        else //if on same chain 
        {
            tele1=pop();
            // now pop and check whether diagonal lies inside the polygon to form a triangle
            int *temp=tele1;
            while(s.top>=0 && diagonal(((tvertex)((tele=pop())[3])),(tvertex)(mergelist[j][3])) ) // important to convert the pointers back to tvertex pointer and typecasting is very specific like tvertex on whole of (mergelist[][]) i guess associativity
            {
                printf("triangle no:%d coordinates are:%d %d, %d %d, %d %d\n",j-1,mergelist[j][0],mergelist[j][1],tele[0],tele[1],temp[0],temp[1]);
                temp=tele;
            }
            push(tele);
            push(mergelist[j]);                
        }
    }
    // now add all the diagonals from un to all the vertices on stack except the first and last one
    while(s.top!=0)
    {
        tele1=pop(); 
        int *temp=pop(); // temparary poping to get previous uj for creating triangle
        push(temp); ///pushing it back to restore state
        printf("triangle no:%d coordinates are:%d %d, %d %d, %d %d\n",j-1,mergelist[j][0],mergelist[j][1],tele1[0],tele1[1],temp[0],temp[1]);                    
    }

}


int area2(tpointi a,tpointi b,tpointi c)
{
    return (b[0]-a[0])*(c[1]-a[1]) -(c[0]-a[0])*(b[1]-a[1]);
}
// if point c lies to the left of ab then area will be positive(counterclockwise)
bool left(tpointi a,tpointi b,tpointi c)
{
    return area2(a,b,c)>0;
}
bool lefton(tpointi a,tpointi b,tpointi c)
{
    return area2(a,b,c)>=0;
}
bool collinear(tpointi a,tpointi b,tpointi c)
{
    return area2(a,b,c)==0;
}
bool xor(bool x,bool y)
{
    return !x^!y;
}
bool intersectprop(tpointi a,tpointi b,tpointi c,tpointi d)
{
    // eliminate improper case
    if(collinear(a,b,c)||collinear(a,b,d)||collinear(c,d,a)||collinear(c,d,b)) return FALSE;
    return  xor(collinear(a,b,c),collinear(a,b,d)) && xor(collinear(c,d,a),collinear(c,d,b));

}
bool between(tpointi a,tpointi b,tpointi c)
{
    tpointi ba,ca;
    if(!collinear(a,b,c)) return FALSE;
    // if ab is not vertical then check x coordinates otherwise check betweeness in y coordinates    
    if(a[0]!=b[0])
        return ((a[0]<=c[0]) && (c[0]<=b[0])) || ((a[0]>=c[0]) && (c[0]>=b[0]));
    else
        return ((a[1]<=c[1]) && (c[1]<=b[1])) || ((a[1]>=c[1]) && (c[1]>=b[1]));
        
}

bool intersect(tpointi a,tpointi b,tpointi c,tpointi d)
{
    if(intersectprop(a,b,c,d))
        return TRUE;
    else if(between(a,b,c)||between(a,b,d)||between(c,d,a)||between(c,d,b))
        return TRUE;
    else
        return FALSE;
                
}
bool diagonalise(tvertex a,tvertex b)
{
    tvertex c,c1;
    c=vertices;
    do{
    c1=c->next;
    if (( c!=a)&&(c1!=a)&&( c!=b)&&(c1!=b)&& intersect(a->v,b->v,c->v,c1->v))
        return FALSE;
    c=c->next;    
    } while(c!=vertices);
    return TRUE;    
}
bool incone(tvertex a,tvertex b)
{
    tvertex a0,a1;
    a1=a->next;
    a0=a->prev;
    //if a is convex vertex
    if(lefton(a->v,a1->v,a0->v))
        return left(a->v,b->v,a0->v) && left(b->v,a->v,a1->v);
    else // if reflex
        return !(lefton(a->v,b->v,a1->v) && left(b->v,a->v,a0->v));        
}
void printtruth(bool x)
{
    if(x==1) printf("TRUE\n");
    else printf("FALSE\n");
}
bool diagonal(tvertex a,tvertex b)
{
    //printtruth(incone(a,b));
    //printtruth(incone(b,a));
    return incone(a,b) && incone(b,a) && diagonalise(a,b);
}

void push (int ele[4])
{
    int num;
    if (s.top == (MAXSIZE - 1))
    {
        printf ("Stack is Full\n");
    }
    else
    {
        s.top = s.top + 1;
        s.list[s.top][0] = ele[0];
        s.list[s.top][1] = ele[1];
        s.list[s.top][2] = ele[2];
        s.list[s.top][3] = ele[3];        
    }
}
int* pop ()
{
    int num;
    if (s.top == - 1)
    {
        printf ("Stack is Empty\n");
        return NULL;
    }
    else
    {     
       //s.top = s.top - 1;
       return s.list[s.top--];        
    }
}
// create the list where first and second are x and y and third is to show whether from left or right chain
// temp for anticlockwise and temp1 for clockwise
void mergechain(int mergelist[][4])
{
    tvertex temp,temp1;
    temp=max->next;
    temp1=max->prev;
    mergelist[0][0]=max->v[0];
    mergelist[0][1]=max->v[1];
    mergelist[0][3]=max; // storing the address later to check diagonal inside the polygon or not
    int counter=1;
    while(temp!=min || temp1!=min)
    {
        if(temp->v[1]>temp1->v[1] && temp!=min) // if left chain is higer
        {
            mergelist[counter][0]=temp->v[0];
            mergelist[counter][1]=temp->v[1];
            mergelist[counter][2]=0;
            mergelist[counter][3]=temp;    
            temp=temp->next;
        }
        else
        {
            mergelist[counter][0]=temp1->v[0];            
            mergelist[counter][1]=temp1->v[1];  
            mergelist[counter][2]=1;
            mergelist[counter][3]=temp1;              
            temp1=temp1->prev;  
        }
            counter++;
    }
    mergelist[counter][0]=min->v[0];
    mergelist[counter][1]=min->v[1];
    mergelist[counter][3]=min;    
}

bool checkmonotone(tvertex vertex)
{
    tvertex temp,temp1;
    int imax=vertex->v[1],imin=vertex->v[1];
    temp=vertex->next;
    max=vertex;
    min=vertex;
    while(temp!=vertex)
    {
        if(imax < temp->v[1])
        {   
            imax = temp->v[1];
            max=temp;        
        }
        if(imin > temp->v[1])
        {
            imin = temp->v[1];
            min=temp;
        }
        temp=temp->next;   
    }
    //printf("after  max min %d %d\n",imax,imin);
    //printf("after  max min %d %d\n",max->v[1],min->v[1]);
    //now traverse from min and everything should be monotonically increasing   
    temp=min; // for anticlockwise
    temp1=min; // for clockwise direction and so will use prev
    while(temp!=max || temp1!=max)
    {
        if(temp->v[1] > temp->next->v[1] && temp!=max)
            return 0;
        if(temp1->v[1] > temp1->prev->v[1] && temp1!=max)
            return 0;
        if(temp!=max) temp=temp->next;    
        if(temp1!=max) temp1=temp1->prev;    
    }
    return 1;
}
void entervertices(void )
{
    printf("Please enter number of vertices:");
    scanf("%d",&nvertices);  
    int i;
    tvertex p;
    printf("Please enter vertices:\n");
    for(i=0;i < nvertices;i++)
    {
        //printf("i:%d\n",i);
        NEW(p,tsvertex);
        ADD(vertices,p); 
        scanf("%d%d",&(p->v[0]),&(p->v[1]));
        p=NULL;        
    }
}
/*
void hardcodevertices()
{
tvertex temp=vertices;
    temp->v[0]=4;
    temp->v[1]=1;
temp=temp->next;
    temp->v[0]=8;
    temp->v[1]=3;
temp=temp->next;
    temp->v[0]=6;
    temp->v[1]=7;
temp=temp->next;
    temp->v[0]=3;
    temp->v[1]=7;
temp=temp->next;
    temp->v[0]=2;
    temp->v[1]=6;
temp=temp->next;
    temp->v[0]=1;
    temp->v[1]=5;
temp=temp->next;
    temp->v[0]=3;
    temp->v[1]=4;
temp=temp->next;
    temp->v[0]=4;
    temp->v[1]=3;        

}
*/
