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

#include <RCSection/RCSectionLib.h>
#include <RCSection/UnitTest.h>
#include <RCSection/GeneralSection.h>
#include <RCSection/GeneralSectionSlice.h>
#include <RCSection/GeneralSectionSolution.h>
#include <RCSection/GeneralSectionSolver.h>
#include <RCSection/MomentCapacitySolution.h>
#include <RCSection/MomentCapacitySolver.h>
#include <RCSection/CircularColumn.h>
#include <RCSection/ConstantStressBlockFactor.h>
#include <RCSection/VariableStressBlockFactor.h>
#include <RCSection/MomentCurvatureSolution.h>
#include <RCSection/MomentCurvatureSolver.h>
#include <RCSection/InteractionCurveSolution.h>
#include <RCSection/AxialInteractionCurveSolver.h>
#include <RCSection/MomentInteractionCurveSolver.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool WBFL::RCSection::UnitTest::TestMe(WBFL::Debug::Log& rlog)
{
   bool tst = true;

#if defined _UNITTEST
   tst &= GeneralSection::TestMe(rlog);
   tst &= GeneralSectionSlice::TestMe(rlog);
   tst &= GeneralSectionSolution::TestMe(rlog);
   tst &= GeneralSectionSolver::TestMe(rlog);
   tst &= MomentCapacitySolution::TestMe(rlog);
   tst &= MomentCapacitySolver::TestMe(rlog);
   tst &= CircularColumn::TestMe(rlog);
   tst &= ConstantStressBlockFactor::TestMe(rlog);
   tst &= VariableStressBlockFactor::TestMe(rlog);
   tst &= MomentCurvatureSolution::TestMe(rlog);
   tst &= MomentCurvatureSolver::TestMe(rlog);
   tst &= InteractionCurveSolution::TestMe(rlog);
   tst &= AxialInteractionCurveSolver::TestMe(rlog);
   tst &= MomentInteractionCurveSolver::TestMe(rlog);
#endif

   return tst;
}


void WBFL::RCSection::UnitTest::DumpSolution(const std::shared_ptr<GeneralSection>& section, const std::unique_ptr<MomentCapacitySolution>& solution)
{
   const auto& general_solution = solution->GetGeneralSectionSolution();

   IndexType nSlices = general_solution->GetSliceCount();
   std::cout << "Element, Material, Top, Bottom, Ycg, Area, initial strain, incremental strain, total strain, stress, force, moment" << std::endl;
   for (IndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      const auto& slice = general_solution->GetSlice(sliceIdx);
      Float64 area = slice->GetArea();
      const auto& cg = slice->GetCentroid();
      Float64 initial_strain = slice->GetInitialStrain();
      Float64 incremental_strain = slice->GetIncrementalStrain();
      Float64 total_strain = slice->GetTotalStrain();
      Float64 fgStress = slice->GetForegroundStress();

      const auto& ss = slice->GetForegroundMaterial();
      auto material_name = ss->GetName();

      Float64 y = cg.Y();

      const auto& shape = slice->GetShape();
      auto rect = shape->GetBoundingBox();
      auto top = rect.Top();
      auto bottom = rect.Bottom();

      IndexType shapeIdx = slice->GetShapeIndex();

      const auto& shape_name = section->GetName(shapeIdx);

      std::wcout << shape_name << ", " << material_name << ", " << top << ", " << bottom << ", " << y << ", " << area << ", " << initial_strain << ", " << incremental_strain << ", " << total_strain << ", " << fgStress << ", " << area * fgStress << ", " << area * fgStress * y << std::endl;
   }
}
