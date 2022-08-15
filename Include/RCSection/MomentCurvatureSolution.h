///////////////////////////////////////////////////////////////////////
// RCSection - Reinforced concrete section analysis modeling
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
#pragma once

#include <RCSection/RCSectionExp.h>
#include <RCSection/MomentCapacitySolution.h>

namespace WBFL
{
   namespace RCSection
   {
      class MomentCurvatureSolutionImpl;

      /// Solution to a moment curvature analysis.
      class RCSCLASS MomentCurvatureSolution
      {
      public:
         MomentCurvatureSolution();
         MomentCurvatureSolution(const MomentCurvatureSolution& other) = delete; // can't copy because of unique_ptr - also don't need copy semantics
         ~MomentCurvatureSolution();
         
         MomentCurvatureSolution& operator=(const MomentCurvatureSolution& other) = delete; // can't assign

         /// Adds a curvature solution point
         void AddPoint(Float64 M, Float64 k, std::unique_ptr<MomentCapacitySolution>&& capacitySolution);

         /// Maximum moment
         Float64 GetMaxMoment() const;

         /// Curvature associated with the maximum moment
         Float64 GetMaxMomentCurvature() const;

         /// The peak point in the moment curvature solution space
         void GetPeakCapacity(
            Float64& Mmax, ///< [out] Maximum moment
            Float64& k ///< [out] Curvature associated with the maximum moment
         );

         /// Moment at a solution step
         Float64 GetMoment(IndexType idx) const;

         /// Curvature at a solution step
         Float64 GetCurvature(IndexType idx) const;

         /// Strain plan at a solution step
         const WBFL::Geometry::Plane3d& GetStrainPlane(IndexType idx) const;

         /// MomentCapacitySolution object at a solution step
         const std::unique_ptr<MomentCapacitySolution>& GetCapacitySolution(IndexType idx) const;

         /// Number of solution steps
         IndexType GetPointCount() const;

      private:
         std::unique_ptr<MomentCurvatureSolutionImpl> m_pImpl;

#if defined _UNITTEST
      public:
         static bool TestMe(WBFL::Debug::Log& rlog);
#endif // _UNITTEST
      };
   };
};