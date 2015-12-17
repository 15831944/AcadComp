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

	bool readDwg( AcDbDatabase*& pDb, const CString& strPath ) const;

	bool getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb ) const;

private:
};

