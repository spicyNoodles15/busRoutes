#ifndef BUS_ROUTES_HEADER
#define BUS_ROUTES_HEADER

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define clrscr() system("cls")
#else
#define clrscr() system("clear")
#endif

/**************************************
**********Macro Definitions*************
***************************************/
#define MAXPASS 50 		//maximum number of passes available
#define TRANSITAUTHORITY	"Charlotte Transit"
#define STUDENTDISCOUNT .50 	//percent
#define ELDERLYDISCOUNT .50 	//percent
#define EMPLOYEEDISCOUNT  1.0	//percent
#define WEEKENDDISCOUNT .25 	//percent
#define PRORATEMONTH .50 	//percent
#define MONTHLY 0		//pass type
#define PAYPERUSE 1		//pass type
#define EMPLOYEEPASS 2 		//pass type
#define EMPLOYEEID 0		//Employee Master pass
#define BUS 0			//Transportation method
#define SUBWAY 1		//Transportation method
#define RAIL 2			//Transportation method
#define ELDERLYBUS 3		//Transportation method
//Menu Macros (State machine states)
#define INIT 0			
#define MAINMENU 1		
#define DISCOUNTMENU 2		
#define PASSTYPEMENU 3
#define TRANSPORTMENU 4
#define ENTERIDMENU 5
#define USEPASS 6
#define BALANCEMENU 7
#define PURCHASE 8
#define ADDMONEY 9

/**************************************
**********Struct Definitions***********
***************************************/
//Struct definition for each pass
struct pass 
{
	int ID;		//Unique ID of pass
	int Type;	//Pay-per-use or Monthly
	int transType;	//Type of transportation
	float Discount;	//Discount rate of pass
	float Balance;	//Balance on card
	float length;	//For future expansion to calculate cost based on length of trip
	struct tm *purchTime; 	//Purchase time to track expiration of monthly passes
};

/**************************************
*******Function Declarations***********
***************************************/
void menuNav(int menuID);
void printPass(int currPass);
void errorMsg(char message[]);
void useSuccess(int currPass);
float getCost(int currPass);

#endif

