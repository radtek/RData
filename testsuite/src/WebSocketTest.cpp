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

#include "Poco/URI.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HttpClientSession.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/JSON/Object.h"
#include "Poco/DynamicStruct.h"
#include "Poco/Logger.h"
#include "Poco/WindowsConsoleChannel.h"
#include <sstream>

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;

void WebSocketTest::testConnect()
{
	AutoPtr<WindowsConsoleChannel> pChannel = new WindowsConsoleChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel.get());
	//authCode = 8125ec6846b54c71885e68c452f06c44
	const int msgSize = 6400;
	SocketAddress sa("192.168.0.75", 7099);

	HTTPClientSession cs(sa);
	HTTPRequest requset(HTTPRequest::HTTP_GET, "/websocket/funny5", HTTPRequest::HTTP_1_1);
	HTTPResponse response;
	WebSocket ws(cs, requset, response);
	ws.setReceiveTimeout(Timespan(5,0));
	ws.setSendBufferSize(msgSize);
	ws.setReceiveBufferSize(msgSize);
	std::string payload("HeartBeat");

	Object o;
	o.set("mag", "HeartBeat");
	o.set("to", "funny5");
	std::ostringstream json;
	o.stringify(json);

	char buffer[msgSize + 1] = { 0 };
	int flags, n = 0;
	do
	{
		ws.sendFrame(json.str().data(), json.str().size());
		poco_information(root, "HeartBeat");
		try { n += ws.receiveFrame(buffer, sizeof(buffer), flags); }
		catch (Poco::TimeoutException&) {}
		poco_information(root, Poco::format("receive %s : %s",ws.peerAddress().toString() ,std::string(buffer, n)));
		//assert(payload.compare(0, payload.size(), buffer, 0, n) == 0);
	} while (n < msgSize);
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
