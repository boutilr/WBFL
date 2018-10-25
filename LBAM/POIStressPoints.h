///////////////////////////////////////////////////////////////////////
// LBAM - Longitindal Bridge Analysis Model
// Copyright � 1999-2016  Washington State Department of Transportation
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

// POIStressPoints.h : Declaration of the CPOIStressPoints

#ifndef __POISTRESSPOINTS_H_
#define __POISTRESSPOINTS_H_

#include "resource.h"       // main symbols
#include "POIStressPointsItem.h"
#include "POICP.h"

#include <WbflComCollections.h>
#include <map>


/////////////////////////////////////////////////////////////////////////////
// CPOIStressPoints
class ATL_NO_VTABLE CPOIStressPoints : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPOIStressPoints, &CLSID_POIStressPoints>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CPOIStressPoints>,
	public IPOIStressPoints,
   public IPersistImpl<CPOIStressPoints>,
	public IStructuredStorage2,
	public CProxyDPOIEvents< CPOIStressPoints >,
	public IStressPointsEvents,
   public IObjectSafetyImpl<CPOIStressPoints,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public CProxyDPOIStressPointsEvents< CPOIStressPoints >
{
public:
	CPOIStressPoints()
	{
	}

void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_POISTRESSPOINTS)
DECLARE_GET_CONTROLLING_UNKNOWN()
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPOIStressPoints)
   COM_INTERFACE_ENTRY(IPOIStressPoints)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IStressPointsEvents)
   COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IStructuredStorage2)
   COM_INTERFACE_ENTRY(IPersist)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPOIStressPoints)
CONNECTION_POINT_ENTRY(IID_IPOIStressPointsEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

   // IStructuredStorage2
	STDMETHOD(Load)(IStructuredLoad2 * Load);
	STDMETHOD(Save)(IStructuredSave2 * Save);

// IStressPointsEvents
   STDMETHOD(OnStressPointsChanged)(/*[in]*/CollectionIndexType index);
   STDMETHOD(OnStressPointsAdded)(/*[in]*/CollectionIndexType index);
   STDMETHOD(OnStressPointsRemoved)(/*[in]*/CollectionIndexType index);

// IPOIStressPoints
public:
	STDMETHOD(Clone)(/*[out,retval]*/IPOIStressPoints** Clone);
	STDMETHOD(Remove)(/*[in]*/BSTR Stage);
	STDMETHOD(Insert)(/*[in]*/BSTR Stage, /*[in]*/IStressPoints* leftSps, /*[in]*/IStressPoints* rightSps);
	STDMETHOD(Clear)();
	STDMETHOD(get_Item)(/*[in]*/VARIANT idx, /*[out, retval]*/ IPOIStressPointsItem* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ CollectionIndexType *pVal);
   STDMETHOD(get__EnumElements)(IEnumPOIStressPoints **ppenum);
   STDMETHOD(get__NewEnum)(IUnknown** ppUnk);

private:

   typedef CAdapt<CComPtr<IPOIStressPointsItem>>    ItemType;
   typedef std::pair<CComBSTR, ItemType>            StoredType;
   typedef std::map< CComBSTR, ItemType>            ContainerType;
   typedef ContainerType::iterator                  IteratorType;

   // copy functions for enums
   typedef _CopyInterfacePair<IPOIStressPointsItem, std::pair<CComBSTR const, ItemType> >   CustomCopyType;
   typedef _CopyVariantFromInterfacePair<std::pair<CComBSTR const, ItemType> > CopyType;


   ContainerType m_Container;

   void DoErase(IteratorType& it, bool doFire=true);
};

#endif //__POISTRESSPOINTS_H_
