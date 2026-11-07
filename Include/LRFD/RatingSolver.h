///////////////////////////////////////////////////////////////////////
// LRFD - Utility library to support equations, methods, and procedures
//        from the AASHTO LRFD Bridge Design Specification
// Copyright © 1999-2026  Washington State Department of Transportation
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

#include <Lrfd/LrfdExp.h>
#include <functional>
#include <vector>

namespace WBFL
{
   namespace LRFD
   {
      /// @brief Solves a load-rating reserve equation with fixed permanent effects.
      class LRFDCLASS RatingSolver
      {
      public:
         RatingSolver() = delete;
         RatingSolver(const RatingSolver&) = delete;
         RatingSolver& operator=(const RatingSolver&) = delete;
         ~RatingSolver() = delete;

         /// @brief Solves the load-dependent rating equation in MBE C6A.4.2.1.
         /// @param reserve Factored resistance minus factored demand at the trial RF;
         /// permanent loads remain fixed and live load is scaled by RF.
         /// @param transitions Force reversals or other known changes in the resistance path.
         /// The residual must remain nonnegative up to the controlling crossing within
         /// each smooth interval. Supply transitions for intervening failing intervals.
         /// @return Passing side of the first bracketed crossing, zero if permanent
         /// loads fail, or Float64 max if no failing sample is found through RF = 1e6.
         /// @throws std::runtime_error If a sampled residual is not finite.
         static Float64 Solve(const std::function<Float64(Float64)>& reserve,
            const std::vector<Float64>& transitions = {});
      };
   };
};
