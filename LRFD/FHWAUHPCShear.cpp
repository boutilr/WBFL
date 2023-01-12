///////////////////////////////////////////////////////////////////////
// LRFD - Utility library to support equations, methods, and procedures
//        from the AASHTO LRFD Bridge Design Specification
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

#include <Lrfd\LrfdLib.h>
#include <Lrfd\FHWAUHPCShear.h>
#include <Units\Convert.h>
#include <Math\BrentsRootFinder.h>
#include <Math\FixedPointIteration.h>
#include <functional>
#include <System\AutoVariable.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class theta_equation
{
public:
   theta_equation(lrfdUHPCShearData* pData,Float64 fv) :
      m_pData(pData), m_fv(fv)
   {
      m_Alpha = WBFL::Units::ConvertFromSysUnits(pData->alpha, WBFL::Units::Measure::Radian);
      m_cot_alpha = IsEqual(m_Alpha, PI_OVER_2) ? 0 : 1 / tan(m_Alpha);
      m_sin_alpha = sin(m_Alpha);
   }

   Float64 operator()(Float64 theta) const
   {
      Float64 cot_theta = IsEqual(theta,PI_OVER_2) ? 0 : 1 / tan(theta);
      Float64 cot_theta_2 = cot_theta * cot_theta; // cot(theta) * cot(theta) = cot(theta)^2
      Float64 cot_theta_4 = cot_theta_2 * cot_theta_2; // cot(theta)^2 * cot(theta)^2 = cot(theta)^4

      // Compute reinforcement ratio by GS Eq 1.7.3.4.1-5
      // the K coefficient makes it a little easer to deal with cases when theta = PI_OVER_2
      // and cot_theta = 0.... we don't want to divide by zero
      // The limit of cot_alpha/cot_theta goes to zero as theta goes to PI_OVER_2
      // when theta = PI_OVER_2, take m_cot_alpha/cot_theta equal to zero and K is then sin_alpha
      Float64 K = IsZero(cot_theta) ? m_sin_alpha : (1 + m_cot_alpha / cot_theta) * m_sin_alpha;
      m_pData->rho = (m_pData->AvS / m_pData->bv) * K;

      // Right hand side of equation 1.7.3.4.1-1
      Float64 result = (m_pData->es / 2) * (1 + cot_theta_2);
      result += (2 * m_pData->ftloc / m_pData->Ec) * cot_theta_4;
      result += (2 * m_pData->rho * m_fv / m_pData->Ec) * cot_theta_2 * (1 + cot_theta_2);

      // Subtract left hand side so this equation return 0 when the correct value of theta is used
      result -= m_pData->etloc;

      return result;
   }

private:
   lrfdUHPCShearData* m_pData;
   Float64 m_fv;
   Float64 m_Alpha;
   Float64 m_cot_alpha;
   Float64 m_sin_alpha;
};

Float64 SolveForTheta(lrfdUHPCShearData* pData,Float64 fv)
{
   std::function<Float64(Float64)> fn(theta_equation(pData,fv));
   WBFL::Math::BrentsRootFinder root_finder;
   Float64 theta = root_finder.FindRootInRange(fn, 0.000001, PI_OVER_2 - 0.000001, 0.0001);
   return theta;
}


class fv_equation
{
public:
   fv_equation(lrfdUHPCShearData* pData) : m_pData(pData)
   {
   }

   Float64 operator()(Float64 fv_guess) const
   {
      // Compute theta by solving GS Eq 1.7.3.4.1-1
      Float64 theta = SolveForTheta(m_pData, fv_guess);

      Float64 cot_theta = IsEqual(theta, PI_OVER_2) ? 0 : 1 / tan(theta);
      Float64 cot_theta_2 = cot_theta * cot_theta;

      // Compute e2 by GS Eq 1.7.3.4.1-2
      Float64 e2 = -(2 * m_pData->ftloc / m_pData->Ec) * (cot_theta_2) - (2 * m_pData->rho * fv_guess / m_pData->Ec) * (1 + cot_theta_2);

      // Compute ev by GS Eq 1.7.3.4.1-3
      Float64 ev = m_pData->etloc - 0.5 * m_pData->es + e2;

      // Compute fv by GS Eq 1.7.3.4.1-4
      Float64 fv = Min(m_pData->Es * ev, m_pData->fy);

      // record the solution and intermediate values
      m_pData->Theta = WBFL::Units::ConvertToSysUnits(theta, WBFL::Units::Measure::Radian);
      m_pData->e2 = e2;
      m_pData->ev = ev;

      return fv;
   }

private:
   lrfdUHPCShearData* m_pData;
};

void Solve(lrfdUHPCShearData* pData)
{
   WBFL::Math::FixedPointIteration fpi;
   pData->fv = fpi.Solve(std::function<Float64(Float64)>(fv_equation(pData)), IsZero(pData->AvS) ? 0.0 : pData->fy, 0.001, 100000);
}

bool lrfdUHPCShear::ComputeShearResistanceParameters(lrfdUHPCShearData* pData)
{
   Float64 Mu = pData->Mu;
   Float64 Nu = pData->Nu;
   Float64 Vu = pData->Vu;
   Float64 Vp = pData->Vp;
   Float64 dv = pData->dv; // limit on dv in composite sections with UHPC beams need to apply (GS 1.7.3.4.1) so use the basic dv here
   Float64 bv = pData->bv;
   Float64 Es = pData->Es;
   Float64 As = pData->As;
   Float64 Eps = pData->Eps;
   Float64 Aps = pData->Aps;
   Float64 fpo = pData->fpo;
   Float64 Ec = pData->Ec;
   Float64 Ac = pData->Ac;
   Float64 fy = pData->fy;
   Float64 AvS = pData->AvS;
   Float64 ft = pData->ft;
   Float64 ftloc = pData->ftloc;
   Float64 etloc = pData->etloc;
   Float64 etcr = pData->etcr;
   Float64 alpha = pData->alpha;

   // compute net longitudinal tensile strain at the centroid of the tensile reinforcement as shown in LRFD Figure 5.7.3.4.21
   // Per GS Eq 1.7.3.4.1-6
   // If Es*As + Eps*Aps is zero, then this equation is divide by zero. It also means the tension tie is provided only be the concrete
   // so we ant to use the -7 equation below. To force that happen, just set es = -1 because the -7 equation is required of es is negative or less than etcr
   Float64 T = Es * As + Eps * Aps;
   Float64 es = IsZero(T) ? -1 : (fabs(Mu) / dv + 0.5 * Nu + fabs(Vu - Vp) - Aps * fpo - ft * Ac) / T;

   if (es < etcr)
   {
      // GS Eq. 1.7.3.4.1-7
      es = (fabs(Mu) / dv + 0.5 * Nu + fabs(Vu - Vp) - Aps * fpo) / (T + Ec * Ac);
   }

   pData->es = es;

   if (etloc < es)
   {
      // if es is greater than etloc, the solution method isn't valid
      // because the UHPC is fractured.
      pData->fv = 0;
      pData->Theta = 0;
      return false;
   }

   // If there isn't any reinforcement (AvS = 0), force Es to be zero so when GS Eq. 1.7.3.4.1-4 is evaluated, fv,alpha = 0
   // Use the AutoVariable so Es automatically reverts to its original value
   WBFL::System::AutoVariable<Float64> av(&pData->Es, IsZero(AvS) ? 0 : pData->Es);
   
   try
   {
      Solve(pData);
   }
   catch (...)
   {
      pData->fv = 0;
      pData->Theta = 0;
      return false;
   }

   return true;
}

#if defined _UNITTEST
bool lrfdUHPCShear::TestMe(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("lrfdUHPCShear");
   TRY_TESTME(TestCase1(rlog));
   TRY_TESTME(TestCase2(rlog));
   TESTME_EPILOG("lrfdUHPCShear");
}

bool lrfdUHPCShear::TestCase1(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("lrfdUHPCShear::Case1");
   // Test case based on FHWA Prestressed Concrete Design Example
   lrfdUHPCShearData data;
   data.Mu = 1402.3*12;
   data.Nu = 0;
   data.Vu = 377.3;
   data.Vp = 0;
   data.Aps = 10;
   data.fpo = 189;
   data.ft = 0.85 * 1.2;
   data.Ac = 362.61;
   data.Eps = 28500;
   data.Es = 29000;
   data.Ec = 6933;
   data.AvS = 0.20 / 6;
   data.bv = 3.5;
   data.dv = 4.24*12;
   data.fy = 60.0;
   data.ftloc = 1.2;
   data.etloc = 0.004;
   data.etcr = 0.001;

   lrfdUHPCShear::ComputeShearResistanceParameters(&data);

   TRY_TESTME(IsEqual(data.fv, 60.0));

   Float64 theta = WBFL::Units::ConvertFromSysUnits(data.Theta, WBFL::Units::Measure::Degree); // convert to degrees
   TRY_TESTME(IsEqual(theta, 30.34975)); // FHWA Example 30.35

   TRY_TESTME(IsEqual(data.es, -0.0004223));
   TRY_TESTME(IsEqual(data.e2, -0.0016554));
   TRY_TESTME(IsEqual(data.ev, 0.0025557));

   // Test zero reinforcement case
   data.AvS = 0;

   lrfdUHPCShear::ComputeShearResistanceParameters(&data);

   TRY_TESTME(IsEqual(data.fv, 0.0));

   theta = WBFL::Units::ConvertFromSysUnits(data.Theta, WBFL::Units::Measure::Degree); // convert to degrees
   TRY_TESTME(IsEqual(theta, 27.13625)); // FHWA Example 30.35

   TRY_TESTME(IsEqual(data.es, -0.0004223));
   TRY_TESTME(IsEqual(data.e2, -0.0013178));
   TRY_TESTME(IsEqual(data.ev,  0.0028933));
   TESTME_EPILOG("lrfdUHPCShear::Case1");
}


bool lrfdUHPCShear::TestCase2(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("lrfdUHPCShear::Case2");
   // Test case based on FHWA Reinforced Concrete Example
   lrfdUHPCShearData data;
   data.Mu = 100.4 * 12;
   data.Nu = 0;
   data.Vu = 61.4;
   data.Vp = 0;
   data.Aps = 0;
   data.As = 14.04;
   data.fpo = 0;
   data.ft = 0.85 * 1.2;
   data.Ac = 139.32;
   data.Eps = 28500;  
   data.Es = 29000;
   data.Ec = 6933;
   data.AvS = 2 * 0.20 / 6;
   data.bv = 12;
   data.dv = 1.64 * 12;
   data.fy = 60.0;
   data.ftloc = 1.2;
   data.etloc = 0.003;
   data.etcr = 0.001;

   lrfdUHPCShear::ComputeShearResistanceParameters(&data);

   // Note FHWA example has some rounding that we don't do here

   TRY_TESTME(IsEqual(data.fv, 52.386743)); // FHWA Example 54.2

   Float64 theta = WBFL::Units::ConvertFromSysUnits(data.Theta, WBFL::Units::Measure::Degree); // convert to degrees
   TRY_TESTME(IsEqual(theta, 32.437149)); // FHWA Example 31.72

   TRY_TESTME(IsEqual(data.es, 0.0000893));
   TRY_TESTME(IsEqual(data.e2, -0.00115));
   TRY_TESTME(IsEqual(data.ev, 0.001806));

   // Test zero reinforcement case
   data.AvS = 0;

   lrfdUHPCShear::ComputeShearResistanceParameters(&data);

   TRY_TESTME(IsEqual(data.fv, 0.0));

   theta = WBFL::Units::ConvertFromSysUnits(data.Theta, WBFL::Units::Measure::Degree); // convert to degrees
   TRY_TESTME(IsEqual(theta, 30.60244)); // FHWA Example 30.35

   TRY_TESTME(IsEqual(data.es,  0.0000893));
   TRY_TESTME(IsEqual(data.e2, -0.0009896));
   TRY_TESTME(IsEqual(data.ev,  0.0019658));

   TESTME_EPILOG("lrfdUHPCShear::Case2");
}
#endif // _UNITTEST#endif // _UNITTEST