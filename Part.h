// Part.h: Include file for inventory database. 
// Author: Julianna Hensey
// Date: 2010/4/15 version 1.0

/*
This header file describes the the class CPart. CPart includes a constructor and 
destructor (CPart() and ~CPart()). It also includes transformer and accessor functions.
Transformer Functions: 
	RTN Initialize (iInitPartNum, sInitPartDescrip, iInitUnitsOnHand, fInitUnitValue)
	RTN Unitialize ()
	RTN AddUnits (iUnitsToAdd, iUpdatedOnHand)
	RTN SubtractUnits (iUnitsToSubtract, iUpdatedOnHand)
	RTN ReplaceUnitsOnHand (iNewUnitsOnHand)
	RTN ReplaceUnitCost (fNewUnitCost)
	RTN ReplacePartDescrip (sNewDescrip)
	RTN ReplacePartNum (iNewPartNum)
Accessor Functions:
	RTN GetPartState (eOutPartState) 
	RTN GetPartInfo (iOutPartNum, sOutPartDescrip, iOutUnitsOnHand, fOutUnitCost)
	int PartNumIsGreaterThan (partOther)
	int PartDescripIsGreaterThan (partOther)
*/

#include <string>
using namespace std;

class CPart
{
public:
	enum STATE					//internal use - object initialization state
	{
		CONSTRUCTED,			//constructed but not initialized
		INITIALIZED				//data members contain valid values
	};
	enum RTN					//external use - CPart return codes
	{
		OK,						//function was successful
		DESCRIP_EMPTY,			//part description has zero length
		UNITS_INVALID_RANGE,	//inventory must be 0 to INT_MAX
		UNIT_COST_NEG,			//cost of unit is negative
		NOT_INITIALIZED,		//ePartState == CONSTRUCTED
		ALREADY_INITIALIZED		//ePartState == INITIALIZED
	};
private:
	int iPartNum;				//number of part
	string sPartDescrip;		//description of part
	int iUnitsOnHand;			//units on hand
	float fUnitCost;			//current value of 1 unit
	STATE ePartState;			//variable of enumerator STATE
public:
	//description: constructor
	//post: ePartState == CONSTRUCTED
	CPart ();

	/**********************************************************************************
	function: RTN Initialize
	description: Initializes all of the private data members
	pre:  iInitPartNum, sInitPartDescrip, iInitUnitsonHand, and fInitUnitValue
		    have been initialized.
	post: if RTN == OK, all data members have been initialized
			if RTN == UNITS_INVALID_RANGE (no changes)
			if RTN == UNIT_COST_NEG (no changes)
			if RTN == DESCRIP_EMPTY (no changes)
			if RTN == ALREADY_INITIALIZED (no changes)
	*/
	RTN Initialize (/*in*/ int iInitPartNum,
					/*in*/ string sInitPartDescrip,
					/*in*/ int iInitUnitsOnHand,
					/*in*/ float fInitUnitValue);

	/**********************************************************************************
	function: RTN Uninitialize
	description: uninitializes part in database
	pre: none
	post: RTN == OK, ePartState == CONSTRUCTED
	*/
	RTN Uninitialize ();

	/**********************************************************************************
	function: RTN GetPartState
	description: returns part state of current part
	pre: none
	post: RTN == OK and eOutPartState == INITIALIZED || CONSTRUCTED
	*/
	RTN GetPartState (/*out*/ STATE& eOutPartState) const;

	/**********************************************************************************
	function: RTN AddUnits
	description: adds units to the units on hand
	pre: iUnitsToAdd has been assigned
	post: if RTN == OK, iUpdatedOnHand == iUnitsOnHand == iUnitsToAdd + iUnitsOnHand
		  if RTN == NOT_INITIALIZED(no changes)
	*/
	RTN AddUnits (/*in */ int iUnitsToAdd,
				   /*out*/ int& iUpdatedOnHand);

	/**********************************************************************************
	function: RTN SubtractUnits
	description: subtracts units from the units on hand
	pre: iUnitsToSubtract has been assigned
	post: if RTN == OK, iUpdatedOnHand == iUnitsOnHand == iUnitsOnHand - iUnitsToSubtract
		  if RTN == UNITS_INVALID_RANGE (no changes)
		  if RTN == NOT_INITIALIZED (no changes)
	*/
	RTN SubtractUnits (/*in */ int iUnitsToSubtract,
						/*out*/ int& iUpdatedOnHand);
	/**********************************************************************************
	function: RTN ReplaceUnitsOnHand
	description: Replaces current Units on Hand value 
	pre: iNewUnitsOnHand has been assigned
	post: if RTN == OK, iUnitsOnHand == iNewUnitsOnHand
		  if RTN == UNITS_INVALID_RANGE (no changes)
		  if RTN == NOT_INITIALIZED (no changes)
	*/
	RTN ReplaceUnitsOnHand (/*in*/ int iNewUnitsOnHand);

	/**********************************************************************************
	function: RTN ReplaceUnitsCost
	description: Replaces Unit Cost 
	pre: fNewUnitCost has been assigned
	post: if RTN == OK, fUnitCost == fNewUnitCost
		  if RTN == UNIT_COST_NEG (no changes)
		  if RTN == NOT_INITIALIZED (no changes)
	*/
	RTN ReplaceUnitCost (/*in*/ float fNewUnitCost);

	/**********************************************************************************
	function: RTN ReplacePartDescrip
	description: Replaces part description
	pre: sNewDescrip has been assigned
	post: if RTN == OK, sPartDescrip == sNewDescrip
		  if RTN == DESCRIP_EMPTY (no changes)
		  if RTN == NOT_INITIALIZED (no changes)
	*/
	RTN ReplacePartDescrip (/*in*/ string sNewDescrip);

	/**********************************************************************************
	function: RTN ReplacePartNum
	description: Replace part number
	pre: iNewPartNum has been assigned
	post: if RTN == OK, iPartNum == iNewPartNum
		  if RTN == NOT_INITIALIZED (no changes)
	*/
	RTN ReplacePartNum (/*in*/ int iNewPartNum);
	
	/**********************************************************************************
	function: RTN GetPartInfo
	description: retrieve values of all private members
	pre: none
	post: if RTN == OK, iOutPartNum == iPartNum
						sOutPartDescrip == iPartDescrip
						iOutUnitsOnHand == iUnitsOnHand
						fOutUnitCost == fUnitCost
		  if RTN == NOT_INITIALIZED (no changes)
	*/
	RTN GetPartInfo (/*out*/ int& iOutPartNum,
					 /*out*/ string& sOutPartDescrip,
					 /*out*/ int& iOutUnitsOnHand,
					 /*out*/ float& fOutUnitCost) const;

	/**********************************************************************************
	function: int PartNumIsGreaterThan
	description: compares two parts and determines which part number is greater
	pre: partOther has been assigned
	post: if 1,  this object > other
		  if 0,  this object == other
		  if -1, this object < other
	*/
	int PartNumIsGreaterThan (/*in*/ CPart partOther);

	/**********************************************************************************
	function: int PartDescripIsGreaterThan
	description: compares two parts and determines which part description is greater
	pre: partOther has been assigned
	post: if 1,  this object > other
		  if 0,  this object == other
		  if -1, this object < other
	*/
	int PartDescripIsGreaterThan (/*in*/ CPart partOther);
};