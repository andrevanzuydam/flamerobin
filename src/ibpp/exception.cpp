//	Subject : IBPP, Initialization of the library
/*
    (C) Copyright 2000-2006 T.I.P. Group S.A. and the IBPP Team (www.ibpp.org)

    The contents of this file are subject to the IBPP License (the "License");
    you may not use this file except in compliance with the License.  You may
    obtain a copy of the License at http://www.ibpp.org or in the 'license.txt'
    file which must have been distributed along with this file.

    This software, distributed under the License, is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See the
    License for the specific language governing rights and limitations
    under the License.
*/

#ifdef _MSC_VER
#pragma warning(disable: 4786 4996)
#ifndef _DEBUG
#pragma warning(disable: 4702)
#endif
#endif

#include "_ibpp.h"

#ifdef HAS_HDRSTOP
#pragma hdrstop
#endif

#include <cstdarg>
#include <cstdio>

using namespace ibpp_internals;

IBPP::Exception::~Exception() throw()
{
}

IBPP::LogicException::~LogicException() throw()
{
}

IBPP::SQLException::~SQLException() throw()
{
}

IBPP::WrongType::~WrongType() throw()
{
}

//
//	(((((((( ExceptionBase Implementation ))))))))
//

void ExceptionBase::buildErrorMessage(const char* message)
{
	if (! mContext.empty())
		mWhat.append(_("Context: ")).append(mContext).append("\n");

	if (message != 0 && *message != 0 )
		mWhat.append(_("Message: ")).append(message).append("\n");
	
	mWhat.append("\n");
}

void ExceptionBase::raise(const std::string& context, const char* message, va_list argptr)
{
	mContext.assign(context);

	if (message != 0)
	{
		char buffer[1024];
#if defined(_MSC_VER) 
		_vsnprintf(buffer, sizeof(buffer)-1, message, argptr);
#else
		vsnprintf(buffer, sizeof(buffer)-1, message, argptr);
#endif
		buffer[sizeof(buffer)-1] = 0;
	
		buildErrorMessage(buffer);
	}
	else
		buildErrorMessage(0);
}

ExceptionBase::ExceptionBase() throw()
{
}

ExceptionBase::ExceptionBase(const ExceptionBase& copied) throw()
{
	mContext = copied.mContext;
	mWhat = copied.mWhat;
}

ExceptionBase& ExceptionBase::operator=(const ExceptionBase& copied) throw()
{
	mContext = copied.mContext;
	mWhat = copied.mWhat;
	return *this;
}

ExceptionBase::ExceptionBase(const std::string& context,
								const char* message, ...) throw()
{
	va_list argptr;
	va_start(argptr, message);
	mWhat.assign("*** IBPP::Exception ***\n");
	raise(context, message, argptr);
	va_end(argptr);
}

ExceptionBase::~ExceptionBase() throw()
{
}

const char* ExceptionBase::Origin() const throw()
{
	return mContext.c_str();
}

const char* ExceptionBase::what() const throw()
{
	return mWhat.c_str();
}

//	(((((((( LogicExceptionImpl Implementation ))))))))

LogicExceptionImpl::LogicExceptionImpl() throw()
	: ExceptionBase()
{
}

LogicExceptionImpl::LogicExceptionImpl(const LogicExceptionImpl& copied) throw()
	: IBPP::LogicException(), ExceptionBase(copied)
{
}

LogicExceptionImpl& LogicExceptionImpl::operator=(const LogicExceptionImpl& copied) throw()
{
	ExceptionBase::operator=(copied);
	return *this;
}

LogicExceptionImpl::LogicExceptionImpl(const std::string& context,
										const char* message, ...) throw()
{
	va_list argptr;
	va_start(argptr, message);
	mWhat.assign("*** IBPP::LogicException ***\n");
	raise(context, message, argptr);
	va_end(argptr);
}

LogicExceptionImpl::~LogicExceptionImpl() throw ()
{
}

const char* LogicExceptionImpl::Origin() const throw()
{
	return ExceptionBase::Origin();
}

const char* LogicExceptionImpl::what() const throw()
{
	return ExceptionBase::what();
}

//	(((((((( SQLExceptionImpl Implementation ))))))))

SQLExceptionImpl::SQLExceptionImpl() throw()
	: ExceptionBase(), mSqlCode(0), mEngineCode(0)
{
}

SQLExceptionImpl::SQLExceptionImpl(const SQLExceptionImpl& copied) throw()
	: IBPP::SQLException(), ExceptionBase(copied), mSqlCode(copied.mSqlCode),
		mEngineCode(copied.mEngineCode)
{
}

SQLExceptionImpl& SQLExceptionImpl::operator=(const SQLExceptionImpl& copied) throw()
{
	ExceptionBase::operator=(copied);
	mSqlCode = copied.mSqlCode;
	mEngineCode = copied.mEngineCode;
	return *this;
}

SQLExceptionImpl::SQLExceptionImpl(const IBS& status, const std::string& context,
									const char* message, ...) throw()
{
	va_list argptr;
	va_start(argptr, message);
	mWhat.assign("*** IBPP::SQLException ***\n");
	raise(context, message, argptr);
	va_end(argptr);
	mSqlCode = status.SqlCode();
	mEngineCode = status.EngineCode();
	mWhat.append(status.ErrorMessage());
}

SQLExceptionImpl::~SQLExceptionImpl() throw ()
{
}

const char* SQLExceptionImpl::Origin() const throw()
{
	return ExceptionBase::Origin();
}

const char* SQLExceptionImpl::what() const throw()
{
	return ExceptionBase::what();
}

int SQLExceptionImpl::SqlCode() const throw()
{
	return mSqlCode;
}

int SQLExceptionImpl::EngineCode() const throw()
{
	return mEngineCode;
}

//	(((((((( WrongTypeImpl Implementation ))))))))

// The following constructors are small and could be inlined, but for object
// code compacity of the library it is much better to have them non-inlined.
// The amount of code generated by compilers for a throw is well-enough.

WrongTypeImpl::WrongTypeImpl() throw()
	: IBPP::WrongType(), ExceptionBase()
{
}

WrongTypeImpl::WrongTypeImpl(const WrongTypeImpl& copied) throw()
	: IBPP::WrongType(), ExceptionBase(copied)
{
}

WrongTypeImpl& WrongTypeImpl::operator=(const WrongTypeImpl& copied) throw()
{
	ExceptionBase::operator=(copied);
	return *this;
}

WrongTypeImpl::WrongTypeImpl(const std::string& context, int sqlType, IITYPE varType,
				const char* message, ...) throw()
{
	va_list argptr;
	va_start(argptr, message);
	mWhat.assign("*** IBPP::WrongType ***\n");
	raise(context, message, argptr);
	va_end(argptr);

	std::string info;
	switch (sqlType & ~1)
	{
		case SQL_BOOLEAN :		info.append("BOOLEAN"); break; // v3
		case SQL_TEXT :			info.append("CHAR"); break;
		case SQL_VARYING :		info.append("VARCHAR"); break;
		case SQL_SHORT :		info.append("SMALLINT"); break;
		case SQL_LONG :			info.append("INTEGER"); break;
		case SQL_INT64 :		info.append("BIGINT"); break;
		case SQL_FLOAT :		info.append("FLOAT"); break;
		case SQL_DOUBLE :		info.append("DOUBLE"); break;
		case SQL_TIMESTAMP :	info.append("TIMESTAMP"); break;
		case SQL_TYPE_DATE :	info.append("DATE"); break;
		case SQL_TYPE_TIME :	info.append("TIME"); break;
		case SQL_BLOB :			info.append("BLOB"); break;
		case SQL_ARRAY :		info.append("ARRAY"); break;
	}
	info.append(" ").append(_(" and ")).append(" ");
	switch (varType)
	{
		case ivArray :		info.append("Array"); break;
		case ivBlob :		info.append("Blob"); break;
		case ivDate :		info.append("Date"); break;
		case ivTime :		info.append("Time"); break;
		case ivTimestamp :	info.append("Timestamp"); break;
		case ivString :		info.append("std::string"); break;
		case ivInt16 :		info.append("int16_t"); break;
		case ivInt32 :		info.append("int32_t"); break;
		case ivInt64 :		info.append("int64_t"); break;
		case ivFloat :		info.append("float"); break;
		case ivDouble :		info.append("double"); break;
		case ivBool :		info.append("bool"); break;
		case ivDBKey :		info.append("DBKey"); break;
		case ivByte :		info.append("int8_t"); break;
	}
	mWhat.append(info).append("\n");
}

WrongTypeImpl::~WrongTypeImpl() throw ()
{
}

const char* WrongTypeImpl::Origin() const throw()
{
	return ExceptionBase::Origin();
}

const char* WrongTypeImpl::what() const throw()
{
	return ExceptionBase::what();
}
