///////////////////////////////////////////////////////////////////////
// LBAM - Longitindal Bridge Analysis Model
// Copyright (C) 2009  Washington State Department of Transportation
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

// POI.h : Declaration of the CPOI

#ifndef __POI_H_
#define __POI_H_

#include "resource.h"       // main symbols
#include "POICP.h"

/////////////////////////////////////////////////////////////////////////////
// CPOI
class ATL_NO_VTABLE CPOI : 
	public CComObjectRootEx<CComSingleThreadModel>,
//   public CComRefCountTracer<CPOI,CComObjectRootEx<CComSingleThreadModel> >,
	public CComCoClass<CPOI, &CLSID_POI>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CPOI>,
	public IPOI,
   public IPersistImpl<CPOI>,
	public IStructuredStorage2,
	public CProxyDPOIEvents< CPOI >,
   public IObjectSafetyImpl<CPOI,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public IPOIStressPointsEvents
{
public:
   CPOI():
   m_ID(-1),
   m_Location(0.0),
   m_MemberID(-1),
   m_MemberType(mtUndefined)
	{
      m_POIStressPointsCookie=0;
	}

void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_POI)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPOI)
	COM_INTERFACE_ENTRY(IPOI)
	COM_INTERFACE_ENTRY(IStructuredStorage2)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IPOIStressPointsEvents)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPOI)
CONNECTION_POINT_ENTRY(IID_IPOIEvents)
END_CONNECTION_POINT_MAP()

   // IStructuredStorage2
	STDMETHOD(Load)(IStructuredLoad2 * Load);
	STDMETHOD(Save)(IStructuredSave2 * Save);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// _IPOIStressPointsEvents
   STDMETHOD(OnPOIStressPointsChanged)(IPOIStressPoints*);
   STDMETHOD(OnPOIStressPointsAdded)(IPOIStressPointsItem*);
   STDMETHOD(OnPOIStressPointsBeforeRemove)(IPOIStressPointsItem*);

// IPOI
public:
	STDMETHOD(get_POIStressPoints)(/*[out, retval]*/ IPOIStressPoints* *pVal);
	STDMETHOD(putref_POIStressPoints)(/*[in]*/ IPOIStressPoints* newVal);
	STDMETHOD(get_Location)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_Location)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_MemberID)(/*[out, retval]*/ MemberIDType *pVal);
	STDMETHOD(put_MemberID)(/*[in]*/ MemberIDType newVal);
	STDMETHOD(get_MemberType)(/*[out, retval]*/ MemberType *pVal);
	STDMETHOD(put_MemberType)(/*[in]*/ MemberType newVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ PoiIDType *pVal);
	STDMETHOD(put_ID)(/*[in]*/ PoiIDType newVal);
	STDMETHOD(Clone)(/*[out]*/IPOI** poi);

protected:

   PoiIDType    m_ID;
   Float64      m_Location;
   MemberIDType m_MemberID;
   MemberType   m_MemberType;

   CComPtr<IPOIStressPoints> m_POIStressPoints;
   DWORD                     m_POIStressPointsCookie;
};

#endif //__POI_H_
