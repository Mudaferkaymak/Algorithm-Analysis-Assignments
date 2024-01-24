#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char data;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjacencyList;
};


struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};

struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
int dequeue(struct Queue* queue);
void BFSUtil(int vertex, struct Graph* graph, int* visited, int* groupSize, int currentGroup);
int findConnectedComponents(struct Graph* graph, int minSize);
void BFS(struct Graph* graph, char start, char target, int** edgeMatrix);
struct Node* createNode(char data);
struct Graph* createGraph(int numVertices);
void addEdge(struct Graph* graph, char src, const char* dest);
void printGraph(struct Graph* graph);
void deleteEdge(char line, char col, struct Graph* graph);
void findMaxEdge(int** edgeMatrix, struct Graph* graph,int t);
void implementBFStoAll(struct Graph* graph,int t);
void findMaxEdge(int** edgeMatrix, struct Graph* graph,int t);

int main() {
	int t,k;
	printf("Enter t = ");
	scanf("%d",&t);
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Dosya açma hatasý");
        return 1;
    }

    char line[100];
    struct Graph* graph = createGraph(8);

    while (fgets(line, sizeof(line), file) != NULL) {
        char src;
        char dest[10];

        sscanf(line, "%c:%[^;];", &src, dest);

        addEdge(graph, src, dest);
        
    }
	t++;
    printGraph(graph);
    fclose(file);
	int control = findConnectedComponents(graph, t);
	if(!control){
		implementBFStoAll(graph,t);
	}
		
    // Belleði serbest býrak
    int i;
    for (i = 0; i < graph->numVertices; ++i) {
        struct Node* current = graph->adjacencyList[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
	
	
    free(graph->adjacencyList);
    free(graph);

    return 0;
}
struct Queue* createQueue(unsigned capacity) {
	/**
	 * @brief Creates a new queue with the given capacity.
	 *
	 * @param capacity Capacity of the queue.
	 * @return Pointer to the newly created queue.
	 */
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // Bu sýfýrdan baþlar, böylece ileride artýrýlabilir.
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}
int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}
int isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}
void enqueue(struct Queue* queue, int item) {
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
int dequeue(struct Queue* queue) {
    if (isEmpty(queue))
        return -1; // Hata durumu, kuyruk boþ
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
void BFSUtil(int vertex, struct Graph* graph, int* visited, int* groupSize, int currentGroup) {
	/**
	 * @brief Performs Breadth-First Search (BFS) traversal on a graph.
	 *
	 * @param vertex       The starting vertex for BFS.
	 * @param graph        The graph to be traversed.
	 * @param visited      Array to track visited vertices.
	 * @param groupSize    Array to store the size of connected components.
	 * @param currentGroup Current connected component index.
	 */
    struct Queue* queue = createQueue(graph->numVertices);

    visited[vertex] = 1;
    groupSize[currentGroup]++;
    enqueue(queue, vertex);

    printf("Group %d: ", currentGroup + 1);

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        printf("%c ", currentVertex + 'A'); // Elemaný yazdýr

        struct Node* temp = graph->adjacencyList[currentVertex];
        while (temp != NULL) {
            int neighborIndex = temp->data - 'A';
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = 1;
                groupSize[currentGroup]++;
                enqueue(queue, neighborIndex);
            }
            temp = temp->next;
        }
    }

    printf("\n");
    free(queue->array);
    free(queue);
}
int findConnectedComponents(struct Graph* graph, int minSize) {
	/**
	 * @brief Finds connected components in the graph and prints their sizes.
	 *
	 * @param graph   The graph to find connected components in.
	 * @param minSize The minimum size allowed for a connected component.
	 * @return 1 if there is a group smaller than minSize, 0 otherwise.
	 */
    int* visited = (int*)malloc(graph->numVertices * sizeof(int));
    int* groupSize = (int*)malloc(graph->numVertices * sizeof(int));

    int i, currentGroup = 0;

    for (i = 0; i < graph->numVertices; ++i) {
        visited[i] = 0;
        groupSize[i] = 0;
    }

    for (i = 0; i < graph->numVertices; ++i) {
        if (!visited[i]) {
            BFSUtil(i, graph, visited, groupSize, currentGroup);
            currentGroup++;
        }
    }

    printf("Number of Connected Components: %d\n", currentGroup);
    printf("Connected Component Sizes:\n");
    
    int hasGroupSmallerThanMinSize = 0;
    for (i = 0; i < currentGroup; ++i) {
        printf("Group %d: %d elements\n", i + 1, groupSize[i]);
        if (groupSize[i] < minSize) {
            hasGroupSmallerThanMinSize = 1;
        }
    }

    free(visited);
    free(groupSize);
    
    return hasGroupSmallerThanMinSize;
}
void BFS(struct Graph* graph, char start, char target, int** edgeMatrix) {
	/**
	 * @brief Performs Breadth-First Search (BFS) on the graph to find the shortest path.
	 *
	 * @param graph      The graph to perform BFS on.
	 * @param start      The starting node for BFS.
	 * @param target     The target node to reach.
	 * @param edgeMatrix Matrix to store edge occurrences.
	 */
    int visited[graph->numVertices];
    int parent[graph->numVertices];
    int i;

    for (i = 0; i < graph->numVertices; ++i) {
        visited[i] = 0; // Tüm düðümleri ziyaret edilmemiþ olarak iþaretle
        parent[i] = -1; // Tüm düðümlerin ebeveynini baþlangýçta -1 olarak ayarla
    }

    // Baþlangýç düðümünü iþaretle ve kuyruða ekle
    int startIndex = start - 'A';
    visited[startIndex] = 1;
    struct Queue* queue = createQueue(256);
    enqueue(queue, startIndex);

    while (!isEmpty(queue)) {
        // Kuyruktan bir düðüm çýkar
        int currentVertex = dequeue(queue);

        // Hedef düðüme ulaþýldý mý kontrol et
        if (currentVertex == target - 'A') {
            // En kýsa yolu ekrana bas
            printf("Shortest Path: %c", currentVertex + 'A');
            int parentIndex = parent[currentVertex];
            while (parentIndex != -1) {
                printf(" <- %c", parentIndex + 'A');
                //printf("%d  %d ",parentIndex,currentVertex);
                edgeMatrix[parentIndex][currentVertex]++;
            	edgeMatrix[currentVertex][parentIndex]++;
            	currentVertex = parentIndex;
                parentIndex = parent[parentIndex];
                
            }
            printf("\n");
            free(queue->array);
            free(queue);
            return;
        }

        // Komþu düðümleri kuyruða ekle
        struct Node* temp = graph->adjacencyList[currentVertex];
        while (temp != NULL) {
            int neighborIndex = temp->data - 'A';
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = 1;
                parent[neighborIndex] = currentVertex;
                enqueue(queue, neighborIndex);
            }
            temp = temp->next;
        }
    }

    // Hedef düðüme ulaþýlamadý
    printf("Couldn't reach the target.\n");

    free(queue->array);
    free(queue);
}
struct Node* createNode(char data) {
	/**
	 * @brief Creates a new node with the given data.
	 *
	 * @param data The data for the new node.
	 * @return Pointer to the newly created node.
	 */
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->adjacencyList = (struct Node**)malloc(numVertices * sizeof(struct Node*));
	int i;
    for (i = 0; i < numVertices; ++i)
        graph->adjacencyList[i] = NULL;

    return graph;
}
void addEdge(struct Graph* graph, char src, const char* dest) {
	/**
	 * @brief Adds an edge between source and destination vertices in the graph.
	 *
	 * @param graph The graph to which the edge is added.
	 * @param src   Source vertex.
	 * @param dest  Destination vertices as a string.
	 */
    int srcIndex = src - 'A';

    // Check if the destination is already in the adjacency list of the source
    struct Node* temp = graph->adjacencyList[srcIndex];
    while (temp != NULL) {
        if (strchr(dest, temp->data) != NULL) {
            return; // Connection already exists, exit
        }
        temp = temp->next;
    }

    // Create a new node for each character in dest and add it to the source's adjacency list
    int i;
    for (i = 0; dest[i] != '\0'; ++i) {
        if (dest[i] != ',') {  // Ignore commas
            struct Node* newNode = createNode(dest[i]);
            newNode->next = graph->adjacencyList[srcIndex];
            graph->adjacencyList[srcIndex] = newNode;
        }
    }
}
void printGraph(struct Graph* graph) {
    int i;
    for (i = 0; i < graph->numVertices; ++i) {
        struct Node* temp = graph->adjacencyList[i];
        if (temp != NULL) {
            printf("%c: ", i + 'A');

            while (temp != NULL) {
                printf("%c ", temp->data);
                temp = temp->next;
            }

            printf("\n");
        }
    }
}
void deleteEdge(char line, char col, struct Graph* graph){
	/**
	 * @brief Deletes the edge between the specified vertices in the graph.
	 *
	 * @param line  Source vertex.
	 * @param col   Destination vertex.
	 * @param graph The graph from which the edge is deleted.
	 */
	struct Node* temp = graph->adjacencyList[line];
	struct Node* prev = graph->adjacencyList[line];
	struct Node* cont = graph->adjacencyList[line];
	int found = 0;

	while(!found){
		//printf("%c  data ",temp->data);	
		if(temp != NULL && temp->data==col+'A'  ){
			found=1;
		//	printf("found ici %c",temp->data);
			if(cont == temp){
				graph->adjacencyList[line] = temp->next;
			}else{
			//	printf("else %c", prev->data);
				graph->adjacencyList[line] = prev;
				prev->next = NULL;

			}
			
		//	temp->data = '?';
	
		}else{
			temp = temp->next;
			prev->next=temp;
		}
	
	}
	
}
void implementBFStoAll(struct Graph* graph,int t){
	/**
	 * @brief Implements BFS for all pairs of vertices in the graph.
	 *
	 * @param graph The graph to perform BFS on.
	 * @param t     A parameter for controlling the connected component size.
	 */
	int i,j;
	
	int** edgeMatrix = (int**)malloc(graph->numVertices * sizeof(int*));
    for (i = 0; i < graph->numVertices; ++i) {
        edgeMatrix[i] = (int*)malloc(graph->numVertices * sizeof(int));
        for (j = 0; j < graph->numVertices; ++j) {
            edgeMatrix[i][j] = 0;
        }
    }
    
	for(i=0;i<graph->numVertices;i++){
		struct Node* src = graph->adjacencyList[i];
		for(j=0;j<graph->numVertices;j++){
			if(i != j){
				struct Node* dest = graph->adjacencyList[j];
				if(src != NULL && dest != NULL){
					printf("src:%c dest:%c\n",i + 'A',j + 'A');
 					BFS(graph, i + 'A', j + 'A',edgeMatrix);
				}		
			}
			
		}
	}
	
	printf("\nEdge Matrix:\n");
    for (i = 0; i < graph->numVertices; ++i) {
        for (j = 0; j < graph->numVertices; ++j) {
            printf("%d ", edgeMatrix[i][j]);
        }
        printf("\n");
    }
	
	findMaxEdge(edgeMatrix,graph,t);
    // Belleði serbest býrak
    for (i = 0; i < graph->numVertices; ++i) {
        free(edgeMatrix[i]);
    }
    free(edgeMatrix);
}
void findMaxEdge(int** edgeMatrix, struct Graph* graph,int t){
	/**
	 * @brief Finds the maximum edge in the edgeMatrix and deletes it from the graph.
	 *
	 * @param edgeMatrix Matrix representing edge occurrences.
	 * @param graph      The graph from which the edge is deleted.
	 * @param t          A parameter for controlling the connected component size.
	 */
	int i,j,max = 0,line,col;
	
	for(i=0;i<graph->numVertices;i++){
		for(j=i+1;j<graph->numVertices;j++){
			if(edgeMatrix[i][j] > max){
				max = edgeMatrix[i][j];
				line=i;
				col=j;
			}
		}
	}
	
	
	struct Graph* tempGraph = graph;
	
	deleteEdge(line,col,graph);
	deleteEdge(col,line,graph);
	if(!findConnectedComponents(graph, t)){
		printf("The most visited edge %c-%c has been removed\nUpdated Adjacency List\n",line +'A',col + 'A');
		printGraph(graph);
		implementBFStoAll(graph,t);
	}else{
		printf("Deletion cancelled due to value of t\n");
		printGraph(tempGraph);
	}
	

	
}
