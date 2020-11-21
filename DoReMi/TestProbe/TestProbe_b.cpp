#include "TestProbe_b.h"

#include <string>
#include <ostream>
#include <typeinfo>


// **************************************************************************
TestProbe_b::TestProbe_b(std::ostream& aorDump):
	morDump(aorDump)
{
	std::string funcMacro=__FUNCTION__;

	std::string info = typeid(*this).name();

}


// **************************************************************************
TestProbe_b::~TestProbe_b()
{

}


// **************************************************************************
uint32_t
TestProbe_b::msTestAll()
{
	uint32_t uCount = 0;
	uCount += vsTestOrdinary() ;
	uCount += vsTestRegressions() ;
	uCount += vsTestMembers() ;
	uCount += vsTestInherited() ;

	return uCount ;
}


// **************************************************************************
uint32_t
TestProbe_b::vsTestOrdinary()
{
	morDump << "Not Tested" ;
	return 0;
}



// **************************************************************************
uint32_t
TestProbe_b::vsTestRegressions()
{
	morDump << "Not Tested" ;
	return 0;
}


// **************************************************************************
uint32_t
TestProbe_b::vsTestMembers()
{
	morDump << "No members"  ;
	return 0;
}


// **************************************************************************
uint32_t
TestProbe_b::vsTestInherited()
{
	morDump << "No inheritance"  ;
	return 0;
}

