/*
*	hospital.c
*	this file use to store about alocate hospital place
*	and ambulance from .net file.
*	Created by Toto 3475
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "abstractNetwork.h"
/*
 *	this structure store about Patient
 */

typedef struct _patient
{
	int Level;					/*Hold level of sickness of patient*/
	int TimeToPatient;			/*Hold time that ambulance use to go to get patient*/
	int TimeToHospital;			/*Hold time that ambulance use to bring patient to Hospital(after get patient)*/
	char ToHospital[64];		/*Hold name of Hospital patient have to go*/
	char HospitalLoc[64];		/*Hold Hospital Location*/
	char PatientLoc[64];		/*Hold Patient Location*/
	int process;				/*Hold a work process */

	struct _patient * pNext;	/*pointer of next in queue*/

}PATIENT_T;

/*
 *	this structure store about hospital
 */
typedef struct _hospital
{
	char HospitalName[64];		/*store hospital name*/
	char RoadName[64];			/*store the hospital's road location*/
	char StartBlock[64];		/*store start vertex of that edge*/
	char EndBlock[64];			/*store end vertex of that edge*/
	int distance;				/*a range between hospital and intersection*/
	struct _hospital * pNext;	/*pointer of next hospital*/

} HOSPITAL_T;
/*
 * this structure store about ambulance
 * that contain the patient.
 */
typedef struct _ambulance
{
	char AmbulanceLoc[64];		/*Hold a location of Ambulance*/		
	int AmbulanceNum;			/*get a serial number of ambulance*/
	int get;
	PATIENT_T * queue;			/*structure of patient data*/
    struct _ambulance * pNext;	/*pointer of next ambulance*/

} AMBULANCE_T;

int AmbilanceSpeed = 60;			/*average speed of ambulance*/		
HOSPITAL_T* Hospital = NULL;		/*Head pointer of Hospital*/
AMBULANCE_T* Ambulance = NULL;		/*Head pointer of Ambulance*/

/*	Functuion use to update a process of abmulance
 *
 */
void AmbulanceProcess(int Diftime)
{
	AMBULANCE_T* Process = NULL;		/*Hold a Head pointer of Ambulance*/
	PATIENT_T* PatientData = NULL;		/*Hold a pointer of Patient data*/
	PATIENT_T* pTemp = NULL;

	Process = Ambulance;
	while(Process != NULL)
	{
		PatientData = Process->queue;
		if(PatientData != NULL)
		{
			if(PatientData->process == 1)
			{
				PatientData->TimeToPatient = PatientData->TimeToPatient - Diftime;
				if(PatientData->TimeToPatient <= 0)
				{

					PatientData->process = 2;
					strcpy(Process->AmbulanceLoc,PatientData->PatientLoc);
				}
			}
			else if(PatientData->process == 2)
			{
				PatientData->TimeToHospital = PatientData->TimeToHospital - Diftime;
				if(PatientData->TimeToHospital <= 0)
				{
					PatientData->process = 0;
					Process->get = Process->get - 1;
					strcpy(Process->AmbulanceLoc,PatientData->HospitalLoc);
					pTemp = PatientData;
					Process->queue = PatientData->pNext;
					pTemp = NULL;
				}
			}
			
		 	else if(PatientData->process == 0)
			{
				printf("Ambulance Serial : %d.\n",Process->AmbulanceNum);
				printf("sucess\n");
				if(PatientData->pNext != NULL)
				{
					printf("Do next queue.\n");
				}
				/*pTemp = PatientData;
				Process->queue = PatientData->pNext;
				pTemp = NULL;*/
			}
		}
		Process = Process->pNext;
	}
}

/* this function use to worse case patient who level 4 and 5 don't need
 * to wait until ambulance avalible it will call for otner nearest ambulance.
 */
AMBULANCE_T* Temp(PATIENT_T* patient,AMBULANCE_T* pHold)
{
	AMBULANCE_T* pHold2 = NULL;			/*use to hold a */
	AMBULANCE_T* pAmNear = NULL;		/*use a head pointer of Ambulance data*/
	int Compare_Amb1 = 100;				/*hold a leastest way to get Patient*/
	int Compare_Amb2 = 0;				/*compare a leastest way to get Patient*/
	char* Start; 						/*porinter at begin point*/
	char* End;							/*pointer at end point*/
	pAmNear = Ambulance;
	char* patientLoc = patient->PatientLoc;
	char ShorttoPatient[64];			/*Hold a Ambulance location*/
	while(pAmNear != NULL)
	{
		if(pAmNear->get < 2)
		{
			
			Start = findVertex(pAmNear->AmbulanceLoc);
			End = findVertex(patientLoc);

			Compare_Amb2 = printShortestPath(Start,End);
		
			if(Compare_Amb1 > Compare_Amb2)
			{
				if(pAmNear!=pHold
					&&((pAmNear->get)<2))
					{
					if((pAmNear->get==1)
						&&((pAmNear->queue)->Level<4))
						{
						Compare_Amb1 = Compare_Amb2;
						strcpy(ShorttoPatient,pAmNear->AmbulanceLoc);
						pHold2 = pAmNear;	
						break;
						}	
					else if(pAmNear->get==0)
						{
						Compare_Amb1 = Compare_Amb2;
						strcpy(ShorttoPatient,pAmNear->AmbulanceLoc);
						pHold2 = pAmNear;	
						break;	
						}
					}	
			}
		
		}
		pAmNear = pAmNear->pNext;
	}
	return (pHold2);
}

/*	
 * Function use to rearrange a queue
 */
void ArrangeQueue(AMBULANCE_T* pHold)
{
	AMBULANCE_T* pUSE = pHold;		/*Hold a head pointe of ambulance*/
	AMBULANCE_T* pHold2 = NULL;		/*ambulance to pick level 4+ patient */
	PATIENT_T* First = NULL;			/*Hold a first queue*/
	PATIENT_T* Second = NULL;			/*Hold a second queue*/
	PATIENT_T* temp = NULL;				/*use to switch queue*/
	
	First = pUSE->queue;
	Second = First->pNext;


	if((First->Level<4)
		&&(Second->Level>3)
		&&(First->process==1))
		{	
			temp = (pHold->queue)->pNext;
			pHold->queue = temp;
			(pHold->queue)->pNext = First;	
		}
	if((((pHold->queue)->pNext)->Level)>3)
	{
		printf("find new Ambulance\n");
		pHold2 = Temp((pHold->queue)->pNext,pHold);
		if(pHold2!=NULL)
		{
			pHold->get = pHold->get-1;
			/*when new ambulance will get worse case patient*/
			if(pHold2->queue==NULL)
			{
				pHold2->queue =  (pHold->queue)->pNext;
				pHold2->get = pHold2->get + 1;
				(pHold->queue)->pNext = NULL;
				//ArrangeQueue(pHold2);
				/*printf("\n\tPatient has moved to ambulance serial %d. \n",pHold2->AmbulanceNum);
				printf("pHold2->num = %d\n",pHold2->AmbulanceNum);
				printf("pHold2->AmbulanceLoc = %s\n",pHold2->AmbulanceLoc);
				printf("pHold2->level = %d\n",(pHold2->queue)->Level);*/
				return;
			}
			/*if new ambulance have queue, it will compare and rearrange queue.*/
			else
			{

				(pHold2->queue)->pNext =  (pHold->queue)->pNext;
				pHold2->get = pHold2->get + 1;
				(pHold->queue)->pNext = NULL;
				/*printf("111111111111\n");
				printf("pHold2->num = %d\n",pHold2->AmbulanceNum);
				printf("pHold2->level = %d\n",(pHold2->queue)->Level);*/
				return;
			}
		}
	}
	printf("Please wait until the ambulance avalible \n");
	
}

/*	function use to get a queue of ambulance
 *	Argument
 *	pHold	-	hold a pointer of Ambulance that have to bring patient
 *	HosName -	hold a name of hospital that ambulance have to go to
 *	total 	-	get a total distance that use to bring patient go to hospital
 *	time1	-	get a time that ambulance use to go to patient location
 *	time2	- 	get a time that ambulance use to bring patient to hospital
 *	level 	-	get a level of patient
 */
void AddQueue(AMBULANCE_T* pHold,char* HosName,char* HosLoc,char* PatLoc, int total,int time1,int time2,int Level)
{
	PATIENT_T * Data = NULL;	/*Hold a patient data in this structure*/

	Data = (PATIENT_T*)calloc(1,sizeof(PATIENT_T));

	AMBULANCE_T* pHoldCur = pHold;
	
	Data->TimeToPatient = (time1*18000000)/AmbilanceSpeed;
	Data->TimeToHospital = (time2*18000000)/AmbilanceSpeed;
	
	strcpy(Data->ToHospital,HosName);
	strcpy(Data->HospitalLoc,HosLoc);
	strcpy(Data->PatientLoc,PatLoc);
	Data->process = 1;
	Data->Level = Level;
	//pHold->get = 1;
	
	/*printf("Called\n");
	printf("pHold->num = %d\n",pHold->AmbulanceNum);
	printf("pHold->get = %d\n",pHold->get);*/
	if(pHold->queue == NULL)
	{
		pHold->queue = Data;
		pHold->get = pHold->get + 1;
	}
	else
	{
		if(pHold->get>=0)
		{
			pHold->get = pHold->get + 1;
			(pHold->queue)->pNext = Data;
		}
	}
	/*printf("add\n");
	printf("pHold->num = %d\n",pHold->AmbulanceNum);
	printf("pHold->AmbulanceLoc = %s\n",pHold->AmbulanceLoc);
	printf("pHold->get = %d\n",pHold->get);*/
	if(pHold->get > 1)
	{
		ArrangeQueue(pHold);
		/*printf("Arrange\n");
		printf("pHold->num = %d\n",pHold->AmbulanceNum);
		printf("1.pHold->level = %d\n",(pHold->queue)->Level);*/
		if(pHold->get==2)
		{
		/*printf("2.pHold->level = %d\n",((pHold->queue)->pNext)->Level);*/
		}
	}
	/*printf("Current queue\n");
	printf("pHold->num = %d\n",pHold->AmbulanceNum);
	printf("pHold->get = %d\n",pHold->get);*/
	
}

/*	function use to random a location of ambulance
 *	Argument
 *	HospitalNum		-	get a number of hospital	
 */
void RNDAmbulance(int HospitalNum)
{
	FILE* pInterSec = NULL;			/*hold a pointer of file*/
	AMBULANCE_T* ambuHead = NULL;	/*ambulance head pointer*/
	AMBULANCE_T* ambuNew = NULL;	/*ambulance allocate pointer*/
	AMBULANCE_T* ambuTail = NULL;	/*ambulance tail pointer*/
	time_t t;						/*use to generate random number*/
	char input[124];				/*input data form file*/
	char Road[64];					/*Hold a road name*/
	char district[64];				/*Hold a district name*/
	int Random = 0;					/*hold a random number*/
	int count = 0;					/*use to get a serial number of ambulance*/
	int i = 0;						/*loop count*/

	srand((unsigned) time(&t));

	/*get ambulance location(number of ambulance = number of hospital muiltiple 2)*/
	for(count = 0;count < HospitalNum*2;count++)
	{
		
		pInterSec = fopen("intersection.net","r");
		if(pInterSec == NULL)
		{	
			printf("Error - can't open \"intersection.net\".\n");
			exit(1);
		}

		Random = (rand() % 45) + 1;

		while(i < Random)
		{
			fgets(input, sizeof(input), pInterSec);
			i++;
		}
		i = 0;

		ambuNew = (AMBULANCE_T*)calloc(1,sizeof(AMBULANCE_T));
		fclose(pInterSec);

		sscanf(input, "%s %s %s", ambuNew->AmbulanceLoc, Road, district);
		ambuNew->AmbulanceNum = count + 1;

		if(count == 0)
		{
			ambuHead = ambuNew;
			ambuTail = ambuNew;
		}
		else
		{
			ambuTail->pNext = ambuNew;
			ambuTail = ambuNew;
		}
	}

	/*add*/
	HOSPITAL_T* pCurrHos = Hospital;
	int count2;

	for(count2 = count;(count2 < HospitalNum*3)&&(pCurrHos!=NULL);count2++)
	{
		ambuNew = (AMBULANCE_T*)calloc(1,sizeof(AMBULANCE_T));
		ambuNew->AmbulanceNum = count2+1;
		strcpy(ambuNew->AmbulanceLoc,pCurrHos->StartBlock);
		ambuTail->pNext = ambuNew;
		ambuTail = ambuNew;
		pCurrHos = pCurrHos->pNext;		
	}

    Ambulance = ambuHead;
}

/*
 *	this function use to read and allocate 
 *  the hospital name and it's location.
 *	Argument
 *  	num - get pointer to store number of hospital 
 */
void HospitalList(int * num)
{
	HOSPITAL_T * pHosHead = NULL;	/*Head pointer*/
	HOSPITAL_T * pHosTail = NULL;	/*Tail pointer*/
	HOSPITAL_T * pNewData = NULL;	/*allcate pointer*/
	FILE * pHos = NULL;				/*Hold file pointer*/
	char input[124];				/*input data form file*/
	int count = 0;					/*count a number of hospital*/

	pHos = fopen("hospital.net","r"); /*open file of hospital*/
	if(pHos == NULL)
	{
		printf("Error can not open file");
	}

	/*store data of hospital*/
	while(fgets(input, sizeof(input),pHos) != NULL)
	{
		pNewData = (HOSPITAL_T*)calloc(1,sizeof(HOSPITAL_T));
		sscanf(input, "%s %s %d %s %s", pNewData->HospitalName, pNewData->RoadName, &pNewData->distance
											, pNewData->StartBlock, pNewData->EndBlock);
		if(count == 0)
		{
			pHosHead = pNewData;
			pHosTail = pNewData;
		}
		else
		{
			pHosTail->pNext = pNewData;
			pHosTail = pNewData;
		}
		count++;
	}

	*num = count;
	fclose(pHos);
	Hospital = pHosHead;
}

/*
 *	this function use to find a nearrest Ambulance
 *	and nearrest hospital to bring a patient
 *	to hospital
 *	Argument
 *  patientLoc	-	get a location of patient
 *	Level		-	get a level of sickness of patient	  		
 */
void HospitalDetail(char* patientLoc, int Level)
{
	HOSPITAL_T* pNearHos = NULL;		/*use a head pointer of hospital data*/
	AMBULANCE_T* pAmNear = NULL;		/*use a head pointer of Ambulance data*/
	AMBULANCE_T* pHold = NULL;			/*use to hold a */
	int Compare_Hos1 = 100;				/*hold a leastest way to hospital*/
	int Compare_Hos2 = 0;				/*compare a leastest way to hospital*/
	int Compare_Amb1 = 100;				/*hold a leastest way to get Patient*/
	int Compare_Amb2 = 0;				/*compare a leastest way to get Patient*/
	int count = 0;						/*hold a number of hospital*/
	int Total = 0;						/*get total distance*/
	int num = 0;						/*get a constant distance*/		
	char ShortWaytoHos[64];				/*Hold a hospital location*/
	char ShorttoPatient[64];			/*Hold a Ambulance location*/
	char HosName[64];					/*hold a name of hospital*/
	char* Start; 						/*porinter at begin point*/
	char* End;							/*pointer at end point*/
	time_t go;							/*start timer*/
	time_t stop;						/*stop timer*/	

	if(Ambulance == NULL && Hospital == NULL)
	{

		HospitalList(&count);
		RNDAmbulance(count);
	}

	pNearHos = Hospital;
	pAmNear = Ambulance;
	
	/*find nearrest ambulance*/
	while(pAmNear != NULL)
	{
		if(pAmNear->get < 2)
		{
			Start = findVertex(pAmNear->AmbulanceLoc);
			End = findVertex(patientLoc);

			Compare_Amb2 = printShortestPath(Start,End);
		
			if(Compare_Amb1 > Compare_Amb2)
			{
				Compare_Amb1 = Compare_Amb2;
				strcpy(ShorttoPatient,pAmNear->AmbulanceLoc);
				pHold = pAmNear;
			}
		}
		pAmNear = pAmNear->pNext;
	}
	/*find nearrest hospital*/
	while(pNearHos != NULL)
	{
		Start = findVertex(patientLoc);
		End = findVertex(pNearHos->StartBlock);

		Compare_Hos2 = printShortestPath(Start,End);
		if(Compare_Hos1 > Compare_Hos2)
		{
			Compare_Hos1 = Compare_Hos2;
			strcpy(ShortWaytoHos,pNearHos->StartBlock);
			strcpy(HosName,pNearHos->HospitalName);
			num = pNearHos->distance;
		}
		pNearHos = pNearHos->pNext;
	}


	Total = Compare_Hos1 + Compare_Amb1 + num;

	printf("\n\nHospital Name : %s \n",HosName);
	printf("Ambulance Serial : %d \n",pHold->AmbulanceNum);
	printf("Distance : %d Km\n", Total);
	printf("Time use to travel : %d minutes\n\n",(Total*60)/AmbilanceSpeed);

	
	AddQueue(pHold,HosName,ShortWaytoHos,patientLoc,Total,Compare_Amb1,Compare_Hos1,Level);

	/*add*/
	AMBULANCE_T* abc = Ambulance;
	abc = Ambulance;
		while(abc!=NULL)
			{
			if(abc->get>0)
				{
				printf("\n\n\nNum ambulance %d\n",abc->AmbulanceNum);
				printf("Location Ambulance = %s\n",abc->AmbulanceLoc);
				printf("queue = %d\n",abc->get);
				printf("1.queue Level = %d\n",(abc->queue)->Level);
				printf("1.queue process = %d\n",(abc->queue)->process);
				printf("1.queue Location = %s\n",(abc->queue)->PatientLoc);
				if(abc->get>1&&((abc->queue)->pNext!=NULL))
					{
					printf("2.queue Level = %d\n",((abc->queue)->pNext)->Level);
					printf("2.queue process = %d\n",((abc->queue)->pNext)->process);
					printf("2.queue Location = %s\n",((abc->queue)->pNext)->PatientLoc);	
					}
				printf("\n\n\n\n\n\n");
				}
			abc = abc->pNext;
			}
}

/*	function use to print a current location of ambulance
 *
 */
void AmbulanceLocation()
{
	
	AMBULANCE_T* pUSE = NULL;	/*Hold a head pointer of ambulance*/

	pUSE = Ambulance;
	
	while(pUSE != NULL)
	{
		if(pUSE->queue != NULL)
		{	
			printf("\nAmbulance Serial : %d \n",pUSE->AmbulanceNum);
			printf("Current Location : %s \n",pUSE->AmbulanceLoc);
			printf("get  %d queue\n",pUSE->get);
			if(pUSE->queue->process == 1)
			{
				printf("Ambulance Process : Inprogress \n");
			}
			else if(pUSE->queue->process == 2)
			{
				printf("Ambulance Process : Recieved patient \n");
				printf("				  : Traveling to Hospital\n");
			}
			else
			{
				printf("Ambulance Process : Free****\n");
			}

			if(pUSE->get > 0)
			{
				printf("queue 1 : %d\n",pUSE->queue->Level);
				printf("Process 1 : %d\n",pUSE->queue->process);
				if(pUSE->get>1)
				{
					printf("queue 2 : %d\n",((pUSE->queue)->pNext)->Level);
					printf("Process 2 : %d\n",((pUSE->queue)->pNext)->process);
				}
			}
		}
		else if(pUSE->queue == NULL)
		{
			printf("\nAmbulance Serial : %d \n",pUSE->AmbulanceNum);
			printf("Current Location : %s \n",pUSE->AmbulanceLoc);
			printf("Ambulance Process : Free \n");
			
		}

		pUSE = pUSE->pNext;

	}

}