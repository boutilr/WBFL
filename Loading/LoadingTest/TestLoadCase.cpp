// TestLoadCase.cpp: implementation of the CTestLoadCase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TestLoadCase.h"
#include <string>
#include <fstream>
#include <MathEx.h>

#define TEST_LoadCase_FIRED()     TRY_TEST(pMe->m_csLoadCaseFired, true); pMe->m_csLoadCaseFired=false; pMe->m_csLoadCasesFired=false;
#define TEST_CONTAINER_FIRED() TRY_TEST(pMe->m_csLoadCasesFired, true); pMe->m_csLoadCaseFired=false; pMe->m_csLoadCasesFired=false;

HRESULT TestContainer(CTestLoadCase* pMe);

#include "TestSetContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestLoadCase::CTestLoadCase():
m_csLoadCaseFired(false),
m_CsCookie(0),
m_LastID(0)
{
}

CTestLoadCase::~CTestLoadCase()
{
}

void CTestLoadCase::Test()
{
   HRESULT hr;

   // create a listener
   CComObject<CTestLoadCase>* pMe;
   CComObject<CTestLoadCase>::CreateInstance(&pMe);
   pMe->AddRef();

   // test the container for LoadCases
   hr = BSTRSetContainerTester<ldILoadCase,ldILoadCases,CTestLoadCase,ldILoadCasesEvents,&IID_ldILoadCasesEvents, ldIEnumLoadCase>::TestContainer(pMe,CLSID_ldLoadCase,CLSID_ldLoadCases);
   TRY_TEST(hr, S_OK);

   // test a LoadCase by itself
   CComPtr<ldILoadCase> pLoadCase;
   TRY_TEST(pLoadCase.CoCreateInstance( CLSID_ldLoadCase ), S_OK );
   CComBSTR str;
   TRY_TEST( pLoadCase->get_Name(&str), S_OK );
   TRY_TEST( str.Length(), 0 );
   str.Empty();
   TRY_TEST( pLoadCase->get_Description(&str), S_OK );
   TRY_TEST( str.Length(), 0 );

   // let's listen to pcs
   DWORD dwCookie;
   CComQIPtr<ldILoadCaseEvents> events(pMe);
   CComPtr<IUnknown> punk(events);
   TRY_TEST(AtlAdvise(pLoadCase,punk,IID_ldILoadCaseEvents,&dwCookie),S_OK);

   // cause some events to fire
   TRY_TEST( pLoadCase->put_Name(CComBSTR("LoadCase 1")), S_OK );
   TEST_LoadCase_FIRED();
   CComBSTR name;
   TRY_TEST( pLoadCase->get_Name(&name), S_OK );
   TRY_TEST( name, CComBSTR("LoadCase 1"));

   TRY_TEST( pLoadCase->put_Description(CComBSTR("This is a description")), S_OK );
   TEST_LoadCase_FIRED();
   CComBSTR desc;
   TRY_TEST( pLoadCase->get_Description(&desc), S_OK );
   TRY_TEST( desc, CComBSTR("This is a description"));

   CComVariant varint = 2;
   TRY_TEST( pLoadCase->put_ItemData(varint), S_OK );
   TEST_LoadCase_FIRED();
   VARIANT vint;
   TRY_TEST( pLoadCase->get_ItemData(&vint), S_OK );
   TRY_TEST( vint.intVal , 2);
   TRY_TEST( vint.vt , VT_I4);


   CollectionIndexType cnt;
   TRY_TEST( pLoadCase->get_LoadGroupCount(&cnt), S_OK );
   TRY_TEST(cnt, 0);

   TRY_TEST( pLoadCase->AddLoadGroup(CComBSTR("Load Group 0")), S_OK );
   TEST_LoadCase_FIRED();
   TRY_TEST( pLoadCase->get_LoadGroupCount(&cnt), S_OK );
   TRY_TEST(cnt, 1);

   TRY_TEST( pLoadCase->AddLoadGroup(CComBSTR("Load Group 1")), S_OK );
   TEST_LoadCase_FIRED();
   TRY_TEST( pLoadCase->get_LoadGroupCount(&cnt), S_OK );
   TRY_TEST(cnt, 2);

   CComBSTR lgnm;
   TRY_TEST( pLoadCase->GetLoadGroup(1, &lgnm), S_OK );
   TRY_TEST( lgnm, CComBSTR("Load Group 1"));

   TRY_TEST( pLoadCase->SetLoadGroup(1, CComBSTR("New Load Group 1")), S_OK );
   TEST_LoadCase_FIRED();

   lgnm.Empty();
   TRY_TEST( pLoadCase->GetLoadGroup(1, &lgnm), S_OK );
   TRY_TEST( lgnm, CComBSTR("New Load Group 1"));

   TRY_TEST( pLoadCase->GetLoadGroup(1, &lgnm), S_OK );

   TRY_TEST( pLoadCase->AddLoadGroup(CComBSTR("Load Group 2")), S_OK );
   TEST_LoadCase_FIRED();
   TRY_TEST( pLoadCase->get_LoadGroupCount(&cnt), S_OK );
   TRY_TEST(cnt, 3);

   TRY_TEST( pLoadCase->RemoveLoadGroup(CComBSTR("New Load Group 1")), S_OK );
   TEST_LoadCase_FIRED();
   TRY_TEST( pLoadCase->get_LoadGroupCount(&cnt), S_OK );
   TRY_TEST(cnt, 2);

   lgnm.Empty();
   TRY_TEST( pLoadCase->GetLoadGroup(1, &lgnm), S_OK );
   TRY_TEST( lgnm, CComBSTR("Load Group 2"));

   TRY_TEST( pLoadCase->RemoveLoadGroupByIndex(0), S_OK );
   TEST_LoadCase_FIRED();
   TRY_TEST( pLoadCase->get_LoadGroupCount(&cnt), S_OK );
   TRY_TEST(cnt, 1);

   lgnm.Empty();
   TRY_TEST( pLoadCase->GetLoadGroup(0, &lgnm), S_OK );
   TRY_TEST( lgnm, CComBSTR("Load Group 2"));

   TRY_TEST( pLoadCase->RemoveLoadGroupByIndex(1), E_INVALIDARG);
   TRY_TEST( pLoadCase->RemoveLoadGroupByIndex(-1), E_INVALIDARG);
   TRY_TEST( pLoadCase->RemoveLoadGroup(CComBSTR("New Load Group 1")), E_INVALIDARG );
   lgnm.Empty();
   TRY_TEST( pLoadCase->GetLoadGroup(1, &lgnm), E_INVALIDARG );
   TRY_TEST( pLoadCase->SetLoadGroup(1, CComBSTR("New Load Group 1")), E_INVALIDARG );
   TRY_TEST( pLoadCase->SetLoadGroup(0, CComBSTR("")), LOADING_E_BLANK_NAMES_NOT_ALLOWED );


   // test persistence by saving and retrieving data and comparing raw xml
   {
      CComPtr<IStructuredSave2> pss;
      TRY_TEST( pss.CoCreateInstance(CLSID_StructuredSave2), S_OK );
      TRY_TEST( pss->Open( CComBSTR("Test.xml")), S_OK );

      hr = pss->BeginUnit(CComBSTR("TestLoadCase"), 1.0);

      // get interface pointer to Save our cross section
      CComPtr<IStructuredStorage2> piss;
      TRY_TEST( pLoadCase.QueryInterface(&piss), S_OK);

      // save
      piss->Save( pss );

      hr = pss->EndUnit();
      TRY_TEST( pss->Close(), S_OK );
   }

   {
      // make a new member to work with
      CComPtr<ldILoadCase> psm2;
      TRY_TEST(psm2.CoCreateInstance( CLSID_ldLoadCase ), S_OK );

      // now let's load and save to another file
      {
         CComPtr<IStructuredLoad2> psl;
         TRY_TEST( psl.CoCreateInstance( CLSID_StructuredLoad2 ), S_OK );
         TRY_TEST( psl->Open( CComBSTR("Test.xml")), S_OK );

         hr = psl->BeginUnit(CComBSTR("TestLoadCase"));

         IStructuredStorage2* piss2=NULL;
         TRY_TEST( psm2.QueryInterface(&piss2), S_OK);

         // Load 
         TRY_TEST( piss2->Load(psl), S_OK );

         piss2->Release();

         VARIANT_BOOL end;
         hr = psl->EndUnit(&end);

         TRY_TEST( psl->Close(), S_OK );

         // now save to a different file so we can compare files
         CComPtr<IStructuredSave2> pss2;
         TRY_TEST( pss2.CoCreateInstance( CLSID_StructuredSave2 ), S_OK );
         TRY_TEST( pss2->Open( CComBSTR("Test2.xml")), S_OK );

         // save our loaded version
         hr = pss2->BeginUnit(CComBSTR("TestLoadCase"), 1.0);
         TRY_TEST( piss2->Save(pss2), S_OK );
         hr = pss2->EndUnit();
         TRY_TEST( pss2->Close(), S_OK );
      }
   }

   // compare two files by comparing strings. note that xml parser based files have only two lines
   // the first line is the xml processing instruction and the second line is all of the data
   // for this simple case we can simply compare the two data strings. For huge files, we
   // might have to resort to less memory intensive methods.
   try
   {
      std::_tstring x1;
      std::_tifstream if1("Test.xml");
      std::getline(if1, x1);
      std::getline(if1, x1); // second line is the data

      std::_tstring x2;
      std::_tifstream if2("Test2.xml");
      std::getline(if2, x2);
      std::getline(if2, x2);

      TRY_TEST( x1, x2 );

   }
   catch(...)
   {
      ATLASSERT(false); // big problems
      TRY_TEST(0,1); 
   }

   // create a clone and save it
   CComPtr<ldILoadCase> pclone;
   TRY_TEST(pLoadCase->Clone(&pclone), S_OK);

   {
      CComPtr<IStructuredSave2> pss;
      TRY_TEST( pss.CoCreateInstance(CLSID_StructuredSave2), S_OK );
      TRY_TEST( pss->Open( CComBSTR("Test.xml")), S_OK );

      hr = pss->BeginUnit(CComBSTR("TestLoadCase"), 1.0);

      // get interface pointer to Save
      CComPtr<IStructuredStorage2> piss;
      TRY_TEST( pclone.QueryInterface(&piss), S_OK);

      // save
      piss->Save( pss );

      hr = pss->EndUnit();
      TRY_TEST( pss->Close(), S_OK );
   }


   // compare clone's file with original
   try
   {
      std::_tstring x1;
      std::_tifstream if1("Test.xml");
      std::getline(if1, x1);
      std::getline(if1, x1); // second line is the data

      std::_tstring x2;
      std::_tifstream if2("Test2.xml");
      std::getline(if2, x2);
      std::getline(if2, x2);

      TRY_TEST( x1, x2 );

   }
   catch(...)
   {
      ATLASSERT(false); // big problems
      TRY_TEST(0,1); 
   }


   // disconnect our connections
   TRY_TEST(AtlUnadvise(pLoadCase,IID_ldILoadCaseEvents,dwCookie),S_OK);
   pMe->Release();
}

HRESULT CTestLoadCase::OnLoadCaseRenamed(BSTR oldName, BSTR newName)
{
   m_csLoadCaseFired = true;

   return S_OK;
}

HRESULT CTestLoadCase::OnLoadCaseChanged(ldILoadCase* LoadCase, ldLoadingChangeType change)
{
   if (change==ldcgCombination)
      m_csLoadCaseFired = true;
   else if (change==ldcgDescription)
      m_csLoadCaseFired = true;
   else if (change==ldcgLoading)
      m_csLoadCaseFired = true;
   else if (change==ldcgUserItemData)
      m_csLoadCaseFired = true;
   else
      ATLASSERT(false);

   return S_OK;
}

HRESULT CTestLoadCase::OnLoadCasesChanged(ldILoadCase* LoadCase, ldLoadingChangeType change)
{
   if (change==ldcgLoading)
      m_csLoadCasesFired = true;
   else if (change==ldcgDescription)
      m_csLoadCasesFired = true;
   else
      ATLASSERT(false);

   return S_OK;
}

HRESULT CTestLoadCase::OnLoadCasesRenamed(/*[in]*/BSTR oldName, /*[in]*/BSTR newName)
{
   m_csLoadCasesFired = true;
   return S_OK;
}

HRESULT CTestLoadCase::OnLoadCasesAdded(/*[in]*/ldILoadCase* load)
{
   m_csLoadCasesFired = true;
   return S_OK;
}

HRESULT CTestLoadCase::OnLoadCasesBeforeRemove(/*[in]*/ldILoadCase* load)
{
   m_csLoadCasesFired = true;
   return S_OK;
}

HRESULT CTestLoadCase::OnCreateSegment(ldILoadCase* pLoadCase)
{
   // give LoadCase a unique name
   TCHAR str[45];
   int cOut = _stprintf_s( str,45, _T("LoadCase %d"), m_LastID++);
   ATLASSERT( cOut < 45 );

   CComBSTR name(str);

   return pLoadCase->put_Name(name);
}
