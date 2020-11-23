#include "TestProbe_b.h"

#include <string>
#include <ostream>
#include <typeinfo>


std::ostream* TestProbe_b::mopDump = nullptr;


// **************************************************************************
TestProbe_b::TestProbe_b()
{
	std::string funcMacro=__FUNCTION__;

	std::string info = typeid(*this).name();
}


// **************************************************************************
void
TestProbe_b::svSetOutput(std::ostream* aopDump){
	mopDump = aopDump;

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
	*mopDump << "Not Tested" ;
	return 0;
}



// **************************************************************************
uint32_t
TestProbe_b::vsTestRegressions()
{
	*mopDump << "Not Tested" ;
	return 0;
}


// **************************************************************************
uint32_t
TestProbe_b::vsTestMembers()
{
	*mopDump << "No members"  ;
	return 0;
}


// **************************************************************************
uint32_t
TestProbe_b::vsTestInherited()
{
	*mopDump << "No inheritance"  ;
	return 0;
}

