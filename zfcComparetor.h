#pragma once

#include "zfcTypeDef.h"
#include "ResultCompEntity.h"

class zfcComparetor
{
public:
	zfcComparetor(void);
	virtual ~zfcComparetor(void);

	//	�}�ʔ�r���s��
	Acad::ErrorStatus execute( const CString& strPathOldDwg, const CString strPathNewDwg );

protected:
	//	�t�@�C�����(�X�V�����E�T�C�Y)�Ŕ�r
	bool compareFileStatus( const CString& strPathOldDwg, const CString strPathNewDwg ) const;

	bool readDwg( AcDbDatabase*& pDb, const CString& strPath ) const;

	bool getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb ) const;

private:
};

