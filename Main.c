/*
 *		Main.c
 *		this file use to store the structure
 *		of the hospital, ambulance, interface of programs, 
 *		find nearest hospital
 *		and get input from user.
 *		Created by Yacht 3456, Mos 3486
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "AddMap.h"
#include "Inform.h"
#include "hospital.h"

int main ()
{
	time_t start_t;		/*use to start timer*/
	time_t end_t;		/*use to end timer*/
	time_t go;			/*use to start timer*/
	time_t stop;		/*use to stop timer*/
	double Diftime = 0;	/*use to hold a sum of time*/
	int check = 0;		/*check map can build*/
	int option = 0;		/*use to hold option number that user choose*/
	char input[32];		/*input option by user*/

	printf("\n--------WELCOME TO MEDICAL DISPATCH SYSTEM--------\n");
	printf("\n.....Loading Map...\n");

	check = MapBuild();
	/*check map can build or not*/
	if(check == 0)
	{
		printf(".....Success...\n\n");
	}
	/*loop to ask user to choose option*/
	while(1)
	{
		memset(input,0,sizeof(input));
		option = 0;
		time(&start_t);	/*start timer*/
		time(&go);

		/*after 20 sec*/
		/*if(Diftime > 20)
		{
			printf("\nAmbulance location 	:	\n");

			AmbulanceLocation();
			printf("Time(Sec) : %f\n\n", Diftime);
			Diftime = 0;
		}*/

		printf("System Menu:\n");
		printf("	1	-	Information.\n");
		printf("	2	-	Check Ambulance location.\n");
		printf("	3	-	Exit\n\n");

		printf("What do you want ot do?		:	");
		fgets(input, sizeof(input), stdin);
		
		if(isdigit(input[0]) && strlen(input)-1 == 1)
		{
			sscanf(input , "%d", &option);
			
			AmbulanceProcess(difftime(stop,go));
			if(option > 0 && option < 4)
			{
				if(option == 2)
				{
					time(&end_t);	/*stop timer*/
					Diftime = Diftime + difftime(end_t,start_t);
					printf("\nAmbulance location 	:	\n");

					AmbulanceLocation();
					printf("Time(Sec) : %f\n\n", Diftime);
				}
				if(option == 1)
				{
					PatientLocation();
					time(&end_t);	/*stop timer*/
					Diftime = Diftime + difftime(end_t,start_t);	
				}
				if(option == 3)
				{
					break;
				}
			}
			else
			{
				printf("Error - Not have this option.\n");
			}
		}
		else
		{
			printf("Error - please choose option that showed.\n");
		}
		

		time(&stop);
		
		

	}

	printf("\n.....Bye!!!!\n");
}