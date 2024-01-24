#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int getMax(int a, int b);
void freeMatrix(int** matrix, int rows);
int getDir(int, int);
int** fillMatrix(const char*, const char*);
void printMatrix(int**, int, int);
void printSubSeq(const char *subSeq);
int isSubsequencePrinted(const char *, const char **, int );
void backTrack(int **, const char *, const char *, int , int , char *, int *, const char **, int *);

int main() {
    char first[25], second[25];
    int i;

    printf("Enter the first String = ");
    scanf("%s", first);
    printf("\nEnter the second String = ");
    scanf("%s", second);

    int **matrix = fillMatrix(first, second);
    printMatrix(matrix, strlen(first), strlen(second));

    char subSeq[25] = "";
    const char *printedSubsequences[25]; 
    int numPrinted = 0;

    int maxLength = matrix[strlen(first)][strlen(second)];
    printf("\nLongest Common Subsequence Length = %d",maxLength);
    printf("\nLongest Common Subsequences\n");
    backTrack(matrix, first, second, strlen(first), strlen(second), subSeq, &maxLength, printedSubsequences, &numPrinted);
	
    //free memory
    for (i = 0; i < numPrinted; i++) {
        free((char *)printedSubsequences[i]);
    }

    return 0;
}
int** fillMatrix(const char* first, const char* second){
	/** 
	* @brief Fills 2 matrices one with dynammic programming and the other one shows the path
	*
	* @param first  First string.
	* @param second Second string.
	* @return The filled matrix.
	*/
	int i,j,k;
	char enter;
	int** matrix = (int**)malloc((strlen(first) + 1) * sizeof(int*));
	int** secMatrix = (int**)malloc((strlen(first)+1) * sizeof(int*));
    for (i = 0; i < strlen(first) + 1; i++) {
    	secMatrix[i] = (int*)malloc((strlen(second) + 1) * sizeof(int));
        matrix[i] = (int*)malloc((strlen(second) + 1) * sizeof(int));
    }
	for(i=0 ; i<(strlen(first) + 1) ; i++){
		printf("\n\n%d. row of dynamic matrix = ",i+1);
		for(j=0 ; j < (strlen(second)+1) ; j++){
			if(!i || !j){
				// first row and column should be filled with zeros
				matrix[i][j] = 0;
				secMatrix[i][j] = 0;
			}
			else if(first[i-1] == second [j-1]){
				// if chars match we take the value from left cross and add 1
				matrix[i][j] = matrix [i-1][j-1] + 1;
				// if we come from left cross the path should be 3 ( it is my encoding)
				secMatrix[i][j] = 3;
			}
			else{
				matrix[i][j] = getMax(matrix[i-1][j],matrix[i][j-1]);     // if chars doesnt match get the max value from top or left
				secMatrix[i][j] = getDir(matrix[i-1][j],matrix[i][j-1]);  // 1-> skipY  || 0 -> skipX
			}
			printf("%d ",matrix[i][j]);
		}
		
		printf("\n%d. row of direction matrix = ",i+1);
		for(k=0;k<(strlen(second) + 1); k++){
			printf("%d ",secMatrix[i][k]);
		}
			
	} 
	enter = getchar();
	printf("\n\nPress Enter to see matrices and LCS!");
	enter = getchar();
	printf("\n\nDirection Matrix\n(0->skipX || 1->skipY || 2->skipX and skipY || 3->leftCross)\n");
	for (i = 0; i < strlen(first)+1; i++) {
        for (j = 0; j < strlen(second)+1; j++) {
            printf("%d ", secMatrix[i][j]);
        }
        printf("\n");
    }
	return matrix; 
}
void printMatrix(int** matrix, int rows, int cols) {
	
	int i,j;
	
	printf("\nDynamic Matrix \n");
    for (i = 0; i < rows+1; i++) {
        for (j = 0; j < cols+1; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
void backTrack(int **matrix, const char *first, const char *second, int curLine, int curCol, char *subSeq, int *maxLength, const char **printedSubsequences, int *numPrinted) {
	/**
	* @brief Backtracks through the matrix to find and print the longest common subsequences.
	*
	* @param matrix               Dynamic programming matrix.
	* @param first                First string.
	* @param second               Second string.
	* @param curLine              Current line in the matrix.
	* @param curCol               Current column in the matrix.
	* @param subSeq               Current subsequence being constructed.
	* @param maxLength            Maximum length of common subsequences.
	* @param printedSubsequences Array of already printed subsequences.
	* @param numPrinted           Number of subsequences already printed.
	*/
	int i;
	
    if (curLine == 0 || curCol == 0) {
    //if matrix is done	
        if (strlen(subSeq) == *maxLength && !isSubsequencePrinted(subSeq, printedSubsequences, *numPrinted)) {
        	
            for (i = strlen(subSeq) - 1; i >= 0; i--) {
                printf("%c", subSeq[i]);
            }
            printf("\n");

            printedSubsequences[*numPrinted] = strdup(subSeq);
            (*numPrinted)++;
        }
        return;
    }
	
    if (first[curLine - 1] == second[curCol - 1]) {
    	//if chars match it means we are coming from left cross so we call this funciton for this cell
        subSeq[strlen(subSeq)] = first[curLine - 1];
        backTrack(matrix, first, second, curLine - 1, curCol - 1, subSeq, maxLength, printedSubsequences, numPrinted);
        subSeq[strlen(subSeq) - 1] = '\0'; // Backtrack: remove the last character
    } else if (matrix[curLine - 1][curCol] > matrix[curLine][curCol - 1]) {
    	//if chars doesnt match and top cell is bigger than other it means we come from there so call function for this cell
        backTrack(matrix, first, second, curLine - 1, curCol, subSeq, maxLength, printedSubsequences, numPrinted);
    } else if (matrix[curLine][curCol - 1] > matrix[curLine - 1][curCol]) {
        backTrack(matrix, first, second, curLine, curCol - 1, subSeq, maxLength, printedSubsequences, numPrinted);
    } else {
    	// if top and left is the same we call function for both of them in case of different LCS
        backTrack(matrix, first, second, curLine, curCol - 1, subSeq, maxLength, printedSubsequences, numPrinted);
        backTrack(matrix, first, second, curLine - 1, curCol, subSeq, maxLength, printedSubsequences, numPrinted);
    }
}
int isSubsequencePrinted(const char *subSeq, const char **printedSubsequences, int numPrinted) {
	/**
	* @brief Checks if a subsequence is already printed.
	*
	* @param subSeq               Subsequence to be checked.
	* @param printedSubsequences Array of already printed subsequences.
	* @param numPrinted           Number of subsequences already printed.
	* @return 1 if subsequence is printed, 0 otherwise.
	*/
	int i;
    for (i = 0; i < numPrinted; i++) {
        if (strcmp(subSeq, printedSubsequences[i]) == 0) {
            return 1; // Subsequence is already printed
        }
    }
    return 0; // Subsequence is not printed yet
}
void printSubSeq(const char *subSeq) {
    if (strlen(subSeq) > 0) {
        printf("%s\n", subSeq);
    }
}
int getDir(int a, int b){
	if(a >b){
		return 1;
	}
	else if(a < b){
		return 0;
	}else{
		return 2;
	}
}
int getMax(int a, int b){
	if(a>b){
		return a;
	}else{
		return b;
	}
}
void freeMatrix(int** matrix, int rows) {
	int i;
    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
