#pragma once

#include "CppUnit/TestCase.h"

class WebSocketTest : public CppUnit::TestCase
{
public:
	WebSocketTest(const std::string& name);
	~WebSocketTest();

	void testConnect();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};

