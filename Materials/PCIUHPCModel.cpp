///////////////////////////////////////////////////////////////////////
// Materials - Analytical and Product modeling of civil engineering materials
// Copyright � 1999-2022  Washington State Department of Transportation
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
#include <Materials/PCIUHPCModel.h>

#include <Units/Convert.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WBFL::Materials;

PCIUHPCModel::PCIUHPCModel(const std::_tstring& strName) :
   StressStrainModel(strName)
{
}

void PCIUHPCModel::SetFc(Float64 fc)
{
   PRECONDITION(0 < fc);
   m_fc = WBFL::Units::ConvertFromSysUnits(fc,WBFL::Units::Measure::KSI);
   ASSERT(17.4 <= m_fc); // 17.4 ksi is the minimum value for f'c, PCI-UHPC GS
}

Float64 PCIUHPCModel::GetFc() const
{
   return WBFL::Units::ConvertToSysUnits(m_fc,WBFL::Units::Measure::KSI);
}

std::unique_ptr<StressStrainModel> PCIUHPCModel::Clone() const
{
   return std::make_unique<PCIUHPCModel>(*this);
}

Float64 PCIUHPCModel::GetYieldStrain() const
{
   return 0.00012;
}

Float64 PCIUHPCModel::GetModulusOfElasticity() const
{
   Float64 Ec = GetEc(); // this is in KSI, we need to return it in system units
   Ec = WBFL::Units::ConvertToSysUnits(Ec, WBFL::Units::Measure::KSI);
   return Ec;
}

std::pair<Float64, bool> PCIUHPCModel::ComputeStress(Float64 strain) const
{
   Float64 Ec = GetEc();

   Float64 stress = 0;
   bool bWithinStrainLimits = true;
   if (0 < strain)
   {
      // tension
      Float64 e_tcr = 0.00012;
      if (strain < e_tcr)
      {
         // strain is less than cracking
         stress = strain * 0.75 / e_tcr;
      }
      else if (::IsLE(strain, 0.005))
      {
         stress = 0.75; // ksi
      }
      else
      {
         // beyond localization so can't carry any tension
         stress = 0.0; // ksi
      }
   }
   else
   {
      // compression
      strain = fabs(strain);
      stress = 0.001 < strain ? -0.85 * m_fc : -0.85 * m_fc * strain / 0.001;
   }

   // The stress is in KSI, convert it to base units because that is what the caller expects
   stress = WBFL::Units::ConvertToSysUnits(stress, WBFL::Units::Measure::KSI);

   // NOTE: This stress-strain accomodates strains that are beyond the strain limit. For this reason,
   // we don't return false for strains exceeding the limits. If we did, the moment capacity solver would
   // not converge.

   return std::make_pair(stress, bWithinStrainLimits);
}

void PCIUHPCModel::GetStrainLimits(Float64* pMinStrain, Float64* pMaxStrain) const
{
   PRECONDITION(pMinStrain != nullptr);
   PRECONDITION(pMaxStrain != nullptr);

   *pMinStrain = -0.003;
   *pMaxStrain = 0.005;
}

Float64 PCIUHPCModel::GetStrainAtPeakStress() const
{
   return -0.003;
}

Float64 PCIUHPCModel::GetEc() const
{
   return 2500 * pow(m_fc, 0.33);
}


#if defined _DEBUG
bool PCIUHPCModel::AssertValid() const
{
   return true;
}

void PCIUHPCModel::Dump(WBFL::Debug::LogContext& os) const
{
   os << _T("Dump for PCIUHPCModel")         << WBFL::Debug::endl;
   os << _T("====================")         << WBFL::Debug::endl;
}
#endif // _DEBUG

#if defined _UNITTEST
#include <Units/Units.h>
bool PCIUHPCModel::TestMe(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("PCIUHPCModel");

   WBFL::Units::AutoSystem au;
   WBFL::Units::System::SetSystemUnits(WBFL::Units::Measure::_12KSlug,WBFL::Units::Measure::Inch,WBFL::Units::Measure::Second,WBFL::Units::Measure::Fahrenheit,WBFL::Units::Measure::Degree);

   PCIUHPCModel model;
   model.SetFc(18.0);

   // compression
   auto result = model.ComputeStress(-0.0005);
   TRY_TESTME(IsEqual(result.first, -0.85 * 18.0 / 2));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(-0.001);
   TRY_TESTME(IsEqual(result.first, -0.85 * 18.0));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(-0.002);
   TRY_TESTME(IsEqual(result.first, -0.85 * 18.0));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(-0.003);
   TRY_TESTME(IsEqual(result.first, -0.85 * 18.0));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(-0.004);
   TRY_TESTME(IsEqual(result.first, -0.85 * 18.0));
   TRY_TESTME(result.second == true);

   // tension
   result = model.ComputeStress(0.00006);
   TRY_TESTME(IsEqual(result.first, 0.75/2));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(0.00012);
   TRY_TESTME(IsEqual(result.first, 0.75));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(0.005);
   TRY_TESTME(IsEqual(result.first, 0.75));
   TRY_TESTME(result.second == true);

   result = model.ComputeStress(0.010);
   TRY_TESTME(IsEqual(result.first, 0.0));
   TRY_TESTME(result.second == true);

   TESTME_EPILOG("PCIUHPCModel");
}
#endif // _UNITTEST
