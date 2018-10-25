///////////////////////////////////////////////////////////////////////
// GeomModel - Geometric Modeling of shapes and sections
// Copyright � 1999-2014, Washington State Department of Transportation, All Rights Reserved
//                        Bridge and Structures Office
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

#include <GeomModel\GeomModelLib.h>
#include <GeomModel\UnitTest.h>
#include <System\dllTest.h>

#include <initguid.h>
#include <WBFLGeometry_i.c>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Every DLL has an entry point DllEntryPoint
BOOL WINAPI DllEntryPoint( HINSTANCE /*hinstDll*/,
                           DWORD     /*fdwRreason*/,
                           LPVOID    /*plvReserved*/)
{
    return 1;   // Indicate that the DLL was initialized successfully.
}

// call unit test routines for all packages belonging to this dll.
bool WINAPI UnitTest(dbgLog& rlog)
{
   return gmUnitTest::TestMe(rlog);
}
