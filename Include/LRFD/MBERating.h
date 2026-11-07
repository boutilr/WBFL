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

namespace WBFL
{
   namespace LRFD
   {
      /// @brief MBE load-rating rules, independent of specification edition management.
      class LRFDCLASS MBERating
      {
      public:
         MBERating() = delete;
         MBERating(const MBERating&) = delete;
         MBERating& operator=(const MBERating&) = delete;
         ~MBERating() = delete;

         /// @brief MBE 4 Table 6A.4.2.2-2, design-load inventory Service III.
         /// @param refinedLosses Uses LRFD 5.9.3.4 refined time-dependent losses.
         /// @param elasticGain Takes advantage of elastic gain.
         static Float64 GetServiceIIIInventoryLiveLoadFactor(bool refinedLosses, bool elasticGain);
      };
   };
};
