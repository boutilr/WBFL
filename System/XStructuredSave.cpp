///////////////////////////////////////////////////////////////////////
// System - WBFL low level system services
// Copyright � 1999-2022  Washington State Department of Transportation
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

#include <System\SysLib.h>
#include <System\XStructuredSave.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WBFL::System;

XStructuredSave::XStructuredSave(XStructuredSave::Reason reason, LPCTSTR file, Int16 line) :
XBase(file,line),
m_Reason(reason)
{
}

void XStructuredSave::Throw() const
{
   throw* this; //*static_cast<const sysXStructuredSave*>(this);
}

Int32 XStructuredSave::GetReason() const noexcept
{
   return m_Reason;
}

XStructuredSave::Reason XStructuredSave::GetReasonCode() const noexcept
{
   return m_Reason;
}

void XStructuredSave::GetErrorMessage(std::_tstring* pMsg) const
{
   __super::GetErrorMessage(pMsg);

   switch (m_Reason)
   {
   case BadWrite:
      *pMsg += _T("A unknown error occured writing to the file");
      break;
   case CantInitializeTheParser:
      *pMsg += _T("Could not initialize the parser. Perhaps the parser component is not installed");
      break;
   case Unspecified:
      *pMsg += _T("Unspecified error writing structured data");
      break;
   default:
      *pMsg += _T("Unknown error writing structured data");
      break;
   }
   *pMsg += _T("\n\n");
}
