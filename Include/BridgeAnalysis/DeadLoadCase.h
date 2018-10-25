///////////////////////////////////////////////////////////////////////
// BAM - Bridge Analysis Model, Analytical modeling of bridge structures
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

#ifndef INCLUDED_BRIDGEANALYSIS_DEADLOADCASE_H_
#define INCLUDED_BRIDGEANALYSIS_DEADLOADCASE_H_
#pragma once

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <BridgeAnalysis\BridgeAnalysisExp.h>
#include <BridgeAnalysis\LoadCase.h>


// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//

// MISCELLANEOUS
//

class bamDeadLoadCase : public bamLoadCase
{
public:
   // LIFECYCLE
   bamDeadLoadCase(IDType loadCaseIdx);
   virtual ~bamDeadLoadCase(); 

   // OPERATORS
   // OPERATIONS
   void Apply();
   void Remove();

   void ExcludeSpan(SpanIDType spanElementId);
   void ExcludeSupport(SupportIDType supportElementId);
   void ExcludeSpans(SpanIDType** ppSpans,SpanIndexType count);
   void ExcludeSupports(SupportIDType** ppSupports,SupportIndexType count);

   // ACCESS
   // INQUIRY

protected:
   // DATA MEMBERS
   // LIFECYCLE
   // OPERATORS
   // OPERATIONS
   // ACCESS
   // INQUIRY

private:
   // DATA MEMBERS
   std::vector<SpanIDType> m_ExcludedSpans;
   std::vector<SupportIDType> m_ExcludedSupports;

   // :COMPILER: rab 11.17.96 : Supressed warning
   // :FILE: DeadLoadCase.h
   // Warning C4251 has been suppressed.
   // not be exported.
   #pragma warning (disable : 4251)
   LoadContainer m_PrivateLoads; // loads generated by this load case

   // LIFECYCLE
   bamDeadLoadCase(const bamDeadLoadCase& /*rDeadLoadCase*/);

   // OPERATORS
   bamDeadLoadCase& operator = (const bamDeadLoadCase& /*rDeadLoadCase*/);

   // OPERATIONS
   // ACCESS
   // INQUIRY

public:
   // GROUP: DEBUG
   #if defined _DEBUG
   //------------------------------------------------------------------------
   // Returns true if the object is in a valid state, otherwise returns false.
   virtual bool AssertValid() const;

   //------------------------------------------------------------------------
   // Dumps the contents of the object to the given dump context.
   virtual void Dump(dbgDumpContext& os) const;
   #endif // _DEBUG

   #if defined _UNITTEST
   //------------------------------------------------------------------------
   // Runs a self-diagnostic test.  Returns true if the test passed,
   // otherwise false.
   static bool TestMe(dbgLog& rlog);
   #endif // _UNITTEST
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif //  INCLUDED_BRIDGEANALYSIS_DEADLOADCASE_H_
