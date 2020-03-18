//
// SessionImpl.cpp
//
// Library: Data/SOF
// Package: SOF
// Module:  SessionImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Reach/Data/SOF/SessionImpl.h"
#include "Reach/Data/SOF/SOFException.h"
#include "Reach/Data/Session.h"
#include "Reach/Data/SOF/Utility.h"
#include "GMCrypto.h"
#include "Poco/Stopwatch.h"
#include "Poco/String.h"
#include "Poco/Mutex.h"
#include "Poco/Thread.h"
#include "Reach/Data/DataException.h"
#include "SoFProvider.h"
#include "SOFErrorCode.h"
#include <cstdlib>
#include <sstream>
#include "Poco/StreamCopier.h"
#include "Poco/Base64Decoder.h"

#ifndef SOF_OPEN_URI
#define SOF_OPEN_URI 0
#endif


namespace Reach {
namespace Data {
namespace SOF {


SessionImpl::SessionImpl(const std::string& connectionString, std::size_t loginTimeout):
	Reach::Data::AbstractSessionImpl<SessionImpl>(connectionString, loginTimeout),
	_connector(Connector::KEY),
	_connected(false),
	_random_size(0),
	_connectionString(connectionString)
{
	open();
	selectMode();
	setConnectionTimeout(loginTimeout);

	addProperty("connectionTimeout", &SessionImpl::setConnectionTimeout, &SessionImpl::getConnectionTimeout);
}


SessionImpl::~SessionImpl()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}

void SessionImpl::open(const std::string& connect)
{
	if (connect != connectionString())
	{
		if (isConnected())
			throw Poco::InvalidAccessException("Session already connected", defaultError);

		if (!connect.empty())
			setConnectionString(connect);
	}

	poco_assert_dbg (!connectionString().empty());

	try
	{
		int rc = 0;
		size_t tout = getLoginTimeout();
		Poco::Stopwatch sw; sw.start();
		while (true)
		{
			rc = SOF_OpenDevice();
			if (rc == SAR_OK) break;
			if (sw.elapsedSeconds() >= tout)
			{
				close();
				throw SOFException(rc);
			}
			else Poco::Thread::sleep(10);
		}
	} 
	catch (SOFException& ex)
	{
		throw ConnectionFailedException(ex.displayText(), defaultError);
	}

	_connected = true;
}


void SessionImpl::close()
{
	SOF_CloseDevice();

	_connected = false;
}


bool SessionImpl::isConnected()
{
	return _connected;
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
	int tout = static_cast<int>(1000 * timeout);
	_timeout = tout;
}

void SessionImpl::selectMode()
{
	std::string userString;
	Utility::spiltEntries(getUserList(), _containerString, userString);
	Utility::selectEncryptMethod(_containerString);
	Utility::selectSignMethod(_containerString);
	_random_size = Utility::GetRandomSize();
}

void SessionImpl::setConnectionTimeout(const std::string& prop, const Poco::Any& value)
{
	setConnectionTimeout(Poco::RefAnyCast<std::size_t>(value));
}


Poco::Any SessionImpl::getConnectionTimeout(const std::string& prop)
{
	return Poco::Any(_timeout/1000);
}

bool SessionImpl::login(const std::string& passwd)
{
	return SOF_Login(_containerString, passwd);
}

bool SessionImpl::changePW(const std::string& oldCode, const std::string& newCode)
{
	return SOF_ChangePassWd(_containerString, oldCode, newCode);
}

std::string SessionImpl::getUserList()
{
	return SOF_GetUserList();
}

std::string SessionImpl::getCertBase64String(short ctype)
{
	enum certType { sign = 1, crypto };

	std::string _content;

	switch (ctype) {

	case certType::sign:
		_content = SOF_ExportUserCert(_containerString);
		break;
	case certType::crypto:
		_content = SOF_ExportExChangeUserCert(_containerString);
		break;
	default:
		throw Poco::NotImplementedException(Utility::lastError(_containerString), defaultError);
	}

	if (_content.empty())
		throw Poco::DataException(Utility::lastError(_containerString), defaultError);

	return _content;
}

int SessionImpl::getPinRetryCount()
{
	return SOF_GetPinRetryCount(_containerString);
}

std::string SessionImpl::getCertInfo(const std::string& base64, int type)
{
	std::string item;

	if (SGD_CERT_VERSION == type) {
		item = Utility::GetCertVersion(base64);
	}
	else if (SGD_CERT_VALID_TIME == type) {
		item = Utility::GetCertVaildTime(base64);
	}
	else if (SGD_OID_IDENTIFY_NUMBER == type) {
		/// only support user id card
		item = Utility::GetCertOwnerID(base64);
	}
	else
	{
		item = SOF_GetCertInfo(base64, type);
	}

	return item;
}

std::string SessionImpl::getSerialNumber()
{
	std::string serialNumber;
	
	serialNumber = SOF_GetDeviceInfo(_containerString, SGD_DEVICE_SERIAL_NUMBER);

	if (serialNumber.empty()) {
		Poco::DataException(Utility::lastError(_containerString), defaultError);
	}

	return serialNumber;
}

std::string SessionImpl::getKeyID()
{
	return getSerialNumber();
}

std::string SessionImpl::encryptData(const std::string& paintText, const std::string& base64)
{
	///ֻ�������֤�����
	std::string encryptData;

	encryptData = SOF_AsEncrypt(base64, paintText);

	if (encryptData.empty()) {
		throw Poco::DataException(Utility::lastError(_containerString), defaultError);
		//throw RequestHandleException(RAR_ENCYPTFAILED);
	}

	return encryptData;
}

std::string SessionImpl::decryptData(const std::string& encryptBuffer)
{
	std::string decryptBuffer = SOF_AsDecrypt(_containerString, encryptBuffer);

	if (decryptBuffer.empty())
		throw  Poco::DataException(Utility::lastError(_containerString), defaultError);

	std::string text;
	std::istringstream istr(decryptBuffer);

	Poco::Base64Decoder decoder(istr);
	Poco::StreamCopier::copyToString(decoder, text);

	return text;
}

std::string SessionImpl::signByP1(const std::string& message)
{
	return SOF_SignData(_containerString, message);
}

bool SessionImpl::verifySignByP1(const std::string& base64, const std::string& msg, const std::string& signature)
{
	return SOF_VerifySignedData(base64, msg, signature);
}

std::string SessionImpl::signByP7(const std::string& textual, int mode)
{
	return SOF_SignMessage(mode, _containerString, textual);
}

bool SessionImpl::verifySignByP7(const std::string& textual, const std::string& signature)
{
	return SOF_VerifySignedMessage(signature, textual);
}

} } } // namespace Reach::Data::SOF
