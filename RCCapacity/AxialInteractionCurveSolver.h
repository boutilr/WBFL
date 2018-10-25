///////////////////////////////////////////////////////////////////////
// RCCapacity - Reinforced Concrete Capacity Analysis Library
// Copyright � 2003  Washington State Department of Transportation
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

// AxialInteractionCurveSolver.h : Declaration of the CAxialInteractionCurveSolver

#ifndef __AxialInteractionCurveSolver_H_
#define __AxialInteractionCurveSolver_H_

#include "resource.h"       // main symbols
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CAxialInteractionCurveSolver
class ATL_NO_VTABLE CAxialInteractionCurveSolver : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAxialInteractionCurveSolver, &CLSID_AxialInteractionCurveSolver>,
	public ISupportErrorInfo,
	public IAxialInteractionCurveSolver
{
public:
	CAxialInteractionCurveSolver()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_AXIALINTERACTIONCURVESOLVER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAxialInteractionCurveSolver)
	COM_INTERFACE_ENTRY(IAxialInteractionCurveSolver)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

private:
   CComPtr<IMomentCapacitySolver> m_Solver;

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAxialInteractionCurveSolver
public:
	STDMETHOD(putref_Section)(/*[in]*/IGeneralSection* pSection);
	STDMETHOD(get_Section)(/*[out,retval]*/IGeneralSection** pSection);
	STDMETHOD(put_Slices)(/*[in]*/long nSlices);
	STDMETHOD(get_Slices)(/*[out,retval]*/long* nSlices);
	STDMETHOD(put_AxialTolerance)(/*[in]*/Float64 tolerance);
	STDMETHOD(get_AxialTolerance)(/*[out,retval]*/Float64* tolerance);
	STDMETHOD(put_MaxIterations)(/*[in]*/long maxIter);
	STDMETHOD(get_MaxIterations)(/*[out,retval]*/long* maxIter);
   STDMETHOD(CompressionLimit)(/*[out]*/Float64* Fz,/*[out]*/Float64* Mx,/*[out]*/Float64* My,/*[out]*/Float64* eo);
   STDMETHOD(TensionLimit)(/*[out]*/Float64* Fz,/*[out]*/Float64* Mx,/*[out]*/Float64* My,/*[out]*/Float64* eo);
	STDMETHOD(Solve)(/*[in]*/Float64 na,/*[in]*/long nFzSteps,/*[out,retval]*/IInteractionCapacityCurveSolution** solution);
};

#endif //__AxialInteractionCurveSolver_H_
