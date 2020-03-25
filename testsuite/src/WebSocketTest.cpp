#include "WebSocketTest.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TestCaller.h"

WebSocketTest::WebSocketTest(const std::string& name)
	:CppUnit::TestCase(name)
{
}


WebSocketTest::~WebSocketTest()
{
}

void WebSocketTest::testConnect()
{

}

void WebSocketTest::setUp()
{
}

void WebSocketTest::tearDown()
{
}


CppUnit::Test * WebSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WebSocketTest");
	
	CppUnit_addTest(pSuite, WebSocketTest, testConnect);

	return pSuite;
}
