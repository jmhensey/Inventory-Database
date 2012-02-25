//CPartDB.cpp
//Author: Julianna Hensey
//Date: 2010/5/9 version 1.0
//Description: This source file includes the function definitions for all functions within the 
//CPartDB.h header file.
//Assumptions: Data has been checked for stream failure

#include "CPartDB.h"
#include <fstream>
using namespace std;


CPartDB::CPartDB() //default constructor
{
	iPartsMax = 100; //set default iPartsMax
	iPartsLength = 0; //set length of array to 0
	papPart = new CPart*[iPartsMax]; //dynamically allocate array of pointers to CPart
	for (int i = 0; i < iPartsMax; i++) //nullify all pointers in new array
		papPart [i] = NULL;
	ePartDBSearch = NUMBER;
	iPartCurrent = -1; //set Current Part to DOES NOT EXIST
	pNdxHead = pNdxTail = pNdxCurrent = NULL; //nullifies current part in linked list
}//default constructor

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::CPartDB(/*in*/int iNumberOfParts) //custom constructor
{
	iPartsMax = iNumberOfParts;
	if (iPartsMax <=0)
		iPartsMax = 100;
	iPartsLength = 0;
	papPart = new CPart*[iPartsMax];
	for (int i = 0; i < iPartsMax; i++)
		papPart [i] = NULL;
	ePartDBSearch = NUMBER;
	iPartCurrent = -1;
	pNdxHead = pNdxTail = pNdxCurrent = NULL; //nullify linked list anchors
}//custom constructor

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::~CPartDB()//destructor
{
	//loop through array and delete all dynamically acquired part objects
	for (int i = 0; i < iPartsMax; i++)
	{
		delete papPart[i]; //delete part
		papPart[i] = NULL; //nullify pointer
	}
	delete papPart; //delete array
	papPart = NULL; //nullify all pointers in the array

	CPartNdxNode* pNdxTemp; //temporary pointer to current index node
	pNdxCurrent = pNdxHead; //start at the beginning of linked list

	//loop through linked list and delete all parts
	while (pNdxCurrent != NULL)
	{
		pNdxTemp = pNdxCurrent;
		pNdxCurrent = pNdxCurrent->pNdxNext;
		delete pNdxTemp;
		pNdxTemp = NULL;
	}
}//destructor

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::SetSearchType(/*in*/ SEARCH_TYPE eNewSearchType)
{
	if (eNewSearchType == NUMBER)
	{
		ePartDBSearch = NUMBER; //search type set to search by number
		return OK;
	}
	else if (eNewSearchType == DESCRIPTION)
	{
		ePartDBSearch = DESCRIPTION; //search type set to search by description
		return OK;
	}
	else return INVALID_SEARCH_TYPE;
}//SetSearchType

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::GetSearchType(/*out*/ CPartDB::SEARCH_TYPE& eSearchType)const
{
	if (ePartDBSearch == NUMBER)
		eSearchType = NUMBER;
	else 
		eSearchType = DESCRIPTION;
	return OK;
}//SetSearchType

///////////////////////////////////////////////////////////////////////////////////////

int CPartDB::GetNumberOfParts () const
{
	return iPartsLength;
}//GetNumberOfParts

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::InsertPart(/*in*/ int iNewPartNum,
								 /*in*/ string sNewPartDescrip,
								 /*in*/ int iNewUnitsOnHand,
								 /*in*/ float fNewUnitCost)
{
	if (iPartsMax == iPartsLength)
		return DATABASE_FULL;
	if(BinarySearch (0, iPartsLength-1, iNewPartNum) != -1) //check part num to see if
		return PARTNUM_DUP;									//part num already exists

	CPart* pNewPart = new CPart; //allocate and construct new object
	CPart::RTN rReturn = pNewPart->Initialize(iNewPartNum, 
		sNewPartDescrip, 
		iNewUnitsOnHand, 
		fNewUnitCost); //initialize part object
	if (rReturn != CPart::OK) //initialize didn't work
	{
		delete pNewPart;	//delete new part object
		pNewPart = NULL;	//disarm the pointer
		//report error
		if (rReturn == CPart::UNITS_INVALID_RANGE)
			return PARTUNITS_INVALID_RANGE;
		if (rReturn == CPart::UNIT_COST_NEG)
			return PARTUNIT_COST_NEG;
		if (rReturn == CPart::DESCRIP_EMPTY)
			return PARTDESCRIP_EMPTY;
		return UNKNOWN_ERROR; //safety
	}

	if (papPart[0] == NULL) //array is empty
	{
		papPart[0] = pNewPart; //insert into first element
		iPartCurrent = 0; //assign new part as current part
	}
	else
	{
		int iIndex = iPartsLength - 1; //loop control
		//compare object to object pointed to by papPart[iIndex]
		while (iIndex >= 0 && (pNewPart->PartNumIsGreaterThan(*papPart[iIndex]) == -1))
		{
			papPart[iIndex + 1] = papPart[iIndex]; //move the array down by one
			iIndex--;
		}
		papPart[iIndex + 1] = pNewPart; //insert part into proper sorted place
		iPartCurrent = iIndex + 1; //assign new part as current part
	}
	iPartsLength++;		//increase array length by 1
	InsertPartIndexNode(papPart[iPartCurrent]); //insert part into linked list
	return OK;
}//InsertPart

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::DeletePart()
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function
	int iFound; //was number found in array? 
	//GetPartInfo variables
	int iReturnedPartNum;
	string sReturnedPartDescrip;
	int iReturnedUnitsOnHand;
	float fReturnedUnitCost;

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	if (ePartDBSearch == NUMBER)
	{
		pPart = papPart[iPartCurrent]; //array
		DeletePartIndexNode(pPart); //call delete node function
		rReturn = pPart->GetPartInfo(iReturnedPartNum, 
			sReturnedPartDescrip, 
			iReturnedUnitsOnHand, 
			fReturnedUnitCost); //get part number
		if (rReturn != OK)
			return UNKNOWN_ERROR; //safety
	}
	else
	{
		pPart = pNdxCurrent->pThisPart; //point to current part
		DeletePartIndexNode(pPart); //delete part in linked list
		rReturn = pPart->GetPartInfo(iReturnedPartNum, 
			sReturnedPartDescrip, 
			iReturnedUnitsOnHand, 
			fReturnedUnitCost); //get part number
		if (rReturn != OK)
			return UNKNOWN_ERROR; //safety
		iFound = CPartDB::BinarySearch(0, 
									   iPartsLength, 
									   iReturnedPartNum); // find part in database
		if (iFound == -1) //if not found
			return PART_NOT_FOUND;
		iPartCurrent = iFound;
	}

	for (int iIndex = iPartCurrent; iIndex < iPartsLength - 1; iIndex++)
		papPart[iIndex] = papPart [iIndex + 1]; //moves the objects up in the list

	iPartsLength--; //reduce length of array
	papPart[iPartsLength] = NULL;
	delete pPart;

	if (iPartsLength == 0) //database empty
		iPartCurrent = -1; //Invalidate current position by invalidating iPartCurrent

	if (iPartCurrent == iPartsLength) //deleted part was last in array
		iPartCurrent--;

	return OK;
}//DeletePart

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::SearchForPart(/*in*/ int iSearchForPartNum) //by number
{
	int iLocation;	//location of number to find in array (iLocation == -1 Part DNE)
	//check for values in array
	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	iLocation = BinarySearch (0, iPartsLength-1, iSearchForPartNum); //find number
	if (iLocation == -1)	//number returned by BinarySearch if part DNE
		return PART_NOT_FOUND;
	iPartCurrent = iLocation; //assign place in array to iPartCurrent
	ePartDBSearch = NUMBER;
	return OK;
}//SearchForPart (number)

CPartDB::RTN CPartDB::SearchForPart(/*in*/ string sSearchDescrip) //by description
{
	//GetPartInfo variables
	int iCurrentPartNum;
	string sCurrentPartDescrip;
	int iCurrentUnitsOnHand;
	float fCurrentUnitCost;

	CPart::RTN rReturn;		//return code from CPart function
	CPartNdxNode* pNdxTemp; //current place in search

	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	pNdxTemp = pNdxHead; //start at the head
	while (pNdxTemp != NULL)
	{
		rReturn = pNdxTemp->pThisPart->GetPartInfo(iCurrentPartNum, 
			sCurrentPartDescrip, 
			iCurrentUnitsOnHand, 
			fCurrentUnitCost);
		if (rReturn != CPart::OK)
			return UNKNOWN_ERROR; //safety
		if (sCurrentPartDescrip == sSearchDescrip) //descriptions match
		{
			pNdxCurrent = pNdxTemp; //assign current position
			ePartDBSearch = DESCRIPTION; //iterate search type
			return OK;
		}
		else if (sCurrentPartDescrip > sSearchDescrip) //part not in database
			return PART_NOT_FOUND;

		else
			pNdxTemp = pNdxTemp->pNdxNext; //go to the next part
	}
	return PART_NOT_FOUND; //reached end of list
}//SearchForPart (descrip)

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::PositionNext ()
{
	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER)//check if we are moving through array or linked list
	{
		if (iPartCurrent == iPartsLength - 1) //are we at the last part?
			return PART_NOT_FOUND;
		iPartCurrent++; //move to the next part
		return OK;
	}
	if (pNdxCurrent -> pNdxNext == NULL) //check to see if end of link list
		return PART_NOT_FOUND;
	pNdxCurrent = pNdxCurrent -> pNdxNext; //go to next part in link list
	return OK;
}//PositionNext

CPartDB::RTN CPartDB::PositionPrevious()
{
	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER)//check if we are moving through array or linked list
	{
		if (iPartCurrent == 0) //are we at the first part?
			return PART_NOT_FOUND;
		iPartCurrent--; //move to the previous part
		return OK;
	}
	if (pNdxCurrent -> pNdxPrevious == NULL) //check to see if at start of link list
		return PART_NOT_FOUND;
	pNdxCurrent = pNdxCurrent -> pNdxPrevious; //go to previous part in link list
	return OK;
}//PositionPrevious

CPartDB::RTN CPartDB::PositionFirst()
{
	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER) 
		iPartCurrent = 0;
	else 
		pNdxCurrent = pNdxHead;
	return OK;
}//PositionFirst

CPartDB::RTN CPartDB::PositionLast()
{
	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER)
		iPartCurrent = iPartsLength - 1;
	else
		pNdxCurrent = pNdxTail;
	return OK;
}//PositionLast

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::GetPartInfo (/*out*/ int& iGetPartNum,
								   /*out*/ string& sGetPartDescrip,
								   /*out*/ int& iGetUnitsOnHand,
								   /*out*/ float& fGetUnitCost)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart->GetPartInfo(iGetPartNum, 
		sGetPartDescrip, 
		iGetUnitsOnHand, 
		fGetUnitCost);

	if (rReturn == OK) 
		return OK;
	else
		return UNKNOWN_ERROR; //safety
}//GetPartInfo

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::GetPartState(/*out*/ CPart::STATE& eOutPartState)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else 
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart->GetPartState(eOutPartState); //get part state of current part

	if (rReturn == OK)
		return OK;
	else
		return UNKNOWN_ERROR; //safety
}//GetPartState

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::AddUnits(/*in*/ int iAddUnitsOnHand, 
							   /*out*/ int& iUpdatedUnitsOnHand)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;
	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart->AddUnits(iAddUnitsOnHand, iUpdatedUnitsOnHand); //add units to current part

	if (rReturn == OK)
		return OK;
	else
		return UNKNOWN_ERROR; //safety
}//AddUnits

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::SubtractUnits(/*in*/ int iSubtractUnitsOnHand, 
									/*out*/ int &iUpdatedUnitsOnHand)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart->SubtractUnits(iSubtractUnitsOnHand, //subtract units from 
		iUpdatedUnitsOnHand); //current part

	if (rReturn == OK)
		return OK;
	else
		return PARTUNITS_INVALID_RANGE; //safety
}//SubtractUnits

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::ReplaceUnitsOnHand(/*in*/ int iReplaceUnitsOnHand)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart->ReplaceUnitsOnHand(iReplaceUnitsOnHand); //replace units in current part

	if (rReturn == OK)
		return OK;
	else
		return PARTUNITS_INVALID_RANGE; //safety
}//ReplaceUnitsOnHand

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::ReplaceUnitCost(/*in*/ float fReplaceUnitCost)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart->ReplaceUnitCost(fReplaceUnitCost); //replace cost of current part

	if (rReturn == OK)
		return OK;
	else
		return PARTUNIT_COST_NEG; //safety
}//ReplaceUnitCost

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::ReplacePartNum(/*in*/ int iReplacePartNum)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function
	CPartDB::RTN rdbReturn; //return code from CPartDB function

	//get part info & insert part variables
	int iTempPartNum; 
	string sTempPartDescrip;
	int iTempUnitsOnHand;
	float fTempUnitCost;

	int iResponse; //return from ReplacePartNum

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	//check for duplicate number in database
	iResponse = CPartDB::BinarySearch(0, iPartsLength-1, iReplacePartNum);
	if (iResponse != -1)
		return PARTNUM_DUP;

	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart;

	rReturn = pPart->ReplacePartNum(iReplacePartNum);
	if (rReturn != OK)
		return UNKNOWN_ERROR; //safety

	rdbReturn = GetPartInfo (iTempPartNum, 
		sTempPartDescrip, 
		iTempUnitsOnHand, 
		fTempUnitCost);
	if (rdbReturn != OK)
		return UNKNOWN_ERROR; //safety

	DeletePart(); //delete current part

	InsertPart(iTempPartNum, 
			   sTempPartDescrip, 
			   iTempUnitsOnHand, 
			   fTempUnitCost); //insert part deleted with new part number

	iPartCurrent = BinarySearch (0, iPartsLength - 1, iReplacePartNum);

	return OK;
}//ReplacePartNum

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::ReplacePartDescrip(/*in*/ string sReplacePartDescrip)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list

	rReturn = pPart -> ReplacePartDescrip(sReplacePartDescrip); //replace descrip of current part

	if (rReturn != OK) 
		return PARTDESCRIP_EMPTY;

	DeletePartIndexNode(pPart); //delete the old node
	InsertPartIndexNode(pPart); //insert in proper place
	return OK;
}//ReplacePartDescrip

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::CalcPartCost(/*out*/ float &fPartTotalCost)
{
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function
	//get part info
	int iGetPartNum;
	string sGetPartDescrip;
	int iGetUnitsOnHand;
	float fGetUnitCost;

	if (iPartsLength == 0)
		return DATABASE_EMPTY;

	if (ePartDBSearch == NUMBER)
		pPart = papPart[iPartCurrent]; //array
	else
		pPart = pNdxCurrent->pThisPart; //linked list


	rReturn = pPart->GetPartInfo(iGetPartNum, 
		sGetPartDescrip, 
		iGetUnitsOnHand, 
		fGetUnitCost); //retrieve UnitCost
	if (rReturn != OK)
		return UNKNOWN_ERROR; //safety

	fPartTotalCost = float(iGetUnitsOnHand) * fGetUnitCost;	//calculate total cost

	return OK; 
}//CalcPartCost

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::CalcInventoryCost(/*out*/ float& fInventoryCost)
{
	float fAccumulator = 0; //total inventory cost at the moment
	CPart* pPart;
	CPart::RTN rReturn; //return code from CPart function		
	CPartDB::RTN rdbReturn; //return code from CPartDB function
	//get part info variables
	int iGetPartNum;
	string sGetPartDescrip;
	int iGetUnitsOnHand;
	float fGetUnitCost;

	//move current pointer to start of array
	rdbReturn = CPartDB::PositionFirst();
	if (rdbReturn != OK)
		return UNKNOWN_ERROR; //safety

	for (int i = 0; i < iPartsLength; i++)
	{ 
		if (ePartDBSearch == NUMBER)
			pPart = papPart[iPartCurrent]; //array
		else
			pPart = pNdxCurrent->pThisPart; //linked list

		rReturn = pPart->GetPartInfo(iGetPartNum, 
			sGetPartDescrip, 
			iGetUnitsOnHand, 
			fGetUnitCost); //get unit cost of current part
		if (rReturn != OK)
			return UNKNOWN_ERROR; //safety
		//calculate total cost and add to acumulator
		fAccumulator += (float(iGetUnitsOnHand) * fGetUnitCost);

		if (i != iPartsLength - 1) //not the last element of the array
		{
			rdbReturn = CPartDB::PositionNext();
			if (rdbReturn != OK)
				return UNKNOWN_ERROR; //safety
		}
	}
	fInventoryCost = fAccumulator;

	return OK;
}//CalcInventoryCost

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::SavePartDB(/*in*/ string sFileName, 
								 /*out*/ int& iNumRecords)
{
	ofstream ofSaveFile;
	CPart::RTN rReturn; //return code from CPart function
	CPartDB::RTN rdbReturn; //return code from CPartDB function
	int iNumSaved; //counter for how many parts are being saved
	//GetPartInfo variables
	int iGetPartNum;
	string sGetPartDescrip;
	int iGetUnitsOnHand;
	float fGetUnitCost;

	if (iPartsLength == 0)
		return DATABASE_EMPTY;


	ofSaveFile.open(sFileName.c_str());
	if (!ofSaveFile)
		return FILE_OPEN_ERROR;
	ofSaveFile << iPartsLength << endl;

	iNumSaved = 0; //initialize counter
	rdbReturn = CPartDB::PositionFirst(); //start at beginning of array or linked list

	for (int i = 0; i < iPartsLength; i++) //loop to save parts to file
	{
		rReturn = papPart[i]->GetPartInfo(iGetPartNum, 
			sGetPartDescrip, 
			iGetUnitsOnHand, 
			fGetUnitCost);
		if (rReturn != OK)
			return UNKNOWN_ERROR; //safety

		//save data members of part
		ofSaveFile << iGetPartNum << ' ';
		ofSaveFile << sGetPartDescrip << ' '; 
		ofSaveFile << iGetUnitsOnHand << ' ';
		ofSaveFile << fGetUnitCost << endl;

		if (i != iPartsLength - 1) //not last element of array/linked list
		{
			rdbReturn = CPartDB::PositionNext();
			if (rdbReturn != OK)
				return UNKNOWN_ERROR; //safety
		}
		iNumSaved++;
	}
	ofSaveFile.close();
	iNumRecords = iNumSaved;

	return OK;
}//SavePartDB

///////////////////////////////////////////////////////////////////////////////////////

CPartDB::RTN CPartDB::RestorePartDB(/*in*/ string sFileName, 
									/*out*/ int& iNumRecords)
{
	ifstream ifRestoreFile;
	int iSavedSize; //size of saved database
	int iRecordCounter; //amount of 
	int iRestoreSize;
	CPartDB::RTN rdbReturn;  //return code from CPart function
	//variables for part information from file
	int iFilePartNum;
	string sFilePartDescrip;
	int iFileUnitsOnHand;
	float fFileUnitCost;

	ifRestoreFile.open(sFileName.c_str());
	if (!ifRestoreFile)
		return FILE_OPEN_ERROR;

	ifRestoreFile >> iSavedSize; //get size of saved database
	if (iSavedSize < 1)
		return DATABASE_EMPTY;

	ePartDBSearch = NUMBER; //set database to search by number

	//delete all parts in memory database leaving room for saved parts in file
	for (int i=0; i < iPartsLength; i++)
	{
		iPartCurrent = i;
		rdbReturn = DeletePart();
		if (rdbReturn != OK)
			return UNKNOWN_ERROR; //safety
	}

	if (iPartsMax < iSavedSize) //more parts in saved file than room in database
		iRestoreSize = iPartsMax; //number of parts restored == size of database
	else
		iRestoreSize = iSavedSize; //number of parts restored == num saved parts

	iRecordCounter = 0; //initialize record counter
	for (int i = 0; i < iRestoreSize; i++)
	{
		iPartCurrent = i; //incrememt iPartCurrent with counter
		//load part members from file
		ifRestoreFile >> iFilePartNum;
		ifRestoreFile >> sFilePartDescrip;
		ifRestoreFile >> iFileUnitsOnHand;
		ifRestoreFile >> fFileUnitCost;

		rdbReturn = InsertPart(iFilePartNum, 
			sFilePartDescrip, //insert part with members from file
			iFileUnitsOnHand, //into the database
			fFileUnitCost); 
		if (rdbReturn != OK)
			return UNKNOWN_ERROR; //safety
		iRecordCounter++;
	}
	iNumRecords = iRecordCounter;

	ifRestoreFile.close();

	iPartCurrent = 0; //set current part to beginning of array
	pNdxCurrent = pNdxHead; //set current part to beginning of linked list

	if (iRecordCounter < iSavedSize) //number of restored parts exceeded limits of database size
		return DATABASE_FULL;

	return OK;
}//RestorePartDB

///////////////////////////////////////////////////////////////////////////////////////

void CPartDB::InsertPartIndexNode(/*in*/ CPart* pPart)
{
	CPartNdxNode* pNdxTempNew; //pointer to new node
	CPartNdxNode* pNdxTempWork; //moving pointer
	CPartNdxNode* pNdxTempPrev; //pointer to node before *pNdxTempWork

	pNdxTempNew = new CPartNdxNode; //allocate new node
	pNdxCurrent = pNdxTempNew; //it becomes the current position
	pNdxTempNew->pThisPart = pPart; //assign component member

	//find insertion point
	pNdxTempPrev = NULL;
	pNdxTempWork = pNdxHead;
	while (pNdxTempWork != NULL &&
		(pPart->PartDescripIsGreaterThan(*pNdxTempWork->pThisPart) == 1))
	{
		pNdxTempPrev = pNdxTempWork;
		pNdxTempWork = pNdxTempWork->pNdxNext; //chase chain until NULL
	}

	//At this point, pNdxTempNew points to new node, which has no links
	//The link configurations depend on contents of pNdxTempPrev and pNdxTmpWork:
	//1. insert node as first and last (the only node)
	//2. insert node as first with others following
	//3. insert node as the new last node
	//4. insert node in the middle, with others before and after

	//first and only node
	if ((pNdxTempPrev == NULL) && (pNdxTempWork == NULL))
	{	
		pNdxTempNew->pNdxPrevious = NULL;
		pNdxTempNew->pNdxNext = NULL;
		pNdxHead = pNdxTempNew;
		pNdxTail = pNdxTempNew;
	}
	//first node
	else if ((pNdxTempPrev == NULL) && (pNdxTempWork != NULL))
	{
		pNdxTempNew->pNdxNext = pNdxTempWork;
		pNdxTempNew->pNdxPrevious = NULL;
		pNdxHead = pNdxTempNew;
		pNdxTempWork->pNdxPrevious = pNdxTempNew;
	}
	//last node
	else if ((pNdxTempPrev != NULL) && (pNdxTempWork == NULL))
	{
		pNdxTempNew->pNdxNext = NULL;
		pNdxTempNew->pNdxPrevious = pNdxTempPrev;
		pNdxTempPrev->pNdxNext = pNdxTempNew;
		pNdxTail = pNdxTempNew;
	}
	//insert in the middle
	else
	{
		pNdxTempNew->pNdxNext = pNdxTempWork;
		pNdxTempNew->pNdxPrevious = pNdxTempPrev;
		pNdxTempPrev->pNdxNext = pNdxTempNew;
		pNdxTempWork->pNdxPrevious = pNdxTempNew;
	}
}//InsertPartIndexNode

///////////////////////////////////////////////////////////////////////////////////////

void CPartDB::DeletePartIndexNode(/*in*/ CPart* pPart)
{
	CPartNdxNode* pNdxTempWork; //moving pointer
	CPartNdxNode* pNdxTempDelete; //pointer to node to be deleted

	//check if first node is the one to be deleted
	if (pPart == pNdxHead->pThisPart)
	{
		pNdxTempDelete = pNdxHead;
		if (pNdxHead->pNdxNext != NULL) //not only node in linked list
			pNdxHead->pNdxNext->pNdxPrevious = NULL; //set next node's pNdxPrevious to NULL
		pNdxHead = pNdxHead->pNdxNext; //next node now pointed to by pNdxHead
		if (pNdxHead == NULL) //list is empty
			pNdxTail = NULL;
		pNdxCurrent = pNdxHead; //set current part to beginning of linked list
	}
	//part other than first node is deleted
	else
	{
		pNdxTempWork = pNdxHead;
		//transverse array until pPart is found
		while (pNdxTempWork->pNdxNext->pThisPart != pPart)
			pNdxTempWork = pNdxTempWork->pNdxNext;

		pNdxTempDelete = pNdxTempWork->pNdxNext; //pNdxTempDelete == part to be deleted
		//part preceeding part to be deleted points through to part proceeding part to be deleted
		pNdxTempDelete->pNdxPrevious->pNdxNext = pNdxTempDelete->pNdxNext; 
		//if deleted node is the last node
		if(pNdxTempDelete->pNdxNext == NULL)
		{
			//reassign pNdxTail
			pNdxTail = pNdxTempDelete->pNdxPrevious;
			//move part current to new last member of linked list
			pNdxCurrent = pNdxTempDelete->pNdxPrevious;
		}
		else
		{
			//point from preceeding part through to next part bypassing pNdxTempDelete
			pNdxTempDelete->pNdxNext->pNdxPrevious = pNdxTempDelete->pNdxPrevious;
			//move part current to part after deleted part
			pNdxCurrent = pNdxTempDelete->pNdxNext;
		}
	}
	delete pNdxTempDelete; //delete part to be deleted
	pNdxTempDelete = NULL; //disarm pointer
}//DeletePartIndexNode

///////////////////////////////////////////////////////////////////////////////////////
/*
int CPartDB::BinarySearch(//in// int iLow, 
						  //in// int iHigh, 
						  //in// int iFindPartNum)
{
CPart::RTN rReturn; //declare holder for return code
int iMiddle; //middle array index
int iMiddlePartNum; //part number associated with list entry
string sMiddlePartDescrip;
int iMiddleUnitsOnHand;
float fMiddleUnitCost;

while (iLow <= iHigh)
{
iMiddle = (iHigh + iLow)/2; //claculate iMiddle between iLow and iHigh

rReturn = papPart[iMiddle]->GetPartInfo(iMiddlePartNum, 
sMiddlePartDescrip, 
iMiddleUnitsOnHand, 
fMiddleUnitCost);
if (rReturn != OK)
return UNKNOWN_ERROR; //safety

if (iFindPartNum > iMiddlePartNum)
iLow = iMiddle + 1;
else if (iFindPartNum < iMiddlePartNum)
iHigh = iMiddle - 1;
else
return iMiddle;
}
return -1;
}//BinarySearch */


int CPartDB::BinarySearch(/*in*/ int iLow, 
						  /*in*/ int iHigh, 
						  /*in*/ int iFindPartNum) //recursive
{
	CPart::RTN rReturn; //declare holder for return code
	int iMiddle; //middle array index
	int iPosition; //number return from BinarySearch
	//GetPartInfo variables
	int iMiddlePartNum;
	string sMiddlePartDescrip;
	int iMiddleUnitsOnHand;
	float fMiddleUnitCost;

	if (iLow <= iHigh)
	{
		iMiddle = (iHigh + iLow)/2; //claculate iMiddle between iLow and iHigh
		rReturn = papPart[iMiddle]->GetPartInfo(iMiddlePartNum, 
			sMiddlePartDescrip, 
			iMiddleUnitsOnHand, 
			fMiddleUnitCost);
		if (rReturn != OK)
			return UNKNOWN_ERROR; //safety

		if (iFindPartNum > iMiddlePartNum) //discard lower half
		{
			iLow = iMiddle + 1; //set part after iMiddle as the low
			iPosition = BinarySearch(iLow, iHigh, iFindPartNum);
		}
		else if (iFindPartNum < iMiddlePartNum) //discard upper half
		{
			iHigh = iMiddle -1; //set part before iMiddle as the high
			iPosition = BinarySearch(iLow, iHigh, iFindPartNum);
		}
		else if (iFindPartNum == iMiddlePartNum)
			return iMiddle; //part found
	}
	//part does not exist in database
	return -1;
}//BinarySearch
