///////////////////////////////////////////////////////////////////////
// Geometry - Geometric Modeling Library
// Copyright � 1999-2024  Washington State Department of Transportation
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
#pragma once

// PCIDeckedIBeam.h : Declaration of the CPCIDeckedIBeam

#include "resource.h"       // main symbols
#include <GeomModel/PCIDeckedIBeam.h>
#include "IShapeImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CPCIDeckedIBeam
class ATL_NO_VTABLE CPCIDeckedIBeam : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPCIDeckedIBeam, &CLSID_PCIDeckedIBeam>,
   public ISupportErrorInfo,
   public IObjectSafetyImpl<CPCIDeckedIBeam,INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IPCIDeckedIBeam,
	public IShapeXYPositionImpl<CPCIDeckedIBeam, WBFL::Geometry::PCIDeckedIBeam>
{
public:
	CPCIDeckedIBeam()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

	void SetShape(const WBFL::Geometry::PCIDeckedIBeam& shape);
	virtual WBFL::Geometry::PCIDeckedIBeam& GetShape() override { return m_Beam; }

DECLARE_REGISTRY_RESOURCEID(IDR_PCIDECKEDIBEAM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPCIDeckedIBeam)
	COM_INTERFACE_ENTRY(IPCIDeckedIBeam)
   COM_INTERFACE_ENTRY(IShape)
   COM_INTERFACE_ENTRY(IXYPosition)
   COM_INTERFACE_ENTRY(ISupportErrorInfo)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

private:
	WBFL::Geometry::PCIDeckedIBeam m_Beam;
	CComPtr<IPoint2d> m_HookPoint;

// ISupportErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) override;

// IPCIDeckedIBeam
public:
	STDMETHOD(get_XYPosition)(/*[out, retval]*/ IXYPosition* *pVal) override;
	STDMETHOD(get_Shape)(/*[out, retval]*/ IShape* *pVal) override;
	STDMETHOD(GetBottomFlangePoints)(/*[out]*/IPoint2d** ppLeftTop, /*[out]*/IPoint2d** ppLeftBottom, /*[out]*/IPoint2d** ppRightTop, /*[out]*/IPoint2d** ppRightBottom);
	STDMETHOD(GetTopFlangePoints)(/*[out]*/IPoint2d** ppLeftTop, /*[out]*/IPoint2d** ppLeftBottom, /*[out]*/IPoint2d** ppTopCL, /*[out]*/IPoint2d** ppTopCentral,/*[out]*/IPoint2d** ppRightTop, /*[out]*/IPoint2d** ppRightBottom);
	STDMETHOD(GetTopFlangeThickness)(/*[out]*/Float64* pHl, /*[out]*/Float64* pHc, /*[out]*/Float64* pHr);
	STDMETHOD(get_TopFlangeWidth)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(get_BottomFlangeWidth)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(get_AvgWebWidth)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(get_Height)(/*[out, retval]*/ Float64 *pVal) override;
   STDMETHOD(get_HookPoint)(/*[out,retval]*/ IPoint2d** hookPnt) override;
   STDMETHOD(putref_HookPoint)(/*[in]*/ IPoint2d* hookPnt) override;
	STDMETHOD(get_R3)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_R3)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_R2)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_R2)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_R1)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_R1)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_T)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_T)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_H)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_H)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D9)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_D9)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D8)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_D8)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D7)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_D7)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D6)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_D6)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D5)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_D5)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D4)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_D4)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D3)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_D3)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D2)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_D2)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_D1)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_D1)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_W6)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_W6)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_W5)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_W5)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_W4)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_W4)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_W3)(/*[out, retval]*/ Float64* pVal) override;
	STDMETHOD(put_W3)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_W2)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_W2)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_W1)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_W1)(/*[in]*/ Float64 newVal) override;
	STDMETHOD(get_C1)(/*[out, retval]*/ Float64 *pVal) override;
	STDMETHOD(put_C1)(/*[in]*/ Float64 newVal) override;
};
