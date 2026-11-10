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

#include <Lrfd/LrfdLib.h>
#include <Lrfd/MBERating.h>
#include <Lrfd/RatingSolver.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

using namespace WBFL::LRFD;

Float64 MBERating::GetServiceIIIInventoryLiveLoadFactor(bool refinedLosses, bool elasticGain)
{
   if (refinedLosses && elasticGain)
   {
      return 1.0;
   }
   return 0.8;
}

Float64 MBERating::GetReducedDemandMinimumReinforcementFactor(Float64 mr, Float64 crackingLimit,
   Float64 resistanceModifier)
{
   if (!std::isfinite(mr) || !std::isfinite(crackingLimit) ||
       !std::isfinite(resistanceModifier) || resistanceModifier <= 0.0)
      throw std::invalid_argument("Invalid minimum-reinforcement rating parameters");
   if (mr == 0.0) return 0.0;
   if (crackingLimit == 0.0 || (mr < 0.0) != (crackingLimit < 0.0)) return 1.0;
   Float64 crackingRatio = std::fabs(crackingLimit) / std::fabs(mr);
   if (crackingRatio <= 1.0) return 1.0;
   // Normalize by |Mr| so the solution is independent of the moment units.
   return RatingSolver::Solve([&](Float64 k) {
      Float64 minimumRatio = (std::min)(crackingRatio, 1.33 * resistanceModifier * k);
      Float64 allowedK = minimumRatio <= 1.0 ? 1.0 : 1.0 / minimumRatio;
      return allowedK - k;
   });
}
