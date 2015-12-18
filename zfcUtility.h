#pragma once

#include "zfcTypeDef.h"

class zfcUtility
{
public:

	static Acad::ErrorStatus readDwg( AcDbDatabase*& pDb, const CString& strPath );

	static Acad::ErrorStatus getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb );

	static Acad::ErrorStatus getAllEntity( zfc::entityContainer& conEntity, AcDbDatabase* pDb, AcDb::OpenMode mode );

	static Acad::ErrorStatus getAllEntity( zfc::entityContainer& conEntity, AcDbBlockTableRecordIterator* pBlkRecItr, AcDb::OpenMode mode );

	static Acad::ErrorStatus getAllAttribute( zfc::entityContainer& conEntity, AcDbBlockReference* pBlockRef, AcDb::OpenMode mode );

	//	���O�t�@�C������Ԃ�
	static CString logFileName();

	//	�t�@�C���p�X��Ԃ�
	static CString filePath( const CString& strFolderPath, const CString& strFileName, const CString& strExt = _T("") );

	//	���O�o�͂���
	static void writeLog1( int nResourceId, const CString& strFmt1 );

	//	���O�o�͂���
	static void writeLog2( int nResourceId, const CString& strFmt1, const CString& strFmt2 );

	//	�t�@�C���������O�o�͏o�͂���
	static void writeFileName( const zfc::pathContainer& conPath );

	//	�t�@�C���p�X����t�@�C������Ԃ�
	static CString fileName( const CString& strPath );

protected:

	zfcUtility(void);
	virtual ~zfcUtility(void);

};

