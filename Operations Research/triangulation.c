#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define X 0
#define Y 1
typedef enum{FALSE, TRUE} bool;
#define DIM 2
typedef int tpointi[DIM];

typedef struct tvertexstructure tsvertex;
typedef tsvertex *tvertex;
tvertex vertices=NULL; //head of circular list

//vertex structure
struct tvertexstructure{
    int vnum;
    tpointi v;
    bool ear;
    tvertex next,prev;
};
int nvertices;

#define exitfailure 1

#define NEW(p,type) if((p=(type *) malloc(sizeof(type)))==NULL) {printf("New() out of memeory\n"); exit(exitfailure);}

#define ADD(head,p) if(head){ p->next=head; p->prev=head->prev;head->prev=p;p->prev->next=p;} else {head=p;\
                             head->next=head->prev=p;} 
#define FREE(p) if(p) {free((char*)p); p=NULL}
void triangulate(void);

void entervertices(void )
{
    printf("Please enter number of vertices:");
    scanf("%d",&nvertices);  
    int i;
    tvertex p;
    printf("Please enter vertices:\n");
    for(i=0;i < nvertices;i++)
    {
        printf("i:%d\n",i);
        NEW(p,tsvertex);
        ADD(vertices,p); 
        scanf("%d%d",&(p->v[0]),&(p->v[1]));
        p=NULL;        
    }
}

int main()
{
    entervertices();
    printf("Diagonals in triangulation\n");
    triangulate();
}

int area2(tpointi a,tpointi b,tpointi c)
{
    return (b[0]-a[0])*(c[1]-a[1]) -(c[0]-a[0])*(b[1]-a[1]);
}

int areapoly2(void)
{
    int sum=0;
    tvertex p,a;
    p=vertices;
    a=p->next;
    do{
    sum+=area2(p->v,a->v,a->next->v);
    a=a->next;
    }
    while(a->next!=vertices);
    return sum;
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
    //getchar();
    return incone(a,b) && incone(b,a) && diagonalise(a,b);
}

void earinit(void)
{
    tvertex  v0,v1,v2;
    v1=vertices;
    do{
    v2=v1->next;
    v0=v1->prev;    
    v1->ear=diagonal(v0,v2);
    v1=v1->next;
    } while(v1!=vertices);
           
}
void triangulate(void)
{
    tvertex  v0,v1,v2,v3,v4;
    //printvertices();    
    int n=nvertices;
    earinit();
    // each of the step remove an ear
    while(n>3)
    {
        v2=vertices;
        do{
            if(v2->ear)
            {
                v3=v2->next;
                v4=v3->next;
                v1=v2->prev;
                v0=v1->prev;
                printf("v1 X:%dY:%d v3 X:%dY:%d\n",v1->v[0],v1->v[1],v3->v[0],v3->v[1]);
                v1->ear=diagonal(v0,v3);
                v3->ear=diagonal(v1,v4);
                
                v1->next=v3;
                v3->prev=v1;
                vertices=v3;
                n--;
                break;
            }
        v2=v2->next;
        }while(v2!=vertices);
    
    }
}
void printvertices()
{
    tvertex v;
    v=vertices;
    do{
        printf("%d\t%d\n",v->v[0],v->v[1]);
        v=v->next;
    }while(v!=vertices);
}
