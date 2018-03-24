/*  Short job 3
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <algorithm>
#include <bitset>
#define MAX_LENGTH 1050000

using namespace std;


const long pages_cnt = 8192; //numbers
const long pages_cnt_bits = 13;
const long pages_cnt_bits_rest = 32 - pages_cnt_bits;

// const long page_size = 4294967296 / pages_cnt; //bits
const long page_size = 536870912 / pages_cnt; //bits
const long page_size_bits = 32 - pages_cnt_bits - 3;

class MySet
{
public:
  MySet() {
    length = 0;
    pages = new uint8_t*[pages_cnt];
    sizes = new uint8_t*[pages_cnt];
    counts = new uint32_t[pages_cnt];
    for(int x = 0; x < pages_cnt; x++) {
      pages[x] = nullptr;
      sizes[x] = nullptr;
      counts[x] = 0;
    }
  }

  ~MySet() {
    for(int x = 0; x < pages_cnt; x++) {
      delete[] pages[x];
      delete[] sizes[x];
    }
    delete[] pages;
    delete[] sizes;
    delete[] counts;
  }

  int getSize() { return length; }

  int32_t inter1(int32_t valA, int32_t valB) {
    uint32_t a = (uint32_t) valA;
    uint32_t b = (uint32_t) valB;
    uint32_t aPageInx;
    uint32_t bPageInx;
    uint32_t aBucketInx;
    uint32_t bBucketInx;
    uint8_t aOffset;
    uint8_t bOffset;
    uint8_t aOffsetVal;

    aPageInx = a >> pages_cnt_bits_rest;
    bPageInx = b >> pages_cnt_bits_rest;
    aBucketInx = (a << pages_cnt_bits) >> page_size_bits;
    bBucketInx = (b << pages_cnt_bits) >> page_size_bits;
    aOffset = a & 0x07; 
    bOffset = b & 0x07; 

    int32_t found = 0;
    if(aPageInx == bPageInx) {
      uint8_t* page = pages[aPageInx];
      uint8_t* size = sizes[aPageInx];
      found += __builtin_popcount((uint8_t)(page[aBucketInx] >> aOffset));
      found += __builtin_popcount((uint8_t)(page[bBucketInx] << (0x07 - bOffset)));
      for(uint32_t curBucketInx = aBucketInx + 1; curBucketInx < bBucketInx; curBucketInx++)
        found += size[curBucketInx];
    } else {
      uint8_t* page = pages[aPageInx];
      uint8_t* size = sizes[aPageInx];

      if(page != nullptr) {
        found += __builtin_popcount((uint8_t)(page[aBucketInx] >> aOffset));
        for(uint32_t curBucketInx = aBucketInx + 1; curBucketInx < page_size; curBucketInx++)
          found += size[curBucketInx];
      }
      page = pages[bPageInx];
      size = sizes[bPageInx];
      if(page != nullptr) {
        found += __builtin_popcount((uint8_t)(page[bBucketInx] << (0x07 - bOffset)));
        for(uint32_t curBucketInx = 0; curBucketInx < bBucketInx; curBucketInx++)
          found += size[curBucketInx];
      }
      for(uint32_t pageInx = aPageInx + 1; pageInx < bPageInx; pageInx++)
        found += counts[pageInx];
    }
    return found;
  }

  int interval(int32_t *low, int32_t *high, int32_t *res, int n) {
    for(int x = 0; x < n; x++) {
      res[x] = inter1(low[x], high[x]);
    }
  }

  int insert(int32_t *values, int n) {
    uint32_t val;
    uint32_t pageInx;
    uint32_t bucketInx;
    uint8_t offset;
    uint8_t offsetVal;
    for(int x = 0; x < n; x++) {
      val = (uint32_t)values[x];
      pageInx = val >> pages_cnt_bits_rest;
      bucketInx = (val << pages_cnt_bits) >> page_size_bits;
      offset = val & 0x07; 
      offsetVal = 1 << offset;
      uint8_t*& page = pages[pageInx];
      uint8_t*& size = sizes[pageInx];
      uint32_t& count = counts[pageInx];
      if(page == nullptr) {
        page = new uint8_t[page_size];
        size = new uint8_t[page_size];
        for(int y = 0; y < page_size; y++)
          size[y] = page[y] = 0;
      }

      uint8_t& bucket = page[bucketInx];
      if(!(bucket & offsetVal)) {
        bucket |= offsetVal;
        length++; count++; size[bucketInx]++;
      }
    }
  }

  int remove(int32_t* values, int n) {
    uint32_t val;
    uint32_t pageInx;
    uint32_t bucketInx;
    uint8_t offset;
    uint8_t offsetVal;
    for(int x = 0; x < n; x++) {
      val = (uint32_t)values[x];
      pageInx = val >> pages_cnt_bits_rest;
      bucketInx = (val << pages_cnt_bits) >> page_size_bits;
      offset = val & 0x07; 
      offsetVal = 1 << offset;
      uint8_t*&page = pages[pageInx];
      uint8_t*&size = sizes[pageInx];
      uint32_t& count = counts[pageInx];
      if(page == nullptr)
        return -1;

      uint8_t& bucket = page[bucketInx];
      if(bucket & offsetVal) {
        bucket &= ~offsetVal;
        length--; count--; size[bucketInx]--;
      }
    }
  }

  MySet *Union(MySet *A, MySet *B) {
    for(int x = 0; x < pages_cnt; x++) {
      uint8_t*& page = pages[x];
      uint8_t*& size = sizes[x];
      uint32_t& count = counts[x];
      if(A->pages[x] != nullptr || B->pages[x] != nullptr) {
        page = new uint8_t[page_size];
        size = new uint8_t[page_size];
        for(int y = 0; y < page_size; y++) {
          uint8_t& bucket = page[y];
          bucket = 0;
          if(A->pages[x] != nullptr)
            bucket |= A->pages[x][y];
          if(B->pages[x] != nullptr)
            bucket |= B->pages[x][y];
          uint32_t cnt = __builtin_popcount(bucket);
          size[y] = cnt;
          length += cnt;
          count += cnt;
        }
      }
    }
  }

private:
  uint8_t** pages;
  uint8_t** sizes;
  uint32_t* counts;
  int length;
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
          // return -1;
        }
        if (listB->getSize()!=corr2[iteraci]) 
        {
          printf("ERROR2\n");
          // return -1;
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
          // return -1;
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
          printf("modulo: %d\n", modulo);
          printf("correct: %d\n", corr4[iteraci]);
          printf("ERROR4\n");
          // return -1;
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
 
  if(iteraci < 18)
    printf("body: %f\n", 8 * iteraci / 18.0);
  else 
    printf("body: %f\n", min(16.0, 8+2.2/t_time));
  //return 1;
  
  return 0;
}