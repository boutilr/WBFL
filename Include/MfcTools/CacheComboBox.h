///////////////////////////////////////////////////////////////////////
// MfcTools - Extension library for MFC
// Copyright � 1999-2024  Washington State Department of Transportation
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
#pragma once

#include <MfcTools\MfcToolsExp.h>

/////////////////////////////////////////////////////////////////////////////
// CCacheComboBox window

class MFCTOOLSCLASS CCacheComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CCacheComboBox)

public:
	CCacheComboBox();
	virtual ~CCacheComboBox();

// Attributes
public:

// Operations
public:

   // When the edit control is disabled, the window text is cached and the window is clear
   // When enabled, the cached window text is restored.
   void EnableWindow(BOOL bEnable);

   int GetCache();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCacheComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
   int m_Cache;

	//{{AFX_MSG(CCacheComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
