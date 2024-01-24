#include<stdio.h>
#include <stdbool.h>
#include<string.h>
#include<stdlib.h>

typedef struct data{
	char name[25];
	int deleted;
}DATA;

bool isPrime(int n);
int findNextPrime(int n);
int calculateTableSize(int elementCount, float loadFactor);
unsigned long hashOne(long key,long m);
unsigned long hashTwo(long key,long m);
unsigned long hashMain(long key,long m,int i,int mod);
void printMenu();
unsigned long hornerRule(const char *str);
void addData(char* name,long m, DATA* memory,int mod);
void getData(int m,DATA* memory,int mod);
int searchData(DATA* memory, int size,char* name,int mod);
void deleteData(DATA* memory, int size,char* name,int mod);
void clearTheMemory(DATA *memory, int size);
void printTable(DATA* memory, int size);
DATA* rehash(DATA* oldMemory, int size, int mod);
 
int main(){
	int N,selected=0,i,cur,mod;
	long m;
	float loadFactor;
	char name[25];
	printf("Enter N = ");
	scanf("%d",&N);
	
	printf("\nEnter Load Factor = ");                                                        
	scanf("%f",&loadFactor);
	
	m = calculateTableSize(N,loadFactor);
	DATA memory[m];
	clearTheMemory(memory,m);
	do{
		printf("SELECT THE MODE\n1-> Normal Mode\n2-> Debug Mode\n");
		scanf("%d",&mod);
		system("cls");
	}while(mod<1 && mod>2);

	do{
		printMenu();
		scanf("%d",&selected);
		if(selected == 1){
			getData(m,memory,mod);
		}
		else if(selected == 2){
			printf("Enter the String Value You Want to Search = ");
			scanf("%s",name);
			cur = searchData(memory,m,name,mod);
			if(cur == m)
				printf("Data has not found\n");
		}
		else if(selected == 3){
			printf("Enter the String Value You Want to Delete = ");
			scanf("%s",name);			
			deleteData(memory,m,name,mod);			
		}
		else if(selected == 4){
			printTable(memory,m);
		}
		else if(selected == 5){
			DATA* rehashed = rehash(memory, m, mod);
			printf("REHASHED MEMORY\n");
			printTable(rehashed,m);
			printf("Rehashing is succesful.\n");
			memcpy(memory, rehashed, sizeof(DATA) * (m));
			free(rehashed);
		}
			
	}while(selected>0 & selected<6);
	
	return 0;
}	
void printTable(DATA* memory, int size){
	int i;
	for(i=0;i<size;i++){
		if(memory[i].deleted == 0)
			printf("%d -> %s\n",i,memory[i].name);
	}
}
void clearTheMemory(DATA *memory, int size) {
	int i;
	// assing delete attribute to 1 to specify all table is empty
    for (i = 0; i < size; ++i) {
        memory[i].deleted = 1;
    }
}
int calculateTableSize(int elementCount, float loadFactor) {
    int minimumTableSize = elementCount / loadFactor;
    return findNextPrime(minimumTableSize);
}
int findNextPrime(int n) {
    while (!isPrime(n)) {
        n++;
    }
    return n;
}
bool isPrime(int n) {
	int i;
    if (n <= 2) {
        return false;
    }
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
void getData(int m,DATA* memory,int mod){
	/**
	* Get data from the user and add it to the hash table.
	*
	* @param m The size of the hash table.
	* @param memory The hash table data structure.
	* @param mod The mode (normal or debug).
	*/
	int value;
	DATA* newData; 
	// allocate memory for data 
	newData = (DATA*)malloc(sizeof(DATA));
	printf("Enter the String Value = ");
	scanf("%s",newData->name);
	newData->deleted = 0;
	//call function to add data into table
	addData(newData->name,m,memory,mod);
	
}
void printMenu(){
	printf("1-> Add Data to Hash\n2-> Search Data on Hash\n3-> Delete Data from Hash\n4-> Show the Table\n5-> Rehash the table\n");
}
void addData(char* name,long m, DATA* memory,int mod){
	/**
	* Add data to the hash table.
	*
	* @param name The string data to be added.
	* @param m The size of the hash table.
	* @param memory The hash table data structure.
	* @param mod The mode (normal or debug).
	*/
	int succeed = 0,i = 0,location;
	long value;
	// Implement horner method to string
	value = hornerRule(name);	
	// if word is not already in table searchData returns the table size which is 'm'
	if(searchData(memory,m,name,mod) == m){
		while(!succeed && i<m){
			//Implement hash function to get pos in memory
			location = hashMain(value,m,i,mod);
			//if related space is empty fill it with the current data
			if(memory[location].deleted == 1){
				memory[location].deleted = 0;
				strcpy(memory[location].name,name);
				succeed = 1; 
				printf("Data has been added to the address %d\n",location);
			}
			if(mod == 2 && succeed == 0)
				//in debug mode it prints if the location is already filled with different data
				printf("The word %s is already at the address %d\n",memory[location].name,location);
			i++;
	}	
	if(i == m){
		printf("No Space in Table\n");
	}
	}else{
		printf("The data is already in table. \n");
	}
	
}
void deleteData(DATA* memory, int size,char* name,int mod){
	/**
	* Delete data from the hash table.
	*
	* @param memory The hash table data structure.
	* @param size The size of the hash table.
	* @param name The string data to be deleted.
	* @param mod The mode (normal or debug).
	*/
	int location,i = 0,found=0;
	long value;
	//implement the horner method
	value = hornerRule(name);
	while(i<size && found == 0){
		//Implement hash function to get pos in memory
		location = hashMain(value,size,i,mod);
		//if strings match and the one in the memors is not deleted, delete the data
		if(strcmp(memory[location].name,name) == 0 && memory[location].deleted == 0){
			printf("The data has been found at the address %d\n",location);
			memory[location].deleted = 1;
			printf("'%s' has deleted from the table\n",name);
			found =1;
		}
		//for debug mode
		if(mod == 2 && found == 0)
			printf("The Word '%s' was not found in address %d\n",name,location);
		i++;
	}
	//if data is not exist in memory i parameter will be the same as size
	if(i == size)
		printf("Data has not found\n");
}
int searchData(DATA* memory, int size,char* name,int mod){
	/**
	* Search for data in the hash table.
	*
	* @param memory The hash table data structure.
	* @param size The size of the hash table.
	* @param name The string data to search for.
	* @param mod The mode (normal or debug).
	* @return The index where the data is found or the size of the hash table if not found.
	*/	
	int location,i = 0,found=0;
	long value;
	//Implement hash function to get pos in memory
	value = hornerRule(name);
	if(mod == 2){
		printf("h1 = %lu\n",hashOne(value,size));
		printf("h2 = %lu\n",hashTwo(value,size));
	}	
	while(i<size && found == 0){
		location = hashMain(value,size,i,mod);
		//if strings match and the one in the memors is not deleted, delete the data
		if(strcmp(memory[location].name,name) == 0 && memory[location].deleted == 0){
			printf("The word '%s' has been found at the address %d\n",name,location);			
			found =1;
		}
		if(mod == 2 && found ==0)
			printf("The Word '%s' was not found in address %d\n",name,location);
		i++;
	}

	return i;
}
unsigned long hornerRule(const char *str) {
    unsigned long hash = 0;

    while (*str) {

        hash = (hash * 31) + (unsigned char)(*str);
        str++;
    }
    return hash;
}
unsigned long hashMain(long key,long m,int i,int mod){	
	return (hashOne(key,m) + (i*hashTwo(key,m))) % m;   
}
unsigned long hashOne(long key,long m){
	return (unsigned long)key % m; 
}
unsigned long hashTwo(long key,long m){
	return 1 + ((unsigned long)key % (m-2));
}

DATA* rehash(DATA* oldMemory, int size, int mod) {
	/**
	* Rehash the hash table to a new size.
	*
	* @param oldMemory The old hash table data structure.
	* @param size The size of the old hash table.
	* @param mod The mode (normal or debug).
	* @return The new hash table data structure after rehashing.
	*/
    DATA* newMemory = (DATA*)malloc(size * sizeof(DATA));
    clearTheMemory(newMemory, size);
    unsigned long value;
	int i,location,found = 0,j;
	//Check all the elements in old memory and if it is not deleted, implement the hash function again and place them into new memory
    for (i = 0; i < size; i++) {
        if (oldMemory[i].deleted == 0) {
        	j=0;
        	found = 0;
            value = hornerRule(oldMemory[i].name);
            while(!found){
            	location = hashMain(value,size,j,mod);
            	if(newMemory[location].deleted == 1){
            		newMemory[location].deleted = 0;
            		strcpy(newMemory[location].name,oldMemory[i].name);
            		found = 1;
				}
            	j++;
			}
            
        }
    }

    return newMemory;
}
