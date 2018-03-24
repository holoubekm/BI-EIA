/*  Short job 2
*/ 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <algorithm>
#define MAXIMAL 120  // maximalni pocet predmetu, maximal size of input

// vstupni data, input data
struct item{
int cost; // cena polozky, cost of item
int weight; // vaha polozky, weight of item
} array_of_items[MAXIMAL];   

int tmp_max;
int no_items,capacity;

char max_state[MAXIMAL];
  /*

  tmp_max = maximalni hodnota ceny batohu
  no_items = pocet veci 
  capacity = nosnost batohu
  max_state = obsah optimalniho batohu
  max_state[i] = 0  kdyz i-ta polozka neni v optimalnim batohu

  
  tmp_max = maximal value of knapsack's price
  no_items = number of items 
  capacity = capacity of knapsack
  max_state = content of the optimal knapsack
  max_state[i] = 0  if i-th item is not in the optimal knapsack
  
  muzete pridat nejake globalni promenne
  it is possible to add some global variables
  */


int testing(char *state)
{
  int i;
  int sc=0;
  int sw=0;
  for(i=0;i<no_items;i++)
    if (state[i]) {
    
    sc+=array_of_items[i].cost;
    sw+=array_of_items[i].weight;
    }
    
  if (sw>capacity) return -1; 
  else return sc;  
}
  
  
// zacatek casti k modifikaci
// beginning of part for modification

// chybi: preproccesing, vetve&hranice, paralelizace, ulozeni obsahu optimalniho batohu do max_state 
// missing: preproccesing, B&B, parallelization, storing of content of optimal knapsack to max_state 
  
#define min(a, b) (a) < (b) ? (a) : (b)
#define max(a, b) (a) > (b) ? (a) : (b)

int cmp(const void* a,const void* b) {
	item* ia = (item*)a;
	item* ib = (item*)b;
	return (ia->cost / (float)ia->weight) < (ib->cost / (float)ib->weight);
}

float maxBenefit(int i, int fr_cap, int price) {

// return 100000000;
	
	// float maxBene = 0;
	// if(i)
	// 	maxBene = fr_cap * (array_of_items[i].cost / (float)array_of_items[i].weight);
	// return price + maxBene;

	float maxBenefit = 0;
	while(i < no_items) {
		if(array_of_items[i].weight <= fr_cap) {
			fr_cap -= array_of_items[i].weight;
			maxBenefit += array_of_items[i].cost;
		} else {
			maxBenefit += array_of_items[i].cost * (fr_cap / (float)array_of_items[i].weight);
			break;
		}
		i++;
	}
	return maxBenefit + price;
}

int rekmy(int i, int fr_cap, int price)
{
	// float maxBene = maxBenefit(i + 1, fr_cap, price);
	// printf("tmp_max: %d\tcap: %d\tp: %d\tmaxBene: %f\n", tmp_max, fr_cap, price, maxBene);
	// if(tmp_max > maxBene)
		// return -1;
	/* i=index aktualniho predmetu
	   fr_cap = volna kapacita v batohu
	   price = aktualni cena batohu */

	// if (i == no_items)
	// {
	// 	if ((price > tmp_max) && (fr_cap >= 0))
	// 	{
	// 		tmp_max = price;
	// 		return price;
	// 	}
	// 	return -1;
	// }
	// int a = rekmy(i + 1, fr_cap - array_of_items[i].weight, price + array_of_items[i].cost);
	// if(a > 0)
	// 	max_state[i] = 1;

	// float maxBene = maxBenefit(i, fr_cap, price);
	// if(maxBene > tmp_max) {
	// 	int b = rekmy(i + 1, fr_cap, price);
	// 	if(b > a) {
	// 		max_state[i] = 0;
	// 		return b;
	// 	}
	// } return a;
	// return 0;


	// if(fr_cap - array_of_items[i].weight >= 0) {

	int retA = -1;
	int retB = -1;
	if(fr_cap - array_of_items[i].weight >= 0) {
		if(i < no_items) {
			retA = rekmy(i + 1, fr_cap - array_of_items[i].weight, price + array_of_items[i].cost);
		}

		if(i == no_items - 1 && price + array_of_items[i].cost > tmp_max) {
			return tmp_max = price + array_of_items[i].cost;
		}
	}

	if(maxBenefit(i, fr_cap, price) > tmp_max) {
		if(i < no_items) {
			retB = rekmy(i + 1, fr_cap, price);
		}

		if(i == no_items - 1 && price > tmp_max) {
			return tmp_max = price;
		}
	}

	if(retA > retB)
		max_state[i] = 1;
	else if(retB > retA)
		max_state[i] = 0;

	return max(retA, retB);
}

void rek(int i, int fr_cap, int price) {
	qsort(array_of_items, no_items, sizeof(item), cmp);
	// for(int x = 0; x < no_items; x++)
		// printf("w: %d\tp: %d\tr: \t%f\n", array_of_items[x].weight, array_of_items[x].cost, array_of_items[x].cost / (double)array_of_items[x].weight);
	// printf("fr_cap: %d\n", fr_cap);
	// printf("maxBenefit: %f\n", maxBenefit(i, fr_cap, price));
	rekmy(i, fr_cap, price);
}
// end of part for modification
// konec casti k modifikaci





int main(void) {
int i,j,ok;
double start,end;
int   d,sum;
int correct[MAXIMAL+1]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4274, 4513, 4887, 4959, 5270, 5544, 5638, 5981, 6106, 6411, 6660, 6983, 7364, 7449, 7909, 8174, 8486, 8662, 8826, 9215, 9357, 9561, 9816, 10074, 10304, 10524, 10902, 11097, 11381, 11759, 12054, 12293, 12456, 12605, 12768, 12970, 13218, 13438, 13491, 13654, 13823, 14150, 14414, 14704, 15093, 15365, 15761, 16002, 16244, 16505, 16723, 16990, 17222, 17395, 17684, 17974, 18367, 18764, 19044, 19295, 19572, 20014, 20294, 20425, 20658, 20868, 21065, 21286, 21498, 21806, 22068, 22556, 22930, 23014, 23275, 23536, 23854, 24130, 24373, 24628, 24833, 25084, 25361, 25501, 25790, 25985, 26243, 26492, 26752, 27222, 27508, 27933, 28361, 28601, 28840, 29156, 29522, 29669, 29836, 30153, 30363, 30609, 30826, 30960,31212};


  no_items=15;
  
  start=omp_get_wtime();
  do{

  no_items++;
  sum=0.0;
  for(j=0;j<no_items;j++)
  {
    i=j+no_items;
    array_of_items[j].cost=201+37*(i%5)+23*(i%7)+5*(i%31)+(i%3);
    array_of_items[j].weight=177+19*(i%13)+7*(i%17)+5*(i%19)+(i%3);
    sum+=array_of_items[j].weight;
  }
  capacity=sum/2; 
   
   
  // reset computation 
  tmp_max=-1;      
 
  /* vylepsete vykonnost tohoto volani !
     improve performance of this call ! */
  rek(0,capacity,0);
  
  /*
  vysledek toho volani:
  tmp_max = maximalni hodnota ceny batohu
  max_state = obsah optimalniho batohu
  max_state[i] = 0  kdyz i-ta polozka neni v optimalnim batohu
  
  Result of this call:
  tmp_max = maximal value of knapsack's price
  max_state = content of the optimal knapsack
  max_state[i] = 0  if i-th item is not in the optimal knapsack
  */
  d= correct[no_items]; 
  if (d!=tmp_max) 
  {
    printf("ERROR1=%i ret=%i correct=%i\n",no_items,tmp_max,d);
    return -1;
  }

  d=testing(max_state);
  if (d!=tmp_max)
  {
    printf("ERROR2=%i cap=%i tmp=%i\n",no_items,d,tmp_max);
    return -1;
  }
  ok=1;
  if (no_items>=MAXIMAL) ok=0;
  end=omp_get_wtime();
  if ((end-start)>=15.0) ok=0;

  }while(ok);
  printf("end=%i\n",no_items);
  printf("time=%g\n",end-start);
  
  
  return 0;
}

