///////////////////////////////////////////////////////////////////////
// COGO - Coordinate Geometry
// Copyright � 1999-2015  Washington State Department of Transportation
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

// SurfaceProfile.h : Declaration of the CSurfaceProfile
// A surface profile is similar to a surface template except that it is cut
// at any orientation across the surface (assuming the cut is somewhat transverse to the alignment)

#pragma once

#include "resource.h"       // main symbols
#include "COGOCP.h"


/////////////////////////////////////////////////////////////////////////////
// CSurfaceProfile
class ATL_NO_VTABLE CSurfaceProfile : 
	public CComObjectRootEx<CComSingleThreadModel>,
//   public CComRefCountTracer<CSurfaceProfile,CComObjectRootEx<CComSingleThreadModel> >,
	public CComCoClass<CSurfaceProfile, &CLSID_SurfaceProfile>,
	public ISupportErrorInfo,
   public IObjectSafetyImpl<CSurfaceProfile,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	//public IConnectionPointContainerImpl<CSurfaceProfile>,
   public ISurfaceProfile,
   public IStructuredStorage2,
   //public CProxyDSurfaceProfileEvents< CSurfaceProfile >,
   public IPersistImpl<CSurfaceProfile>
{
public:
	CSurfaceProfile()
	{
	}

	HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_SURFACEPROFILE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSurfaceProfile)
	COM_INTERFACE_ENTRY(ISurfaceProfile)
	COM_INTERFACE_ENTRY(IStructuredStorage2)
   COM_INTERFACE_ENTRY(ISupportErrorInfo)
	//COM_INTERFACE_ENTRY(IConnectionPointContainer)
	//COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IObjectSafety)

   COM_INTERFACE_ENTRY(IPersist)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CSurfaceProfile)
//CONNECTION_POINT_ENTRY(IID_ISurfaceProfileEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ISurfaceProfile
public:
   STDMETHOD(get_Surface)(ISurface* *pVal);
   STDMETHOD(putref_Surface)(ISurface* newVal);
   STDMETHOD(put_Station)(VARIANT varStation);
   STDMETHOD(get_Station)(IStation** station);
   STDMETHOD(put_Direction)(VARIANT varDirection);
   STDMETHOD(get_Direction)(IDirection** direction);
   STDMETHOD(AddPoint)(VARIANT varStation,Float64 normalOffset,Float64 cutLineOffset,Float64 elev,IPoint2d* pnt);
   STDMETHOD(AddPointEx)(ISurfacePoint* point);
   STDMETHOD(RemovePoint)(CollectionIndexType idx);
   STDMETHOD(get_Item)(CollectionIndexType idx,ISurfacePoint** point);
   STDMETHOD(get_Count)(CollectionIndexType* count);
   STDMETHOD(Clear)();
   STDMETHOD(GetRidgePointElevationChange)(CollectionIndexType ridgePointIdx1,CollectionIndexType ridgePointIdx2,Float64* deltaElevation);
   STDMETHOD(GetElevationChange)(CollectionIndexType ridgePointIdx,Float64 offset,Float64* deltaElevation);
   STDMETHOD(GetSlope)(CollectionIndexType ridgePointIdx,Float64 offset,Float64* pSlope);
   STDMETHOD(GetSegmentSlope)(CollectionIndexType segmentIdx,Float64* pSlope);
   STDMETHOD(GetRidgePointOffset)(IndexType ridgePointIdx,Float64* pOffset);
   STDMETHOD(GetRidgePointElevation)(IndexType ridgePointIdx,Float64* pOffset,Float64* pElev);
   STDMETHOD(Clone)(ISurfaceProfile* *clone);
   STDMETHOD(get_StructuredStorage)(IStructuredStorage2* *pVal);

// IStructuredStorage2
public:
   STDMETHOD(Save)(IStructuredSave2* pSave);
   STDMETHOD(Load)(IStructuredLoad2* pLoad);

private:
   ISurface* m_pSurface; // weak reference
   CComPtr<IStation> m_Station;
   CComPtr<IDirection> m_Direction;
   std::vector<CComPtr<ISurfacePoint>> m_SurfacePoints;

   HRESULT ValidateStation(IStation* station);
   HRESULT SurfaceProfileError(UINT nHelpString,HRESULT hRes);
};
