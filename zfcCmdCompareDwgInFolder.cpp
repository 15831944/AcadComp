#include "stdafx.h"
#include "resource.h"
#include "zfcCmdCompareDwgInFolder.h"
#include "zfcUtility.h"
#include "zfcLogger.h"
#include "zfcComparetor.h"
#include "zfcDlgSelectFolder.h"

zfcCmdCompareDwgInFolder::zfcCmdCompareDwgInFolder(void)
{
}

zfcCmdCompareDwgInFolder::~zfcCmdCompareDwgInFolder(void)
{
	VERIFY( zfcLogger::instance().close() );
}

bool zfcCmdCompareDwgInFolder::execute()
{
	zfc::pathContainer conPathOld;
	zfc::pathContainer conPathNew;

	//	�e��t�H���_��I������
	if( !selectFolder() )
		return true;

	VERIFY( zfcLogger::instance().open( zfcUtility::filePath( folderOutput(), zfcUtility::logFileName() ) ) );

	zfcUtility::writeLog1( IDS_OLD_DWG_FOLDER, folderOldDwg() );
	zfcUtility::writeLog1( IDS_NEW_DWG_FOLDER, folderNewDwg() );

	//	�t�H���_���̃t�@�C�����擾
	getDwgInFoder( conPathOld, folderOldDwg() );
	getDwgInFoder( conPathNew, folderNewDwg() );

	// ��r���s
	zfc::for_each( conPathNew, [=](zfc::pathContainer::const_reference pairNew){ compare( pairNew, conPathOld ); } );

	// ���}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
	writeLogOnlyExistInOldDwgFolder( conPathOld );

	// �V�}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
	writeLogOnlyExistInNewDwgFolder();

	return true;
}

//	�e��t�H���_��I������
bool zfcCmdCompareDwgInFolder::selectFolder()
{
	auto& dlg = zfcDlgSelectFolder::instance();
	bool bSelect = false;

	//	�t�H���_�I���_�C�A���O
	if( dlg.DoModal() == IDOK ){
		m_strFolderOldDwg = dlg.folderOldDwg();
		m_strFolderNewDwg = dlg.folderNewDwg();
		m_strFolderOutput = dlg.folderCompoundDwg();
		bSelect = true;
	}

	return bSelect;
}

//	�t�H���_���̐}�ʃt�@�C�����擾
void zfcCmdCompareDwgInFolder::getDwgInFoder( zfc::pathContainer& conPath, const CString& strFolder ) const
{
	CFileFind ff;
	CString strFindFile;
	BOOL bFlg = TRUE;

	strFindFile.Format( _T("%s\\*.dwg"), strFolder );
	bFlg = ff.FindFile( strFindFile );

	while( bFlg ){
		bFlg = ff.FindNextFile();

		if( bFlg && !ff.IsDots() && !ff.IsDirectory() ){
			auto itp = conPath.insert( zfc::pathContainer::value_type( ff.GetFileTitle(), ff.GetFilePath() ) );
			assert( itp.second );
		}
	}
}

//	�t�@�C���p�X�����o
bool zfcCmdCompareDwgInFolder::findPath( zfc::pathContainer::const_iterator& itPath, const CString& strFind, const zfc::pathContainer& conPath ) const
{
	bool bFind = false;
	auto it = conPath.find( strFind );

	if( conPath.end() == it ){
		auto strNearFile = strFind.Left( strFind.GetLength()-1 );

		it = conPath.find( strNearFile );
	}

	if( conPath.end() != it ){
		bFind = true;
		itPath = it;
	}
	
	return bFind;
}

//	�}�ʂ��r����
void zfcCmdCompareDwgInFolder::compare( zfc::pathContainer::const_reference pairNew, const zfc::pathContainer& conPathOld ) const
{
	const CString& strTitleNew = pairNew.first;
	zfc::pathContainer::const_iterator itOld;
	bool bFind = findPath( itOld, strTitleNew, conPathOld );
	zfcComparetor comparetor;

	if( bFind ){
		auto es = comparetor.execute( itOld->second, pairNew.second );
		assert( Acad::eOk == es );
		addProcessed( itOld->first, itOld->second );
	}
	else{
		addUnProcessed( pairNew.first, pairNew.second );
	}
}

// ���}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
void zfcCmdCompareDwgInFolder::writeLogOnlyExistInOldDwgFolder(zfc::pathContainer& conPathOld) const
{
	zfcUtility::writeLog1( IDS_ONLY_EXIST_OLD_DWG_FOLDER, _T("") );

	//	���}�ʃt�H���_�ɑ��݂����t�@�C���R���e�i����A�����ς̃t�@�C��������
	zfc::for_each( m_conProcessed, [&](zfc::pathContainer::const_reference pair){conPathOld.erase(pair.first);} );

	//	���}�ʃt�H���_�݂̂ɑ��݂����t�@�C�����o��
	zfcUtility::writeFileName( conPathOld );
}

// �V�}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
void zfcCmdCompareDwgInFolder::writeLogOnlyExistInNewDwgFolder() const
{
	zfcUtility::writeLog1( IDS_ONLY_EXIST_NEW_DWG_FOLDER, _T("") );
	zfcUtility::writeFileName( m_conUnProcessed );
}
