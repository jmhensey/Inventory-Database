// CLIENT PROGRAM PartDBConsoleDriver.cpp
// Version: 2008.06.05
// Author: K.R. Johnson
/* Test driver program for class CPartDB
All class descriptions/declarations and interface descriptions are in PartDB.h
*/
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
#include "CPartDB.h"
/*
Input: cin
Output: cout
Returns: 0 for normal return; 1 for failed stream
*/
void DisplayRtnCode (CPartDB::RTN); // function proto to examine return code
void DisplayMenu ();				// function prototype to display main menu
bool DisplayDB (CPartDB*);			// function prototype to display entire database

int main ()
{
	// variables to hold and update part-level data members
	int		iPartNum = 0;	
	string	sPartDescrip = "NONE";
	int		iUnitsOnHand = 0;
	float	fUnitCost = 0.0f;	
	CPart::STATE ePartState;

	int		iUnitsToAdd = 0;			
	int		iUnitsToSubtract = 0;
	int		iUpdatedUnitsOnHand = 0;	// receive new inventory level
	float	fPartInventoryCost;			// receive calculated value of a part

	// variables for PartDB class-level functions
	float	fTotalInventoryCost;		// receive calulated value of all parts
	string	sFileName;					// name of file to save/restore database
	int		iNumRecords;				// number of records in database

	// database variables
	int		iLengthDB;					// capacity of DB in number of parts
	CPartDB::RTN partdbRtn;				// return code from function calls
	CPartDB* pPartDB;					// head pointer to the part database
	CPartDB::SEARCH_TYPE eSearchType;	// receive or set current search type

	//  variables for interacting with user
	string  sCommand;					// user input command
	string::size_type cLen;				// holds length of a string

	// initialization
	cout << "What is the maximum number of parts this database will hold? ";
	cin >> iLengthDB;
	if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
	while (iLengthDB < 1) // database length must be >= 1
	{
		cout << "Database size must be at least 1. Re-enter: ";
		cin >> iLengthDB;
		if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
	}
	pPartDB = new CPartDB(iLengthDB); // dynamically acquire the database
	cout << "New database allocated with capacity of " << iLengthDB;
	if (iLengthDB == 1) cout << " part" << endl;
	else cout << " parts" << endl;

	cout << fixed << showpoint << setprecision(2);	// set up output for floats

	// main loop -- one iteration per command
	do
	{
		// get and validate user command
		cout << "Enter command, or ME to see menu: "; // priming
		cin >> sCommand;			// priming read
		cLen = sCommand.length();	// get length user input
		while (cLen != 2)			// command must be length 2
		{
			cout << "Command must be 2 characters -- re-enter: ";
			cin >> sCommand;			// priming read
			cLen = sCommand.length();	// get length user input
		}
		sCommand[0] = toupper(sCommand[0]); // upper case both characters
		sCommand[1] = toupper(sCommand[1]);

		//**********************************************
		// handle all valid commands in this section
		//**********************************************

		// ME Menu
		if (sCommand == "ME") DisplayMenu();

		// IP Insert Part
		else if (sCommand == "IP")
		{
			cin >> iPartNum >> sPartDescrip >> iUnitsOnHand >> fUnitCost;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->InsertPart  
				(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << "Inserted: ";
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end IP

		// DP Delete Part
		else if (sCommand == "DP")
		{
			partdbRtn = pPartDB->DeletePart();
			DisplayRtnCode(partdbRtn);
		} // end DP

		// PN Position Next
		else if (sCommand == "PN")
		{
			partdbRtn = pPartDB->PositionNext();
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' '
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end PN

		// PP Position Previous
		else if (sCommand == "PP")
		{
			partdbRtn = pPartDB->PositionPrevious();
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' '
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end PP

		// PF Position First
		else if (sCommand == "PF")
		{
			partdbRtn = pPartDB->PositionFirst();
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' '
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end PF

		// PL Position Last
		else if (sCommand == "PL")
		{
			partdbRtn = pPartDB->PositionLast();
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' '
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end PL

		// AU Add Units
		else if (sCommand == "AU")
		{
			cin >> iUnitsToAdd;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->AddUnits(iUnitsToAdd,iUpdatedUnitsOnHand);
			if (partdbRtn == CPartDB::OK)
			{
				cout << "Updated units = " << iUpdatedUnitsOnHand << endl;
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end AU

		// SU Subtract Units
		else if (sCommand == "SU")
		{
			cin >> iUnitsToSubtract;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->SubtractUnits(iUnitsToSubtract,iUpdatedUnitsOnHand);
			if (partdbRtn == CPartDB::OK)
			{
				cout << "Updated units = " << iUpdatedUnitsOnHand << endl;
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end SU

		// SS Set Search type
		else if (sCommand == "SS")
		{
			char cSearchType;
			cin >> cSearchType;
			if (tolower(cSearchType) == 'n')
			{
				eSearchType = CPartDB::NUMBER;
				cout << "Setting search by number" << endl;
				partdbRtn = pPartDB->SetSearchType(eSearchType);
				DisplayRtnCode(partdbRtn);
			}
			else if (tolower(cSearchType) == 'd')
			{
				eSearchType = CPartDB::DESCRIPTION;
				cout << "Setting search by description" << endl;
				partdbRtn = pPartDB->SetSearchType(eSearchType);
				DisplayRtnCode(partdbRtn);
			}
			else cout << "Invalid search type -- no action taken" << endl;			
		} // end SS

		// GS Get Search type
		else if (sCommand == "GS")
		{
			partdbRtn = pPartDB->GetSearchType(eSearchType);
			DisplayRtnCode(partdbRtn);
			if (eSearchType == CPartDB::NUMBER)
				cout << "Search by number" << endl;
			else if (eSearchType == CPartDB::DESCRIPTION)
				cout << "Search by description" << endl;
			else cout << "Unknown search type" << endl; // safety net
		} // end GS

		// SN Search for part Number
		else if (sCommand == "SN")
		{
			cin >> iPartNum;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->SearchForPart(iPartNum);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end SN

		// SD Search for part Description
		else if (sCommand == "SD")
		{
			cin >> sPartDescrip;
			partdbRtn = pPartDB->SearchForPart(sPartDescrip);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end SD

		// GI Get part Info (all data members for part)
		else if (sCommand == "GI")
		{
			partdbRtn = pPartDB->GetPartInfo 
				(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartState(ePartState);
				if (partdbRtn == CPartDB::OK) 
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost;
					if (ePartState == CPart::CONSTRUCTED) cout << " CONSTRUCTED\n";
					else if (ePartState == CPart::INITIALIZED) cout<<" INITIALIZED\n";
					else cout << " INVALID STATE\n";
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end GI

		// DA Display All parts in daatabase
		else if (sCommand == "DA")
		{
			if (DisplayDB(pPartDB)) cout << "Display all parts was successful" << endl;
			else cout << "Display all parts was unsuccessful" << endl;
		} // end DA

		// RU Replace Units on hand
		else if (sCommand == "RU")
		{
			cin >> iUnitsOnHand;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->ReplaceUnitsOnHand(iUnitsOnHand);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end RU

		// RC Replace unit Cost
		else if (sCommand == "RC")
		{
			cin >> fUnitCost;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->ReplaceUnitCost(fUnitCost);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end RC

		// RN Replace part Number
		else if (sCommand == "RN")
		{
			cin >> iPartNum;
			if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
			partdbRtn = pPartDB->ReplacePartNum(iPartNum);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end RN

		// RD Replace part Description
		else if (sCommand == "RD")
		{
			cin >> sPartDescrip;
			partdbRtn = pPartDB->ReplacePartDescrip(sPartDescrip);
			if (partdbRtn == CPartDB::OK)
			{
				partdbRtn = pPartDB->GetPartInfo 
					(iPartNum,sPartDescrip,iUnitsOnHand,fUnitCost);
				if (partdbRtn == CPartDB::OK)
				{
					cout << iPartNum << ' ' << sPartDescrip << ' ' 
						<< iUnitsOnHand << " $" << fUnitCost << endl;
				}
				else DisplayRtnCode(partdbRtn);
			}
			else DisplayRtnCode(partdbRtn);
		} // end RD

		// CP Calculate Part cost
		else if (sCommand == "CP")
		{
			partdbRtn = pPartDB->CalcPartCost(fPartInventoryCost);
			DisplayRtnCode(partdbRtn);
			if (partdbRtn == CPartDB::OK)
				cout << "Cost of inventory for part " << iPartNum
				<< "= $" << fPartInventoryCost << endl;
		} // end CP

		// CI Calculate Inventory cost
		else if (sCommand == "CI")
		{
			partdbRtn = pPartDB->CalcInventoryCost(fTotalInventoryCost);
			DisplayRtnCode(partdbRtn);
			if (partdbRtn == CPartDB::OK)
				cout << "Cost of total inventory = $" << fTotalInventoryCost << endl;
		} // end CI

		// GN Get Number of parts (show size of DB)
		else if (sCommand == "GN")
		{
			int iNumParts = pPartDB->GetNumberOfParts();
			if (iNumParts == 1) cout << "There is 1 part in the database" << endl;
			else cout << "There are " << iNumParts << " parts in the database" << endl;
		} // end GN

		// FS File Save database
		else if (sCommand == "FS")
		{
			cin >> sFileName;
			partdbRtn = pPartDB->SavePartDB(sFileName,iNumRecords);
			cout << iNumRecords << " parts saved" << endl;
			DisplayRtnCode(partdbRtn);
		} // end FS

		// FR File Restore database
		else if (sCommand == "FR")
		{
			cin >> sFileName;
			partdbRtn = pPartDB->RestorePartDB (sFileName, iNumRecords);
			cout << iNumRecords << " parts restored" << endl;
			DisplayRtnCode(partdbRtn);
		} // end FR

		// FN File New delete and reallocate database
		else if (sCommand == "FN")
		{
			char cChallenge;
			cout << "Are you sure you want to replace the entire database? Y or N: ";
			cin >> cChallenge;
			if (toupper(cChallenge) == 'Y')
			{
				delete pPartDB; // delete entire database
				cout << "Database has been deleted" << endl;
				cout << "What is the maximum number of parts the new database can hold? ";
				cin >> iLengthDB;
				if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
				while (iLengthDB < 1) // database length must be >= 1
				{
					cout << "Database size must be at least 1. Re-enter: ";
					cin >> iLengthDB;
					if (!cin) {cout << "Input stream failed -- program ending\n"; return 1;}
				}
				pPartDB = new CPartDB(iLengthDB); // dynamically acquire the database
				cout << "New database allocated with capacity of " << iLengthDB 
					<< " parts" << endl;
			}
		} // end FN

		//******************************************
		// no valid command was found
		//******************************************

		else cout << "Invalid command" << endl;

	} while (sCommand != "QU"); // handle command QU

	cout << "Program ending normally" << endl;
	return 0;
} // main

//**************************************************************************************
void DisplayMenu ()
// Function: display main menu to console
// Pre: none
// Post: transaction menu has been output to cout
{
	cout << endl;
	cout << "ME                                (Display this MEnu)" << endl;
	cout << "QU                                (QUit)" << endl << endl;

	cout << "IP  number descrip units unitcost (Insert Part)" << endl;
	cout << "DP                                (Delete Part)" << endl << endl;

	cout << "SS  n || d                        (Set Search type)" << endl;
	cout << "GS                                (Get Search type)" << endl;
	cout << "SN  number                        (Search for Number)" << endl;
	cout << "SD  descrip                       (Search for Description)" << endl;
	cout << "PN                                (Position at Next part)" << endl;
	cout << "PP                                (Position at Previous part)" << endl;
	cout << "PF                                (Position at First part)" << endl;
	cout << "PL                                (Position at Last part)" << endl;
	cout << "GI                                (Get current part Info)" << endl << endl;

	cout << "RN  number                        (Replace part Number)" << endl;
	cout << "RD  descrip                       (Replace part Description)" << endl;
	cout << "RC  dollars.cents                 (Replace unit Cost)" << endl;
	cout << "RU  units                         (Replace Units on hand)" << endl;
	cout << "AU  units                         (Add Units)" << endl;
	cout << "SU  units                         (Subtract Units)" << endl << endl;

	cout << "CP                                (Calculate Part inventory cost)" << endl;
	cout << "CI                                (Calculate total Inventory cost)" << endl;
	cout << "DA                                (Display All parts in database)" << endl;
	cout << "GN                                (Get Number of parts in DB)" << endl;
	cout << "FS  filename                      (File Save to file)" << endl;
	cout << "FR  filename                      (File Restore from file)" << endl;
	cout << "FN                                (File New database)" << endl;

}// DisplayMenu

//**************************************************************************************
void DisplayRtnCode (/* in */ CPartDB::RTN partdbRtn)
// Function: examine return code from CPartDB calls and display explanation
// Pre: partdbRtn has been assigned
// Post: descriptive message has been output
{
	switch (partdbRtn)
	{
	case CPartDB::OK: cout << "Successful" << endl;
		break;
	case CPartDB::PARTNUM_DUP: cout << "Part number is duplicate" << endl;
		break;
	case CPartDB::PART_NOT_FOUND: cout << "Part not found" << endl;
		break;
	case CPartDB::INVALID_SEARCH_TYPE: cout << "Invalid search type" << endl;
		break;
	case CPartDB::PARTDESCRIP_EMPTY: cout << "Part description is zero length" << endl;
		break;
	case CPartDB::PARTUNITS_INVALID_RANGE: cout << "Inventory negative or too large" 
											   << endl;
		break;
	case CPartDB::PARTUNIT_COST_NEG: cout << "Unit cost cannot be negative" << endl;
		break;
	case CPartDB::FILE_OPEN_ERROR: cout << "File save open error" << endl;
		break;
	case CPartDB::DATABASE_FULL: cout << "Database is full" << endl;
		break;
	case CPartDB::DATABASE_EMPTY: cout << "Database is empty" << endl;
		break;
	case CPartDB::UNKNOWN_ERROR: cout << "Unknown error" << endl;
		break;
	default: cout << "Unidentified return code" << endl; // safety net
	} // switch (partdbRtn)
} // DisplayRtnCode

//**************************************************************************************
bool DisplayDB (/* in */ CPartDB* pPartDB)
// Function: display entire database first by part number, then by description
// Pre: pPartDB has been assigned
// Post: all parts have been displayed to cout in two sort orders
{
	CPartDB::SEARCH_TYPE eSaveSearchType;

	CPartDB::RTN partdbRtn;		// return code from function calls

	int		iShowPartNum;
	string	sShowPartDescrip;
	int		iShowUnitsOnHand;
	float	fShowUnitCost;

	// save current search type so it can be restored at end of function
	partdbRtn = pPartDB->GetSearchType(eSaveSearchType);
	if (partdbRtn != CPartDB::OK)
	{
		cout << "Could not get search type\n";
		DisplayRtnCode(partdbRtn);
		return false;
	}

	// display all database members in part number sequence
	cout << "Displaying all parts in part number sequence:" << endl;
	partdbRtn = pPartDB->SetSearchType(CPartDB::NUMBER);
	if (partdbRtn != CPartDB::OK) 
	{
		cout << "Could not set search type by number\n";
		DisplayRtnCode(partdbRtn);
		return false;
	}

	partdbRtn = pPartDB->PositionFirst();
	if (partdbRtn != CPartDB::OK)
	{
		cout << "Could not set position first\n";
		DisplayRtnCode(partdbRtn);
		return false;
	}

	do 
	{
		partdbRtn = pPartDB->GetPartInfo 
			(iShowPartNum,sShowPartDescrip,iShowUnitsOnHand,fShowUnitCost);
		if (partdbRtn != CPartDB::OK)
		{
			cout << "Could not get current part information\n";
			DisplayRtnCode(partdbRtn);
			return false;
		}
		cout << iShowPartNum << ' ' << sShowPartDescrip << ' ' << iShowUnitsOnHand
			<< " $"	<< fShowUnitCost << endl;
		partdbRtn = pPartDB->PositionNext();
		if ((partdbRtn != CPartDB::OK) && (partdbRtn != CPartDB::PART_NOT_FOUND))
		{
			cout << "Could not set position next\n";
			DisplayRtnCode(partdbRtn);
			return false;
		}
	} while (partdbRtn == CPartDB::OK);
	cout << endl;

	// display all database members in part description sequence
	cout << "Displaying all parts in part description sequence:" << endl;
	partdbRtn = pPartDB->SetSearchType(CPartDB::DESCRIPTION);
	if (partdbRtn != CPartDB::OK)
	{
		cout << "Could not set search type by description\n";
		DisplayRtnCode(partdbRtn);
		return false;
	}

	partdbRtn = pPartDB->PositionFirst();
	if (partdbRtn != CPartDB::OK)
	{
		cout << "Could not set position first\n";
		DisplayRtnCode(partdbRtn);
		return false;
	}

	do
	{
		partdbRtn = pPartDB->GetPartInfo
			(iShowPartNum,sShowPartDescrip,iShowUnitsOnHand,fShowUnitCost);
		if (partdbRtn != CPartDB::OK)
		{
			cout << "Could not get current part information\n";
			DisplayRtnCode(partdbRtn);
			return false;
		}
		cout << iShowPartNum << ' ' << sShowPartDescrip << ' ' << iShowUnitsOnHand
			<< " $"	<< fShowUnitCost << endl;
		partdbRtn = pPartDB->PositionNext();
		if ((partdbRtn != CPartDB::OK) && (partdbRtn != CPartDB::PART_NOT_FOUND))
		{
			cout << "Could not set position next\n";
			DisplayRtnCode(partdbRtn);
			return false;
		}
	} while (partdbRtn == CPartDB::OK);

	// restore current search type to what it was at function entry
	partdbRtn = pPartDB->SetSearchType(eSaveSearchType);
	if (partdbRtn != CPartDB::OK)
	{
		cout << "Could not restore search type\n";
		DisplayRtnCode(partdbRtn);
		return false;
	}
	cout << endl;
	return true;
} // DisplayDB