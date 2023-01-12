///////////////////////////////////////////////////////////////////////
// Materials - Analytical and Product modeling of civil engineering materials
// Copyright � 1999-2023  Washington State Department of Transportation
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

#include <Materials/MatLib.h>
#include <Materials/StressStrainModel.h>
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WBFL::Materials;

StressStrainModel::StressStrainModel(const std::_tstring& strName) :
   m_Name(strName)
{
}

void StressStrainModel::SetName(const std::_tstring& name)
{
   m_Name = name;
}

std::_tstring StressStrainModel::GetName() const
{
   return m_Name;
}

#if defined _DEBUG
bool StressStrainModel::AssertValid() const
{
   return true;
}

void StressStrainModel::Dump(WBFL::Debug::LogContext& os) const
{
   os << _T("Dump for StressStrainModel")         << WBFL::Debug::endl;
   os << _T("====================")         << WBFL::Debug::endl;
   os << _T("Name    : ") << m_Name.c_str() << WBFL::Debug::endl;
}
#endif // _DEBUG

#if defined _UNITTEST
bool StressStrainModel::TestMe(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("StressStrainModel");
   TRY_TESTME(true);
   TESTME_EPILOG("StressStrainModel");
}
#endif // _UNITTEST
