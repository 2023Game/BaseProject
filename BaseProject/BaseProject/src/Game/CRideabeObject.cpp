#include "CRideableObject.h"

CRideableObject::CRideableObject()
	: CObjectBase(ETag::eRideableObject, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
}

CRideableObject::~CRideableObject()
{
}
