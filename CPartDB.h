// CPartDB.h: header file holding declarations for CPartDB.cpp
// Author: Julianna Hensey
// Date: 2010/5/4  version 1.0

/*
This header file describes the the class CPartDB. CPartDB includes a constructor, 
copy constructor and destructor (CPartDB(), CPartDB(iNumberofParts) and ~CPartDB()). 
It also includes transformer and accessor functions.
Transformer Functions: 
	RTN SetSearchType (SEARCH_TYPE eSearchType)
	RTN InsertPart (int iNewPartNum, string sNewPartDescrip, 
					int iNewUnitsOnHand, float fNewUnitCost)
	RTN DeletePart ()
	RTN AddUnits (int iAddUnitsOnHand, int& iUpdatedUnitsOnHand)
	RTN SubtractUnits (int iSubtractUnitsOnHand, int& iUpdatedUnitsOnHand)
	RTN ReplaceUnitsOnHand (int iReplaceUnitsOnHand)
	RTN ReplaceUnitCost (float fReplaceUnitCost)
	RTN ReplacePartNum (int iReplacePartNum)
	RTN ReplacePartDescrip (string sReplacePartDescrip)
	RTN CalcPartCost (float& fPartTotalCost)
	RTN CalcInventoryCost (float& fInventoryCost)
	RTN SavePartDB (string sFileName, int& iNumRecords)
	RTN RestorePartDB (string sFileName, int& iNumRecords)
	void InsertPartIndexNode (CPart* pPart)
	void DeletePartIndexNode (CPart* pPart)
Accessor Functions:
	RTN GetSearchType (SEARCH_TYPE& eSearchType) const
	int GetNumberOfParts () const
	RTN SearchForPart (int iSearchForPartNum)
	RTN SearchForPart (string sSearchDescrip)
	RTN PositionNext ()
	RTN PositionPrevious ()
	RTN PositionFirst ()
	RTN PositionLast ()
	RTN GetPartInfo (int& iGetPartNum, string& sGetPartDescrip,
					 int& iGetUnitsOnHand, float& fGetUnitCost)
	RTN GetPartState (CPart::STATE& eOutPartState)
	int BinarySearch (int iLow, int iHigh, int iFindPartNum)
*/

#include "Part.h"

class CPartDB
{
public:
	enum RTN //return codes for functions
	{
		OK,							//everything executed correctly
		INVALID_SEARCH_TYPE,		//search type is not NUMBER or DESCRIP
		DATABASE_FULL,				//database is full
		DATABASE_EMPTY,				//database is empty
		PARTNUM_DUP,				//part number is a duplicate
		PARTUNITS_INVALID_RANGE,	//part units must be between 0 & INT_MAX
		PARTUNIT_COST_NEG,			//part unit cost must be >= 0
		PARTDESCRIP_EMPTY,			//no value in part description
		PART_NOT_FOUND,				//part is not found in the database
		FILE_OPEN_ERROR,			//file did not open for save/restore correctly
		UNKNOWN_ERROR				//safety net error
	};
	enum SEARCH_TYPE
	{
		NUMBER,			//searching by number
		DESCRIPTION		//searching by description
	};
private:
	struct CPartNdxNode //linked list struct
	{
		CPart* pThisPart;
		CPartNdxNode* pNdxPrevious;
		CPartNdxNode* pNdxNext;
	};
	CPart** papPart;			//anchor to array of pointers
	int iPartsMax;				//maximum length of array
	int iPartsLength;			//current length of array
	int iPartCurrent;			//current position by num
	SEARCH_TYPE ePartDBSearch;	//variable of SEARCH TYPE
	CPartNdxNode* pNdxHead;		//linked list head pointer
	CPartNdxNode* pNdxTail;		//linked list tail pointer
	CPartNdxNode* pNdxCurrent;	//current position by descrip

public:
	/*********************************************************************************/
	//Constructor
	CPartDB ();
	//Copy Constructor
	CPartDB (/*in*/ int iNumberofParts);

	//Destructor
	~CPartDB ();

	/*********************************************************************************/
	//FUNCTION: SetSearchType
	//Description: sets a new search type
	//pre: eNewSearchType has been assigned
	//post: RTN == OK and ePartDBSearch == eNewSearchType
	//	 or RTN == INVALID_SEARCH_TYPE (no changes)
	RTN SetSearchType (/*in*/ SEARCH_TYPE eSearchType);

	/*********************************************************************************/
	//FUNCTION: GetSearchType
	//Description: fetch current search type for database
	//pre: none
	//post: RTN == OK, eSearchType == ePartDBSearch
	RTN GetSearchType (/*out*/ SEARCH_TYPE& eSearchType) const;

	/*********************************************************************************/
	//FUNCTION: GetNumberOfParts
	//Description: fetch number of parts for current part
	//pre: none
	//post: 0 <= FCTVAL <= iPartsMax
	int GetNumberOfParts () const;

	/*********************************************************************************/
	//FUNCTION: InsertPart
	//Description: Insert part into array and linked list
	//pre: iNewPartNum, sNewPartDescrip, iNewUnitsOnHand, and fNewUnitCost have been
	//	   assigned.
	//post: RTN == OK, part has been inserted into the database
	//		RTN == DATABASE_FULL (no changes)
	//		RTN == PARTNUM_DUP (no changes)
	//		RTN == PARTUNITS_INVALID_RANGE (no changes)
	//		RTN == PARTUNIT_COST_NEG (no changes)
	//		RTN == PARTDESCRIP_EMPTY (no changes)
	RTN InsertPart (/*in*/ int iNewPartNum,
					/*in*/ string sNewPartDescrip,
					/*in*/ int iNewUnitsOnHand,
					/*in*/ float fNewUnitCost);

	/*********************************************************************************/
	//FUNCTION: DeletePart
	//Description: deletes part from array and linked list
	//pre: none
	//post: RTN == OK, part has been deleted
	//		RTN == DATABASE_EMPTY (no changes)
	RTN DeletePart ();

	/*********************************************************************************/
	//FUNCTION: SearchForPart
	//Description: searches for part in database
	//pre:iSearchForPartNum has been assigned
	//post: RTN == OK, part was found
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN = PART_NOT_FOUND (no changes)
	RTN SearchForPart (/*in*/ int iSearchForPartNum);

	//pre: iSearchForPartDescrip has been assigned
	//post: RTN == OK, part was found
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN = PART_NOT_FOUND (no changes)
	RTN SearchForPart (/*in*/ string sSearchDescrip);

	/*********************************************************************************/
	//FUNCTION: PositionNext, PositionPrevious, PositionFirst, and PositionLast
	//Description: transverses the linked list and array
	//pre: none
	//post: RTN == OK, position has been changed
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == PART_NOT_FOUND (no changes)
	RTN PositionNext ();
	RTN PositionPrevious ();
	RTN PositionFirst ();
	RTN PositionLast ();

	/*********************************************************************************/
	//FUNCTION: GetPartInfo
	//Description: retrieve part info for current part
	//pre: none
	//post: RTN == OK, iGetPartNum==iPartNum, sGetPartDescrip==sPartDescrip, 
	//				   iGetUnitsOnHand==iUnitsOnHand, fGetUnitCost==fUnitCost
	//		RTN == DATABASE_EMPTY (no changes)
	RTN GetPartInfo (/*out*/ int& iGetPartNum,
					/*out*/ string& sGetPartDescrip,
					/*out*/ int& iGetUnitsOnHand,
					/*out*/ float& fGetUnitCost);

	/*********************************************************************************/
	//FUNCTION: GetPartState
	//Description: retrieve part state INITIALIZED||CONSTRUCTED of current part
	//pre: none
	//post: RTN == OK, eOutPartState == ePartState
	//		 RTN == DATABASE_EMPTY (no changes)
	RTN GetPartState (/*out*/ CPart::STATE& eOutPartState);

	/*********************************************************************************/
	//FUNCTION: AddUnits
	//Description: units are added to current part units in database
	//pre: iAddUnitsOnHand has been assigned
	//post: RTN == OK, iUnitsOnHand == iAddUnitsOnHand + iUnitsOnHand
	//				   iUpdatedUnitsOnHand == iUnitsOnHand
	//		RTN == DATABASE_EMPTY (no changes)
	RTN AddUnits (/*in*/ int iAddUnitsOnHand,
				  /*out*/ int& iUpdatedUnitsOnHand);

	/*********************************************************************************/
	//FUNCTION: SubtractUnits
	//Description: units are subtracted from current part units in database
	//pre: iSubtractUnitsOnHand has been assigned
	//post: RTN == OK, iUnitsOnHand == iUnitsOnHand - iSubtractUnitsOnHand
	//				   iUpdatedUnitsOnHand == iUnitsOnHand
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == PARTUNITS_INVALID_RANGE (no changes)
	RTN SubtractUnits (/*in*/ int iSubtractUnitsOnHand,
					   /*out*/ int& iUpdatedUnitsOnHand);

	/*********************************************************************************/
	//FUNCTION: ReplaceUnitsOnHand
	//Description: replace UnitsOnHand for current part in database
	//pre: iReplaceUnitsOnHand has been assigned
	//post: RTN == OK, iUnitsOnHand == iReplaceUnitsOnHand
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == PARTUNITS_INVALID_RANGE (no changes)
	RTN ReplaceUnitsOnHand (/*in*/ int iReplaceUnitsOnHand);

	/*********************************************************************************/
	//FUNCTION: ReplaceUnitCost
	//Description: replace unit cost of current part in database
	//pre: fReplaceUnitCost has been assigend
	//post: RTN == OK, fUnitCost == fReplaceUnitCost
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == PARTUNIT_COST_NEG (no changes)
	RTN ReplaceUnitCost (/*in*/ float fReplaceUnitCost);

	/*********************************************************************************/
	//FUNCTION: ReplacePartNum
	//Description: replace part number of current part in database
	//pre: iReplacePartNum has been assigned
	//post: RTN == OK, iPartNum == iReplacePartNum
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == PARTNUM_DUP (no changes)
	RTN ReplacePartNum (/*in*/ int iReplacePartNum);

	/*********************************************************************************/
	//FUNCTION: ReplacePartDescrip
	//Description: replace part description of current part in database
	//pre: sReplacePartDescrip has been assigned
	//post: RTN == OK, sPartDescrip == sReplacePartDescrip
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == PARTDESCRIP_EMPTY (no changes)
	RTN ReplacePartDescrip (/*in*/ string sReplacePartDescrip);

	/*********************************************************************************/
	//FUNCTION: CalcPartCost
	//Descriptoin: calculate the cost of all the units together of a part in database
	//pre: none
	//post: RTN == OK, fPartTotalCost == fUnitCost*iUnitsOnHand
	//		RTN == DATABASE_EMPTY (no changes)
	RTN CalcPartCost (/*out*/ float& fPartTotalCost);

	/*********************************************************************************/
	//FUNCTION: CalcInventoryCost
	//Description: calculate the cost of all of the units of all of the parts in the
	//			   database together
	//pre: none
	//post: RTN == OK, fInventoryCost has been assigned
	RTN CalcInventoryCost (/*out*/ float& fInventoryCost);

	/*********************************************************************************/
	//FUNCTION: SavePartDB
	//Description: save database to a file
	//pre: sFileName and iNumRecords have been assigned
	//post: RTN == OK, file has been saved
	//		RTN == FILE_OPEN_ERROR (no changes)
	//		RTN == DATABASE_EMPTY (no changes)
	RTN SavePartDB (/*in*/ string sFileName,
					/*out*/ int& iNumRecords);
	
	/*********************************************************************************/
	//FUNCTION: RestorePartDB
	//Description: restore a previously saved database from a file
	//pre: sFileName and iNumRecords have been assigned
	//post: RTN == OK, file has been restored
	//		RTN == FILE_OPEN_ERROR (no changes)
	//		RTN == DATABASE_EMPTY (no changes)
	//		RTN == DATABASE_FULL (no changes)
	RTN RestorePartDB (/*in*/ string sFileName,
					   /*out*/ int& iNumRecords);

	/*********************************************************************************/
	//FUNCTION: InsertPartIndexNode and DeletePartIndexNode
	//Description: inserts a new node into the partdescrip index
	//pre: pPart and pNdxHead have been assigned
	//post: new node has been created (all data members) and inserted
	//		pNdxHead, pNdxTail, pNdxCurrent have been adjusted
	//		current index position has been established on this node
	void InsertPartIndexNode (/*in*/ CPart* pPart);
	
	//FUNCTION: DeletePartIndexNode
	//Description: deletes a node from the partdescrip index
	//pre: pPart and pNdxHead have been assigned
	//post: node has been deleted
	//		pNdxHead, pNdxTail, pNdxCurrent have been assigned
	//		current index position has been establised according to which node was deleted:
	//		if last node was deleted, position == new last node
	//		if a middle node was deleted, position == next node
	//		if first node was deleted, position == new first node
	//		if only node was deleted, position == NULL
	void DeletePartIndexNode (/*in*/ CPart* pPart);

	/*********************************************************************************/
	//FUNCTION: BinarySearch (recursive and regular)
	//Description: search through array for a part number
	//pre: iLow, iHigh, and iFindPartNum have been assigned and array is in ascending
	//	   order
	//post: if object has been found, function value == its index in array
	//	    if not found, function value == -1
	int BinarySearch (/*in*/ int iLow,
					  /*in*/ int iHigh,
					  /*in*/ int iFindPartNum);

	/*********************************************************************************/

};