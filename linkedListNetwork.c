/*
 *  linkedListNetwork.c
 *
 *  Implements an abstractNetwork using adjacency lists (linked lists).
 *  This is a structure with two levels of list. There is a master
 *  list of vertices, linked in series. Each vertex points to a subsidiary
 *  linked list with references to all the other vertices to which it
 *  is connected.
 *
 *  Each vertex has an integer weight and a pointer to a parent vertex 
 *  which can be used for route finding and spanning tree algorithms
 *
 *  Key values are strings and are copied when vertices are inserted into
 *  the graph. Every vertex has a void* pointer to ancillary data which
 *  is simply stored. 
 *
 *  Modified by Sritdyot Sukkasikorn(To) ID.3475
 *  1 April 2018
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"
#include "abstractQueue.h"
#include "minPriorityQueue.h"
#include "hospital.h"

#define WHITE 0
#define GRAY  1
#define BLACK 2

char* colorName[] = {"WHITE", "GRAY", "BLACK"};

/* List items for the adjacency list.
 * Each one is a reference to an existing vertex
 */
typedef struct _adjacent
{
    void * pVertex;           /* pointer to the VERTEX_T this 
                               * item refers to.
                               */
    char roadName[128];  
    char district[64];                         
    unsigned int weight;      /* weight of this edge */

    struct _adjacent * next;  /* next item in the ajacency list */ 
} ADJACENT_T;

/* List items for the main vertex list.*/
typedef struct _vertex
{
    char * key;               /* key for this vertex */
    char district[64];
    char data[64];              /* ancillary data for this vertex */
    int color;                /* used to mark nodes as visited */
    int dValue;               /* sum of weights for shortest path so far to this vertex */
    struct _vertex * parent;  /* pointer to parent found in Dijkstra's algorithm */     
    struct _vertex * next;    /* next vertex in the list */
    ADJACENT_T * adjacentHead;    /* pointer to the head of the
		               * adjacent vertices list
                               */
    ADJACENT_T * adjacentTail;    /* pointer to the tail of the
			       * adjacent vertices list
                               */
}  VERTEX_T;


VERTEX_T * vListHead = NULL;  /* head of the vertex list */
VERTEX_T * vListTail = NULL;  /* tail of the vertex list */
int bGraphDirected = 0;       /* if true, this is a directed graph */


/** Private functions */

/* Free the adjacencyList for a vertex 
 * Argument
 *   pVertex    - vertex whose edges we want to delete 
 */
void freeAdjacencyList(VERTEX_T *pVertex)
{
   ADJACENT_T * pCurRef = pVertex->adjacentHead;
   while (pCurRef != NULL)
      {
      ADJACENT_T * pDelRef = pCurRef;
      pCurRef = pCurRef->next;
      free(pDelRef);
      }
   pVertex->adjacentHead = NULL;
   pVertex->adjacentTail = NULL;
}

/* Check if there is already an edge between
 * two vertices. We do not want to add a duplicate.
 * Arguments
 *   pFrom        -  Start point of edge
 *   pTo          -  End point of edge
 * Return 1 if an edge already exists, 0 if it does
 * not.
 */
int edgeExists(VERTEX_T* pFrom, VERTEX_T* pTo)
{
    int bEdgeExists = 0;
    ADJACENT_T * pCurRef = pFrom->adjacentHead;
    while ((pCurRef != NULL) && (!bEdgeExists))
       {
       if (pCurRef->pVertex == pTo)
          {
	  bEdgeExists = 1;  /* the 'To' vertex is already in the
                             * 'From' vertex's adjacency list */ 
          }
       else
          {
	  pCurRef = pCurRef->next;
          }
       } 
    return bEdgeExists;
}

/* Color all vertices to the passed color.
 * Argument
 *    A color constant
 */
void colorAll(int color)
{
    VERTEX_T* pVertex = vListHead;
    while (pVertex != NULL)
       {
       pVertex->color = color;
       pVertex = pVertex->next;
       }
}

/* Initialize the dValue and parent for all
 * vertices. dValue should be very big, parent
 * will be set to NULL. Also add to the minPriority queue.
 */
void initAll()
{
    VERTEX_T* pVertex = vListHead;
    while (pVertex != NULL)
       {
       pVertex->dValue = 999999999;
       pVertex->parent = NULL;
       enqueueMin(pVertex);
       pVertex = pVertex->next;
       }
}


/* Execute a breadth first search from a vertex,
 * calling the function (*vFunction) on each vertex
 * as we visit it and color it black.
 * Arguments
 *    pVertex    -  starting vertex for traversal
 */
void traverseBreadthFirst(VERTEX_T* pVertex, void (*vFunction)(VERTEX_T*))
{
    VERTEX_T * pCurrentVertex = NULL;
    VERTEX_T * pAdjVertex = NULL;    
    queueClear();
    colorAll(WHITE);
    pVertex->color = GRAY;
    enqueue(pVertex);
    while (queueSize() > 0)
       {
       pCurrentVertex = (VERTEX_T*) dequeue();
       if (pCurrentVertex->color != BLACK)
          {
          (*vFunction)(pCurrentVertex);
          pCurrentVertex->color = BLACK;
	  ADJACENT_T* pAdjacent = pCurrentVertex->adjacentHead;
	  while (pAdjacent != NULL)
             {
	     pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
	     if (pAdjVertex ->color != BLACK)
	         {
		 pAdjVertex->color = GRAY;
		 enqueue(pAdjVertex);
	         }
	     pAdjacent = pAdjacent->next;
             }
	  }
       } /* end while queue has data */
}


/* Execute a breadth first search from a single vertex,
 * calling the function (*vFunction) on the lowest level
 * vertex we visit, and coloring it black.
 * Arguments
 *    pVertex    -  starting vertex for traversal
 */
void traverseDepthFirst(VERTEX_T* pVertex, void (*vFunction)(VERTEX_T*))
{
    VERTEX_T * pAdjVertex = NULL;    
    ADJACENT_T* pAdjacent = pVertex->adjacentHead;
    while (pAdjacent != NULL)
       {
       pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
       if (pAdjVertex->color == WHITE)
	   {
	   pAdjVertex->color = GRAY;
           traverseDepthFirst(pAdjVertex,vFunction);
           }
       pAdjacent = pAdjacent->next;  
       } /* end while queue has data */
    /* when we return from the bottom, call the 
     * function and color this node black.
     */
    (*vFunction)(pVertex);
    pVertex->color = BLACK;
}

/********************************/
/** Public functions start here */
/********************************/

/* Initialize or reintialize the graph.
 * Argument 
 *    maxVertices  - how many vertices can this graph
 *                   handle.
 *    bDirected    - If true this is a directed graph.
 *                   Otherwise undirected.
 * Returns 1 unless there is a memory allocation error,
 * in which case it returns zero.
 */
int initGraph(int maxVertices, int bDirected)
{ 
    /* for a linked list graph, we call
     * clearGraph and then initialize bGraphDirected
     */
  //  clearGraph();
    bGraphDirected = bDirected;
    return 1;  /* this implementation of initGraph can never fail */ 
}

/* Finds the vertex that holds the passed key
 * (if any) and returns a pointer to that vertex.
 * Arguments
 *       key    -  Key we are looking for
 *       pPred  -  used to return the predecessor if any
 * Returns pointer to the vertex structure if one is found       
 */
VERTEX_T * findVertexByKey(char* key, VERTEX_T** pPred) 
{
    VERTEX_T * pFoundVtx = NULL;
    VERTEX_T * pCurVertex = vListHead;
    *pPred = NULL;
    /* while there are vertices left and we haven't found
     * the one we want.
     */
    while ((pCurVertex != NULL) && (pFoundVtx == NULL))
       {
       if (strcmp(pCurVertex->key,key) == 0)
          {
    pFoundVtx = pCurVertex;
    }
       else
          {
    *pPred = pCurVertex;
          pCurVertex = pCurVertex->next;
          }
       }
    return pFoundVtx;
}

/* Finds the vertex that holds the passed key
 * (if any) and returns a pointer to that vertex.
 * Arguments
 *       key    -  Key we are looking for
 *       pPred  -  used to return the predecessor if any
 * Returns pointer to the vertex structure if one is found       
 */
VERTEX_T * findRoadPointer(char* key, VERTEX_T** pPred) 
{
    VERTEX_T * pFoundVtx = NULL;
    VERTEX_T * pCurVertex = vListHead;
    *pPred = NULL;
    /* while there are vertices left and we haven't found
     * the one we want.
     */
    while ((pCurVertex != NULL) && (pFoundVtx == NULL))
       {
       if (strcmp(pCurVertex->key,key) == 0)
          {
    pFoundVtx = pCurVertex;
    }
       else
          {
    *pPred = pCurVertex;
          pCurVertex = pCurVertex->next;
          }
       }
    return pFoundVtx;
}


/* Add a vertex into the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex
 *     pData -   Additional information that can
 *               be associated with the vertex.
 * Returns 1 unless there is an error, in which case
 * it returns a 0. An error could mean a memory allocation 
 * error or running out of space, depending on how the 
 * graph is implemented. Returns -1 if the caller tries
 * to add a vertex with a key that matches a vertex
 * already in the graph.
 */
int addVertex(char* key, char* Data,char district[64])
{
    int bOk = 1;
    VERTEX_T * pPred;
    VERTEX_T * pFound = findVertexByKey(key, &pPred);
    if (pFound != NULL)  /* key is already in the graph */
       {
       bOk = -1;
       }
    else
       {
       VERTEX_T * pNewVtx = (VERTEX_T *) calloc(1,sizeof(VERTEX_T));
       char * pKeyval = strdup(key);
       if ((pNewVtx == NULL) || (pKeyval == NULL))
          {
	  bOk = 0;  /* allocation error */
	  }
       else
          {
	  pNewVtx->key = pKeyval;
          strcpy(pNewVtx->data,Data);
          strcpy(pNewVtx->district,district);
	  if (vListHead == NULL)  /* first vertex */
	     {
	     vListHead = pNewVtx;
	     }
	  else
	     {
	     vListTail->next = pNewVtx; 
	     }
	  vListTail = pNewVtx;
	  }
       }
    return bOk;
}


/* Add an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 *    weight - weight for this edge
 * Returns 1 if successful, 0 if failed due to
 * memory allocation error, or if either vertex
 * is not found. Returns -1 if an edge already
 * exists in this direction.
 */
int addEdge(char* key1, char* key2, unsigned int weight,char Name[64],char district[64])
{
    int bOk = 1;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFromVtx = findVertexByKey(key1,&pDummy);
    VERTEX_T * pToVtx = findVertexByKey(key2,&pDummy);
    if ((pFromVtx == NULL) || (pToVtx == NULL))
       {
       bOk = 0;
       }
    else if (edgeExists(pFromVtx,pToVtx))
       {
       bOk = -1;	   
       }
    else
       {
       ADJACENT_T * pNewRef = (ADJACENT_T*) calloc(1,sizeof(ADJACENT_T));
       if (pNewRef == NULL)
          {
	  bOk = 0;
          }
       else
          {
	  pNewRef->pVertex = pToVtx;
          pNewRef->weight = weight;
          strcpy(pNewRef->roadName,Name);
          strcpy(pNewRef->district,district); 
	  if (pFromVtx->adjacentTail != NULL)
              {
	      pFromVtx->adjacentTail->next = pNewRef;
	      }
          else
	      {
	      pFromVtx->adjacentHead = pNewRef;
	      }
	  pFromVtx->adjacentTail = pNewRef;
          } 
       } 
    /* If undirected, add an edge in the other direction */
    if ((bOk) && (!bGraphDirected))
       {
       ADJACENT_T * pNewRef2 = (ADJACENT_T*) calloc(1,sizeof(ADJACENT_T));
       if (pNewRef2 == NULL)
          {
	  bOk = 0;
          }
       else
          {
	  pNewRef2->pVertex = pFromVtx;
          pNewRef2->weight = weight;
          strcpy(pNewRef2->roadName,Name);
          strcpy(pNewRef2->district,district);  
	  if (pToVtx->adjacentTail != NULL)
              {
	      pToVtx->adjacentTail->next = pNewRef2;
	      }
          else
	      {
	      pToVtx->adjacentHead = pNewRef2;
	      }
	  pToVtx->adjacentTail = pNewRef2;
          } 
       } 
    return bOk;
}

VERTEX_T * findVertexByName(char* name, VERTEX_T** pPred) 
{
    VERTEX_T * pFoundVtx = NULL;
    VERTEX_T * pCurVertex = vListHead;
    *pPred = NULL;
    /* while there are vertices left and we haven't found
     * the one we want.
     */
    while ((pCurVertex != NULL) && (pFoundVtx == NULL))
       {
       if (strcmp(pCurVertex->data,name) == 0)
          {
    pFoundVtx = pCurVertex;
    }
       else
          {
    *pPred = pCurVertex;
          pCurVertex = pCurVertex->next;
          }
       }
    return pFoundVtx;
}

/* Find a vertex and return its data
 * Arguments
 *    key  -  Key for the vertex to find
 * Returns the data for the vertex or NULL
 * if not found.
 */
void* findVertex(char* key1)
{
    void* pData = NULL;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFoundVtx = findVertexByName(key1,&pDummy);
    if (pFoundVtx != NULL)
       {
       pData = pFoundVtx->key;
       }
    return pFoundVtx->key; 
}

/* Find the edge between two vertices (if any) and return
 * its weight
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns weight if successful and edge exists.
 * Returns -1 if an edge is not found
 */
int findEdge(char* key1, char* key2)
{
    int weight = -1;
    int bEdgeExists = 0;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFrom = findVertexByKey(key1,&pDummy);	
    ADJACENT_T * pCurRef = pFrom->adjacentHead;
    while ((pCurRef != NULL) && (!bEdgeExists))
       {
       VERTEX_T * pFrom = (VERTEX_T*) pCurRef->pVertex;
       if (strcmp(pFrom->key,key2) == 0)
          {
	  weight = pCurRef->weight;  
          bEdgeExists = 1;
          }
       else
          {
	  pCurRef = pCurRef->next;
          }
       } 
    return weight;
}

/* Comparison function to send to the minPriorityQueue
 * Arguments
 *   pV1     First vertex (will be cast to VERTEX_T *)
 *   pV2     Second vertex (will be cast to VERTEX_T *)
 * Compares dValues. Returns -1 if V1 < V2, 0 if dValues are
 * the same, 1 if V1 > V2.
 */
int compareVertices(void * pV1, void * pV2)
{
   VERTEX_T * pVertex1 = (VERTEX_T*) pV1;
   VERTEX_T * pVertex2 = (VERTEX_T*) pV2;
    if (pVertex1->dValue < pVertex2->dValue)
        return -1;
    else if (pVertex1->dValue > pVertex2->dValue)
        return 1;
    else 
        return 0;
}



/*
 * Empty function
 */
void emptyFunction()
{

}

/* Print out the lowest weight path from one vertex to 
 * another through the network using Dijkstra's
 * algorithm. 
 * Arguments
 *    startKey    -  Key of start vertex
 *    endKey      -  Key of ending vertex
 * Returns the sum of the weights along the path.
 * Returns -1 if either key is invalid. Returns -2
 * if network is not directed.
 */
int printShortestPath(char* startKey, char* endKey)
{
    VERTEX_T * start = NULL; /*starting vertex*/
    VERTEX_T * end = NULL;   /*end vertex*/
    VERTEX_T * pDummy = NULL;
    VERTEX_T * current=NULL; /*current vertex*/
    ADJACENT_T * pAdjacent =NULL; /*edge of adjacent*/
    VERTEX_T * pAdjVertex = NULL; /*vertex of adjacent*/
    int i = 0;

    start = findVertexByKey(startKey,&pDummy); /*find vertex*/
    end = findVertexByKey(endKey,&pDummy);
    traverseDepthFirst(start,&emptyFunction); /*check reachable*/
    
    /*Dijkstra*/
    colorAll(WHITE);
    queueMinInit(&compareVertices);
    initAll();
    start->dValue = 0;
    while(queueMinSize() > 0)
        {
        current = dequeueMin();
        current->color=BLACK;
        pAdjacent = current->adjacentHead;
        while (pAdjacent != NULL) /*if adjacent is not NULL*/
            {
            pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
            if(( (current->dValue) + (pAdjacent->weight) ) < (pAdjVertex->dValue))
                {
                pAdjVertex->dValue=current->dValue+pAdjacent->weight;
                pAdjVertex->parent=current;
                }
            pAdjacent = pAdjacent->next; /*next adjacent*/
            } 
        }

    colorAll(WHITE);
    printf(".");
    
    for(i = 0; i < 100;i++){}
    return end->dValue;
}
