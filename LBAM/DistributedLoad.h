///////////////////////////////////////////////////////////////////////
// LBAM - Longitindal Bridge Analysis Model
// Copyright � 1999-2012  Washington State Department of Transportation
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

// DistributedLoad.h : Declaration of the CDistributedLoad

#ifndef __DISTRIBUTEDLOAD_H_
#define __DISTRIBUTEDLOAD_H_

#include "resource.h"       // main symbols
#include "LoadCP.h"

/////////////////////////////////////////////////////////////////////////////
// CDistributedLoad
class ATL_NO_VTABLE CDistributedLoad : 
	public CComObjectRootEx<CComSingleThreadModel>,
//   public CComRefCountTracer<CDistributedLoad,CComObjectRootEx<CComSingleThreadModel> >,
	public CComCoClass<CDistributedLoad, &CLSID_DistributedLoad>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CDistributedLoad>,
   public IPersistImpl<CDistributedLoad>,
	public IStructuredStorage2,
	public IDistributedLoad,
   public IObjectSafetyImpl<CDistributedLoad,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public CProxyDDistributedLoadEvents< CDistributedLoad >
{
public:
   CDistributedLoad():
   m_WEnd(0.0),
   m_WStart(0.0),
   m_StartLocation(0.0),
   m_EndLocation(0.0),
   m_Direction(ldFy),
   m_Orientation(loGlobal),
   m_MemberType(mtUndefined),
   m_MemberID(-1)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DISTRIBUTEDLOAD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDistributedLoad)
	COM_INTERFACE_ENTRY(IDistributedLoad)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IStructuredStorage2)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDistributedLoad)
CONNECTION_POINT_ENTRY(IID_IDistributedLoadEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

   // IStructuredStorage2
	STDMETHOD(Load)(IStructuredLoad2 * Load);
	STDMETHOD(Save)(IStructuredSave2 * Save);

// IDistributedLoad
public:
	STDMETHOD(get_WEnd)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_WEnd)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_WStart)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_WStart)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_Direction)(/*[out, retval]*/ LoadDirection *pVal);
	STDMETHOD(put_Direction)(/*[in]*/ LoadDirection newVal);
	STDMETHOD(get_Orientation)(/*[out, retval]*/ LoadOrientation *pVal);
	STDMETHOD(put_Orientation)(/*[in]*/ LoadOrientation newVal);
	STDMETHOD(get_EndLocation)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_EndLocation)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_StartLocation)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_StartLocation)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_MemberID)(/*[out, retval]*/ MemberIDType *pVal);
	STDMETHOD(put_MemberID)(/*[in]*/ MemberIDType newVal);
	STDMETHOD(get_MemberType)(/*[out, retval]*/ MemberType *pVal);
	STDMETHOD(put_MemberType)(/*[in]*/ MemberType newVal);
	STDMETHOD(Clone)(/*[out,retval]*/IDistributedLoad** clone);

protected:
   Float64          m_WEnd;
   Float64          m_WStart;
   Float64          m_StartLocation;
   Float64          m_EndLocation;
   LoadDirection   m_Direction;
   LoadOrientation m_Orientation;
   MemberType      m_MemberType;
   MemberIDType    m_MemberID;

};

#endif //__DISTRIBUTEDLOAD_H_
