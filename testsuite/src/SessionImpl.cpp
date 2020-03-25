//
// SessionImpl.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SessionImpl.h"
//#include "TestStatementImpl.h"
#include "Connector.h"


namespace Reach {
namespace Data {
namespace Test {


SessionImpl::SessionImpl(const std::string& init, std::size_t timeout):
	Reach::Data::AbstractSessionImpl<SessionImpl>(init, timeout),
	_f(false),
	_connected(true)
{
	addFeature("f1", &SessionImpl::setF, &SessionImpl::getF);
	addFeature("f2", 0, &SessionImpl::getF);
	addFeature("f3", &SessionImpl::setF, 0);
	addFeature("connected", &SessionImpl::setConnected, &SessionImpl::getConnected);
	addProperty("p1", &SessionImpl::setP, &SessionImpl::getP);
	addProperty("p2", 0, &SessionImpl::getP);
	addProperty("p3", &SessionImpl::setP, &SessionImpl::getP);
}


SessionImpl::~SessionImpl()
{
}


void SessionImpl::open(const std::string& connectionString)
{
	_connected = true;
}


void SessionImpl::close()
{
	_connected = false;
}


bool SessionImpl::isConnected()
{
	return _connected;
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
}


std::size_t SessionImpl::getConnectionTimeout()
{
	return 0;
}


//Reach::Data::StatementImpl* SessionImpl::createStatementImpl()
//{
//	return new TestStatementImpl(*this);
//}


void SessionImpl::begin()
{
}


void SessionImpl::commit()
{
}


void SessionImpl::rollback()
{
}


bool SessionImpl::canTransact()
{
	return false;
}


bool SessionImpl::isTransaction()
{
	return false;
}


void SessionImpl::setTransactionIsolation(Poco::UInt32)
{
}


Poco::UInt32 SessionImpl::getTransactionIsolation()
{
	return 0;
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32)
{
	return false;
}


bool SessionImpl::isTransactionIsolation(Poco::UInt32)
{
	return false;
}


const std::string& SessionImpl::connectorName() const
{
	return Connector::KEY;
}


bool SessionImpl::getConnected(const std::string& name)
{
	return _connected;
}


void SessionImpl::setConnected(const std::string& name, bool value)
{
	_connected = value;
}


void SessionImpl::setF(const std::string& name, bool value)
{
	_f = value;
}


bool SessionImpl::getF(const std::string& name)
{
	return _f;
}


void SessionImpl::setP(const std::string& name, const Poco::Any& value)
{
	_p = value;
}


Poco::Any SessionImpl::getP(const std::string& name)
{
	return _p;
}

const std::string& SessionImpl::contianerName() const 
{
	return "";
}

bool SessionImpl::login(const std::string& passwd) 
{
	return false;
}

bool SessionImpl::changePW(const std::string& oldCode, const std::string& newCode) 
{
	return false;
}

std::string SessionImpl::getUserList() 
{
	return "";
}

std::string SessionImpl::getCertBase64String(short ctype) 
{
	return "";
}

int SessionImpl::getPinRetryCount() 
{
	return 1;
}

std::string SessionImpl::getCertInfo(const std::string& base64, int type) 
{
	return "";
}

std::string SessionImpl::getSerialNumber() { return ""; }

std::string SessionImpl::getKeyID() { return ""; }

std::string SessionImpl::encryptData(const std::string& paintText, const std::string& base64) { return ""; }

std::string SessionImpl::decryptData(const std::string& encryptBuffer) { return ""; }

std::string SessionImpl::signByP1(const std::string& message) { return ""; }

bool SessionImpl::verifySignByP1(const std::string& base64, const std::string& msg, const std::string& signature) { return false; }

std::string SessionImpl::signByP7(const std::string& textual, int mode) { return ""; }

bool SessionImpl::verifySignByP7(const std::string& textual, const std::string& signature) { return false; }

} } } // namespace Poco::Data::Test
