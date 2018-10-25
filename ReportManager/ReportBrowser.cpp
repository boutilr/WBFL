///////////////////////////////////////////////////////////////////////
// ReportManager - Manages report definitions
// Copyright � 1999-2014  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This library is a part of the Washington Bridge Foundation Libraries
// and was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// ReportBrowser.cpp: implementation of the CReportBrowser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReportManager.h"
#include <ReportManager\ReportBrowser.h>
#include <ReportManager\ReportBuilder.h>
#include <ReportManager\ReportBuilderManager.h>
#include "WebBrowser.h"
#include "WBCmdGroup.h" // magical cmd_ids for find

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void to_upper( std::_tstring::iterator begin,std::_tstring::iterator end)
{
   while ( begin != end )
   {
      *begin = toupper(*begin);
      begin++;
   }
}

std::_tstring filename_to_URL(const std::_tstring& fname)
{
   //turn into an internet-looking url
   std::_tstring filename(fname);
   CollectionIndexType pos;
   while((pos=filename.find( _T("\\") )) != std::_tstring::npos)
      filename.replace(pos,1,_T("/"));

   filename = _T("file://") + filename;
   return filename;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReportBrowser::CReportBrowser()
{
   m_pWebBrowser = new CWebBrowser;
}

CReportBrowser::~CReportBrowser()
{
   // We are done with our HTML file... Delete it
   if (!m_Filename.empty())
   {
      ::DeleteFile( m_Filename.c_str() );
   }

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   if ( m_pWebBrowser )
   {
      delete m_pWebBrowser;
   }
}

void CReportBrowser::UpdateReport(boost::shared_ptr<rptReport>& pReport,bool bRefresh)
{
   std::_tofstream ofile( m_Filename.c_str() );
   ATLASSERT( ofile.is_open() == true );

   Uint32 logPixX;
   Uint32 logPixY;
   CDC dc;
   dc.CreateDC(CString("DISPLAY"),NULL,NULL,NULL);
   logPixX = dc.GetDeviceCaps(LOGPIXELSX);
   logPixY = dc.GetDeviceCaps(LOGPIXELSY);

   rptHtmlReportVisitor visitor( &ofile, logPixX, logPixY );
   visitor.VisitReport( pReport.get() );

   if (bRefresh)
   {
      m_pWebBrowser->Refresh();
   }
}

bool CReportBrowser::Initialize(HWND hwnd,CReportBuilderManager* pRptMgr,boost::shared_ptr<CReportSpecification>& pRptSpec,boost::shared_ptr<CReportSpecificationBuilder>& pRptSpecBuilder,boost::shared_ptr<rptReport>& pReport)
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());

   // Did you forget something?  This method needs a pointer to a rptReport
   ATLASSERT( pReport != 0 );

   m_pRptMgr = pRptMgr;
   m_pRptSpec = pRptSpec;
   m_pRptSpecBuilder = pRptSpecBuilder;

   // The window associated with hwnd does not exist!
   ATLASSERT( ::IsWindow(hwnd) );

   // use empty file name as flag that browser has not yet been initialized.
   bool bIsNewFile = m_Filename.empty();
   if (bIsNewFile)
   {
      MakeFilename();

      CRect rect(0,0,0,0);
      CWnd* pParent = CWnd::FromHandle( hwnd );
      BOOL bCreated = m_pWebBrowser->Create(TEXT("Browser Control"),
                                            WS_CHILD | WS_VISIBLE, rect, pParent, 100);
      if ( !bCreated )
      {
         TRACE( TEXT("Failed to create browser") );
         return false;
      }
   }

   UpdateReport(pReport,false);

   if (bIsNewFile)
   {
      m_pWebBrowser->Navigate( m_Filename.c_str());
   }
   else
   {
      m_pWebBrowser->Refresh();
   }

   return true;
}

boost::shared_ptr<CReportSpecification> CReportBrowser::GetReportSpecification()
{
   return m_pRptSpec;
}

boost::shared_ptr<rptReport> CReportBrowser::GetReport()
{
   return m_pReport;
}

std::_tstring CReportBrowser::GetReportTitle()
{
   return m_pRptSpec->GetReportTitle();
}

void CReportBrowser::Move(POINT topLeft)
{
   m_pWebBrowser->SetWindowPos(NULL,topLeft.x,topLeft.y,0,0,SWP_NOZORDER | SWP_NOSIZE);
}

void CReportBrowser::Size(SIZE size)
{
   m_pWebBrowser->SetWindowPos(NULL,0,0,size.cx,size.cy,SWP_NOZORDER | SWP_NOMOVE);
}

CWnd* CReportBrowser::GetBrowserWnd()
{
   return m_pWebBrowser;
}

void CReportBrowser::Print(bool bPrompt)
{
   // Build footer string
   std::_tstring lftFoot = m_pRptSpec->GetLeftFooter();
   std::_tstring ctrFoot = m_pRptSpec->GetCenterFooter();
   CString footer;
   footer.Format(_T("%s&b%s&b&d"), lftFoot.c_str(), ctrFoot.c_str());

   // Build Header string
   std::_tstring lftHead = m_pRptSpec->GetLeftHeader();
   std::_tstring ctrHead = m_pRptSpec->GetCenterHeader();
   CString header;
   header.Format(_T("%s&b%s&bPage &p of &P"), lftHead.c_str(), ctrHead.c_str());

   // Print from browser
   m_pWebBrowser->Print(header, footer);
}

bool CReportBrowser::Edit(bool bUpdate)
{
   boost::shared_ptr<CReportBuilder> pRptBuilder = m_pRptMgr->GetReportBuilder(m_pRptSpec->GetReportName());
   CReportDescription rptDesc = pRptBuilder->GetReportDescription();

   boost::shared_ptr<CReportSpecificationBuilder> pReportSpecBuilder(m_pRptSpecBuilder);
   if ( m_pRptSpecBuilder == NULL )
   {
      pReportSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   }

   boost::shared_ptr<CReportSpecification> pReportSpec = pReportSpecBuilder->CreateReportSpec(rptDesc,m_pRptSpec);
   
   // user cancelled.
   if( pReportSpec == NULL )
   {
      return false;
   }

   m_pRptSpec = pReportSpec;

   if ( bUpdate )
   {
      boost::shared_ptr<rptReport> pReport = pRptBuilder->CreateReport( m_pRptSpec );
      UpdateReport(pReport,true);
   }

   return true;
}

void CReportBrowser::Find()
{
   LPDISPATCH lpDispatch = m_pWebBrowser->GetDocument();
   IOleCommandTarget* pIOleCmdTarget;
   if ( S_OK == lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&pIOleCmdTarget ) )
   {
      pIOleCmdTarget->Exec(&CGID_IWebBrowserPriv, CWBCmdGroup::HTMLID_FIND, 0, NULL,NULL);
      pIOleCmdTarget->Release();
   }
}

void CReportBrowser::SelectAll()
{
   LPDISPATCH lpDispatch = m_pWebBrowser->GetDocument();
   IOleCommandTarget* pIOleCmdTarget;
   if ( S_OK == lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&pIOleCmdTarget ) )
   {
      pIOleCmdTarget->Exec(NULL,OLECMDID_SELECTALL, OLECMDEXECOPT_DODEFAULT, NULL,NULL);
      pIOleCmdTarget->Release();
   }
}

void CReportBrowser::ViewSource()
{
   LPDISPATCH lpDispatch = m_pWebBrowser->GetDocument();
   IOleCommandTarget* pIOleCmdTarget;
   if ( S_OK == lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&pIOleCmdTarget ) )
   {
      pIOleCmdTarget->Exec(&CGID_IWebBrowserPriv, CWBCmdGroup::HTMLID_VIEWSOURCE, 0, NULL,NULL);
      pIOleCmdTarget->Release();
   }
}

void CReportBrowser::Refresh()
{
   m_pWebBrowser->Refresh();
}


void CReportBrowser::Back()
{
   m_pWebBrowser->GoBack();
}

void CReportBrowser::Forward()
{
   m_pWebBrowser->GoForward();
}

void CReportBrowser::NavigateAnchor(long id)
{
   std::_tstring filename = filename_to_URL(m_Filename);
   CString anc;
   anc.Format(_T("%s#_%d"),filename.c_str(),id);
   m_pWebBrowser->Navigate(anc);
}

void CReportBrowser::MakeFilename()
{
   TCHAR temp_path[ _MAX_PATH ];
   TCHAR temp_file[ _MAX_PATH ];
   bool should_delete = true;

   if ( ::GetTempPath( _MAX_PATH, temp_path ) == 0 )
      _tcscpy_s(temp_path,_MAX_PATH,_T("C:\\")); // Couldn't establish a temp path, just use the root drive.

   //
   // Make sure the temp path actually exists
   // We do this by looking for any file in the directory.  If nothing shows up, then the
   // path doesn't exist. (Well, this isn't exactly true, but its the best I can come up
   // with).
   CFileFind finder;
   BOOL bExist;
   CString path(temp_path);
   if ( path[path.GetLength()-1] != _T('\\') )
      path += _T("\\");

   path += _T("*.*");

   bExist = finder.FindFile(path);
   if ( !bExist )
      _tcscpy_s( temp_path,_MAX_PATH, _T("C:\\") );

   // This creates a file called "temp_file".TMP
   if ( ::GetTempFileName( temp_path, _T("tmp"), 0, temp_file ) == 0 )
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s( temp_file, _MAX_PATH, _T("wbfl_report.tmp") );
      should_delete = false;
   }

   // Replace the TMP extension with HTM
   m_Filename.assign( temp_file );
   m_Filename.replace( m_Filename.end() - 3, m_Filename.end(), _T("HTM") );

   // We don't want the file Windows created for us
   if ( should_delete )
      ::DeleteFile( temp_file );

   to_upper( m_Filename.begin(), m_Filename.end() );
}
