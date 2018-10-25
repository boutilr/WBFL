///////////////////////////////////////////////////////////////////////
// Reporter - Report Creation and Representation Library
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
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

#include <Reporter\ReporterLib.h>
#include "HtmlUnitTagFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************************************
CLASS
   rptHtmlUnitTagFormat
****************************************************************************/



////////////////////////// PUBLIC     ///////////////////////////////////////

//======================== LIFECYCLE  =======================================
//======================== OPERATORS  =======================================
//======================== OPERATIONS =======================================
std::string rptHtmlUnitTagFormat::Format(const std::string& tag)
{
   std::ostringstream stm;
   std::string fmt;
   bool bSupOn = false;
   bool bSubOn = false;
   Int16 len = tag.length();
   for ( Int16 i = 0; i < len; i++ )
   {
      if ( tag[i] == '^' )
      {
         // ^ = start of superscript or end of subscript
         if ( !bSupOn && !bSubOn )
         {
            // neither in a super or subscript block... start a superscript
            stm << "<SUP>";
            bSupOn = true;
         }
         else if ( bSubOn )
         {
            // currently in a subscript block... end it
            stm << "</SUB>";
            bSubOn = false;
         }
      }
      else if ( tag[i] == '_' )
      {
         // _ = start of subscript or end of superscript
         if ( !bSupOn && !bSubOn )
         {
            // neither in a sub or superscript block... start a subscript
            stm << "<SUB>";
            bSubOn = true;
         }
         else if ( bSupOn )
         {
            // currently in a superscript block... end it
            stm << "</SUP>";
            bSupOn = false;
         }
      }
      else if ( bSupOn && (tag[i] == '/' || tag[i] == ' ') )
      {
         stm << "</SUP>" << tag[i];
         bSupOn = false;
      }
      else if ( bSubOn && tag[i] == ' ' )
      {
         stm << "</SUB>" << tag[i];
         bSubOn = false;
      }
      else
      {
         stm << tag[i];
      }
   }

   // if super got left on, turn it off now.
   if ( bSupOn )
   {
      stm << "</SUP>";
      bSupOn = false;
   }

   // if subscript got left on, turn it off now.
   if ( bSubOn )
   {
      stm << "</SUB>";
      bSubOn = false;
   }

   return stm.str();
}

//======================== ACCESS     =======================================
//======================== INQUIRY    =======================================

////////////////////////// PROTECTED  ///////////////////////////////////////

//======================== LIFECYCLE  =======================================
//======================== OPERATORS  =======================================
//======================== OPERATIONS =======================================
//======================== ACCESS     =======================================
//======================== INQUIRY    =======================================

////////////////////////// PRIVATE    ///////////////////////////////////////

//======================== LIFECYCLE  =======================================
//======================== OPERATORS  =======================================
//======================== OPERATIONS =======================================
//======================== ACCESS     =======================================
//======================== INQUERY    =======================================

//======================== DEBUG      =======================================
#if defined _DEBUG
bool rptHtmlUnitTagFormat::AssertValid() const
{
   return true;
}

void rptHtmlUnitTagFormat::Dump(dbgDumpContext& os) const
{
   os << "Dump for rptHtmlUnitTagFormat" << endl;
}
#endif // _DEBUG

#if defined _UNITTEST
bool rptHtmlUnitTagFormat::TestMe(dbgLog& rlog)
{
   TESTME_PROLOGUE("rptHtmlUnitTagFormat");

   TEST_NOT_IMPLEMENTED("Unit Tests Not Implemented for rptHtmlUnitTagFormat");

   TESTME_EPILOG("HtmlUnitTagFormat");
}
#endif // _UNITTEST
