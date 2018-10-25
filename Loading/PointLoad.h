///////////////////////////////////////////////////////////////////////
// Loading - Generic library to describe bridge loadings
// Copyright � 1999-2010  Washington State Department of Transportation
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

// PointLoad.h : Declaration of the CPointLoad

#ifndef __POINTLOAD_H_
#define __POINTLOAD_H_

#include "resource.h"       // main symbols
#include "LoadingCP.h"
#include "TxnMgrImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CPointLoad
class ATL_NO_VTABLE CPointLoad : 
	public CComObjectRootEx<CComSingleThreadModel>,
//   public CComRefCountTracer<CPointLoad,CComObjectRootEx<CComSingleThreadModel> >,
	public CComCoClass<CPointLoad, &CLSID_ldPointLoad>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CPointLoad>,
	public ldIPointLoad,
   public IPersistImpl<CPointLoad>,
   public ISupportTransactionsImpl,
   public IStructuredStorage2,
   public IObjectSafetyImpl<CPointLoad,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public CProxyIPointLoadEvents< CPointLoad >
{
public:
   CPointLoad():
   m_Location(0.0),
   m_MemberID(-1),
//   m_MemberType(mtUndefined),
   m_Fx(0.0),
   m_Fy(0.0),
   m_Mz(0.0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_POINTLOAD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPointLoad)
	COM_INTERFACE_ENTRY(ldIPointLoad)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IStructuredStorage2)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPointLoad)
CONNECTION_POINT_ENTRY(IID_ldIPointLoadEvents)
END_CONNECTION_POINT_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

   // IStructuredStorage2
	STDMETHOD(Load)(IStructuredLoad2 * Load);
	STDMETHOD(Save)(IStructuredSave2 * Save);

// ldIPointLoad
public:
	STDMETHOD(Clone)(/*[out,retval]*/ldIPointLoad** clone);
	STDMETHOD(GetForce)(/*[out]*/Float64* Fx, /*[out]*/Float64* Fy, /*[out]*/Float64* Mz);
	STDMETHOD(SetForce)(/*[in]*/Float64 Fx, /*[in]*/Float64 Fy, /*[in]*/Float64 Mz);
	STDMETHOD(get_Mz)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_Mz)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_Fy)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_Fy)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_Fx)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_Fx)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_Location)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(put_Location)(/*[in]*/ Float64 newVal);
	STDMETHOD(get_MemberID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MemberID)(/*[in]*/ long newVal);
//	STDMETHOD(get_MemberType)(/*[out, retval]*/ MemberType *pVal);
//	STDMETHOD(put_MemberType)(/*[in]*/ MemberType newVal);

protected:
//   MemberType   m_MemberType;
   long         m_MemberID;
   Float64       m_Location;
   Float64       m_Fx;
   Float64       m_Fy;
   Float64       m_Mz;

   static HRESULT PutLocation(CPointLoad* pThis,Float64 location);
   static HRESULT PutFx(CPointLoad* pThis,Float64 fx);
   static HRESULT PutFy(CPointLoad* pThis,Float64 fy);
   static HRESULT PutMz(CPointLoad* pThis,Float64 mz);
   static HRESULT PutMemberID(CPointLoad* pThis,long id);
};

#endif //__POINTLOAD_H_
