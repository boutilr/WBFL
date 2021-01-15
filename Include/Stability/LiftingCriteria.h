///////////////////////////////////////////////////////////////////////
// Stability
// Copyright � 1999-2020  Washington State Department of Transportation
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

#include <Stability\StabilityExp.h>

/// Criteria for lifting analysis
class STABILITYCLASS stbLiftingCriteria
{
public:
   stbLiftingCriteria();

   Float64 Lambda; ///< concrete density modification factor (see LRFD 5.4.2.8)

   Float64 CompressionCoefficient_GlobalStress; ///< Global compression compression limit coefficient (x*f'c)
   Float64 CompressionCoefficient_PeakStress; ///< Peak compression compression limit coefficient (x*f'c)

   Float64 TensionCoefficient; ///< Tension limit coefficient (x*sqrt(f'c)) for sections without sufficient auxilary reinforcement
   bool bMaxTension; ///< Boolean value indicating if there is a maximum value of the tension stress limit
   Float64 MaxTension; ///< If applicable, the maximum value of the tension stress limit
   Float64 TensionCoefficientWithRebar; ///< Tension limit coefficient (x*sqrt(f'c)) for sections with sufficient auxilary reinforcement

   Float64 MinFScr; ///< minimum factor of safety against cracking
   Float64 MinFSf;  ///< minimum factor of safety against failure

   Float64 AllowableCompression_GlobalStress; ///< compression stress limit
   Float64 AllowableCompression_PeakStress; ///< compression stress limit
   Float64 AllowableTension; ///< tension stress limit for sections without sufficient auxilary reinforcement
   Float64 AllowableTensionWithRebar; ///< tension stress limit for sections with sufficient auxilary reinforcement
};
