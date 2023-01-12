///////////////////////////////////////////////////////////////////////
// Geometry - Geometric Modeling Library
// Copyright � 1999-2023  Washington State Department of Transportation
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

// GenericShape.h : Declaration of the CGenericShape

#pragma once

#include "resource.h"       // main symbols
#include "GeometryCP.h"
#include <WBFLTools.h> // IMohrCircle

/////////////////////////////////////////////////////////////////////////////
// CGenericShape
class ATL_NO_VTABLE CGenericShape : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGenericShape, &CLSID_GenericShape>,
   public ISupportErrorInfo,
   public IObjectSafetyImpl<CGenericShape,INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IGenericShape,
	public IShape,
   public IXYPosition,
   public IStructuredStorage2,
   public IPersist,
   public CProxyDPoint2dEvents< CGenericShape >,
   public IConnectionPointContainerImpl<CGenericShape>,
   public IPoint2dEvents
{
public:
	CGenericShape()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_GENERICSHAPE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGenericShape)
	COM_INTERFACE_ENTRY(IGenericShape)
	COM_INTERFACE_ENTRY(IStructuredStorage2)
   COM_INTERFACE_ENTRY(IShape)
   COM_INTERFACE_ENTRY(IXYPosition)
   COM_INTERFACE_ENTRY(ISupportErrorInfo)
   COM_INTERFACE_ENTRY(IObjectSafety)
   COM_INTERFACE_ENTRY(IPersist)
   COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IPoint2dEvents)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CGenericShape)
	CONNECTION_POINT_ENTRY(IID_IPoint2dEvents)
END_CONNECTION_POINT_MAP()

private:
	CComPtr<IMohrCircle> m_MohrCircle;
   CComPtr<IGeomUtil2d> m_GeomUtil;
   CComPtr<IPoint2d> m_pCG; // CG of the shape... also the hook point
   unsigned long  m_cgCookie;
   Float64 m_Area;
   Float64 m_Ixx;
   Float64 m_Iyy;
   Float64 m_Ixy;
   Float64 m_Perimeter;
   Float64 m_Rotation;
   Float64   m_Xleft;   // Distance from centroid to left edge
   Float64   m_Xright;  // Distance from centroid to right edge
   Float64   m_Ytop;    // Distance from centroid to top edge
   Float64   m_Ybottom; // Distance from centroid to bottom edge

// ISupportErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) override;

// IGenericShape
public:
   STDMETHOD(get_Area)(Float64 *pVal) override;
   STDMETHOD(put_Area)(Float64 newVal) override;
   STDMETHOD(put_Perimeter)(Float64 newVal) override;
   STDMETHOD(get_Centroid)(IPoint2d* *pVal) override;
   STDMETHOD(putref_Centroid)(IPoint2d* newVal) override;
	STDMETHOD(get_Ybottom)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_Ybottom)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_Ytop)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_Ytop)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_Xright)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_Xright)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_Xleft)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_Xleft)(/*[in]*/ Float64 newVal) override;
   STDMETHOD(get_Ixx)(Float64 *pVal) override;
   STDMETHOD(put_Ixx)(Float64 newVal) override;
   STDMETHOD(get_Iyy)(Float64 *pVal) override;
   STDMETHOD(put_Iyy)(Float64 newVal) override;
   STDMETHOD(get_Ixy)(Float64 *pVal) override;
   STDMETHOD(put_Ixy)(Float64 newVal) override;
	STDMETHOD(get_Shape)(IShape* *pVal) override;
	STDMETHOD(get_XYPosition)(IXYPosition* *pVal) override;
   STDMETHOD(get_StructuredStorage)(IStructuredStorage2* *pStrStg) override;

// IShape
public:
	STDMETHOD(FurthestDistance)(/*[in]*/ILine2d* line,/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(get_Perimeter)(/*[out, retval]*/ Float64 *pVal) override;
   STDMETHOD(get_ShapeProperties)(/*[out,retval]*/ IShapeProperties* *pVal) override;
	STDMETHOD(get_BoundingBox)(/*[out, retval]*/ IRect2d* *pVal) override;
	STDMETHOD(get_PolyPoints)(/*[out,retval]*/ IPoint2dCollection** ppPolyPoints) override;
	STDMETHOD(PointInShape)(/*[in]*/ IPoint2d* pPoint,/*[out,retval]*/ VARIANT_BOOL* pbResult) override;
	STDMETHOD(Clone)(/*[out,retval]*/ IShape** pClone) override;
	STDMETHOD(ClipWithLine)(/*[in]*/ ILine2d* pLine,/*[out,retval]*/ IShape** pShape) override;
	STDMETHOD(ClipIn)(/*[in]*/ IRect2d* pRect,/*[out,retval]*/ IShape** pShape) override;

// IXYPosition
public:
	STDMETHOD(Offset)(/*[in]*/ Float64 dx,/*[in]*/ Float64 dy) override;
	STDMETHOD(OffsetEx)(/*[in]*/ ISize2d* pSize) override;
	STDMETHOD(get_LocatorPoint)(/*[in]*/ LocatorPointType lp, /*[out,retval]*/ IPoint2d** point) override;
	STDMETHOD(put_LocatorPoint)(/*[in]*/ LocatorPointType lp, /*[in]*/ IPoint2d* point) override;
	STDMETHOD(MoveEx)(/*[in]*/ IPoint2d* pFrom,/*[in]*/ IPoint2d* pTo) override;
	STDMETHOD(RotateEx)(/*[in]*/ IPoint2d* pPoint,/*[in]*/ Float64 angle) override;
	STDMETHOD(Rotate)(/*[in]*/ Float64 cx,/*[in]*/ Float64 cy,/*[in]*/ Float64 angle) override;

// IPersist
public:
   STDMETHOD(GetClassID)(CLSID* pClassID) override;

// IStructuredStorage2
public:
   STDMETHOD(Save)(IStructuredSave2* pSave) override;
   STDMETHOD(Load)(IStructuredLoad2* pLoad) override;

// IPoint2dEvents
public:
	STDMETHOD(OnPointChanged)(IPoint2d* point) override;
};
