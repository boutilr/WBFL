// WBFLGraphing.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Graphing.h"

#include <System\dllTest.h>
#include <Graphing/UnitTest.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

// call unit test routines for all packages belonging to this dll.
bool WINAPI UnitTest(WBFL::Debug::Log& rlog)
{
	return WBFL::Graphing::UnitTest::TestMe(rlog);
}
