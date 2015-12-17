#include "stdafx.h"
#include "resource.h"
#include "zfcUtility.h"
#include "zfcComparetor.h"
#include "ACDocManager.h"
#include "ResultCompEntity.h"
#include "zfcLogger.h"

zfcComparetor::zfcComparetor(void)
{
}


zfcComparetor::~zfcComparetor(void)
{
}

//	�}�ʔ�r���s��
bool zfcComparetor::execute( const CString& strPathOldDwg, const CString strPathNewDwg )
{
	bool bResult = false;
	AcDbDatabase* pDbOld = nullptr;
	AcDbDatabase* pDbNew = nullptr;
	acd::objectIdContainer conObjectIdOld;
	acd::objectIdContainer conObjectIdNew;
	ResultCompEntity resultCompEntity;
	ACDocManager docManager;
	CString strLog;

	//	�V�E���}�ʓǂݍ���
	bResult = readDwg( pDbOld, strPathOldDwg );
	if( bResult )
		bResult = readDwg( pDbNew, strPathNewDwg );

	//	�V�E���}�ʂ̑S�G���e�B�e�B��ID�擾
	if( bResult )
		bResult = getAllObjectId( conObjectIdOld, pDbOld );
	if( bResult )
		bResult = getAllObjectId( conObjectIdNew, pDbNew );
	
	//	��r���s
	if( bResult ){
		resultCompEntity.SetHashTableSize( conObjectIdOld.GetCount() + conObjectIdNew.GetCount() );
		bResult = docManager.CompDwgs( conObjectIdOld, conObjectIdNew, resultCompEntity );
		
		if( !bResult ){
			AfxFormatString2( strLog, IDS_FAIL_TO_COMPARE_DWG, strPathOldDwg, strPathNewDwg );
			zfcLogger::instance().write( _T("\n%s"), strLog );
		}
	}
	
	//	�����}�ʍ쐬
	if( bResult && 0 < resultCompEntity.GetCount() ){
		bResult = docManager.DrawResultDwg(pDbNew->blockTableId(), pDbOld->blockTableId(), resultCompEntity, conObjectIdNew);

		if( !bResult ){
			AfxFormatString2( strLog, IDS_FAIL_TO_COMPOUND_DWG, strPathOldDwg, strPathNewDwg );
			zfcLogger::instance().write( _T("\n%s"), strLog );
		}
	}

	delete pDbOld;
	delete pDbNew;

	return bResult;
}

bool zfcComparetor::readDwg( AcDbDatabase*& pDb, const CString& strPath ) const
{
	bool bResult = true;
	auto es = zfcUtility::readDwg( pDb, strPath );

	if( Acad::eOk != es ){
		CString str;

		AfxFormatString1( str, IDS_FAIL_TO_READ_DWG, strPath );
		zfcLogger::instance().write( _T("\n%s"), str );
		bResult = false;
	}
	
	return bResult;
}

bool zfcComparetor::getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb ) const
{
	bool bResult = true;
	auto es = zfcUtility::getAllObjectId( conObjectId, pDb );

	if( Acad::eOk != es ){
		CString str;

		// todo API����
		AfxFormatString1( str, IDS_FAIL_TO_GET_ID, pDb->document()->title() );
		zfcLogger::instance().write( _T("\n%s"), str );
		bResult = false;
	}
	
	return bResult;
}
