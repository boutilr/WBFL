///////////////////////////////////////////////////////////////////////
// ReportManager - Manages report definitions
// Copyright � 1999-2023  Washington State Department of Transportation
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

#include <ReportManager\IReportView.h>

#include <wrl.h>
#include <wrl/event.h>
#include <wil/result.h>
#include <wil/com.h>
#include "WebView2.h"
#include "WebView2EnvironmentOptions.h"

class EdgeReportView : public WBFL::Reporting::IReportView
{
public:
   EdgeReportView();

   virtual BOOL Create(
      LPCTSTR lpszWindowName,
      DWORD dwStyle,
      const RECT& rect,
      HWND hwndParent,
      UINT nID) override;

   virtual void FitToParent() override;
   virtual void Move(POINT topLeft) override;
   virtual void Size(SIZE size) override;
   virtual void Print(bool bPrompt) override;
   virtual void Find() override;
   virtual void SelectAll() override;
   virtual void Copy() override;
   virtual void Refresh() override;
   virtual void ViewSource() override;
   virtual void Back() override;
   virtual void Forward() override;
   virtual void Navigate(LPCTSTR uri) override;

private:
   HWND m_hwndParent; // handle of parent window
   std::_tstring m_strURI; // caches the URI

   // Pointer to WebViewController
   wil::com_ptr<ICoreWebView2Controller> m_webviewController;

   // Pointer to WebView window
   wil::com_ptr<ICoreWebView2> m_webview;
};
