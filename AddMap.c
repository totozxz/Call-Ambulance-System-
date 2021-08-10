/*****************************************************
*   AddMap.c
*   use to add a data to build a map.
*   
*   Created by Jee 3457, Toto 3475
*
******************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"

/*function use to created Vertex
 *Arguments
 *input - use to hold data that sent to make Edge
 */
void Makevertex(char input[128])
{
    char key1[64];      /*hold data about keyword of vertex*/
    char Name[64];      /*hold data about name of crossroad*/
    char Name2[64];     /*hold data about name of district*/
    char hold[32];      /*hold data to tell us this is vertex or edge data*/
    char* key = NULL;   /*hold pointer of keyword data*/
    char* data = NULL;  /*hold pointer of crossroad name data*/
    int check = 0;      /*use is check function correct*/

    sscanf(input,"%s %s %s %s",hold,key1,Name,Name2);
    key = strdup(key1);
    data = strdup(Name);
    /*check key1 and Name are got data*/
    if((strlen(key1) == 0) || (strlen(Name) == 0))
    {
        printf("Error : key or data is empty.\n");
        free(key);
        free(data);
    }
    else
    {  
        /*create vertex*/
        check = addVertex(key,data,Name2);
    }
    /*create sucess*/
    if (check == 1)
    {
        /*printf("Vertex |%s| added\n",key);*/
    }
    /*create fail*/
    else if (check < 0)
    {
        printf(">>> Vertex with key |%s| already exists in graph\n", key);
    }
    else
    {
        printf(">>> Memory allocation error or graph full!\n");
    }
}

/*function use to created Edge
 *Arguments
 *input - use to hold data that sent to make Edge
 */
void makeEdge(char input[128])
{
    char key1[64];      /*hold data about head of edge*/
    char key2[64];      /*hold data about tail of edge*/
    char Name[64];      /*hold data about Road name*/
    char Name2[64];     /*hold data about district name*/
    char hold[32];      /*hold data to tell us this is vertex or edge data*/
    int weight = 0;     /*hold weight of edge*/
    int check = 0;      /*use is check function correct*/

    sscanf(input,"%s %s %s %d %s %s",hold,key1,key2,&weight,Name,Name2);
    /*check key1 and key2 are got data*/
    if((strlen(key1) == 0) || (strlen(key2) == 0))
    {
        printf("Input error - a key is empty!\n");
    }
    /*check key1 and key2 is same or not*/
    else if(strcmp(key1,key2) == 0)
    {
        
        printf("Input error - 'From' must be different from 'To'!\n");
    }
    /*weight can not less then 0*/
    else if(weight < 0)
    {
        printf("Input error - weight must not be negative!\n");
    }
    else
    {
        /*created edge*/
        check = addEdge(key1,key2,weight,Name,Name2);
    } 
    /*creat sucess*/
    if(check == 1)
    {
        /*printf("Added edge from |%s| to |%s| with weight %d\n",key1,key2,weight);*/
    }    
    /*created fail*/
    else if(check < 0)
    {
        printf(">>> Edge from |%s| to |%s| already exists!\n",key1,key2);
    }
    else
    {
        printf(">>> Vertex doesn't exist or memory allocation error!\n");
    }
}

/*Function use to open file to input data*/
int MapBuild()
{
    FILE* pIn = NULL;           /*pointer of file*/
    int maxVertices = 1000;     /*hold a number of maximun vertex*/
    int bDirected = 0;          /*use to tell this graph is undirect*/
    int check = 0;              /*use to check function*/
    char input[64];             /*hold a data in file*/

    /*open file*/
    pIn = fopen("Map.net","r");
    if(pIn == NULL)/*check file can open*/
    {
      printf("Error : cannot open this filename.\n");
      exit(0);
    }

    /*check graph can created*/
    if (!initGraph(maxVertices, bDirected))
    {
      printf("Error : Cannot initialize graph.\n");
    }

    /*loop to input data in file to structure*/
    while(fgets(input, sizeof(input),pIn) != NULL)
    {
        /*check data is Vertex or Edge*/
        if(strncmp("VERTEX",input,6) == 0)
        {
            /*created vertex*/
            Makevertex(input);
        }
        else if(strncmp("EDGE",input,4) == 0)
        {
            /*created edge*/
            makeEdge(input);
        }
        else
        {
            check = 1;
        }

    }
    return(check);
}