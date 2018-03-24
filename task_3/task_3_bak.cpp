/*  Short job 3
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <algorithm>
#define MAX_LENGTH 1000000

#define BUCKET_SIZE 32
#define BUCKET_GROW_BY 8

using namespace std;

struct pos {
	int position;
	bool exists;

	pos(int p, bool e) : position(p), exists(e) { }
};

int upTwo(int value) {
	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value++;
	return value;
}

class MySet
{
public:
	MySet() {
		int size = 1000;
		int cnt = upTwo(size / BUCKET_SIZE);
		mask = cnt - 1;
		length = 0;
		buckets = new int32_t*[cnt];
		capacity = new int32_t[cnt];
		count = new int32_t[cnt];
		for(int x = 0; x < cnt; x++) {
			buckets[x] = NULL;
			capacity[x] = count[x] = 0;
		}
	}

	~MySet() {
		int size = 1000;
		int cnt = upTwo(size / BUCKET_SIZE);
		for(int x = 0; x < cnt; x++)
			delete[] buckets[x];
		delete[] buckets;
		delete[] count;
		delete[] capacity;
	}

	pos index(int value, int bucket_index) {
		int32_t* bucket = buckets[bucket_index];
		int cnt = count[bucket_index];
		if(cnt == 0)
			return pos(0, false);

		cnt = cnt / 2;
		int val = bucket[cnt];
		if(value == val)
			return pos(cnt, true);

		int offset = 0;
		if(value > val) {
			offset = cnt;
			bucket = bucket + offset;
		}

		int x = 0;
		for(; x < count[bucket_index] - offset; x++) {
			if(bucket[x] < value)
				continue;
			return pos(offset + x, val == value);
		}
		return pos(offset + x + 1, false);
	}

	int getSize() {
		return length;
	}

	int in(int32_t value) {
		pos p = this->index(value, value & mask);
		if(p.exists)
			return 1;
		return -1;
	}

	int ins1(int32_t value) {
		int index = value & mask;
		pos p = this->index(value, index);
		int32_t* bucket = buckets[index];

		// printf("pos: %d\n", p.position);
		if(p.exists)
			return 1;

		if (length >= MAX_LENGTH - 1) 
			return -1; 

		int cnt = count[index];
		if(capacity[index] == cnt) {
			int32_t* newBucket = new int32_t[cnt + BUCKET_GROW_BY];
			capacity[index] += BUCKET_GROW_BY;
			memcpy(newBucket, bucket, cnt * sizeof(int32_t));
			delete[] bucket;
			bucket = newBucket;
		}

		bucket[cnt] = value;
		if(p.position != cnt) {
			for(int x = cnt; x > p.position; x--) 
				bucket[x] = bucket[x - 1];
			// printf("pos: %d, cnt: %d\n", cnt, p.position);
			bucket[p.position] = value;
		}
		buckets[index] = bucket;
		count[index]++;
		length++;
	}

	int res1(int32_t value) {
		int bucket_index = value & mask;
		int32_t* bucket = buckets[bucket_index];
		pos p = index(value, bucket_index);
		if(!p.exists)
			return -1;

		int cnt = count[bucket_index] - 1;
		int32_t buf = bucket[p.position];
		bucket[p.position] = bucket[cnt];
		bucket[cnt] = buf;
		count[bucket_index]--;
		length--;
		return 1;
	}

	int32_t inter1(int32_t a, int32_t b) {
	}

	int interval(int32_t *low, int32_t *high, int32_t *res, int n) {
		for (int i = 0; i < n; i++)
			res[i] = inter1(low[i], high[i]);
		return 1;
	}

	int insert(int32_t *x, int n) {
		int32_t  x_akt;
		for (int i = 0; i < n; i++) {
			x_akt = x[i];
			if (ins1(x_akt) == -1) 
				return -1;
		}
		return 1;
	}

	int remove(int32_t *values, int cnt) {
		int32_t x_akt;
		for (int i = 0; i < cnt; i++) {
			x_akt = values[i];
			if (res1(x_akt) == -1) 
				return -1;
		}
		return 1;
	}

	MySet *Union(MySet *A, MySet *B) {





	// 	  int i;
 //  size_of_list=0;
 //  for(i=0;i<A->getSize();i++) ins1(A->getAt(i));
 //  for(i=0;i<B->getSize();i++) ins1(B->getAt(i));
 //  return this;

	// 		values := make(map[int]struct{}, sets[0].Len())
	// for i := 0; i < sets.Len(); i++ {
	// 	sets[i].Each(func(value int) {
	// 		values[value] = struct{}{}
	// 	})
	// }
	// s := NewSized(len(values))
	// for value := range values {
	// 	s.Set(value)
	// }
	// return s

	}

private:
	int mask;
	int32_t** buckets;
	int* capacity;
	int* count;
	int length;
};


int main(void) {
	int i, j, ok, iteraci = 0;
	int32_t array[10000];
	int32_t x1, x2, low[2000], high[2000], results[2000];
	double start, t_time = 0.0;
	int32_t   k, modulo;
	int size, size2;
	int corr1[4] = {9545, 39545, 69545, 99545};
	int corr2[4] = {9705, 39705, 69705, 99705};
	int corr3[4] = {17434, 71434, 125434, 179434};
	int32_t corr4[4] = { -1369188758, 1116529847, -349759472, 276335712};
	size = -20000;
	iteraci = -1;

	do {
		iteraci++;
		/*printf("size1=%i\n",size);
		fflush(stdout);   */
		MySet *listA = new MySet();
		MySet *listB = new MySet();
		MySet *listC = new MySet();

		size += 30000;
		size2 = std::min(size, 5000);
		modulo = 191 * size;
		if ((size + 5000) >= MAX_LENGTH) break;
		/*printf("size2=%i\n",size);
		fflush(stdout);    */
		for (k = 0; k < ((size + 4999) / 5000); k++)
		{
			for (i = 0; i < size2; i++) array[i] = 3 * (i + 5000 * k); //(3*i-5*k)%modulo;
			start = omp_get_wtime();
			j = listA->insert(array, size2);
			t_time += omp_get_wtime() - start;
			/*printf("size3a=%i %i %i\n",size,j,listA->getSize());
			fflush(stdout);   */
			for (i = 0; i < size2; i++) array[i] = 11 * (i + 5000 * k); //(i-11*k)%modulo;
			start = omp_get_wtime();
			j = listA->remove(array, size2);
			t_time += omp_get_wtime() - start;
			/*printf("size3c=%i %i\n",size,listA->getSize());
			fflush(stdout);   */
			for (i = 0; i < size2; i++) array[i] = 5 * (i + 5000 * k); //(5*i-7*k)%modulo;
			start = omp_get_wtime();
			j = listB->insert(array, size2);
			t_time += omp_get_wtime() - start;
			/*printf("size3b=%i %i %i\n",size,j,listB->getSize());
			fflush(stdout);    */

			for (i = 0; i < size2; i++) array[i] = 17 * (i + 5000 * k); //(13*i-k)%modulo;
			start = omp_get_wtime();
			j = listB->remove(array, size2);
			t_time += omp_get_wtime() - start;
			/*printf("size3d=%i %i\n",size,listB->getSize());
			fflush(stdout);      */
		}
		//printf("size4=%i\n",size);
		/*printf("size1=%i %i\n",size,listA->getSize());
		printf("size2=%i %i\n",size,listB->getSize());
		fflush(stdout);   */
		if (iteraci < 4)
		{
			if (listA->getSize() != corr1[iteraci])
			{
				printf("ERROR1\n");
				// return -1;
			}
			if (listB->getSize() != corr2[iteraci])
			{
				printf("ERROR2\n");
				// return -1;
			}
		}

		if ((listA->getSize() + listB->getSize()) > MAX_LENGTH) break;
		//listA->printSet();
		//listB->printSet();
		start = omp_get_wtime();
		listC->Union(listA, listB);
		t_time += omp_get_wtime() - start;
		/*printf("size5=%i %i\n",size,listC->getSize());
		fflush(stdout);  */

		//listC->printSet();
		if (iteraci < 4)
		{
			if (listC->getSize() != corr3[iteraci])
			{
				printf("ERROR3\n");
				// return -1;
			}
		}
		for (i = 0; i < 2000; i++)
		{
			x1 = (6 * i * i - 1000);
			x2 = (4 * i + 13);
			low[i] = std::min(x1, x2);
			high[i] = std::max(x1, x2);
		}
		start = omp_get_wtime();
		listC->interval(low, high, results, 2000);
		t_time += omp_get_wtime() - start;
		/*printf("size6=%i %i\n",size,listC->getSize());
		fflush(stdout);   */
		//listC->printSet();

		modulo = 0;
		for (i = 0; i < 2000; i++) modulo = 3 * modulo + results[i];

		if (iteraci < 4)
		{
			if (modulo != corr4[iteraci])
			{
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


		ok = 1;
		//if (no_items>=MAXIMAL) ok=0;
		if ((t_time) >= 10.0) ok = 0;
		//ok=0;
	} while (ok);
	printf("\n");
	printf("iterations=%i\n", iteraci);
	printf("end=%i\n", size);
	//if (no_items>=MAXIMAL)
	printf("time=%g\n", t_time);
	//printf("=%i\n",tmp_max);

	//return 1;

	return 0;
}