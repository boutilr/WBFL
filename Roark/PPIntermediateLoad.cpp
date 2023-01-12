///////////////////////////////////////////////////////////////////////
// Roark - Simple span beam forumla, patterned after Roark's formulas
//         for Stress and Strain
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

#include <Roark/RoarkLib.h>
#include <Roark/PPIntermediateLoad.h>
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WBFL::Beams;

PPIntermediateLoad::PPIntermediateLoad(Float64 w,Float64 La,Float64 l,Float64 ei) :
RoarkBeam(l,ei)
{
   a = La;
   b = l - a;
   W = w;
}

std::unique_ptr<RoarkBeam> PPIntermediateLoad::CreateClone() const
{
   return std::make_unique<PPIntermediateLoad>(W,a,GetL(),GetEI());
}

void PPIntermediateLoad::SetLa(Float64 la)
{
   a = la;
}

Float64 PPIntermediateLoad::GetLa() const
{
   return a;
}

Float64 PPIntermediateLoad::GetLb() const
{
   return b;
}

void PPIntermediateLoad::SetW(Float64 w)
{
   W = w;
}

Float64 PPIntermediateLoad::GetW() const
{
   return W;
}

void PPIntermediateLoad::GetReactions(Float64* pRa,Float64* pRb) const
{
   Float64 L = GetL();
   *pRa = -W*b/L;
   *pRb = -W*a/L;
}

void PPIntermediateLoad::GetMoments(Float64* pMa,Float64* pMb) const
{
   *pMa = 0;
   *pMb = 0;
}

void PPIntermediateLoad::GetRotations(Float64* pra,Float64* prb) const
{
   Float64 L = GetL();
   *pra = ComputeRotation(0);
   *prb = ComputeRotation(L);
}

void PPIntermediateLoad::GetDeflections(Float64* pYa,Float64* pYb) const
{
   Float64 L = GetL();
   *pYa = ComputeDeflection(0);
   *pYb = ComputeDeflection(L);
}

WBFL::System::SectionValue PPIntermediateLoad::ComputeShear(Float64 x) const
{
   WBFL::System::SectionValue V;
   Float64 L = GetL();

   // Compute shear left and right of W
   Float64 Ra, Rb;
   GetReactions( &Ra, &Rb );
   Float64 left  =  Ra;
   Float64 right = -Rb;

   if ( IsZero(a) && IsZero(x) )
   {
      // Special case when load is directly over left support
      // and we want the shear at the left support
      V.Left()  = left;
      V.Right() = 0.0;
   }
   else if ( IsZero(a-L) && IsZero(x-L) )
   {
      // Special case when load is directly over right support
      // and we want the shear at the left support
      V.Left()  = 0.0;
      V.Right() = right;
   }
   else
   {
      if ( IsZero(x-a) )
      {
         V.Left()  = left;
         V.Right() = right;
      }
      else if (x < a)
      {
         V = left;
      }
      else
      {
         V = right;
      } 

      // If we want the shear at one end of the beam of the other.
      // We need to zero out the exterior face shear so we get the correct
      // stepping in the shear diagram.
      if ( IsZero(x) )
         V.Left() = 0.0;

      if ( IsZero(x-L) )
         V.Right() = 0.0;
   }

   return V;
}

WBFL::System::SectionValue PPIntermediateLoad::ComputeMoment(Float64 x) const
{
   Float64 L = GetL();

   Float64 M = 0;

   if (x < a)
      M = -W*b*x/L;
   else
      M = -W*b*x/L + W*(x-a);

   return M;
}

Float64 PPIntermediateLoad::ComputeRotation(Float64 x) const
{
   Float64 r = 0;

   Float64 L, EI;
   GetProperties(&L,&EI);

   if ( x < a )
   {
      r = W*b*(L*L - b*b - x*x)/(6*EI*L) - W*b*x*x/(3*EI*L);
   }
   else
   {
      r = W*a*(L-x)*(2*L - 2*x)/(6*EI*L) - W*a*(L*L - a*a - (L-x)*(L-x))/(6*EI*L);
   }

   return r;
}

Float64 PPIntermediateLoad::ComputeDeflection(Float64 x) const
{
   Float64 y = 0;

   Float64 L, EI;
   GetProperties(&L, &EI);

   if (x < a)
   {
      y = W*b*x*(L*L - b*b - x*x)/(6*EI*L);
   }
   else
   {
      y = W*a*(L-x)*(L*L - a*a - (L-x)*(L-x))/(6*EI*L);
   }

   return y;
}

//======================== DEBUG      =======================================
#if defined _DEBUG
bool PPIntermediateLoad::AssertValid() const
{
   Float64 L = GetL();

   if ( !IsEqual(a+b,L) )
      return false;

   if ( a < 0 || L < a )
      return false;

   if ( b < 0 || L < b )
      return false;

   return __super::AssertValid();
}

void PPIntermediateLoad::Dump(WBFL::Debug::LogContext& os) const
{
   os << "Dump for PPIntermediateLoad" << WBFL::Debug::endl;
   os << " a = " << a << WBFL::Debug::endl;
   os << " b = " << b << WBFL::Debug::endl;
   os << " W = " << W << WBFL::Debug::endl;
   __super::Dump( os );
}
#endif // _DEBUG

#if defined _UNITTEST
#include "Private.h"
bool PPIntermediateLoad::TestMe(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("PPIntermediateLoad");
   // Intermediate load at mid-span
   Float64 w = -10;
   Float64 La = 5;
   Float64 l = 10;
   Float64 ei = 1;
   PPIntermediateLoad beam(w,La,l,ei);

   Float64 Ra, Rb;
   beam.GetReactions( &Ra, &Rb );
   TRY_TESTME( IsEqual(Ra,5.0) );
   TRY_TESTME( IsEqual(Rb,5.0) );

   WBFL::System::SectionValue Mcl = beam.ComputeMoment(5);
   TRY_TESTME( IsEqual(Mcl.Left(),25.0) && IsEqual(Mcl.Right(),25.0) );

   WBFL::System::SectionValue Vcl = beam.ComputeShear(5);
   TRY_TESTME( IsEqual(Vcl.Left(),5.0) && IsEqual(Vcl.Right(),-5.0) );

   WBFL::System::SectionValue Va = beam.ComputeShear(0);
   TRY_TESTME( IsEqual(Va.Left(),0.0) && IsEqual(Va.Right(),5.0) );

   WBFL::System::SectionValue Vb = beam.ComputeShear(l);
   TRY_TESTME( IsEqual(Vb.Left(),-5.0) && IsEqual(Vb.Right(),0.0) );

   TRY_TESTME( IsZero( beam.ComputeDeflection(0) ) );
   TRY_TESTME( IsZero( beam.ComputeDeflection(l) ) );

   Float64 D50 = beam.ComputeDeflection(5);
   Float64 delta =  (w*l*l*l)/(48*ei);
   TRY_TESTME( IsEqual(D50,delta) );

   // Test signs of rotations
   TRY_TESTME( beam.ComputeRotation(0) < 0 );
   TRY_TESTME( beam.ComputeRotation(l) > 0 );

   // Test for symmetry
   Uint16 nPoints = 10;
   Uint16 j;
   for ( j = 0; j < nPoints/2; j++ )
   {
      WBFL::System::SectionValue lShear, rShear;
      WBFL::System::SectionValue lMoment, rMoment;

      Float64 xl = j * l / nPoints;
      Float64 xr = l - j * l / nPoints;

      lShear = beam.ComputeShear( xl );
      rShear = beam.ComputeShear( xr );
      TRY_TESTME( IsEqual( lShear.Left(),  -rShear.Right() ) );
      TRY_TESTME( IsEqual( lShear.Right(), -rShear.Left()  ) );

      lMoment = beam.ComputeMoment( xl );
      rMoment = beam.ComputeMoment( xr );
      TRY_TESTME( IsEqual( lMoment.Left(),  rMoment.Right() ) );
      TRY_TESTME( IsEqual( lMoment.Right(), rMoment.Left()  ) );

      TRY_TESTME( IsEqual( beam.ComputeRotation(xl),  -beam.ComputeRotation(xr)   ) );
      TRY_TESTME( IsEqual( beam.ComputeDeflection(xl), beam.ComputeDeflection(xr) ) );
   }

   TRY_TESTME( Test_Numerical(rlog, beam ) );
   TRY_TESTME( Test_Numerical(rlog, PPIntermediateLoad(-5,3,10,1)) );
   TRY_TESTME( Test_Numerical(rlog, PPIntermediateLoad(-5,7,10,1)) );

   TESTME_EPILOG("PPIntermediateLoad");
}
#endif // _UNITTEST
