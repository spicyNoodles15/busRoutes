/***************************************************************
*ABSTRACT:
*The purpose of this program is to simulate a Transit Authority
*ticket booth using the stdin/stdout.  
*
*The program allows users to:
*	1. Purchase a Pass
*	2. Use a Pass
*	3. Check the balance of a pass
*	4. Add $ to a pass
*All calculations for pass and ride prices, including multiple
*optional discounts are handled within.   
****************************************************************/

/**************************************
***************Includes****************
***************************************/
#include "busRoutes.h"

/**************************************
***********Global Variables************
***************************************/
int highestID = 0;
struct pass passList[MAXPASS] = {};
char a;					//default char for menus
int currPass;			//current pass ID
int lastPass; 			//most recently created new pass
float currPrice; 		//current price
int id;					//customer ID # input
float p;				//Price input from customer
char errMsg[50] = "";	//simple error message string
char prevInput;			//stores previous user input
time_t current_time;    //Current raw time
struct tm *currTime;    //Current formatted time

//price table for all current maximum prices
//Index definition: priceTable[pass.Type][pass.transType]
float priceTable[3][4] = {
	//{bus, subway, rail, elderlybus}
	{20.00, 30.00, 40.00, 25.00}, //Monthly
	{2.00, 3.00, 4.00, 2.50}, //Pay-Per-Use
	{0.00, 0.00, 0.00, 0.00} //Employee
};

char *typeList[3] = {"Monthly", "Pay-Per-Use", "Employee"};
char *transList[4] = {"Bus", "Subway", "Rail", "Elderly Bus"};

/*************************************
**********End Global Variables********
**************************************/


/*************************************
*Function: int main()
*
*Entry Point into Program
**************************************/
int main()
{
  //Call Menu state machine
  menuNav(INIT);
  return(0);
}//end main()

/*************************************
*Function: void menuNav(int menuID)
*
*State machine to display correct menu
**************************************/
void menuNav(int menuID)
{
  //state
  int state = menuID;
  int transTypeSelection;
  int i;
  char str[20]={}; 	//buffer for user input
  char *pErr;  		//Used for error checking float
  char *idErr;		//Used for error checking int

  //Get current time so we can apply discounts
  current_time = time(NULL);
  currTime = localtime( &current_time );

  //clear screen for ease of use
  clrscr();

  //Process menu tasks and navigate to next menu
  switch (menuID)
  {
	//Initialize state machine variables
	case INIT:
		a = 0;	
		id = 0;	
  		currPass = 1;
		lastPass = 1;
		currPrice = 0;
		int transTypeSelection = 0;
		p = 0;	
		i = 0;
		//Number all passes and zero discounts
		for (int k=1; k<=MAXPASS; k++){
			passList[k].ID = k;
			passList[k].Discount = 0;
		} 
		//Give employee pass(0) appropriate discount 
		passList[0].Discount = EMPLOYEEDISCOUNT;
		
		state = MAINMENU;	
		break;
	case MAINMENU:
		//Display Main Menu text
  		printf("*****************************\n");
  		printf("Welcome to %s\n\n", TRANSITAUTHORITY);
		printf("Services:\n");
		printf("Bus: Monthly: $%.2f, Pay-Per-Use: $%.2f\n",priceTable[0][0],priceTable[1][0]);
		printf("Subway: Monthly: $%.2f, Pay-Per-Use: $%.2f\n",priceTable[0][1],priceTable[1][1]);
		printf("Rail: Monthly: $%.2f, Pay-Per-Use: $%.2f\n",priceTable[0][2],priceTable[1][2]);
		printf("Elderly Bus: Monthly: $%.2f, Pay-Per-Use: $%.2f\n",priceTable[0][3],priceTable[1][3]);
		printf("\nDiscounts Available:\n");
		printf("Elderly: %.0f%c, Students: %.0f%c\n",ELDERLYDISCOUNT*100,'%', STUDENTDISCOUNT*100,'%');
		printf("PAY-PER-USE Passes %.0f%c Off on Weekends!\n", WEEKENDDISCOUNT*100,'%'); 
		printf("MONTHLY Passes %.0f%c Off after the 15th of the Month!\n", PRORATEMONTH*100, '%'); 
		printf("*****************************\n\n");
  		printf("Please select from the following options:\n");
  		printf("Purchase Pass (p)\n");
  		printf("Use Pass (u)\n");
  		printf("Check Pass Balance (b)\n");
		printf("Add $ to Pass (a)\n");
  		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
	
		//Process input
  		switch (str[0])
  		{
  			case 'p':
				state = DISCOUNTMENU;
				currPass = lastPass; //Assign new pass #
			break;
			case 'u':
				state = ENTERIDMENU;
				prevInput = 'u';		
			break;
			case 'b':
				state = ENTERIDMENU;
				prevInput = 'b';
			break;
			case 'a':
				state = ENTERIDMENU;
				prevInput = 'a';
			break;
			default:
				strcpy(errMsg,"Invalid Selection!");
				errorMsg(errMsg);
				state = MAINMENU;
				break;			
  		}
		break;
	case DISCOUNTMENU:
		//Display Discount Menu Text
		printf("*****************************\n");
  		printf("Select Discount:\n\n");
  		printf("Elderly (e)\n");
  		printf("Student (s)\n");
  		printf("No Discount (n)\n");
  		printf("Return to Main Menu (x)\n");
  		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		
		//Process input
  		switch (str[0])
  		{
  			case 'e':
				passList[currPass].Discount = ELDERLYDISCOUNT;	
				state = PASSTYPEMENU;	
			break;
			case 's':
				passList[currPass].Discount = STUDENTDISCOUNT;
				state = PASSTYPEMENU;
			break;
			case 'n':
				passList[currPass].Discount = 0;
				state = PASSTYPEMENU;
			break;
			case 'x':
				state = MAINMENU;
			break;
			default:
				strcpy(errMsg,"Invalid Selection!");
				errorMsg(errMsg);
				state = DISCOUNTMENU;
			break;
  		}
		break;
	case PASSTYPEMENU:
		//Display Pass Type Menu Text
		printf("*****************************\n");
  		printf("Select Pass Type:\n\n");
  		printf("Monthly (m)\n");
  		printf("Pay-Per-Ride (p)\n");
  		printf("Return to Main Menu (x)\n");
  		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		
		//Process input
  		switch (str[0])
  		{
  			case 'm':
				//change pass type to monthly
				passList[currPass].Type = MONTHLY;
				state = TRANSPORTMENU;
			break;
			case 'p':
				//change pass type to pay per ride
				passList[currPass].Type = PAYPERUSE;
				state = TRANSPORTMENU;
			break;
			case 'x':
				state = MAINMENU;
			break;
			default:
				strcpy(errMsg,"Invalid Selection!");
				errorMsg(errMsg);
				state = PASSTYPEMENU;
			break;
  		}
		break;
	case TRANSPORTMENU:
		//Display Transport Type Menu Text
		printf("*****************************\n");
  		printf("Select Transport Type:\n\n");
  		printf("Bus (b)\n");
  		printf("Subway (s)\n");
		printf("Commuter Rail (r)\n");
		printf("Elderly Bus (e)\n");
  		printf("Return to Main Menu (x)\n");
  		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		
		//Process input
  		switch (str[0])
  		{
  			case 'b':
				passList[currPass].transType = BUS;
				state = PURCHASE;
			break;
			case 's':
				passList[currPass].transType = SUBWAY;
				state = PURCHASE;
			break;
			case 'r':
				passList[currPass].transType = RAIL;
				state = PURCHASE;
			break;
			case 'e':
				passList[currPass].transType = ELDERLYBUS;
				state = PURCHASE;
			break;
			case 'x':
				state = MAINMENU;
			break;
			default:
				strcpy(errMsg,"Invalid Selection!");
				errorMsg(errMsg);
				state = TRANSPORTMENU;
			break;
  		}

		//Calculate cost and send to payment screen
		if (state != MAINMENU) currPrice = getCost(currPass);
			
		break;
	case ENTERIDMENU:
		printf("*****************************\n\n");
		printf("Enter Pass Number:\n\n");
		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		id = strtol(str, &idErr, 0);
		//Check for input error
		if (str[0] == '\n' || *idErr != '\n' || str == idErr){
			strcpy(errMsg,"Invalid Input!");
			errorMsg(errMsg);
			state = ENTERIDMENU;
			break;
		}
	
		//ID Error processing
		if (id > 0 && id < MAXPASS){		
			currPass = id; //Valid ID 
		}
		else if (id == EMPLOYEEID){
			currPass = EMPLOYEEID; //Employee ID
		}
		else{
			strcpy(errMsg,"Invalid ID!");
			errorMsg(errMsg);
			break;
		}

		//change state based on previous input
  		switch (prevInput)
  		{
			case 'u':
				state = USEPASS;
			break;
			case 'b':
				state = BALANCEMENU;
			break;
			case 'a':
				state = ADDMONEY;
			break;
			default:
				strcpy(errMsg,"Invalid Selection!");
				errorMsg(errMsg);
			break;
  		}
		break;
	case USEPASS:	
		//Display Transport Type Menu Text
		printf("*****************************\n");
  		printf("Select Transport Type:\n\n");
  		printf("Bus (b)\n");
  		printf("Subway (s)\n");
		printf("Commuter Rail (r)\n");
		printf("Elderly Bus (e)\n");
  		printf("Return to Main Menu (x)\n");
  		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		
		//Process input
  		switch (str[0])
  		{
  			case 'b':
				transTypeSelection = BUS;
			break;
			case 's':
				transTypeSelection = SUBWAY;
			break;
			case 'r':
				transTypeSelection = RAIL;
			break;
			case 'e':
				transTypeSelection = ELDERLYBUS;
			break;
			case 'x':
				state = MAINMENU;
			break;
			default:
				strcpy(errMsg,"Invalid Selection!");
				errorMsg(errMsg);
				state = TRANSPORTMENU;
			break;
  		}
		
		//Calulate transit cost
		float transitCost = getCost(currPass);

		//Compare selection with existing pass transport type
		if (transTypeSelection == passList[currPass].transType){
			//check balance on pay per ride pass
			if (passList[currPass].Type == PAYPERUSE){
				if (passList[currPass].Balance >= transitCost){
					//Deduct cost from balance
					passList[currPass].Balance = passList[currPass].Balance - transitCost;
					useSuccess(currPass);
				}
				//else not enough money
				else{
					printf("*****************************\n\n");
					printf("Not Enough Money\nPlease add money from Main Menu\n");
				}
			}
			//Else either monthly use or employee pass
			else {
				useSuccess(currPass);
			}
		}
		//Wrong pass type
		else {
			strcpy(errMsg,"Wrong Pass Type!");
			errorMsg(errMsg);
		}
		//Always return to main(success/fail)
		state = MAINMENU;	
		break;
	case BALANCEMENU:	
		//Process print statement
		printPass(currPass);
		state = MAINMENU;
		break;
	case PURCHASE:		
		//Ask customer to pay
		printf("*****************************\n\n");
  		printf("Enter payment of $%.2f\n\n", currPrice);
		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		//store payment 
		p = strtof(str, &pErr);
		//Check for input error
		if (str[0] == '\n' || *pErr != '\n' || str == pErr){
			strcpy(errMsg,"Invalid Input!");
			errorMsg(errMsg);
			state = PURCHASE;
			break;
		}

		//Payment processing
		//Payment was enough
		if (p >= currPrice) {
			//Calculate change
			float change = p - currPrice;

			//If pay per use, add $ to balance
			if (passList[currPass].Type == PAYPERUSE) passList[currPass].Balance = currPrice;
			//If pass is monthly add date to track expiration
			if (passList[currPass].Type == MONTHLY) passList[currPass].purchTime = currTime;
			
			//Display Purchase Success Menu Text
			//clear screen for ease of use
  			clrscr();
			printf("*****************************\n");
  			printf("Purchase Successful!\n\n");
			printf("Purchase Price: $%.2f, Change: $%.2f\n\n", currPrice, change);

			//Finish by showing summary of new pass
			printPass(currPass);			
			lastPass++;		//Increment pass counter
			currPrice = 0;		//reinit current price
			state = MAINMENU;
		}
		//Not enough payment
		else {
			currPrice = currPrice - p;
			printf("*****************************\n");
  			printf("Remaining Balance Due: $%.2f\n\n", currPrice);
			printf("*****************************\n");
			//get char to pause on output screen
			getchar();
			state = PURCHASE; //Go back to process additional payment	
		}
		break;
	case ADDMONEY:
		//Ask customer to pay
		printf("*****************************\n\n");
  		printf("Enter payment:\n\n");
		printf("*****************************\n\n");

		//Retrieve user input
		fgets(str,sizeof str,stdin);
		//get payment
		p = strtof(str, &pErr);
		//Check for input error
		if (str[0] == '\n' || *pErr != '\n' || str == pErr){
			strcpy(errMsg,"Invalid Input!");
			errorMsg(errMsg);
			state = ADDMONEY;
			break;
		}
		
		//Add to balance
		passList[currPass].Balance = passList[currPass].Balance + p;
		state = MAINMENU;			
	break;		
	default:
		state = MAINMENU;
		break;
  }

  //Travel to next state
  menuNav(state); 
}//end menuNav()

/*************************************
*Function: void printPass(int currPass)
*
*Print Pass information
**************************************/
void printPass(int currPass)
{
	char buffer[26];
	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", currTime);

	printf("*****************************\n");
	puts(buffer);
	printf("\nPass ID: %d\n",passList[currPass].ID);
	printf("Discount: %.0f%c\n", passList[currPass].Discount*100,'%');
	printf("Type: %s\n", typeList[passList[currPass].Type]);
	printf("Transportation Type: %s\n", transList[passList[currPass].transType]);
	printf("Balance: $%.2f (Monthly passes have $0)\n\n", passList[currPass].Balance);
	printf("Press enter to return to Main Menu\n");
  	printf("*****************************\n\n");
		
	getchar(); //get char to pause on output screen
}//end printPass()

/*************************************
*Function: void errorMsg(char message[])
*
*Print Error message
**************************************/
void errorMsg(char message[])
{
	//clear screen for ease of use
  	clrscr();

	printf("*****************************\n\n");
	printf("%s\n\n",message);
	printf("Press any button to try again!\n\n");
	printf("*****************************\n\n");

	getchar(); //get char to pause on output screen
}//end errorMsg()

/*************************************
*Function: void useSuccess(int currPass)
*
*Print successful use message
**************************************/
void useSuccess(int currPass){
	//clear screen for ease of use
  	clrscr();

	printf("*****************************\n");
	printf("%s Used on %s\n\n", typeList[passList[currPass].Type], transList[passList[currPass].transType]);
	printf("Thanks for using %s!\n\n", TRANSITAUTHORITY);
	printf("*****************************\n");
	//Print pass summary
	printPass(currPass);
}//end useSuccess()


/*************************************
*Function: float getCost(int currPass)
*
*Calculate cost of pass or ride
**************************************/
float getCost(int currPass)
{
	float price;

	//Get base cost
	price = priceTable[passList[currPass].Type][passList[currPass].transType];
	//Apply pass discount (student or elderly)
	price = price*(1-passList[currPass].Discount);

	//Pay Per Use passes discounted on Saturday and Sunday
	if (currTime->tm_wday == 0 || currTime->tm_wday == 7){
		if (passList[currPass].Type == PAYPERUSE){
			price = price * (1-WEEKENDDISCOUNT);
		}
	}

	//Monthly Passes discounted after 15th of each month
	if (currTime->tm_mday > 15){
		if (passList[currPass].Type == MONTHLY){
			price = price * (1-PRORATEMONTH);
		}
	}

	//FUTURE 
	//Calculate price based on length of trip

	//return rounded value to 2 decimal places
	return(price);
}//end getCost()

    




