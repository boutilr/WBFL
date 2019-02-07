///////////////////////////////////////////////////////////////////////
// Reporter - Report Creation and Representation Library
// Copyright � 1999-2019  Washington State Department of Transportation
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

#include <Reporter\ReporterLib.h>
#include <Reporter\ReportLayoutItem.h>
#include <Reporter\PageLayout.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************************************
CLASS
   rptReportLayoutItem
****************************************************************************/


////////////////////////// PUBLIC     ///////////////////////////////////////

//======================== LIFECYCLE  =======================================
rptReportLayoutItem::rptReportLayoutItem() :
rptReportItem()
{
   m_pPageLayout = 0;
}


rptReportLayoutItem::rptReportLayoutItem(const rptStyleName& rStyleName, 
                                         const rptPageLayout& rLayout) :
rptReportItem(rStyleName)
{
   m_pPageLayout = new rptPageLayout( rLayout );
}


rptReportLayoutItem::rptReportLayoutItem(const rptReportLayoutItem& rOther):
rptReportItem(rOther)
{
   m_pPageLayout = 0;
   MakeCopy(rOther);
}


rptReportLayoutItem::~rptReportLayoutItem()
{
   delete m_pPageLayout;
}

//======================== OPERATORS  =======================================
rptReportLayoutItem& rptReportLayoutItem::operator=(const rptReportLayoutItem& rOther)
{
   if ( this != &rOther )
      MakeAssignment(rOther);

   return *this;
}

//======================== OPERATIONS =======================================

void rptReportLayoutItem::SetPageLayout( const rptPageLayout& rPageLayout)
{
   delete m_pPageLayout;
   m_pPageLayout = new rptPageLayout( rPageLayout );
}


rptPageLayout* rptReportLayoutItem::GetpPageLayout()
{
   if ( m_pPageLayout )
   {
      return m_pPageLayout;
   }
   else
   {
      // no PageLayout locally - ask parent.

      rptReportLayoutItem* myp = dynamic_cast<rptReportLayoutItem*>(GetpParent());
      CHECKX(myp,_T("Child must have parent of type ReportLayoutItem to get PageLayout"));

      return  myp->GetpPageLayout();
   }
}

//======================== ACCESS     =======================================
//======================== INQUIRY    =======================================

////////////////////////// PROTECTED  ///////////////////////////////////////

//======================== LIFECYCLE  =======================================
//======================== OPERATORS  =======================================
//======================== OPERATIONS =======================================
//======================== ACCESS     =======================================
void rptReportLayoutItem::MakeCopy(const rptReportLayoutItem& rOther)
{
   delete m_pPageLayout;

   if ( rOther.m_pPageLayout )
      m_pPageLayout = new rptPageLayout( *rOther.m_pPageLayout );
   else
      m_pPageLayout = 0;
}


void rptReportLayoutItem::MakeAssignment(const rptReportLayoutItem& rOther)
{
   rptReportItem::MakeAssignment(rOther);
   MakeCopy( rOther );
}


//======================== INQUIRY    =======================================

////////////////////////// PRIVATE    ///////////////////////////////////////

//======================== LIFECYCLE  =======================================
//======================== OPERATORS  =======================================
//======================== OPERATIONS =======================================
//======================== ACCESS     =======================================
//======================== INQUERY    =======================================


