// TestDistributedLoad.cpp: implementation of the CTestDistributedLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestDistributedLoad.h"
#include <string>
#include <fstream>
#include <MathEx.h>

#define TEST_LOAD_FIRED() TRY_TEST(pMe->m_csDistributedLoadFired, true); pMe->m_csDistributedLoadFired=false;
#define TEST_CONTAINER_FIRED() TRY_TEST(pMe->m_csDistributedLoadFired, true); pMe->m_csDistributedLoadFired=false;

#include "TestLoadContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTestDistributedLoad::CTestDistributedLoad():
m_csDistributedLoadFired(false),
m_CsCookie(0)
{
}

CTestDistributedLoad::~CTestDistributedLoad()
{
}

void CTestDistributedLoad::Test()
{
   HRESULT hr;

   // create a listener
   CComObject<CTestDistributedLoad>* pMe;
   CComObject<CTestDistributedLoad>::CreateInstance(&pMe);
   pMe->AddRef();

   // test the container for DistributedLoads
   hr = LoadContainerTester<ldIDistributedLoad,ldIDistributedLoadItem,ldIDistributedLoads,CTestDistributedLoad,ldIDistributedLoadsEvents,&IID_ldIDistributedLoadsEvents, ldIEnumDistributedLoad>::TestContainer(pMe,CLSID_ldDistributedLoad,CLSID_ldDistributedLoads);
   TRY_TEST(hr, S_OK);

   // first test by itself
   CComPtr<ldIDistributedLoad> pDistributedLoad;
   TRY_TEST(pDistributedLoad.CoCreateInstance( CLSID_ldDistributedLoad ), S_OK );
   CComBSTR bst;
   Float64 d;
   TRY_TEST( pDistributedLoad->get_StartLocation(&d), S_OK );
   TRY_TEST( d, 0.0 );
   TRY_TEST( pDistributedLoad->get_EndLocation(&d), S_OK );
   TRY_TEST( d, 0.0 );
   TRY_TEST( pDistributedLoad->get_WStart(&d), S_OK );
   TRY_TEST( d, 0.0 );
   TRY_TEST( pDistributedLoad->get_WEnd(&d), S_OK );
   TRY_TEST( d, 0.0 );
   ldLoadOrientation or;
   TRY_TEST( pDistributedLoad->get_Orientation(&or), S_OK );
   TRY_TEST( or, ldloGlobal);
   ldLoadDirection dir;
   TRY_TEST( pDistributedLoad->get_Direction(&dir), S_OK );
   TRY_TEST( dir, ldldFy);
   long i;
   TRY_TEST( pDistributedLoad->get_MemberID(&i), S_OK );
   TRY_TEST( i, -1 );

   // let's listen to pcs
   DWORD dwCookie;
   CComQIPtr<ldIDistributedLoadEvents> events(pMe);
   CComPtr<IUnknown> punk(events);
   TRY_TEST(AtlAdvise(pDistributedLoad,punk,IID_ldIDistributedLoadEvents,&dwCookie),S_OK);

   // cause some events to fire
   TRY_TEST( pDistributedLoad->put_Orientation(ldloGlobalProjected), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_Orientation(&or), S_OK );
   TRY_TEST( or, ldloGlobalProjected );

   TRY_TEST( pDistributedLoad->put_Direction(ldldFx), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_Direction(&dir), S_OK );
   TRY_TEST( dir, ldldFx );

   TRY_TEST( pDistributedLoad->put_MemberID(3), S_OK );
   TEST_LOAD_FIRED();
   long l;
   TRY_TEST( pDistributedLoad->get_MemberID(&l), S_OK );
   TRY_TEST( l, 3 );

   TRY_TEST( pDistributedLoad->put_StartLocation(-0.2), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_StartLocation(&d), S_OK );
   TRY_TEST( d, -0.2 );
   TRY_TEST( pDistributedLoad->put_StartLocation(2.0), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_StartLocation(&d), S_OK );
   TRY_TEST( d, 2.0 );

   TRY_TEST( pDistributedLoad->put_EndLocation(-0.2), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_EndLocation(&d), S_OK );
   TRY_TEST( d, -0.2 );
   TRY_TEST( pDistributedLoad->put_EndLocation(2.0), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_EndLocation(&d), S_OK );
   TRY_TEST( d, 2.0 );

   TRY_TEST( pDistributedLoad->put_WStart(1.1), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_WStart(&d), S_OK );
   TRY_TEST( d, 1.1);
   TRY_TEST( pDistributedLoad->put_WEnd(2.2), S_OK );
   TEST_LOAD_FIRED();
   TRY_TEST( pDistributedLoad->get_WEnd(&d), S_OK );
   TRY_TEST( d, 2.2);


   TRY_TEST( pDistributedLoad->put_MemberID(-1), E_INVALIDARG );
   TRY_TEST( pDistributedLoad->put_StartLocation(-1.1), E_INVALIDARG );
   TRY_TEST( pDistributedLoad->put_EndLocation(-1.1), E_INVALIDARG );

   // test persistence by saving and retrieving data and comparing raw xml

   {
      CComPtr<IStructuredSave2> pss;
      TRY_TEST( pss.CoCreateInstance(CLSID_StructuredSave2), S_OK );
      TRY_TEST( pss->Open( CComBSTR("Test.xml")), S_OK );

      hr = pss->BeginUnit(CComBSTR("TestDistributedLoad"), 1.0);

      // get interface pointer to Save
      CComPtr<IStructuredStorage2> piss;
      TRY_TEST( pDistributedLoad.QueryInterface(&piss), S_OK);

      // save
      piss->Save( pss );

      hr = pss->EndUnit();
      TRY_TEST( pss->Close(), S_OK );
   }

   {
      // make a new member to work with
      CComPtr<ldIDistributedLoad> pDistributedLoad2;
      TRY_TEST(pDistributedLoad2.CoCreateInstance( CLSID_ldDistributedLoad ), S_OK );

      // now let's load and save to another file
      {
         CComPtr<IStructuredLoad2> psl;
         TRY_TEST( psl.CoCreateInstance( CLSID_StructuredLoad2 ), S_OK );
         TRY_TEST( psl->Open( CComBSTR("Test.xml")), S_OK );

         hr = psl->BeginUnit(CComBSTR("TestDistributedLoad"));

         IStructuredStorage2* piss2=NULL;
         TRY_TEST( pDistributedLoad2.QueryInterface(&piss2), S_OK);

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
         hr = pss2->BeginUnit(CComBSTR("TestDistributedLoad"), 1.0);
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
      std::string x1;
      std::ifstream if1("Test.xml");
      std::getline(if1, x1);
      std::getline(if1, x1); // second line is the data

      std::string x2;
      std::ifstream if2("Test2.xml");
      std::getline(if2, x2);
      std::getline(if2, x2);

      TRY_TEST( x1, x2 );

   }
   catch(...)
   {
      ATLASSERT(0); // big problems
      TRY_TEST(0,1); 
   }

   // create a clone and save it
   CComPtr<ldIDistributedLoad> pclone;
   TRY_TEST(pDistributedLoad->Clone(&pclone), S_OK);

   {
      CComPtr<IStructuredSave2> pss;
      TRY_TEST( pss.CoCreateInstance(CLSID_StructuredSave2), S_OK );
      TRY_TEST( pss->Open( CComBSTR("Test.xml")), S_OK );

      hr = pss->BeginUnit(CComBSTR("TestDistributedLoad"), 1.0);

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
      std::string x1;
      std::ifstream if1("Test.xml");
      std::getline(if1, x1);
      std::getline(if1, x1); // second line is the data

      std::string x2;
      std::ifstream if2("Test2.xml");
      std::getline(if2, x2);
      std::getline(if2, x2);

      TRY_TEST( x1, x2 );

   }
   catch(...)
   {
      ATLASSERT(0); // big problems
      TRY_TEST(0,1); 
   }

   // disconnect our connections
   TRY_TEST(AtlUnadvise(pDistributedLoad,IID_ldIDistributedLoadEvents,dwCookie),S_OK);
   pMe->Release();
}



HRESULT CTestDistributedLoad::OnDistributedLoadChanged(ldIDistributedLoad* DistributedLoad)
{
   m_csDistributedLoadFired = true;

   return S_OK;
}

HRESULT CTestDistributedLoad::OnDistributedLoadsChanged(ldIDistributedLoadItem* DistributedLoad)
{
   m_csDistributedLoadFired = true;
   return S_OK;
}
HRESULT CTestDistributedLoad::OnDistributedLoadsAdded(ldIDistributedLoadItem* DistributedLoad)
{
   m_csDistributedLoadFired = true;
   return S_OK;
}
HRESULT CTestDistributedLoad::OnDistributedLoadsBeforeRemove(ldIDistributedLoadItem* DistributedLoad)
{
   m_csDistributedLoadFired = true;
   return S_OK;
}
