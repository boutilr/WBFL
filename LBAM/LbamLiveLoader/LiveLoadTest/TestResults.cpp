///////////////////////////////////////////////////////////////////////
// LBAM Live Load Test - Test driver for LBAM analysis library
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

// TestResults.cpp: implementation of the TestResults class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestResults.h"

#include <string>
#include <fstream>
#include <iomanip>

#include <MathEx.h>
#include "..\..\LBAMTestUtils.h"
#include "LiveLoadTestUtils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TestResults::TestResults()
{
}

TestResults::~TestResults()
{
}

void TestResults::Test()
{
   TestLiveLoadModelResults();
   TestLiveLoadModelSectionResults();
   TestLiveLoadModelStressResults();
}

void TestResults::TestLiveLoadModelResults()
{
   CComPtr<ILiveLoadModelResults> presult;
   TRY_TEST(presult.CoCreateInstance( CLSID_LiveLoadModelResults ), S_OK );

   CComPtr<ILiveLoadConfiguration> config;
   TRY_TEST(config.CoCreateInstance( CLSID_LiveLoadConfiguration), S_OK );

   TRY_TEST(presult->Reserve(2), S_OK);
   TRY_TEST(presult->Add(1.2, config), S_OK);
   CollectionIndexType cnt;
   TRY_TEST(presult->get_Count(&cnt), S_OK);
   TRY_TEST(cnt, 1);

   TRY_TEST(presult->Add(1.3, nullptr), S_OK);
   TRY_TEST(presult->get_Count(&cnt), S_OK);
   TRY_TEST(cnt, 2);


   CComPtr<ILiveLoadConfiguration> cfg2,cfg3;
   Float64 val;
   TRY_TEST(presult->GetResult(0, &val, &cfg2), S_OK);
   TRY_TEST(val, 1.2);
   TRY_TEST(cfg2.IsEqualObject(config), true);

   TRY_TEST(presult->GetResult(1, &val, &cfg3), S_OK);
   TRY_TEST(val, 1.3);
   TRY_TEST(cfg3, nullptr);

   TRY_TEST(presult->SetResult(0, 1.4, nullptr), S_OK);
   cfg2.Release();
   TRY_TEST(presult->GetResult(0, &val, &cfg2), S_OK);
   TRY_TEST(val, 1.4);
   TRY_TEST(cfg2,nullptr);

   TRY_TEST(presult->SetResult(2, 1.4, nullptr), E_INVALIDARG);
   TRY_TEST(presult->GetResult(2, &val, &cfg2), E_INVALIDARG);

   TestPersistance(presult.p, CLSID_LiveLoadModelResults);
}

void TestResults::TestLiveLoadModelSectionResults()
{
   CComPtr<ILiveLoadModelSectionResults> presult;
   TRY_TEST(presult.CoCreateInstance( CLSID_LiveLoadModelSectionResults ), S_OK );

   CComPtr<ILiveLoadConfiguration> config1, config2;
   TRY_TEST(config1.CoCreateInstance( CLSID_LiveLoadConfiguration), S_OK );
   TRY_TEST(config2.CoCreateInstance( CLSID_LiveLoadConfiguration), S_OK );

   TRY_TEST(presult->Reserve(2), S_OK);
   TRY_TEST(presult->Add(1.2, config1, 2.2, config2), S_OK);
   CollectionIndexType cnt;
   TRY_TEST(presult->get_Count(&cnt), S_OK);
   TRY_TEST(cnt, 1);

   TRY_TEST(presult->Add(1.3, nullptr, 2.3, nullptr), S_OK);
   TRY_TEST(presult->get_Count(&cnt), S_OK);
   TRY_TEST(cnt, 2);

   CComPtr<ILiveLoadConfiguration> cfg1,cfg2;
   Float64 val1, val2;
   TRY_TEST(presult->GetResult(0, &val1, &cfg1, &val2, &cfg2), S_OK);
   TRY_TEST(val1, 1.2);
   TRY_TEST(val2, 2.2);
   TRY_TEST(cfg1.IsEqualObject(config1), true);
   TRY_TEST(cfg2.IsEqualObject(config2), true);

   cfg1.Release();
   cfg2.Release();
   TRY_TEST(presult->GetResult(1, &val1, &cfg1, &val2, &cfg2), S_OK);
   TRY_TEST(val1, 1.3);
   TRY_TEST(val2, 2.3);
   TRY_TEST(cfg1, nullptr);
   TRY_TEST(cfg2, nullptr);

   TRY_TEST(presult->SetResult(0, 1.4, nullptr, 2.4, nullptr), S_OK);
   cfg1.Release();
   cfg2.Release();
   TRY_TEST(presult->GetResult(0, &val1, &cfg1, &val2, &cfg2), S_OK);
   TRY_TEST(val1, 1.4);
   TRY_TEST(val2, 2.4);
   TRY_TEST(cfg1, nullptr);
   TRY_TEST(cfg2, nullptr);

   TRY_TEST(presult->SetResult(2, 1.4, nullptr, 2.4, nullptr), E_INVALIDARG);
   cfg1.Release();
   cfg2.Release();
   TRY_TEST(presult->GetResult(2, &val1, &cfg1, &val2, &cfg2), E_INVALIDARG);

   TestPersistance(presult.p, CLSID_LiveLoadModelSectionResults);
}

void TestResults::TestLiveLoadModelStressResults()
{
   CComPtr<ILiveLoadModelStressResults> presult;
   TRY_TEST(presult.CoCreateInstance( CLSID_LiveLoadModelStressResults ), S_OK );

   CComPtr<ILiveLoadConfiguration> config1, config2;
   TRY_TEST(config1.CoCreateInstance( CLSID_LiveLoadConfiguration), S_OK );
   TRY_TEST(config2.CoCreateInstance( CLSID_LiveLoadConfiguration), S_OK );

   CComPtr<IStressResult> stressr1, stressr2, stressr3, stressr4;
   TRY_TEST(stressr1.CoCreateInstance(CLSID_StressResult), S_OK );
   TRY_TEST(stressr2.CoCreateInstance(CLSID_StressResult), S_OK );
   TRY_TEST(stressr3.CoCreateInstance(CLSID_StressResult), S_OK );
   TRY_TEST(stressr4.CoCreateInstance(CLSID_StressResult), S_OK );

   TRY_TEST(presult->Reserve(2), S_OK);
   TRY_TEST(presult->Add(stressr1, config1, stressr2, config2), S_OK);
   CollectionIndexType cnt;
   TRY_TEST(presult->get_Count(&cnt), S_OK);
   TRY_TEST(cnt, 1);

   TRY_TEST(presult->Add(stressr3, nullptr, stressr4, nullptr), S_OK);
   TRY_TEST(presult->get_Count(&cnt), S_OK);
   TRY_TEST(cnt, 2);

   CComPtr<IStressResult> sr1, sr2;
   CComPtr<ILiveLoadConfiguration> cfg1,cfg2;
   TRY_TEST(presult->GetResult(0, &sr1, &cfg1, &sr2, &cfg2), S_OK);
   TRY_TEST(sr1.IsEqualObject(stressr1), true);
   TRY_TEST(sr2.IsEqualObject(stressr2), true);
   TRY_TEST(cfg1.IsEqualObject(config1), true);
   TRY_TEST(cfg2.IsEqualObject(config2), true);

   sr1.Release();
   sr2.Release();
   cfg1.Release();
   cfg2.Release();
   TRY_TEST(presult->GetResult(1, &sr1, &cfg1, &sr2, &cfg2), S_OK);
   TRY_TEST(sr1.IsEqualObject(stressr3), true);
   TRY_TEST(sr2.IsEqualObject(stressr4), true);
   TRY_TEST(cfg1, nullptr);
   TRY_TEST(cfg2, nullptr);

   TRY_TEST(presult->SetResult(0, stressr3, nullptr, stressr4, nullptr), S_OK);
   sr1.Release();
   sr2.Release();
   cfg1.Release();
   cfg2.Release();
   TRY_TEST(presult->GetResult(0, &sr1, &cfg1, &sr2, &cfg2), S_OK);
   TRY_TEST(sr1.IsEqualObject(stressr3), true);
   TRY_TEST(sr2.IsEqualObject(stressr4), true);
   TRY_TEST(cfg1, nullptr);
   TRY_TEST(cfg2, nullptr);

   // out of bounds
   TRY_TEST(presult->SetResult(2, stressr3, nullptr, stressr4, nullptr), E_INVALIDARG);
   sr1.Release();
   sr2.Release();
   cfg1.Release();
   cfg2.Release();
   TRY_TEST(presult->GetResult(2, &sr1, &cfg1, &sr2, &cfg2), E_INVALIDARG);

   TestPersistance(presult.p, CLSID_LiveLoadModelStressResults);
}