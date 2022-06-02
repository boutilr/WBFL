///////////////////////////////////////////////////////////////////////
// LBAM Test - Test driver for LBAM analysis library
// Copyright � 1999-2022  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This library is a part of the Washington Bridge Foundation Libraries
// and was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Library Open Source License as 
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// PURPOSE.  See the Alternate Route Library Open Source License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// TestPOI.h: interface for the CTestPOI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTPOI_H__4A50DAE5_753F_11D5_B02C_00105A9AF985__INCLUDED_)
#define AFX_TESTPOI_H__4A50DAE5_753F_11D5_B02C_00105A9AF985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestPOI :
public CComObjectRootEx<CComSingleThreadModel>,
public IPOIEvents,
public IPOIsEvents
{
public:
	static void Test();

	CTestPOI();
	virtual ~CTestPOI();

   void Init();

BEGIN_COM_MAP(CTestPOI)
   COM_INTERFACE_ENTRY(IPOIEvents)
   COM_INTERFACE_ENTRY(IPOIsEvents)
END_COM_MAP()

public:
   STDMETHOD(OnPOIChanged)(/*[in]*/IPOI* POI, ChangeType change);
   STDMETHOD(OnPOIRenamed)(/*[in]*/PoiIDType oldId, PoiIDType newId);

   STDMETHOD(OnPOIsChanged)(/*[in]*/IPOI* POI, ChangeType change);
   STDMETHOD(OnPOIsRenamed)(/*[in]*/PoiIDType oldId, PoiIDType newId);
   STDMETHOD(OnPOIsAdded)(/*[in]*/IPOI* POI);
   STDMETHOD(OnPOIsBeforeRemove)(/*[in]*/IPOI* POI);

   bool m_csPOIFired;
   bool m_csDFFired;
   bool m_csSPFired;
   unsigned long m_CsCookie;

};

#endif // !defined(AFX_TESTPOI_H__4A50DAE5_753F_11D5_B02C_00105A9AF985__INCLUDED_)
