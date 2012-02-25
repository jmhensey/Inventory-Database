//Part.cpp: implementation of the header file part.h
//Creator: Julianna Hensey
//Date: 2010/4/17 version 1.0
//Description: This source file includes the function definitions for all functions within the 
//part.h header file.
//Assumptions: Data has been checked for stream failure

#include "Part.h"

CPart::CPart()
{
	ePartState = CONSTRUCTED;
}

CPart::RTN CPart::Initialize(/*in*/ int iInitPartNum, 
							 /*in*/ string sInitPartDescrip, 
							 /*in*/ int iInitUnitsOnHand, 
							 /*in*/ float fInitUnitValue)
{
	//error returns
	if (ePartState == INITIALIZED)
		return ALREADY_INITIALIZED;
	if (sInitPartDescrip.length() == 0)
		return DESCRIP_EMPTY;
	if (iInitUnitsOnHand < 0 || iInitPartNum > INT_MAX)
		return UNITS_INVALID_RANGE;
	if (fInitUnitValue < 0)
		return UNIT_COST_NEG;
	
	iPartNum = iInitPartNum;
	sPartDescrip = sInitPartDescrip;
	//set up and loop to check for non-alphanumeric characters
	string::size_type LENGTH = sInitPartDescrip.length();
	for (int unsigned iCount = 0; LENGTH > iCount; iCount++)
	{
		if (! isalnum(sPartDescrip.at(iCount)))
			sPartDescrip[iCount]= '_';
	}
	iUnitsOnHand = iInitUnitsOnHand;
	fUnitCost = float (int(fInitUnitValue * 100.f + .5f))/100.f;
	ePartState = INITIALIZED;
	return OK;
}//Initialize

CPart::RTN CPart::Uninitialize()
{
	ePartState = CONSTRUCTED;
	return OK;
}//Unitialize

CPart::RTN CPart::GetPartState (/*out*/ CPart::STATE& eOutPartState) const
{
	eOutPartState = ePartState;
	return OK;
}//GetPartState

CPart::RTN CPart::AddUnits (/*in*/ int iUnitsToAdd,
							/*out*/ int& iUpdatedOnHand)
{
	if (ePartState != INITIALIZED)
		return NOT_INITIALIZED;
	else
	{
		iUnitsOnHand += iUnitsToAdd;
		iUpdatedOnHand = iUnitsOnHand;
		return OK;
	}
}//AddUnits

CPart::RTN CPart::SubtractUnits(/*in*/ int iUnitsToSubtract, 
								/*out*/ int& iUpdatedOnHand)
{
	if (ePartState != INITIALIZED)
		return NOT_INITIALIZED;
	if (iUnitsToSubtract > iUnitsOnHand)
		return UNITS_INVALID_RANGE;
	else
	{
		iUnitsOnHand -= iUnitsToSubtract;
		iUpdatedOnHand = iUnitsOnHand;
		return OK;
	}
}//SubtractUnits

CPart::RTN CPart::ReplaceUnitsOnHand(/*in*/ int iNewUnitsOnHand)
{
	if (ePartState != INITIALIZED)
		return NOT_INITIALIZED;
	else if (iNewUnitsOnHand < 0 || iNewUnitsOnHand > INT_MAX)
		return UNITS_INVALID_RANGE;
	else 
	{
		iUnitsOnHand = iNewUnitsOnHand;
		return OK;
	}
}//ReplaceUnitsOnHand

CPart::RTN CPart::ReplaceUnitCost(/*in*/ float fNewUnitCost)
{
	if (ePartState != INITIALIZED)
		return NOT_INITIALIZED;
	else if (fNewUnitCost < 0)
		return UNIT_COST_NEG;
	else
	{
		fUnitCost = float (int(fNewUnitCost * 100.f + .5f))/100.f;;
		return OK;
	}
}//ReplaceUnitCost

CPart::RTN CPart::ReplacePartDescrip(/*in*/ string sNewDescrip)
{
	if (CPart::ePartState != INITIALIZED)
		return NOT_INITIALIZED;
	else if (sNewDescrip.length() == 0)
		return DESCRIP_EMPTY;
	else
	{
		sPartDescrip = sNewDescrip;
		return OK;
	}
}//ReplacePartDescrip

CPart::RTN CPart::ReplacePartNum(/*in*/ int iNewPartNum)
{
	if (ePartState != INITIALIZED)
		return NOT_INITIALIZED;
	else
	{
		iPartNum = iNewPartNum;
		return OK;
	}
}//ReplacePartNum

CPart::RTN CPart::GetPartInfo(/*out*/ int& iOutPartNum, 
							  /*out*/ string& sOutPartDescrip, 
							  /*out*/ int& iOutUnitsOnHand, 
							  /*out*/ float& fOutUnitCost) const
{
	if (ePartState == CONSTRUCTED)
		return NOT_INITIALIZED;
	
	iOutPartNum = iPartNum;
	sOutPartDescrip = sPartDescrip;
	iOutUnitsOnHand = iUnitsOnHand;
	fOutUnitCost = fUnitCost;
	return OK;
	
}//GetPartInfo

int CPart::PartNumIsGreaterThan(/*in*/ CPart partOther)
{
	if (iPartNum > partOther.iPartNum)
		return 1;
	else if (iPartNum == partOther.iPartNum)
		return 0;
	else
		return -1;
}//PartNumIsGreaterThan

int CPart::PartDescripIsGreaterThan(/*in*/ CPart partOther)
{
	if (sPartDescrip > partOther.sPartDescrip)
		return 1;
	else if (sPartDescrip == partOther.sPartDescrip)
		return 0;
	else
		return -1;
}//PartDescripIsGreaterThan