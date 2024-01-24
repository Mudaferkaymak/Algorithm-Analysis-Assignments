#include<stdio.h>

int partition(int[], int, int, int);
void quickSort(int[] , int[] , int, int);
void printArrays(int[] , int[] , int );

int main(){
	
	//inputs
   	int lock[] = {4,5,2,1,3,8,7,6};
   	int key[] = {2,5,1,4,3,7,6};
	
	int m = sizeof(lock)/4;
	int n = sizeof(key)/4;
	
	if(n != m){
		printf("Sizes of 2 arrays should be the same");				// Compare the sizes of 2 arrays in case of wrong input
	}
	else{
		quickSort(lock,key,0,m-1);

		printArrays(lock,key,m);
	}
	

	return 0;
}
int partition(int arr[], int start, int finish, int pivot){
		
	int i = start, j, temp; 
		
	for(j = start ; j < finish ; j++ ){
		
		if( arr[j] < pivot ){										// If current index of the array is smaller than the pivot we should change the locations 
			temp = arr[j];             								
			arr[j] = arr[i];
			arr[i] = temp;
			i++;
		}
		else if( arr[j] == pivot){
			temp = arr[j];											// Pivot should be at the last index, if otherwise we change the locations
			arr[j] = arr[finish];
			arr[finish] = temp;
			j--;													// We need to check current index again so we decrement j
		}
	}
	
	temp = arr[i];													// i holds the place wehere pivot needs to be placed so we change the locations
	arr[i] = arr[finish];
	arr[finish] = temp;
	
	return i;
}
void quickSort(int lock[], int key[], int start, int finish){
	int pivot;
	
	if(start < finish){
		
		pivot = partition(lock, start, finish, key[finish]); 	   // Replace the locks according to last index of keys as pivot and return the pivot index for next use
		
		partition(key, start, finish, lock[pivot]);				   // Replace the keys according to pivot that chosen from locks
		
		quickSort(lock, key, start, pivot -1);                     // Quick Sort for left side of the array
		
		quickSort(lock, key, pivot + 1, finish);				   // Quick Sort for right side of the array
		
	}
}
void printArrays(int lock[], int key[], int length){
	
	int i;
	
	printf("Locks = ");											   
	
	for(i = 0 ; i < length ; i++){
		printf("%d ", lock[i]);
	}
	
	printf("\nKeys = ");
	
	for(i = 0 ; i < length ; i++){
		printf("%d ", key[i]);
	}
	
}
