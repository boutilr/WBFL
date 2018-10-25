///////////////////////////////////////////////////////////////////////
// Stability
// Copyright � 1999-2016  Washington State Department of Transportation
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
#include <Stability\HaulingResults.h>
#include <Stability\Criteria.h>


/*****************************************************************************
CLASS 
   stbHaulingCheckArtifact

DESCRIPTION
   Encapsulates the spec check of a hauling analysis


COPYRIGHT
   Copyright � 1997-2016
   Washington State Department Of Transportation
   All Rights Reserved
*****************************************************************************/

class STABILITYCLASS stbHaulingCheckArtifact
{
public:
   stbHaulingCheckArtifact();
   stbHaulingCheckArtifact(const stbHaulingResults& results,const stbCriteria& criteria);
   void Init(const stbHaulingResults& results,const stbCriteria& criteria);

   const stbHaulingResults& GetHaulingResults() const;
   const stbCriteria& GetCriteria() const;

   bool Passed() const;
   bool PassedCrackingCheck() const;
   bool PassedFailureCheck() const;
   bool PassedDirectStressCheck() const;
   bool PassedDirectCompressionCheck() const;
   bool PassedDirectTensionCheck() const;
   bool PassedStressCheck() const;
   bool PassedCompressionCheck() const;
   bool PassedTensionCheck() const;

protected:
   stbHaulingResults m_Results;
   stbCriteria m_Criteria;
};
