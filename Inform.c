/* 
 * Inform.c
 * This  file use ask user to
 * tell us about patient location
 * 
 *	Created by Jee 3457, Yacht 3456, Mos 3486
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abstractNetwork.h"
#include "hospital.h"

typedef struct _district
{
	char DistName[64];			/*store district name.*/
	struct _district * pNext; 	/*next item in District list*/
}DISTRICT_T;

typedef struct _road
{
	char DistName[64];		/*store district name.*/
	char RoadName[64];		/*store road in each district.*/
	struct _road * pNext;	/*next item in Road list*/ 
}ROAD_T;

typedef struct _intersec
{
	char DistName[64];			/*store district name.*/
	char RoadName[64];			/*store road name.*/
	char InterSecName[64];		/*store inersec name.*/
	struct _intersec * pNext;	/*next item in intersec list*/
}INTERSEC_T;

/*****************************************************************************************************
 *DISTRICT FUNCTION ZONE
 *****************************************************************************************************
 */

/*function use to get the district list
 */
DISTRICT_T* DistrictList()
{
	FILE* pDist = NULL;				/*hold pointer of file*/
	DISTRICT_T* pDistHead = NULL;	/*head of the district list*/
	DISTRICT_T* pNewDist = NULL;	/*New list of district*/
	DISTRICT_T * pDistTail = NULL;	/*tail of the district list*/
	char input[128];					/*Input data form file*/
	int count = 0;					/*count district item*/
	
	/*Open District Data file*/
	pDist = fopen("district.net","r");
	if(pDist == NULL)
	{
		printf("Error - can't open \"district.net\".\n");
	}
	
	while(fgets(input,sizeof(input),pDist) != NULL)
	{
		pNewDist = (DISTRICT_T*)calloc(1,sizeof(DISTRICT_T));
		sscanf(input, "%s", pNewDist->DistName);
		if(count == 0)
		{
			pDistHead = pNewDist;
			pDistTail = pNewDist;
		}
		else
		{
			pDistTail->pNext = pNewDist;
			pDistTail = pNewDist;
		}
		count++;
	}
	fclose(pDist);
	return(pDistHead);
}

/* function use to print Name of district in list
 * Argument
 * Data 	-	use to receive pointer of District data
 */
int printDistrict(DISTRICT_T* Data)
{
	int count = 0;				/*count district item*/
	DISTRICT_T* pDist = Data;	/*receive pointer of District data*/

	while(pDist !=NULL)
	{
		printf("	%d	-	%s\n",count+1,pDist->DistName);
		pDist = pDist->pNext;
		count++;
	}
	return(count);
}

/*	function use to search a district that user want
 *	Argument
 *	Name 	-	use to hold a name of district that user want
 *	num 	-	use to get a number of round of loop that need to run
 *	Data 	-	use to receive pointer of District data
 */
void getDist(char Name[64], int num, DISTRICT_T* Data)
{
	int count = 0;	/*use to count loop round*/

	for(count = 0; count < num;count++)
	{
		Data = Data->pNext;
	}
	strcpy(Name,Data->DistName);
}

/*	function use to ask use to key the information
 *	Argument
 *	DistUse -	use to hold a name of district that user want
 *	Data 	-	use to receive pointer of District data
 */
void AskDistrict(char DistUse[64],DISTRICT_T* Data)
{
	DISTRICT_T* DistData = Data;	/*receive pointer of District data*/
	int count = 0;					/*use to count loop round*/
	int choose = 0;					/*get the position of data that user want*/
	char input[128];					/*input a choice that user choose*/

	printf("\nWhere are you District?\n");
	count = printDistrict(DistData);
	while(1)
	{
		printf("-----------	:	");

		fgets(input, sizeof(input), stdin);
		if(isdigit(input[0]) && strlen(input)-1 == 1)
		{
			sscanf(input, "%d", &choose);
			
			if(choose > 0 && choose <= count)
			{
				getDist(DistUse,choose-1,DistData);
				printf("%s\n",DistUse);
				printf("-----------\n");
				break;
			}
			else
			{
				printf("Error	-	Dont have that district.\n");
			}
		}
		else
		{
			printf("Error	-	please enter number of district.\n");
		}
	}
}

/*END DISTRICT FUNCTION ZONE***
 *****************************************************************************************************
 *ROAD FUNCTION ZONE
 *****************************************************************************************************
 */

/*function use to get the road list
 */
ROAD_T* RoadList()
{
	FILE* pRoad = NULL;			/*hold pointer of file*/
	ROAD_T* pRoadHead = NULL;	/*head of the Road list*/
	ROAD_T* pNewRoad = NULL;	/*New list of district*/
	ROAD_T* pRoadTail = NULL;	/*tail of the Road list*/	
	char input[128];				/*Input data form file*/
	int count = 0;				/*count road item*/
	
	/*Open Road Data file*/
	pRoad = fopen("road.net","r");
	if(pRoad == NULL)
	{
		printf("Error - can't open \"road.net\".\n");
	}
	
	while(fgets(input,sizeof(input),pRoad) != NULL)
	{
		pNewRoad = (ROAD_T*)calloc(1,sizeof(ROAD_T));
		sscanf(input, "%s %s", pNewRoad->RoadName, pNewRoad->DistName);
		if(count == 0)
		{
			pRoadHead = pNewRoad;
			pRoadTail = pNewRoad;
		}
		else
		{
			pRoadTail->pNext = pNewRoad;
			pRoadTail = pNewRoad;
		}
		count++;
	}
	fclose(pRoad);
	return(pRoadHead);
}

/* function use to print Name of Road in list
 * Argument
 * DistName -	hold a name of district
 * Data 	-	use to receive pointer of Road data
 */
int printRoad(char* DistName, ROAD_T* Data)
{
	int count = 0;			/*count road item*/
	ROAD_T* pRoad = Data;	/*receive pointer of road data*/

	while(pRoad !=NULL)
	{	
		if(strcmp(DistName,pRoad->DistName) == 0)
		{
			printf("	%d	-	%s\n",count+1,pRoad->RoadName);
			count++;
		}
		
		pRoad = pRoad->pNext;
	}
	return(count);
}

/*	function use to search a road that user want
 *	Argument
 *	Name 	-	use to hold a name of road that user want
 *	num 	-	use to get a number of round of loop that need to run
 *	Data 	-	use to receive pointer of road data
 *	Distname-	use to hold name of district
 */
void getRoad(char Name[64], int num,ROAD_T* Data,char* DistName)
{
	int count = 0;	/*use to count loop round*/

	while(Data != NULL)
	{
		if(strcmp(DistName,Data->DistName) == 0 && count <= num)
		{
			count++;
			if(count == num)
			{
				break;
			}
		}
		
		Data = Data->pNext;
	}
	strcpy(Name,Data->RoadName);
}

/*	function use to ask use to key the information
 *	Argument
 *	RoadUse -	use to hold a name of road that user want
 *	Distname-	use to hold a name of district
 *	Data 	-	use to receive pointer of road data
 */
void AskRoad(char RoadUse[64],char* DistName,ROAD_T* Data)
{
	ROAD_T* RoadData = Data;	/*receive pointer of Road data*/
	int count = 0;				/*use to count loop round*/
	int choose = 0;				/*get the position of data that user want*/
	char input[128];				/*input a choice that user choose*/

	printf("\nWhat the nearest road?\n");
	count = printRoad(DistName,RoadData);
	while(1)
	{
		printf("-----------	:	");

		fgets(input, sizeof(input), stdin);
		if(isdigit(input[0]) && strlen(input)-1 == 1)
		{
			sscanf(input, "%d", &choose);

			if(choose > 0 && choose <= count)
			{
				getRoad(RoadUse,choose,RoadData,DistName);
				printf("%s\n",RoadUse);
				printf("-----------\n");
				break;
			}
			else 
			{
				printf("Error	-	Dont have that Road.\n");
			}
		}
		else
		{
			printf("Error	-	please enter number of Road.\n");
		}
	}
}

/*END ROAD FUNCTION ZONE***
 *****************************************************************************************************
 *INTERSECTION FUNCTION ZONE
 *****************************************************************************************************
 */

/*function use to get the intersection list
 */
INTERSEC_T* IntersectionList()
{
	FILE* pInterSec = NULL;				/*hold pointer of file*/
	INTERSEC_T* pInterSecHead = NULL;	/*head of the Road list*/
	INTERSEC_T* pInterSecTail = NULL;	/*tail of the Road list*/
	INTERSEC_T* pNewInterSec = NULL;	/*New list of district*/
	char input[128];						/*Input data form file*/
	int count = 0;						/*count road item*/
	
	/*Open intersection Data file*/
	pInterSec = fopen("intersection.net","r");
	if(pInterSec == NULL)
	{
		printf("Error - can't open \"intersection.net\".\n");
	}
	
	while(fgets(input,sizeof(input),pInterSec) != NULL)
	{
		pNewInterSec = (INTERSEC_T*)calloc(1,sizeof(INTERSEC_T));
		sscanf(input, "%s %s %s", pNewInterSec->InterSecName, pNewInterSec->RoadName, pNewInterSec->DistName);
		if(count == 0)
		{
			pInterSecHead = pNewInterSec;
			pInterSecTail = pNewInterSec;
		}
		else
		{
			pInterSecTail->pNext = pNewInterSec;
			pInterSecTail = pNewInterSec;
		}
		count++;
	}
	fclose(pInterSec);
	return(pInterSecHead);
}

/* function use to print Name of intersection in list
 * Argument
 * DistName -	hold a name of district
 * Roadname -	hold a name of road	
 * Data 	-	use to receive pointer of Intersection data
 */
int printInterSec(char* DistName, char* RoadName,INTERSEC_T* Data)
{
	int count = 0;					/*count intersection item*/
	INTERSEC_T* pInterSec = Data;	/*receive pointer of intersection*/

	while(pInterSec != NULL)
	{	
		if(strcmp(DistName,pInterSec->DistName) == 0 && strcmp(RoadName,pInterSec->RoadName) == 0)
		{
			printf("	%d	-	%s\n",count+1,pInterSec->InterSecName);
			count++;
		}
		
		pInterSec = pInterSec->pNext;
	}
	return(count);
}

/*	function use to search a intersection that user want
 *	Argument
 *	Name 	-	use to hold a name of intersection that user want
 *	num 	-	use to get a number of round of loop that need to run
 *	Data 	-	use to receive pointer of intersection data
 *	DistName-	use to hold name of district
 *	RoadName-	use to hold name of road
 */
void getInterSec(char Name[64], int num,INTERSEC_T* Data,char* DistName,char* RoadName)
{
	int count = 0;	/*use to count loop round*/

	while(Data != NULL)
	{

		if(strcmp(DistName,Data->DistName) == 0 && strcmp(RoadName,Data->RoadName) == 0)
		{
			count++;
			if(count == num)
			{
				break;
			}
		}
		Data = Data->pNext;
	}
	strcpy(Name,Data->InterSecName);
}

/*	function use to ask use to key the information
 *	Argument
 *	InterSecUse -	use to hold a name of intersection that user want
 *	RoadName 	-	use to hold a name of road
 *	DistName 	-	use to hold a name of district
 *	Data 		-	use to receive pointer of District data
 */
void AskInterSec(char InterSecUse[64],char* RoadName,char* DistName,INTERSEC_T* Data)
{
	INTERSEC_T* InterSecData = Data;	/*receive pointer of Intersection data*/
	int count = 0;						/*use to count loop round*/
	int choose = 0;						/*get the position of data that user want*/
	char input[128];						/*input a choice that user choose*/

	printf("\nWhat the nearest intersection?\n");
	count = printInterSec(DistName,RoadName,InterSecData);
	while(1)
	{
		printf("-----------	:	");

		fgets(input, sizeof(input), stdin);
		if(isdigit(input[0]) && strlen(input)-1 == 1)
		{
			sscanf(input, "%d", &choose);

			if(choose > 0 && choose <= count)
			{
				getInterSec(InterSecUse,choose,InterSecData,DistName,RoadName);
				printf("%s\n",InterSecUse);
				printf("-----------\n\n");
				break;
			}
			else
			{
				printf("Error	-	Dont have that intersection.\n");
			}
		}
		else
		{
			printf("Error	-	please enter number in the list.\n");
		}
	}

}

/*END INTERSECTION FUNCTION ZONE***
 *****************************************************************************************************
 */
 
 /*	function use to ask user to complete the information
  */
void PatientLocation()
{
	DISTRICT_T* DistData = NULL;		/*Hold pointer of district data*/
	ROAD_T* RoadData = NULL;			/*Hold pointer of road data*/
	INTERSEC_T* InterSecData = NULL;	/*Hold pointer of intersection data*/
	char DistUse[64];					/*Hold name of district that user choose*/
	char RoadUse[64];					/*Hold name of road that user choose*/
	char InterSecUse[64];				/*hold name of interseection that user choose*/
	char input[128];					/*input a choice that user choose*/
	int choose = 0;						/*hold a choice that user choose*/
	int level = 0;						/*get levet of sickness*/
	int i = 0;

	DistData = DistrictList();
	AskDistrict(DistUse,DistData);

	RoadData = RoadList();
	AskRoad(RoadUse,DistUse,RoadData);

	InterSecData = IntersectionList();
	AskInterSec(InterSecUse,RoadUse,DistUse,InterSecData);

	printf("Enter a level of sickness (Level 1-5).\n");
	printf("	1	-	A Little hurt\n");
	printf("	2	-	A Little hurt but cannot move\n");
	printf("	3	-	Sickness Moderate\n");
	printf("	4	-	Serious symptoms\n");
	printf("	5	-	Risk of death\n");
	while(1)
	{
		printf("-----------	:	");

		fgets(input, sizeof(input), stdin);
		if(isdigit(input[0]) && strlen(input)-1 == 1)
		{
			sscanf(input, "%d", &choose);

			if(choose > 0 && choose <= 5)
			{
				level = choose;
				printf("-----------\n\n");
				break;
			}
			else
			{
				printf("Error	-	Dont have that choice.\n");
			}
		}
		else
		{
			printf("Error	-	please enter number a level of sickness.\n");
		}
	}

	HospitalDetail(InterSecUse,level);

}