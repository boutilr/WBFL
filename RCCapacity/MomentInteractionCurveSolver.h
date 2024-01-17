///////////////////////////////////////////////////////////////////////
// RCCapacity - Reinforced Concrete Capacity Analysis Library
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

// MomentInteractionCurveSolver.h : Declaration of the CMomentInteractionCurveSolver

#ifndef __MomentInteractionCurveSolver_H_
#define __MomentInteractionCurveSolver_H_

#include "resource.h"       // main symbols
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CMomentInteractionCurveSolver
class ATL_NO_VTABLE CMomentInteractionCurveSolver : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMomentInteractionCurveSolver, &CLSID_MomentInteractionCurveSolver>,
	public ISupportErrorInfo,
	public IMomentInteractionCurveSolver
{
public:
	CMomentInteractionCurveSolver()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_MOMENTINTERACTIONCURVESOLVER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMomentInteractionCurveSolver)
	COM_INTERFACE_ENTRY(IMomentInteractionCurveSolver)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

private:
   CComPtr<IMomentCapacitySolver> m_Solver;

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) override;

// IMomentInteractionCurveSolver
public:
	STDMETHOD(putref_Section)(/*[in]*/IGeneralSection* pSection) override;
	STDMETHOD(get_Section)(/*[out,retval]*/IGeneralSection** pSection) override;
	STDMETHOD(put_Slices)(/*[in]*/long nSlices) override;
	STDMETHOD(get_Slices)(/*[out,retval]*/long* nSlices) override;
	STDMETHOD(put_AxialTolerance)(/*[in]*/Float64 tolerance) override;
	STDMETHOD(get_AxialTolerance)(/*[out,retval]*/Float64* tolerance) override;
	STDMETHOD(put_MaxIterations)(/*[in]*/long maxIter) override;
	STDMETHOD(get_MaxIterations)(/*[out,retval]*/long* maxIter) override;
   STDMETHOD(CompressionLimit)(/*[out]*/Float64* Fz,/*[out]*/Float64* Mx,/*[out]*/Float64* My,/*[out]*/Float64* eo) override;
   STDMETHOD(TensionLimit)(/*[out]*/Float64* Fz,/*[out]*/Float64* Mx,/*[out]*/Float64* My,/*[out]*/Float64* eo) override;
   STDMETHOD(Solve)(/*[in]*/Float64 Fz,/*[in]*/Float64 startNA,/*[in]*/Float64 endNA,/*[in]*/long nSteps,/*[out,retval]*/IInteractionCapacityCurveSolution** solution) override;
};

#endif //__MomentInteractionCurveSolver_H_
