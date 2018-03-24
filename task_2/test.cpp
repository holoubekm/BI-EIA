#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

int fnBound(int curPrice, int curWeight, int i, int capacity, int aiP[9], int no_items, int aiW[9])
{
	float iUb;
	if (i + 1) 
		iUb = curPrice + (capacity - curWeight) * (aiP[i + 1] / (float)aiW[i + 1]);
	else
		iUb = curPrice;
	return iUb;
}

void fnBranch_bound(int i, int curPrice, int curWeight, int capacity, int aiW[9], int aiPr[9], int no_items)
{
	static int sSol[9];
	static int sFp, sFw;
	int iI, iJ;
	if (curWeight + aiW[i] <= capacity)
	{
		sSol[i] = 1;
		if (i) 
			fnBranch_bound(i + 1, curPrice + aiPr[i], curWeight + aiW[i], capacity, aiW, aiPr, no_items);

		if (i == no_items && (curPrice + aiPr[i]) > sFp){
			printf("\nSolution vectors are : ");
			for (iI = 0; iI < no_items; iI++)
				printf(" %d ", sSol[iI]);
			sFp = curPrice + aiPr[i];
			sFw = curWeight + aiW[i - 1];
			printf("\nProfit is : %d", sFp);
			printf("\nWeight is : %d", sFw);
		}
	}

	if (fnBound(curPrice, curWeight, i, capacity, aiPr, no_items, aiW) > sFp){
		sSol[i] = 0;
		if (i)
			fnBranch_bound(i + 1, curPrice, curWeight, capacity, aiW, aiPr, no_items);

		if (i == no_items && curPrice > sFp){
			sFp = curPrice;
			sFw = curWeight;
			for (iJ = 0; iJ < no_items; iJ++)
				printf(" %d ", sSol[iJ]);
			printf("\nProfit is : %d", sFp);
			printf("\nWeight is : %d", curWeight);
		}
	}
}



int main() {
	int no_items, aiWeight[9], aiProfit[9], iI, iJ, capacity;
	void fnBranch_bound(int i, int curPrice, int curWeight, int capacity, int aiW[9], int aiPr[9], int no_items);

	printf("Knap Sack Problem Solving using branch and bound method.\n");
	printf("How many item do you want : ");
	scanf("%d", &no_items);
	printf("Enter the weight and profit for each item \n");
	printf("Weight Profit\n");
	for (iJ = 0; iJ < no_items; iJ++) 
		scanf("%d%d", &aiWeight[iJ], &aiProfit[iJ]);
	printf("Enter the maximum capacity of knapsack : ");
	scanf("%d", &capacity);
	fnBranch_bound(0, 0, 0, capacity, aiWeight, aiProfit, no_items);

	return 0;
}