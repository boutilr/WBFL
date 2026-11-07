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
#include <Lrfd/RatingSolver.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

using namespace WBFL::LRFD;

Float64 RatingSolver::Solve(const std::function<Float64(Float64)>& reserve,
   const std::vector<Float64>& transitions)
{
   auto evaluate = [&](Float64 rf) {
      Float64 r = reserve(rf);
      if (!std::isfinite(r)) throw std::runtime_error("Non-finite load-rating residual");
      return r;
   };
   if (evaluate(0.0) < 0.0) return 0.0;
   std::vector<Float64> samples;
   for (Float64 rf = 0.125; rf < 1e6; rf = rf < 1.0 ? rf + 0.125 : rf * 1.25)
      samples.push_back(rf);
   samples.push_back(1e6);
   // Sample both sides of discontinuities (e.g. a different tension tie at
   // moment reversal), rather than skipping a failing interval during growth.
   for (Float64 rf : transitions)
      if (std::isfinite(rf) && 0 < rf && rf <= 1e6)
      {
         samples.push_back(std::nextafter(rf, 0.0));
         samples.push_back(rf);
         if (rf < 1e6) samples.push_back(std::nextafter(rf, 1e6));
      }
   std::sort(samples.begin(), samples.end());
   Float64 lo = 0.0;
   for (Float64 hi : samples)
   {
      if (evaluate(hi) < 0.0)
      {
         for (int i = 0; i < 80 && hi - lo > 1e-7 * (1.0 + lo); ++i)
         {
            Float64 mid = lo + (hi - lo) / 2.0;
            if (evaluate(mid) < 0.0) hi = mid; else lo = mid;
         }
         return lo; // retain the passing side of the bracket
      }
      lo = hi;
   }
   return (std::numeric_limits<Float64>::max)();
}
