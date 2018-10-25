///////////////////////////////////////////////////////////////////////
// PGSuper - Prestressed Girder SUPERstructure Design and Analysis
// Copyright � 1999-2015  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// StatusMessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "StatusMessageDialog.h"
#include <EAF\EAFStatusItem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusMessageDialog dialog
CStatusMessageDialog::CStatusMessageDialog(CEAFStatusItem* pStatusItem,eafTypes::StatusSeverityType severity,BOOL bRemoveableOnError,UINT helpID,CWnd* pParent /*=NULL*/)
	: CDialog(CStatusMessageDialog::IDD, pParent),
   m_Message(pStatusItem->GetDescription()),m_HelpID(helpID), m_Severity(severity), m_bRemoveableOnError(bRemoveableOnError)
{
	//{{AFX_DATA_INIT(CStatusMessageDialog)
	//}}AFX_DATA_INIT
}


void CStatusMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusMessageDialog)
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CStatusMessageDialog)
	ON_BN_CLICKED(IDHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusMessageDialog message handlers
void CStatusMessageDialog::OnHelp() 
{
   ::HtmlHelp( *this, AfxGetApp()->m_pszHelpFilePath, HH_HELP_CONTEXT, m_HelpID );
}

BOOL CStatusMessageDialog::OnInitDialog() 
{

   if (m_HelpID != 0)
   {
      m_Message += CString(_T("\r\n\r\nPress Help for more details."));
   }

	CDialog::OnInitDialog();
	
   if ( m_HelpID == 0 )
   {
      // If there isn't a help ID, hide the Help button and
      // shift the OK and Close buttons to the right
      CWnd* pwndHelp = GetDlgItem(IDHELP);
      CRect rHelp;
      pwndHelp->GetWindowRect(&rHelp);
      ScreenToClient(rHelp);

      CWnd* pwndClose = GetDlgItem(IDCANCEL);
      CRect rClose;
      pwndClose->GetWindowRect(&rClose);
      ScreenToClient(rClose);

      pwndHelp->ShowWindow(SW_HIDE);
      pwndClose->MoveWindow(rHelp);
      GetDlgItem(IDOK)->MoveWindow(rClose);
   }

   bool bHideClose = m_bRemoveableOnError ? false : (m_Severity == eafTypes::statusError ? true : false);
   if ( bHideClose )
   {
      CWnd* pwndClose = GetDlgItem(IDCANCEL);
      CRect rClose;
      pwndClose->GetWindowRect(&rClose);
      ScreenToClient(rClose);
      
      pwndClose->ShowWindow(SW_HIDE);
      GetDlgItem(IDOK)->MoveWindow(rClose);
   }

   // string for group box
   CString strGroupLabel;
   HICON hIcon;
   switch(m_Severity)
   {
   case eafTypes::statusOK:
      strGroupLabel = _T("Information");
      hIcon = ::LoadIcon(NULL,IDI_INFORMATION);
      break;
   
   case eafTypes::statusWarning:
      strGroupLabel = _T("Warning");
      hIcon = ::LoadIcon(NULL,IDI_WARNING);
      break;

   case eafTypes::statusError:
      strGroupLabel = _T("Error");
      hIcon = ::LoadIcon(NULL,IDI_ERROR);
      break;

   default:
      ATLASSERT(false); // should never get here
   }
	GetDlgItem(IDC_GROUP)->SetWindowText(strGroupLabel);

   CStatic* pIcon = (CStatic*)GetDlgItem(IDC_MY_ICON);
   pIcon->SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
