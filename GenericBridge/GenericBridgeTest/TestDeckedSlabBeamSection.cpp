///////////////////////////////////////////////////////////////////////
// GenericBridgeTest - Test driver for generic bridge library
// Copyright � 1999-2020  Washington State Department of Transportation
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

#include "stdafx.h"
#include "TestDeckedSlabBeamSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CTestDeckedSlabBeamSection::Test()
{
   CComPtr<IDeckedSlabBeamSection> beam_section;
   TRY_TEST(beam_section.CoCreateInstance(CLSID_DeckedSlabBeamSection), S_OK);

   CComPtr<IDeckedSlabBeam> beam;
   beam_section->get_Beam(&beam);
   TRY_TEST(beam != nullptr, true);

   // TxDOT 8DS23
   beam->put_A(95.75);
   beam->put_B(18);
   beam->put_C(15);
   beam->put_F(1.75);
   beam->put_W(6);
   beam->put_Tt(8);
   beam->put_Tb(7);

   CComQIPtr<IPrestressedGirderSection> section(beam_section);

   std::vector<std::pair<Float64, Float64>> vExpectedValues
   { 
      std::pair<Float64, Float64>( -8.0,12.0),
      std::pair<Float64, Float64>(-16.0,12.0)
   };

   TestWebSections(section, vExpectedValues);
}
