///////////////////////////////////////////////////////////////////////
// COGO - Coordinate Geometry
// Copyright � 1999-2016  Washington State Department of Transportation
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

// PointCollection.h : Declaration of the CPointCollection

#ifndef __PointCollection_H_
#define __PointCollection_H_
#pragma once

#include "resource.h"       // main symbols
#include "COGOCP.h"
#include "Collections.h"

class CPointCollection;
typedef PersistentKeyedCollection<CPointCollection,IPointCollection,&IID_IPointCollection,CogoElementKey,IPoint2d> PointCollectionImpl;

/////////////////////////////////////////////////////////////////////////////
// CPointCollection
class ATL_NO_VTABLE CPointCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
// public CComRefCountTracer<CPointCollection,CComObjectRootEx<CComSingleThreadModel> >,
	public CComCoClass<CPointCollection, &CLSID_PointCollection>,
	public IConnectionPointContainerImpl<CPointCollection>,
   public IPoint2dEvents,
   public CProxyDPointCollectionEvents< CPointCollection >,
   public PointCollectionImpl
{
public:
	CPointCollection()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_POINTCOLLECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPointCollection)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)

   COM_INTERFACE_ENTRY(IPoint2dEvents)

   COM_INTERFACE_ENTRY_CHAIN(PointCollectionImpl)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPointCollection)
   CONNECTION_POINT_ENTRY(IID_IPointCollectionEvents)
END_CONNECTION_POINT_MAP()

   virtual CComBSTR GetCollectionName() { return CComBSTR("Points"); }
   virtual CComBSTR GetItemName() { return CComBSTR("Point"); }

//// ISupportsErrorInfo
//	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPointCollection
public:
//   STDMETHOD(get_StructuredStorage)(/*[out,retval]*/IStructuredStorage2* *pStg);
   STDMETHOD(Clone)(/*[out,retval]*/IPointCollection* *clone);
	STDMETHOD(Clear)();
	STDMETHOD(Add)(/*[in]*/ CogoElementKey key,/*[in]*/ Float64 x,/*[in]*/ Float64 y,/*[out,retval]*/ IPoint2d* *point);
	STDMETHOD(AddEx)(/*[in]*/ CogoElementKey key,/*[in]*/ IPoint2d* newVal);
	STDMETHOD(Remove)(/*[in]*/ CogoElementKey key);
	STDMETHOD(get_Count)(/*[out, retval]*/ CollectionIndexType *pVal);
	STDMETHOD(get_Item)(/*[in]*/ CogoElementKey key, /*[out, retval]*/ IPoint2d* *pVal);
	STDMETHOD(putref_Item)(/*[in]*/ CogoElementKey key, /*[in]*/ IPoint2d* newVal);
// STDMETHOD(get__NewEnum)(IUnknown** retval);
   STDMETHOD(get__EnumKeys)(IEnumKeys** ppenum);
	STDMETHOD(FindKey)(/*[in]*/ IPoint2d* point,/*[out,retval]*/CogoElementKey* key);
	STDMETHOD(get_Factory)(/*[out,retval]*/IPoint2dFactory** factory);
	STDMETHOD(putref_Factory)(/*[in]*/IPoint2dFactory* factory);
	STDMETHOD(Key)(/*[in]*/ CollectionIndexType index,/*[out,retval]*/ CogoElementKey* key);
   STDMETHOD(get__EnumPoints)(/*[out,retval]*/ IEnumPoint2d** ppenum);

// IPointEvents
public:
	STDMETHOD(OnPointChanged)(IPoint2d* point);

private:
   CComPtr<IPoint2dFactory> m_Factory;

   HRESULT OnBeforeSave(IStructuredSave2* pSave);
   HRESULT OnBeforeLoad(IStructuredLoad2* pLoad);

   HRESULT PointNotFound(CogoElementKey key);
   HRESULT PointAlreadyDefined(CogoElementKey key);
   HRESULT PointKeyError(CogoElementKey key,UINT nHelpString,HRESULT hRes);

   void Advise(CogoElementKey key,IPoint2d* point);
   void Unadvise(CogoElementKey key,IPoint2d* point);
   void UnadviseAll();
   std::map<CogoElementKey,DWORD> m_Cookies;
};

#endif //__PointCollection_H_
