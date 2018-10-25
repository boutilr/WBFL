///////////////////////////////////////////////////////////////////////
// COGOTest - Test Driver for Coordinate Geometry Library
// Copyright � 1999-2016  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This library is a part of the Washington Bridge Foundation Libraries
// and was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// PURPOSE.  See the Alternate Route Library Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// TestVertCurveCollection.cpp: implementation of the CTestVertCurveCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestVertCurveCollection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestVertCurveCollection::CTestVertCurveCollection()
{

}

void CTestVertCurveCollection::Test()
{
   CComPtr<IVertCurveCollection> pColl;
   TRY_TEST(pColl.CoCreateInstance( CLSID_VertCurveCollection ), S_OK);

   CollectionIndexType count;
   TRY_TEST(pColl->get_Count(NULL),E_POINTER);
   TRY_TEST(pColl->get_Count(&count),S_OK);
   TRY_TEST(count,0);

   CComPtr<IProfilePoint> pbg, pvi, pfg;
   pbg.CoCreateInstance(CLSID_ProfilePoint);
   pvi.CoCreateInstance(CLSID_ProfilePoint);
   pfg.CoCreateInstance(CLSID_ProfilePoint);

   pbg->put_Station(CComVariant(4850));
   pbg->put_Elevation(436.34);

   pvi->put_Station(CComVariant(5250));
   pvi->put_Elevation(432.34);

   pfg->put_Station(CComVariant(5650));
   pfg->put_Elevation(441.14);

   CComPtr<IVertCurveFactory> factory;
   TRY_TEST(pColl->get_Factory(NULL),E_POINTER);
   TRY_TEST(pColl->get_Factory(&factory),S_OK);
   TRY_TEST( factory != NULL, true );
   TRY_TEST(pColl->putref_Factory(NULL),E_INVALIDARG);
   TRY_TEST(pColl->putref_Factory(factory),S_OK);

   //
   // Test Add method
   //
   TRY_TEST(pColl->Add(1,NULL,pvi,pfg,400,400,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,NULL,pfg,400,400,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,pvi,NULL,400,400,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,pvi,pfg,-1,400,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,pvi,pfg,0,400,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,pvi,pfg,400,-1,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,pvi,pfg,400,0,NULL),E_INVALIDARG);
   TRY_TEST(pColl->Add(1,pbg,pvi,pfg,400,400,NULL),S_OK);
   CComPtr<IVertCurve> vc;
   TRY_TEST(pColl->get_Item(1,&vc),S_OK);
   TRY_TEST(pColl->get_Count(&count),S_OK);
   TRY_TEST(count,1);

   //
   // Test AddEx method
   //
   TRY_TEST(pColl->AddEx(0,NULL),E_INVALIDARG);
   TRY_TEST(pColl->AddEx(1,vc),COGO_E_VERTCURVEALREADYDEFINED);

   // Test Item property
   vc.Release();
   TRY_TEST(pColl->get_Item(-1,&vc),COGO_E_VERTCURVENOTFOUND);
   TRY_TEST(pColl->get_Item(1,NULL),E_POINTER);
   TRY_TEST(pColl->get_Item(1,&vc),S_OK);

   //
   // Test Remove
   //
   TRY_TEST(pColl->Remove(-1),COGO_E_VERTCURVENOTFOUND);
   TRY_TEST(pColl->Remove(1),S_OK);

   //
   // Test Clear
   //
   pColl->Add(1,pbg,pvi,pfg,400,400,NULL);
   pColl->get_Count(&count);
   TRY_TEST(count,1);
   TRY_TEST( pColl->Clear(), S_OK );
   pColl->get_Count(&count);
   TRY_TEST(count,0);

   //
   // Test putref_Item
   //
   TRY_TEST(pColl->AddEx(1,vc),S_OK);

   TRY_TEST(pColl->putref_Item(1,NULL),E_INVALIDARG);
   TRY_TEST(pColl->putref_Item(-1,vc),COGO_E_VERTCURVENOTFOUND);
   TRY_TEST(pColl->putref_Item(1,vc),S_OK);

   //
   // Test FindKey
   //
   CogoElementKey key;
   TRY_TEST(pColl->FindKey(NULL,&key),E_INVALIDARG);
   TRY_TEST(pColl->FindKey(vc,NULL),E_POINTER);
   TRY_TEST(pColl->FindKey(vc,&key),S_OK);
   TRY_TEST(key,1);

   //
   // Test Key
   //
   // load the collection with the same curve, it doesn't really matter
   pColl->Clear();
   pColl->AddEx(1,vc);
   pColl->AddEx(2,vc);
   pColl->AddEx(3,vc);
   pColl->AddEx(4,vc);

   TRY_TEST(pColl->Key(-1,&key),E_INVALIDARG);
   TRY_TEST(pColl->Key(500,&key),E_INVALIDARG);
   TRY_TEST(pColl->Key(3,NULL),E_POINTER);
   TRY_TEST(pColl->Key(3,&key),S_OK);
   TRY_TEST(key,4);
   
   //
   // Test _EnumKeys
   //
   pColl->Clear();
   TRY_TEST(pColl->AddEx(1,vc),S_OK);
   TRY_TEST(pColl->AddEx(2,vc),S_OK);
   TRY_TEST(pColl->AddEx(3,vc),S_OK);
   TRY_TEST(pColl->AddEx(4,vc),S_OK);
   CComPtr<IEnumKeys> pEnum;
   TRY_TEST(pColl->get__EnumKeys(NULL), E_POINTER );
   TRY_TEST( pColl->get__EnumKeys(&pEnum), S_OK );

   ULONG fetched;
   CogoElementKey target_key = 1;
   while( pEnum->Next(1,&key,&fetched ) == S_OK )
   {
      TRY_TEST(key,target_key++);
   }

   // Test __EnumVertCurves
   //
   pColl->Clear();
   TRY_TEST(pColl->AddEx(1,vc),S_OK);
   TRY_TEST(pColl->AddEx(2,vc),S_OK);
   TRY_TEST(pColl->AddEx(3,vc),S_OK);
   TRY_TEST(pColl->AddEx(4,vc),S_OK);
   CComPtr<IEnumVertCurves> pEnumVC;
   TRY_TEST(pColl->get__EnumVertCurves(NULL), E_POINTER );
   TRY_TEST( pColl->get__EnumVertCurves(&pEnumVC), S_OK );

   CComPtr<IVertCurve> vertCurve;
   while( pEnumVC->Next(1,&vertCurve,&fetched ) == S_OK )
   {
      TRY_TEST(vc.IsEqualObject(vertCurve),true);
      vertCurve.Release();
   }

   //
   // Test Events
   //
   pColl->Clear(); // start with an empty container

   CComObject<CTestVertCurveCollection>* pTestEvents;
   CComObject<CTestVertCurveCollection>::CreateInstance(&pTestEvents);
   pTestEvents->AddRef();

   DWORD dwCookie;
   CComPtr<IUnknown> punk(pTestEvents);
   TRY_TEST(AtlAdvise(pColl,punk,IID_IVertCurveCollectionEvents,&dwCookie),S_OK);

   // Add a VertCurve to the collection
   pTestEvents->InitEventTest(1);
   pColl->AddEx(1,vc);
   TRY_TEST(pTestEvents->PassedEventTest(),true);

   // Move a VertCurve... Event should fire
   pTestEvents->InitEventTest(1);
   pbg->put_Station(CComVariant(150));
   TRY_TEST(pTestEvents->PassedEventTest(),true);

   pTestEvents->InitEventTest(1);
   pbg->put_Elevation(150);
   TRY_TEST(pTestEvents->PassedEventTest(),true);

   pColl->AddEx(2,vc);
   pColl->AddEx(3,vc);
   pColl->AddEx(4,vc);

   // Remove a VertCurve
   pTestEvents->InitEventTest(1);
   pColl->Remove(1);
   TRY_TEST(pTestEvents->PassedEventTest(),true);

   // Change VertCurve references
   pTestEvents->InitEventTest(2);
   pColl->putref_Item(2,vc);
   TRY_TEST(pTestEvents->PassedEventTest(),true);

   // Clear
   pTestEvents->InitEventTest(-1);
   pColl->Clear();
   TRY_TEST(pTestEvents->PassedEventTest(),true);

   TRY_TEST(AtlUnadvise(pColl,IID_IVertCurveCollectionEvents,dwCookie),S_OK);
   pTestEvents->Release();

   //
   // Test ISupportErrorInfo
   //
   CComQIPtr<ISupportErrorInfo> eInfo(pColl);
   TRY_TEST( eInfo->InterfaceSupportsErrorInfo( IID_IVertCurveCollection ), S_OK );
   TRY_TEST( eInfo->InterfaceSupportsErrorInfo( IID_IStructuredStorage2 ), S_OK );
   TRY_TEST( eInfo->InterfaceSupportsErrorInfo( IID_ISupportErrorInfo ), S_FALSE );

   // Test IObjectSafety
   TRY_TEST( TestIObjectSafety(CLSID_VertCurveCollection,IID_IVertCurveCollection,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA), true);
   TRY_TEST( TestIObjectSafety(CLSID_VertCurveCollection,IID_IStructuredStorage2,INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA), true);
}

STDMETHODIMP CTestVertCurveCollection::OnVertCurveChanged(CogoElementKey key,IVertCurve* vc)
{
//   MessageBox(NULL,"VertCurveChanged","Event",MB_OK);
   if ( key == m_expectedKey )
      Pass();

   return S_OK;
}

STDMETHODIMP CTestVertCurveCollection::OnVertCurveAdded(CogoElementKey key,IVertCurve* vc)
{
//   MessageBox(NULL,"VertCurveAdded","Event",MB_OK);
   if ( key == m_expectedKey )
      Pass();

   return S_OK;
}

STDMETHODIMP CTestVertCurveCollection::OnVertCurveRemoved(CogoElementKey key)
{
//   MessageBox(NULL,"VertCurveRemoved","Event",MB_OK);
   if ( key == m_expectedKey )
      Pass();

   return S_OK;
}

STDMETHODIMP CTestVertCurveCollection::OnVertCurvesCleared()
{
//   MessageBox(NULL,"VertCurveCleared","Event",MB_OK);
   Pass();
   return S_OK;
}
