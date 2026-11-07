///////////////////////////////////////////////////////////////////////
// LBAM Live Loader - Longitindal Bridge Analysis Model
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
#include <Units/Units.h>
#include <cmath>
#include <stdexcept>

// MBE 4 6A.4.5.4.1. ADTT is resolved by the application before choosing the
// llaMBE4PermitLane vehicle type. The truck remains applicable everywhere.
inline bool MBE4PermitLaneSpan(Float64 lengthFeet)
{
   return 200.0 - 1e-8 <= lengthFeet && lengthFeet <= 300.0 + 1e-8;
}

inline bool MBE4PermitLaneApplies(ISupportLocations* supports, ILiveLoadNegativeMomentRegion* negative,
   IInfluenceLine* influence, BSTR stage, ForceEffectType effect, OptimizationType optimization,
   bool rightFace, bool reaction, bool force = true)
{
   if (!influence) return false;
   CHRException hr;
   Float64 x;
   hr = influence->get_Location(&x);
   CComPtr<IDblArray> locations;
   hr = supports->get_SupportLocations(&locations);
   IndexType count;
   hr = locations->get_Count(&count);
   Float64 tolerance = WBFL::Units::ConvertToSysUnits(1e-7, WBFL::Units::Measure::Feet);
   for (IndexType i = 1; i < count; ++i)
   {
      Float64 start, end;
      hr = locations->get_Item(i - 1, &start);
      hr = locations->get_Item(i, &end);
      Float64 length = WBFL::Units::ConvertFromSysUnits(end - start, WBFL::Units::Measure::Feet);
      if (!MBE4PermitLaneSpan(length)) continue;
      if (start + tolerance < x && x < end - tolerance) return true;
      // At a support, each section face belongs to its adjacent span. A
      // reaction includes both faces and qualifies if either span qualifies.
      if (fabs(x - start) <= tolerance && (rightFace || reaction)) return true;
      if (fabs(x - end) <= tolerance && (!rightFace || reaction)) return true;
   }
   if (count <= 2 || (!reaction && !force)) return false;
   CComQIPtr<IContraflexureResponse> contraflexure(negative);
   if (!contraflexure) throw std::runtime_error("Permit lane analysis requires contraflexure response");
   if (reaction)
   {
      CComPtr<IDblArray> zones;
      hr = contraflexure->ComputeContraflexureLocations(stage, &zones);
      IndexType zoneCount;
      hr = zones->get_Count(&zoneCount);
      for (IndexType i = 1; i + 1 < count; ++i)
      {
         Float64 support;
         hr = locations->get_Item(i, &support);
         if (fabs(x - support) > tolerance) continue;
         for (IndexType z = 0; z + 1 < zoneCount; z += 2)
         {
            Float64 start, end;
            hr = zones->get_Item(z, &start);
            hr = zones->get_Item(z + 1, &end);
            if (start < x && x < end) return true;
         }
      }
      return false;
   }
   if (effect != fetFy && !(effect == fetMz && optimization == optMinimize)) return false;
   PoiIDType poi;
   hr = influence->get_POI(&poi);
   InZoneType zone;
   hr = contraflexure->get_IsPOIInContraflexureZone(poi, stage, &zone);
   return zone == izInside || (zone == izLeftEdge && rightFace) || (zone == izRightEdge && !rightFace);
}
