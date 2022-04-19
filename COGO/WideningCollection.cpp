///////////////////////////////////////////////////////////////////////
// COGO - Coordinate Geometry Library
// Copyright � 1999-2022  Washington State Department of Transportation
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

// WideningCollection.cpp : Implementation of CWideningCollection
#include "stdafx.h"
#include "WBFLCOGO.h"
#include "WideningCollection.h"
#include "Widening.h"
#include <WBFLCogo\CogoHelpers.h>

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class SortWidenings
{
public:
   SortWidenings(IProfile* pProfile) { m_pProfile = pProfile; }
   bool operator()(CComVariant& pX, CComVariant& pY)
   {
      CComVariant& varX = pX;
      CComVariant& varY = pY;
      CComPtr<IStation> staX, staY;
      
      CComQIPtr<IWidening> csX(varX.pdispVal);
      CComQIPtr<IWidening> csY(varY.pdispVal);

      csX->get_BeginTransition(&staX);
      csY->get_BeginTransition(&staY);
      return 0 < cogoUtil::Compare(m_pProfile,staX,staY);
   }
private:
   IProfile* m_pProfile; // weak reference
};

/////////////////////////////////////////////////////////////////////////////
// CWideningCollection
HRESULT CWideningCollection::FinalConstruct()
{
   m_pSurface = nullptr;
   return S_OK;
}

void CWideningCollection::FinalRelease()
{
}

STDMETHODIMP CWideningCollection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IWideningCollection,
      &IID_IStructuredStorage2,
	};
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CWideningCollection::get_Surface(ISurface* *pVal)
{
   CHECK_RETOBJ(pVal);
   if ( m_pSurface )
   {
      (*pVal) = m_pSurface;
      (*pVal)->AddRef();
   }

   return S_OK;
}

STDMETHODIMP CWideningCollection::putref_Surface(ISurface* pSurface)
{
   m_pSurface = pSurface;

   CComPtr<IEnumWidenings> enumWidenings;
   get__EnumWidenings(&enumWidenings);

   CComPtr<IWidening> widening;
   while ( enumWidenings->Next(1,&widening,nullptr) != S_FALSE )
   {
      widening->putref_Surface(m_pSurface);
      widening.Release();
   };

   return S_OK;
}

//STDMETHODIMP CWideningCollection::get__NewEnum(IUnknown** retval)
//{
//}

STDMETHODIMP CWideningCollection::get_Item(CollectionIndexType idx, IWidening* *pVal)
{
   CHECK_RETOBJ(pVal);

   if ( !IsValidIndex(idx,m_coll) )
      return E_INVALIDARG;

   CComVariant& p = m_coll[idx];
   CComVariant& varItem = p;
   varItem.pdispVal->QueryInterface(pVal);
   return S_OK;
}

STDMETHODIMP CWideningCollection::putref_Item(CollectionIndexType idx,IWidening* pVal)
{
   CHECK_IN(pVal);

   HRESULT hr = S_OK;

   if ( !IsValidIndex(idx,m_coll) )
      return E_INVALIDARG;

   //hr = ValidateStation(pVal);
   //if ( FAILED(hr) )
   //   return hr;

   // Get the item
   CComVariant& cst = m_coll[idx];
   CComVariant& var = cst; // Variant holding IDispatch to Widening
   pVal->putref_Surface(m_pSurface);

   var = pVal; // Associate new Widening with this variant

   return S_OK;
}

STDMETHODIMP CWideningCollection::GetWidening(VARIANT varStation,IWidening** widening)
{
   // searches for the Widening that contains the specified station
   CHECK_RETOBJ(widening);
   *widening = nullptr;

   CComPtr<IStation> station;
   HRESULT hr = cogoUtil::StationFromVariant(varStation,false,&station);
   if ( FAILED(hr) )
      return hr;

   CComPtr<IProfile> profile;
   if ( m_pSurface )
   {
      m_pSurface->get_Profile(&profile); 
   }

   auto iter(m_coll.begin());
   auto end(m_coll.end());
   for ( ; iter != end; iter++ )
   {
      CComQIPtr<IWidening> thisWidening(iter->pdispVal);

      CComPtr<IStation> startStation,endStation;
      thisWidening->get_BeginTransition(&startStation);
      thisWidening->get_EndTransition(&endStation);

      if ( 0 <= cogoUtil::Compare(profile,startStation,station) &&
           0 <= cogoUtil::Compare(profile,station,endStation) )
      {
         return thisWidening.CopyTo(widening);
      }
   }

   return S_FALSE;
}

STDMETHODIMP CWideningCollection::get_Count(CollectionIndexType *pVal)
{
   CHECK_RETVAL(pVal);
   *pVal = m_coll.size();
   return S_OK;
}

STDMETHODIMP CWideningCollection::AddEx(IWidening* widening)
{
   CHECK_IN(widening);

   HRESULT hr = S_OK;

   widening->putref_Surface(m_pSurface);

   m_coll.emplace_back(CComVariant(widening));

   CComPtr<IProfile> profile;
   if ( m_pSurface )
   {
      m_pSurface->get_Profile(&profile); 
   }

   std::sort(std::begin(m_coll),std::end(m_coll),SortWidenings(profile));

   return S_OK;
}

STDMETHODIMP CWideningCollection::Add(VARIANT varBeginStation,VARIANT varBeginFullStation,VARIANT varEndFullStation,VARIANT varEndStation,Float64 widening,IndexType pnt1,IndexType pnt2,IWidening* *pWidening)
{
   if ( pWidening != nullptr )
   {
      CHECK_RETOBJ(pWidening);
   }

   CComObject<CWidening>* pNewWidening;
   CComObject<CWidening>::CreateInstance(&pNewWidening);
   CComPtr<IWidening> newWidening;
   newWidening = pNewWidening;

   HRESULT hr = newWidening->Init(m_pSurface,varBeginStation,varBeginFullStation,varEndFullStation,varEndStation,widening,pnt1,pnt2);
   if ( FAILED(hr) )
      return hr;

   if ( pWidening != nullptr )
   {
      (*pWidening) = newWidening;
      (*pWidening)->AddRef();
   }

   return AddEx(newWidening);
}

STDMETHODIMP CWideningCollection::Remove(CollectionIndexType idx)
{
   if ( idx < 0 || m_coll.size() <= idx )
      return E_INVALIDARG;

   m_coll.erase(m_coll.begin() + idx );
   return S_OK;
}

STDMETHODIMP CWideningCollection::Clear()
{
   m_coll.clear();
   return S_OK;
}

STDMETHODIMP CWideningCollection::Clone(IWideningCollection* *clone)
{
   CHECK_RETOBJ(clone);

   CComObject<CWideningCollection>* pClone;
   CComObject<CWideningCollection>::CreateInstance(&pClone);

   (*clone) = pClone;
   (*clone)->AddRef();

   (*clone)->putref_Surface(m_pSurface);

   CComPtr<IEnumWidenings> enumWidenings;
   get__EnumWidenings(&enumWidenings);

   CComPtr<IWidening> widening;
   while ( enumWidenings->Next(1,&widening,nullptr) != S_FALSE )
   {
      CComPtr<IWidening> wideningClone;
      widening->Clone(&wideningClone);

      (*clone)->AddEx(wideningClone);

      widening.Release();
   };


   return S_OK;
}

STDMETHODIMP CWideningCollection::get_StructuredStorage(IStructuredStorage2* *pStg)
{
   CHECK_RETOBJ(pStg);
   return QueryInterface(IID_IStructuredStorage2,(void**)pStg);
}

STDMETHODIMP CWideningCollection::get__EnumWidenings(IEnumWidenings** retval)
{
   CHECK_RETOBJ(retval);

   typedef CComEnumOnSTL<IEnumWidenings,
                         &IID_IEnumWidenings, 
                         IWidening*,
                         _CopyVariantToInterface<IWidening>,
                         std::vector<CComVariant>> Enum;
   CComObject<Enum>* pEnum;
   HRESULT hr = CComObject<Enum>::CreateInstance(&pEnum);
   if ( FAILED(hr) )
      return hr;

   hr = pEnum->Init( nullptr, m_coll );
   if ( FAILED(hr) )
      return hr;

   pEnum->QueryInterface( retval );

   return S_OK;
}

// IStructuredStorage2
STDMETHODIMP CWideningCollection::Save(IStructuredSave2* pSave)
{
   CHECK_IN(pSave);

   pSave->BeginUnit(CComBSTR("Widenings"),1.0);
   CollectionIndexType count = m_coll.size();
   pSave->put_Property(CComBSTR("Count"),CComVariant(count));
   for ( CollectionIndexType i = 0; i < count; i++ )
   {
      pSave->put_Property(CComBSTR("Widening"),m_coll[i]);
   }

   return S_OK;
}

STDMETHODIMP CWideningCollection::Load(IStructuredLoad2* pLoad)
{
   CHECK_IN(pLoad);

   CComVariant var;
   pLoad->BeginUnit(CComBSTR("Widenings"));

   pLoad->get_Property(CComBSTR("Count"),&var);
   long count = var.lVal;

   for ( long i = 0; i < count; i++ )
   {
      pLoad->get_Property(CComBSTR("Widening"),&var);
      CComPtr<IWidening> widening;
      _CopyVariantToInterface<IWidening>::copy(&widening,&var);
      AddEx(widening);
   }

   VARIANT_BOOL bEnd;
   pLoad->EndUnit(&bEnd);

   return S_OK;
}

HRESULT CWideningCollection::OnBeforeSave(IStructuredSave2* pSave)
{
   return S_OK;
}

HRESULT CWideningCollection::OnBeforeLoad(IStructuredLoad2* pLoad)
{
   return S_OK;
}
//
//HRESULT CWideningCollection::ValidateStation(IWidening* csect)
//{
//   CComPtr<IStation> station;
//   csect->get_Station(&station);
//   CComPtr<IStation> sta;
//   return ValidateStation(CComVariant(station),false,&sta);
//}
//
//HRESULT CWideningCollection::ValidateStation(VARIANT varStation,bool bClone,IStation** station)
//{
//   HRESULT hr = cogoUtil::StationFromVariant(varStation,bClone,station);
//   if ( FAILED(hr) )
//      return hr;
//
//   if ( m_pProfile == nullptr )
//   {
//      ZoneIndexType staEqnZoneIdx;
//      (*station)->get_StationZoneIndex(&staEqnZoneIdx);
//      if ( staEqnZoneIdx != INVALID_INDEX )
//         return E_INVALIDARG; // station must be normalized
//   }
//
//   return S_OK;
//}