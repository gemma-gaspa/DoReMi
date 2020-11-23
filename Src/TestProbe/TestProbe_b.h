/*
 * Herde publicamente esta classe.
 *
 * Se as classes herdadas, elas mesmas contem TestProbe_b, chame-as
 * usando:
 *		vsTestMembers() para membros
 *		vsTestInherited() para herdadas
 *
 * Ao encontrar uma falha, chame mvRegisterFailure();
*/


#ifndef TESTPROBE_B_H
#define TESTPROBE_B_H

#include <string>
#include <ostream>

class TestProbe_b
{
public:
	TestProbe_b();
	virtual ~TestProbe_b() ;

	static void svSetOutput(std::ostream* aopDump);

	static void result();

	uint32_t msTestAll();

protected:
	void mvRegisterFailure();

private:
	virtual std::string vsMyName() = 0;

	virtual uint32_t vsTestOrdinary() ;
	virtual uint32_t vsTestRegressions() ;
	virtual uint32_t vsTestMembers() ;
	virtual uint32_t vsTestInherited() ;

	static std::ostream* mopDump ;
};

#endif // TESTPROBE_B_H
