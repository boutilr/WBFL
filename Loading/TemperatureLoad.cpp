///////////////////////////////////////////////////////////////////////
// Loading - Generic library to describe bridge loadings
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

// TemperatureLoad.cpp : Implementation of CTemperatureLoad
#include "stdafx.h"
#include "WBFLLoading.h"
#include "TemperatureLoad.h"
#include "LoadingUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTemperatureLoad

STDMETHODIMP CTemperatureLoad::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ldITemperatureLoad
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CTemperatureLoad::get_MemberID(long *pVal)
{
   CHECK_RETVAL(pVal);
	*pVal = m_MemberID;

	return S_OK;
}

STDMETHODIMP CTemperatureLoad::put_MemberID(long newVal)
{
	if (newVal<0)
      return E_INVALIDARG;

   if (newVal!=m_MemberID)
   {
      m_MemberID = newVal;
      Fire_OnTemperatureLoadChanged(this);
   }

	return S_OK;
}

STDMETHODIMP CTemperatureLoad::get_TTop(Float64 *pVal)
{
	CHECK_RETVAL(pVal);

   *pVal = m_TTop;

	return S_OK;
}

STDMETHODIMP CTemperatureLoad::put_TTop(Float64 newVal)
{
   if (newVal!=m_TTop)
   {
      m_TTop = newVal;
      Fire_OnTemperatureLoadChanged(this);
   }

	return S_OK;
}

STDMETHODIMP CTemperatureLoad::get_TBottom(Float64 *pVal)
{
	CHECK_RETVAL(pVal);

   *pVal = m_TBottom;


	return S_OK;
}

STDMETHODIMP CTemperatureLoad::put_TBottom(Float64 newVal)
{
   if (newVal!=m_TBottom)
   {
      m_TBottom = newVal;
      Fire_OnTemperatureLoadChanged(this);
   }

	return S_OK;
}

STDMETHODIMP CTemperatureLoad::Clone(ldITemperatureLoad **clone)
{
   CHECK_RETOBJ(clone);

   // create a new and fill it up
   CComObject<CTemperatureLoad>* pnew;
   HRESULT hr = CComObject<CTemperatureLoad>::CreateInstance(&pnew);
   if (FAILED(hr))
      return hr;

   CComPtr<ldITemperatureLoad> pscs(pnew); // for reference count

   pnew->m_MemberID    = m_MemberID;

   pnew->m_TTop = m_TTop;
   pnew->m_TBottom = m_TBottom;

   return pscs.CopyTo(clone);
}

// IStructuredStorage2
static const Float64 MY_VER=1.0;
STDMETHODIMP CTemperatureLoad::Load(IStructuredLoad2 * pload)
{
   CHECK_IN(pload);

   HRESULT hr;

   hr = pload->BeginUnit(CComBSTR("TemperatureLoad"));
   if (FAILED(hr))
      return hr;

   Float64 ver;
   hr = pload->get_Version(&ver);
   if (FAILED(hr))
      return hr;

   if (ver!=MY_VER)
      return STRLOAD_E_BADVERSION;

   {        
      CComVariant var;

      hr = pload->get_Property(CComBSTR("MemberID"),&var);
      if (FAILED(hr))
         return hr;

      m_MemberID = var.lVal;
      var.Clear();

      hr = pload->get_Property(CComBSTR("TTop"),&var);
      if (FAILED(hr))
         return hr;

      m_TTop = var.dblVal;
      var.Clear();

      hr = pload->get_Property(CComBSTR("TBottom"),&var);
      if (FAILED(hr))
         return hr;

      m_TBottom = var.dblVal;
      var.Clear();
   }

   VARIANT_BOOL eb;
   hr = pload->EndUnit(&eb);
   if (FAILED(hr))
      return hr;

   if (eb!=VARIANT_TRUE)
      return STRLOAD_E_INVALIDFORMAT;

   return S_OK;
}

STDMETHODIMP CTemperatureLoad::Save(IStructuredSave2 * psave)
{
   CHECK_IN(psave);

   HRESULT hr;
   hr = psave->BeginUnit(CComBSTR("TemperatureLoad"), MY_VER);
   if (FAILED(hr))
      return hr;

   {
      hr = psave->put_Property(CComBSTR("MemberID"),CComVariant(m_MemberID));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("TTop"),CComVariant(m_TTop));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("TBottom"),CComVariant(m_TBottom));
      if (FAILED(hr))
         return hr;
   }

   hr = psave->EndUnit();
   return hr;
}
