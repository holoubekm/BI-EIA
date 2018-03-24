/*  Short job 3
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <inttypes.h>
#include <algorithm>
#define MAX_LENGTH 1000000



class MySet
{
public:
    MySet(){
    list=new int32_t[MAX_LENGTH]; 
    size_of_list=0;}

    ~MySet(){
    delete[] list; 
    size_of_list=0;
    }

    int getSize(){return size_of_list;}

    int32_t getAt(int i){return list[i];}

    void printSet()
    {
      int i,j;
      j=getSize();
      printf("%i: ",j);
      for(i=0;i<j;i++) printf("%i,",getAt(i)); 
      printf("\n");
     }

int in(int32_t x)
{
  /*
  je prvek v mnozine?
  test if x is in the set
  */
  int i;
  for(i=0;i<size_of_list;i++)
    if (list[i]==x) return 1;
  return 0;
}

int ins1(int32_t x)
{
  /*
  vlozeni jednoho prvku
  insert one element 
  */
  if (in(x)) return 1;
  if (size_of_list>=(MAX_LENGTH-1)) return -1;  /* prilis mnoho prvku
                                                   too many elements  */
  list[size_of_list++]=x;  
  return 1;
}

int res1(int32_t x)
{
  /*
  smazani jednoho prvku
  delete one element 
  */
  int i;
  for(i=0;i<size_of_list;i++)
    if (list[i]==x) 
    {
      list[i]=list[size_of_list-1]; // prepis poslednim, replace by the last
      size_of_list--;
      return 1;
    }  
  return 1;  
}

int32_t inter1(int32_t a,int32_t b)
{
  /*
  intervalovy test jednoho prvku
  interval test of one element 
  */
  int i;
  int32_t p=0;
  for(i=0;i<size_of_list;i++)
    if ((list[i]>=a)&&(list[i]<=b)) p++;     
  return p;  
}

int interval(int32_t *low,int32_t *high,int32_t *res, int n)
{
  /*
  intervalovy test vice prvku
  interval test of mupliple elements 
  */
  for(int i=0;i<n;i++)
    res[i]=inter1(low[i],high[i]);
  return 1;  
}

int insert(int32_t *x,int n)
{
  /*
  vlozeni vice prvku
  insert multiple elements 
  */

  int i;
  int32_t  x_akt;
  for(i=0;i<n;i++)
  {
    x_akt=x[i];
    if (ins1(x_akt)==-1) return -1;
  }
  return 1;
}

int remove(int32_t *x,int n)
{
  /*
  smazani vice prvku
  delete multiple elements 
  */
  int i;
  int32_t  x_akt;
  for(i=0;i<n;i++)
  {
    x_akt=x[i];
    if (res1(x_akt)==-1) return -1;
  }
  return 1;
}    


MySet *Union(MySet *A,MySet *B)
{
  /*
  sjednoceni 2 mnozin
  union of 2 sets 
  */
  int i;
  size_of_list=0;
  for(i=0;i<A->getSize();i++) ins1(A->getAt(i));
  for(i=0;i<B->getSize();i++) ins1(B->getAt(i));
  return this;
}
private:
    int32_t *list;  // neserazeny seznam prvku v mnozine, unsorted list of items in set
    int size_of_list; //pocet prvku mnoziny, number of items 
};


int main(void) {
int i,j,ok,iteraci=0;
int32_t array[10000];
int32_t x1,x2,low[2000],high[2000],results[2000];
double start,t_time=0.0;
int32_t   k,modulo;
int size,size2;
int corr1[4]={9545,39545,69545,99545};
int corr2[4]={9705,39705,69705,99705};
int corr3[4]={17434,71434,125434,179434};
int32_t corr4[4]={-1369188758,1116529847,-349759472,276335712};  
  size=-20000;   
  iteraci=-1;

do{
      iteraci++;
      /*printf("size1=%i\n",size);
      fflush(stdout);   */
      MySet *listA= new MySet();
      MySet *listB= new MySet();
      MySet *listC= new MySet();
     
      size+=30000;
      size2=std::min(size,5000);
      modulo=191*size;
     if ((size+5000)>=MAX_LENGTH) break;
      /*printf("size2=%i\n",size);
      fflush(stdout);    */
      for(k=0;k<((size+4999)/5000);k++)
      {
        for(i=0;i<size2;i++) array[i]=3*(i+5000*k);//(3*i-5*k)%modulo;
        start=omp_get_wtime();
        j=listA->insert(array,size2);
        t_time+=omp_get_wtime()-start;
      /*printf("size3a=%i %i %i\n",size,j,listA->getSize());
      fflush(stdout);   */ 
        for(i=0;i<size2;i++) array[i]=11*(i+5000*k); //(i-11*k)%modulo;
        start=omp_get_wtime();
        j=listA->remove(array,size2);
        t_time+=omp_get_wtime()-start;
      /*printf("size3c=%i %i\n",size,listA->getSize());
      fflush(stdout);   */       
         for(i=0;i<size2;i++) array[i]=5*(i+5000*k);//(5*i-7*k)%modulo;
        start=omp_get_wtime();
        j=listB->insert(array,size2);
        t_time+=omp_get_wtime()-start;
      /*printf("size3b=%i %i %i\n",size,j,listB->getSize());
      fflush(stdout);    */

        for(i=0;i<size2;i++) array[i]=17*(i+5000*k);//(13*i-k)%modulo;
        start=omp_get_wtime();
        j=listB->remove(array,size2); 
        t_time+=omp_get_wtime()-start;       
      /*printf("size3d=%i %i\n",size,listB->getSize());
      fflush(stdout);      */
      }
      //printf("size4=%i\n",size);
      /*printf("size1=%i %i\n",size,listA->getSize());
      printf("size2=%i %i\n",size,listB->getSize());
      fflush(stdout);   */
      if (iteraci<4)
      {
        if (listA->getSize()!=corr1[iteraci]) 
        {
          printf("ERROR1\n");
          return -1;
        }
        if (listB->getSize()!=corr2[iteraci]) 
        {
          printf("ERROR2\n");
          return -1;
        }
      }

      if ((listA->getSize()+listB->getSize())>MAX_LENGTH) break;
      //listA->printSet();
      //listB->printSet();
      start=omp_get_wtime();
      listC->Union(listA,listB);
      t_time+=omp_get_wtime()-start;
      /*printf("size5=%i %i\n",size,listC->getSize());
      fflush(stdout);  */

      //listC->printSet();
      if (iteraci<4)
      {
        if (listC->getSize()!=corr3[iteraci]) 
        {
          printf("ERROR3\n");
          return -1;
        }
      }      
      for(i=0;i<2000;i++) 
      {
        x1=(6*i*i-1000);
        x2=(4*i+13);
        low[i]=std::min(x1,x2);
        high[i]=std::max(x1,x2);
      }
      start=omp_get_wtime();
      listC->interval(low,high,results,2000);
      t_time+=omp_get_wtime()-start;
      /*printf("size6=%i %i\n",size,listC->getSize());
      fflush(stdout);   */
      //listC->printSet();
      
      modulo=0;
      for(i=0;i<2000;i++) modulo=3*modulo+results[i];
      
      if (iteraci<4)
      {
        if (modulo!=corr4[iteraci]) 
        {
          printf("ERROR4\n");
          return -1;
        }
      } 
      /*printf("size7=%zu\n",modulo);
      fflush(stdout);     */ 
      delete listA;
      delete listB;
      delete listC;
      /*printf("size8=%i\n",size);
      fflush(stdout);   */
      

  ok=1;
  //if (no_items>=MAXIMAL) ok=0;
  if ((t_time)>=10.0) ok=0;
  //ok=0;
  }while(ok);
  printf("\n");
  printf("iterations=%i\n",iteraci);
  printf("end=%i\n",size);
  //if (no_items>=MAXIMAL) 
  printf("time=%g\n",t_time);
  //printf("=%i\n",tmp_max);
 
  //return 1;
  
  return 0;
}