///////////////////////////////////////////////////////////////////////
// Stability
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

#include <Stability/StabilityLib.h>
#include <Stability/HaulingResults.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WBFL::Stability;

HaulingSectionResult::HaulingSectionResult()
{
   AnalysisPointIndex = INVALID_INDEX;

   Mg = 0;
   Mw = 0;
   Mcf = 0;

   fps.fill(0);
   fg.fill(0);
   fw.fill(0);
   fcf.fill(0);

   memset((void*)fDirect, 0, sizeof(fDirect));
   memset((void*)fTilt, 0, sizeof(fTilt));
   memset((void*)f, 0, sizeof(f));

   memset((void*)fMinDirect, 0, sizeof(fMinDirect));
   memset((void*)MinDirectStressImpactDirection, 0, sizeof(MinDirectStressImpactDirection));
   memset((void*)MinDirectStressCorner, 0, sizeof(MinDirectStressCorner));

   memset((void*)fMaxDirect, 0, sizeof(fMaxDirect));
   memset((void*)MaxDirectStressImpactDirection, 0, sizeof(MaxDirectStressImpactDirection));
   memset((void*)MaxDirectStressCorner, 0, sizeof(MaxDirectStressCorner));

   memset((void*)fMin, 0, sizeof(fMin));
   memset((void*)MinStressImpactDirection, 0, sizeof(MinStressImpactDirection));
   memset((void*)MinStressWindDirection, 0, sizeof(MinStressWindDirection));
   memset((void*)MinStressCorner, 0, sizeof(MinStressCorner));

   memset((void*)fMax, 0, sizeof(fMax));
   memset((void*)MaxStressImpactDirection, 0, sizeof(MaxStressImpactDirection));
   memset((void*)MaxStressWindDirection, 0, sizeof(MaxStressWindDirection));
   memset((void*)MaxStressCorner, 0, sizeof(MaxStressCorner));

   memset((void*)Mcr, 0, sizeof(Mcr));
   memset((void*)ThetaCrack, 0, sizeof(ThetaCrack));

   memset((void*)FScr, 0, sizeof(FScr));

   FScrMin.fill(0);
   FScrImpactDirection.fill(NoImpact);
   FScrWindDirection.fill(Left);
   FScrCorner.fill(BottomLeft);


   for (int s = 0; s < 2; s++)
   {
      HaulingSlope slope = (HaulingSlope)s;
      fMinDirect[slope][Top] = Float64_Max;
      fMinDirect[slope][Bottom] = Float64_Max;
      fMaxDirect[slope][Top] = -Float64_Max;
      fMaxDirect[slope][Bottom] = -Float64_Max;

      fMin[slope][Top] = Float64_Max;
      fMin[slope][Bottom] = Float64_Max;
      fMax[slope][Top] = -Float64_Max;
      fMax[slope][Bottom] = -Float64_Max;

      FScrMin[slope] = Float64_Max;
   }

}

//////////////////////////////////////////

HaulingResults::HaulingResults()
{
   for ( int s = 0; s < 2; s++ )
   {
      HaulingSlope slope = (HaulingSlope)s;

      MaxDirectStress[slope] = -Float64_Max;
      MaxDirectStressAnalysisPointIndex[slope] = 0;
      MaxDirectStressImpactDirection[slope] = NoImpact;
      MaxDirectStressCorner[slope] = TopLeft;

      MinDirectStress[slope] = Float64_Max;
      MinDirectStressAnalysisPointIndex[slope] = 0;
      MinDirectStressImpactDirection[slope] = NoImpact;
      MinDirectStressCorner[slope] = TopLeft;

      MaxStress[slope] = -Float64_Max;
      MaxStressAnalysisPointIndex[slope] = 0;
      MaxStressImpactDirection[slope] = NoImpact;
      MaxStressWindDirection[slope] = Left;
      MaxStressCorner[slope] = TopLeft;

      MinStress[slope] = Float64_Max;
      MinStressAnalysisPointIndex[slope] = 0;
      MinStressImpactDirection[slope] = NoImpact;
      MinStressWindDirection[slope] = Left;
      MinStressCorner[slope] = TopLeft;

      MinFScr[slope] = Float64_Max;
      FScrAnalysisPointIndex[slope] = 0;
      FScrImpactDirection[slope] = NoImpact;
      FScrWindDirection[slope] = Left;
      FScrCorner[slope] = TopLeft;

      FSfImpactDirection[slope] = NoImpact;
      FSfWindDirection[slope] = Left;
      MinFsFailure[slope] = Float64_Max;
      MinAdjFsFailure[slope] = Float64_Max;

      FSroImpactDirection[slope] = NoImpact;
      FSroWindDirection[slope] = Left;
      MinFsRollover[slope] = Float64_Max;

      for (int i = 0; i < 3; i++)
      {
         ImpactDirection impact = (ImpactDirection)i;
         for (int w = 0; w < 2; w++)
         {
            WindDirection wind = (WindDirection)w;
            bRotationalStability[slope][impact][wind] = true;
            bRolloverStability[slope][impact][wind] = true;
         }
      }
   }


   memset((void*)ThetaMax, 0, sizeof(ThetaMax));
   memset((void*)FsFailure, 0, sizeof(FsFailure));
   memset((void*)AdjFsFailure, 0, sizeof(AdjFsFailure));

   memset((void*)ThetaRollover, 0, sizeof(ThetaRollover));
   memset((void*)FsRollover,0,sizeof(FsRollover));

   AssumedTiltDirection = Left;
}

bool HaulingResults::HasRotationalStablity() const
{
   for (int s = 0; s < 2; s++)
   {
      HaulingSlope slope = (HaulingSlope)s;
      for (int i = 0; i < 3; i++)
      {
         ImpactDirection impact = (ImpactDirection)i;
         for (int w = 0; w < 2; w++)
         {
            WindDirection wind = (WindDirection)w;
            if (!bRotationalStability[slope][impact][wind])
            {
               // only one has to be false
               return false;
            }
         }
      }
   }
   return true;
}

bool HaulingResults::HasRolloverStability() const
{
   for (int s = 0; s < 2; s++)
   {
      HaulingSlope slope = (HaulingSlope)s;
      for (int i = 0; i < 3; i++)
      {
         ImpactDirection impact = (ImpactDirection)i;
         for (int w = 0; w < 2; w++)
         {
            WindDirection wind = (WindDirection)w;
            if (!bRolloverStability[slope][impact][wind])
            {
               // only one has to be false
               return false;
            }
         }
      }
   }

   return true;
}

bool HaulingResults::IsStable() const
{
   return HasRotationalStablity() && HasRolloverStability();
}
