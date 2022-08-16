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

#include <RCSection\RCSectionLib.h>
#include <RCSection\MomentCapacitySolver.h>
#include "MomentCapacitySolverImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WBFL::RCSection;

MomentCapacitySolver::MomentCapacitySolver()
{
   m_pImpl = std::make_unique<MomentCapacitySolverImpl>();
}

MomentCapacitySolver::~MomentCapacitySolver() = default;

void MomentCapacitySolver::SetSection(const std::shared_ptr<const IGeneralSection>& section)
{
   m_pImpl->SetSection(section);
}

const std::shared_ptr<const IGeneralSection>& MomentCapacitySolver::GetSection() const
{
   return m_pImpl->GetSection();
}

void MomentCapacitySolver::SetSlices(IndexType nSlices)
{
   m_pImpl->SetSlices(nSlices);
}

IndexType MomentCapacitySolver::GetSlices() const
{
   return m_pImpl->GetSlices();
}

void MomentCapacitySolver::SetSliceGrowthFactor(Float64 sliceGrowthFactor)
{
   m_pImpl->SetSliceGrowthFactor(sliceGrowthFactor);
}

Float64 MomentCapacitySolver::GetSliceGrowthFactor() const
{
   return m_pImpl->GetSliceGrowthFactor();
}

void MomentCapacitySolver::SetTolerance(Float64 tolerance)
{
   m_pImpl->SetTolerance(tolerance);
}

Float64 MomentCapacitySolver::GetTolerance() const
{
   return m_pImpl->GetTolerance();
}

void MomentCapacitySolver::SetMaxIterations(IndexType maxIter)
{
   m_pImpl->SetMaxIterations(maxIter);
}

IndexType MomentCapacitySolver::GetMaxIterations() const
{
   return m_pImpl->GetMaxIterations();
}

std::unique_ptr<MomentCapacitySolution> MomentCapacitySolver::Solve(Float64 Fz, Float64 angle, Float64 k_or_ec, Float64 strainLocation, SolutionMethod solutionMethod) const
{
   return m_pImpl->Solve(Fz, angle, k_or_ec, strainLocation, solutionMethod);
}

WBFL::Geometry::Point2d MomentCapacitySolver::GetPlasticCentroid() const
{
   return m_pImpl->GetPlasticCentroid();
}

void MomentCapacitySolver::GetCompressionLimit(Float64* Fz, Float64* Mx, Float64* My, Float64* eo) const
{
   m_pImpl->GetCompressionLimit(Fz, Mx, My, eo);
}

void MomentCapacitySolver::GetTensionLimit(Float64* Fz, Float64* Mx, Float64* My, Float64* eo) const
{
   m_pImpl->GetTensionLimit(Fz, Mx, My, eo);
}

#if defined _UNITTEST
#include <Units/Units.h>
#include <Materials/Materials.h>
#include <GeomModel/GeomModel.h>
bool MomentCapacitySolver::TestMe(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("MomentCapacitySolver");
   TRY_TESTME(Test1(rlog));
   TRY_TESTME(Test2(rlog));
   TRY_TESTME(Test3(rlog));
   TRY_TESTME(Test4(rlog));
   TESTME_EPILOG("MomentCapacitySolver");
}

bool MomentCapacitySolver::Test1(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("MomentCapacitySolver::Test1");

   // base units of kip and ksi
   WBFL::Units::AutoSystem au;
   WBFL::Units::System::SetSystemUnits(WBFL::Units::Measure::_12KSlug, WBFL::Units::Measure::Inch, WBFL::Units::Measure::Second, WBFL::Units::Measure::Fahrenheit, WBFL::Units::Measure::Degree);

   std::shared_ptr<GeneralSection> section(std::make_shared<GeneralSection>());

   //
   // materials
   //

   // concrete
   std::shared_ptr<WBFL::Materials::UnconfinedConcreteModel> concrete(std::make_shared<WBFL::Materials::UnconfinedConcreteModel>(_T("Concrete"), 4.0));

   // rebar
   std::shared_ptr<WBFL::Materials::RebarModel> rebar(std::make_shared<WBFL::Materials::RebarModel>(_T("Rebar"), 60.0, 29000.0, 0.11));

   //
   // shapes
   //

   // main beam
   Float64 H = WBFL::Units::ConvertToSysUnits(4, WBFL::Units::Measure::Feet);
   Float64 W = WBFL::Units::ConvertToSysUnits(2, WBFL::Units::Measure::Feet);
   WBFL::Geometry::Rectangle beam;
   beam.SetHeight(H);
   beam.SetWidth(W);

   // #10 rebar
   Float64 Ab = 1.27;
   WBFL::Geometry::GenericShape bar1(Ab, WBFL::Geometry::Point2d((W / 2 - 2), (H / 2 - 2)));
   WBFL::Geometry::GenericShape bar2(Ab, WBFL::Geometry::Point2d(-(W / 2 - 2), (H / 2 - 2)));
   WBFL::Geometry::GenericShape bar3(Ab, WBFL::Geometry::Point2d(-(W / 2 - 2), -(H / 2 - 2)));
   WBFL::Geometry::GenericShape bar4(Ab, WBFL::Geometry::Point2d((W / 2 - 2), -(H / 2 - 2)));

   section->AddShape(_T("Beam"), beam, concrete, nullptr, nullptr, 1.0, true);
   section->AddShape(_T("Bar 1"), bar1, rebar, nullptr, nullptr, 1.0);
   section->AddShape(_T("Bar 2"), bar2, rebar, nullptr, nullptr, 1.0);
   section->AddShape(_T("Bar 3"), bar3, rebar, nullptr, nullptr, 1.0);
   section->AddShape(_T("Bar 4"), bar4, rebar, nullptr, nullptr, 1.0);

   MomentCapacitySolver solver;
   solver.SetSlices(10);
   solver.SetSliceGrowthFactor(3);
   solver.SetTolerance(0.001);
   solver.SetSection(section);

   auto solution = solver.Solve(0.00, 0.00, -0.003, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain);

   Float64 Fz = solution->GetFz();
   Float64 Mx = solution->GetMx();
   Float64 My = solution->GetMy();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -6888.22801));
   TRY_TESTME(IsZero(My));

   auto strainPlane = solution->GetStrainPlane();

   Float64 ec = strainPlane.GetZ(0.00, H / 2);
   TRY_TESTME(IsEqual(ec, -0.003));

   solution = solver.Solve(0.00, M_PI, -0.003, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); //compression bottom, use angle = M_PI

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, 6888.22801));
   TRY_TESTME(IsZero(My));

   ec = strainPlane.GetZ(0.00, -H / 2);
   TRY_TESTME(IsEqual(ec, -0.003));

   solution = solver.Solve(0.00, PI_OVER_2, -0.003, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); //compression left, use angle = PI_OVER_2

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsZero(Mx));
   TRY_TESTME(IsEqual(My, -3374.92488));

   ec = strainPlane.GetZ(-W / 2, 0.00);
   TRY_TESTME(IsEqual(ec, -0.003));

   solution = solver.Solve(0.00, 3*PI_OVER_2, -0.003, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); //compression left, use angle = 3*PI_OVER_2

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsZero(Mx));
   TRY_TESTME(IsEqual(My, 3374.92488));

   ec = strainPlane.GetZ(W / 2, 0.00);
   TRY_TESTME(IsEqual(ec, -0.003));

   // make bar3 and bar4 be unbonded for 5 ft
   Float64 Le = WBFL::Units::ConvertToSysUnits(5.0, WBFL::Units::Measure::Feet);

   section->SetElongationLength(3, Le);
   section->SetElongationLength(4, Le);

   solution = solver.Solve(0.00, M_PI, -0.003, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); // compression bottom, use angle = M_PI

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, 6884.17511, 0.0001));
   TRY_TESTME(IsZero(My));

   ec = strainPlane.GetZ(0.00, -H / 2);
   TRY_TESTME(IsEqual(ec, -0.003));

   TESTME_EPILOG("MomentCapacitySolver::Test1");
}

bool MomentCapacitySolver::Test2(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("MomentCapacitySolver::Test2");
   // base units of kip and ksi
   WBFL::Units::AutoSystem au;
   WBFL::Units::System::SetSystemUnits(WBFL::Units::Measure::_12KSlug, WBFL::Units::Measure::Inch, WBFL::Units::Measure::Second, WBFL::Units::Measure::Fahrenheit, WBFL::Units::Measure::Degree);

   std::shared_ptr<GeneralSection> section(std::make_shared<GeneralSection>());

   //
   // materials
   //

   // concrete
   std::shared_ptr<WBFL::Materials::UHPCModel> concrete(std::make_shared<WBFL::Materials::UHPCModel>(_T("UHPC")));
   concrete->SetFc(22.0);
   concrete->SetCompressiveStrainLimit(-0.0035);
   concrete->SetFtcr(1.2);
   concrete->SetFtloc(1.2);
   concrete->Set_etloc(0.0045);

   // rebar
   std::shared_ptr<WBFL::Materials::RebarModel> rebar(std::make_shared<WBFL::Materials::RebarModel>(_T("Rebar"), 60.0, 29000.0, 0.11));

   //
   // shapes
   //

   // main beam
   Float64 H = WBFL::Units::ConvertToSysUnits(4, WBFL::Units::Measure::Feet);
   Float64 W = WBFL::Units::ConvertToSysUnits(2, WBFL::Units::Measure::Feet);
   WBFL::Geometry::Rectangle beam;
   beam.SetHeight(H);
   beam.SetWidth(W);

   // #10 rebar
   Float64 Ab = 1.27;
   WBFL::Geometry::GenericShape bar1(Ab, WBFL::Geometry::Point2d((W / 2 - 2), (H / 2 - 2)));
   WBFL::Geometry::GenericShape bar2(Ab, WBFL::Geometry::Point2d(-(W / 2 - 2), (H / 2 - 2)));
   WBFL::Geometry::GenericShape bar3(Ab, WBFL::Geometry::Point2d(-(W / 2 - 2), -(H / 2 - 2)));
   WBFL::Geometry::GenericShape bar4(Ab, WBFL::Geometry::Point2d((W / 2 - 2), -(H / 2 - 2)));

   section->AddShape(_T("Beam"), beam, concrete, nullptr, nullptr, 1.0, true);
   section->AddShape(_T("Bar 1"), bar1, rebar, nullptr, nullptr, 1.0);
   section->AddShape(_T("Bar 2"), bar2, rebar, nullptr, nullptr, 1.0);
   section->AddShape(_T("Bar 3"), bar3, rebar, nullptr, nullptr, 1.0);
   section->AddShape(_T("Bar 4"), bar4, rebar, nullptr, nullptr, 1.0);

   MomentCapacitySolver solver;
   solver.SetSlices(10);
   solver.SetSliceGrowthFactor(3);
   solver.SetTolerance(0.001);
   solver.SetSection(section);

   // Use strain compatibility analysis to determine moment capacity at the onset of the crack localization strain
   auto solution = solver.Solve(0.00, 0.00, 0.0045, 0.0, MomentCapacitySolver::SolutionMethod::FixedTensionStrain); // compression top, use angle = 0
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == false);

   Float64 Fz = solution->GetFz();
   Float64 Mx = solution->GetMx();
   Float64 My = solution->GetMy();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -30283.2818122));
   TRY_TESTME(IsZero(My));

   auto strainPlane = solution->GetStrainPlane();

   Float64 ec = strainPlane.GetZ(0.00,-H / 2);
   TRY_TESTME(IsEqual(ec, 0.0045));

   // compute curvature
   Float64 k = ec / (H / 2);
   TRY_TESTME(IsEqual(k, 0.0045 / (H / 2)));


   // Solve again but this time set the strain at the bottom
   solution = solver.Solve(0.00, 0.00, 0.0045, -H/2, MomentCapacitySolver::SolutionMethod::FixedStrain); // compression top, use angle = 0
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == false);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -30283.27790, 0.0001));
   TRY_TESTME(IsZero(My));

   ec = strainPlane.GetZ(0.00, -H / 2);
   TRY_TESTME(IsEqual(ec, 0.0045));

   // compute curvature
   k = ec / (H / 2);
   TRY_TESTME(IsEqual(k, 0.0045 / (H / 2)));

   // Solve for concrete crushing
   // this example will have tension strains the the rebar that exceed the fracture limit)
   solution = solver.Solve(0.00, 0.0, -0.0035, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); // compression top, use angle = 0
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -33162.03191));
   TRY_TESTME(IsZero(My));

   ec = strainPlane.GetZ(0.00, H / 2);
   TRY_TESTME(IsEqual(ec, -0.0035));

   // compute curvature
   k = ec / (H / 2);
   TRY_TESTME(IsEqual(k, -0.0035 / (H / 2)));

   // Solve again but this time set the strain at the top
   // this example will have tension strains the the rebar that exceed the fracture limit)
   solution = solver.Solve(0.00, 0.0, -0.0035, H/2, MomentCapacitySolver::SolutionMethod::FixedStrain); // compression top, use angle = 0
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -33162.033845));
   TRY_TESTME(IsZero(My));

   ec = strainPlane.GetZ(0.00, H / 2);
   TRY_TESTME(IsEqual(ec, -0.0035));

   // compute curvature
   k = ec / (H / 2);
   TRY_TESTME(IsEqual(k, -0.0035 / (H / 2)));

   TESTME_EPILOG("MomentCapacitySolver::Test2");
}

bool MomentCapacitySolver::Test3(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("MomentCapacitySolver::Test3");

   // Source: Design Example of a UHPC Girder Bridge with a Conventional Concrete Deck (Graybeal, et.al.)
   // Presented April 5, 2021 to AASHTO T-10
   WBFL::Units::AutoSystem au;
   WBFL::Units::System::SetSystemUnits(WBFL::Units::Measure::_12KSlug, WBFL::Units::Measure::Inch, WBFL::Units::Measure::Second, WBFL::Units::Measure::Fahrenheit, WBFL::Units::Measure::Degree);

   WBFL::Geometry::NUBeam beam;
   beam.SetD1(3.8125);
   beam.SetD2(1.6875);
   beam.SetD3(35.5);
   beam.SetD4(5.5);
   beam.SetD5(7.5);
   beam.SetR1(8);
   beam.SetR2(8);
   beam.SetR3(0);
   beam.SetR4(2);
   beam.SetT(3.5/*6.5*/);
   beam.SetW1(21/*34*/);
   beam.SetW2(27/*30*/);
   beam.SetC1(0/*0.75*/);

   Float64 h = beam.GetHeight();

   // put top center of beam at (0,0)
   beam.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter, WBFL::Geometry::Point2d(0, 0));

   // haunch
   WBFL::Geometry::Rectangle haunch;
   haunch.SetHeight(1.5);
   haunch.SetWidth(21);
   haunch.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, beam.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter));

   // deck
   WBFL::Geometry::Rectangle deck;
   deck.SetHeight(8.5);
   deck.SetWidth(108);
   deck.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, haunch.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter));

   // strands
   WBFL::Geometry::GenericShape layer1(13 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 2)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer2(13 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 4)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer3(13 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 6)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer4( 9 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 8)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer5( 5 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 10)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer6( 1 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 12)); // 54" down from top + up 2" per layer to the center of layer

   // materials

   // deck
   std::shared_ptr<WBFL::Materials::UnconfinedConcreteModel> deck_concrete(std::make_shared<WBFL::Materials::UnconfinedConcreteModel>(_T("Deck"), 4.0));

   // girder
   std::shared_ptr<WBFL::Materials::UHPCModel> girder_concrete(std::make_shared<WBFL::Materials::UHPCModel>(_T("Girder")));
   girder_concrete->SetFc(22.0);
   girder_concrete->SetCompressiveStrainLimit(-0.0035);
   girder_concrete->SetFtcr(1.2);
   girder_concrete->SetFtloc(1.2);
   girder_concrete->Set_etloc(0.0045);

   // strand
   std::shared_ptr<WBFL::Materials::PSPowerFormulaModel> strand(std::make_shared<WBFL::Materials::PSPowerFormulaModel>(_T("Strand")));
   strand->SetStrandGrade(WBFL::Materials::StrandGrade::Grade270);

   Float64 ei = 166.2 / 28500; // fps = 166.2 ksi at Service III
   std::shared_ptr<WBFL::Geometry::Plane3d> initialStrain(std::make_shared<WBFL::Geometry::Plane3d>(ei));

   std::shared_ptr<GeneralSection> section(std::make_shared<GeneralSection>());
   section->AddShape(_T("Deck"), deck, deck_concrete, nullptr, nullptr, 1.0);
   section->AddShape(_T("Haunch"), haunch, deck_concrete, nullptr, nullptr, 1.0);
   section->AddShape(_T("Girder"), beam, girder_concrete, nullptr, nullptr, 1.0, true);
   section->AddShape(_T("Strand Layer 1"), layer1, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 2"), layer2, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 3"), layer3, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 4"), layer4, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 5"), layer5, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 6"), layer6, strand, nullptr, initialStrain, 1.0);

   MomentCapacitySolver solver;
   solver.SetSlices(50);
   solver.SetSliceGrowthFactor(1);
   solver.SetTolerance(0.0001);
   solver.SetSection(section);

   // Compute nominal capacity when the stress in the bottom layer of reinforcement is equal to 0.80fy
   // Msl and Ysl
   Float64 fu = 270;
   Float64 fy = 0.9 * fu;
   Float64 esl = 0.8 * fy / 27931;// 28500;
   Float64 Y = layer1.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::CenterCenter).Y();

   auto solution = solver.Solve(0.00, 0.00, esl - ei, Y, MomentCapacitySolver::SolutionMethod::FixedStrain);
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == false);
   //std::cout << "Nominal moment when stress in bottom layer of strand is equal to 0.80fy, Msl" << std::endl;
   //DumpSolution(section,solution);

   Float64 Fz, Mx, My;
   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   auto strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -178123.6634, 0.0001)); //-164292.0 (13691 k-ft) from FHWA example slides
   TRY_TESTME(IsZero(My));
   Float64 Msl = -Mx;

   Float64 ec = strainPlane.GetZ(0.00, Y);
   TRY_TESTME(IsEqual(ec, esl - ei));

   Float64 Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis below top of girder
   TRY_TESTME(IsEqual(Yna, -20.58941)); // 40.7-10" = 30.7" from top of girder from FHWN example slides

   ec = strainPlane.GetZ(0.00, -54); // strain at bottom of girder
   Float64 Ysl = ec / (54 + Yna); // curvature
   TRY_TESTME(IsEqual(Ysl, .000035925, 0.00000001)); // 0.0000545 from FHWA example slides

   // Nominal moment capacity at crack localization, Ml and Yl
   solution = solver.Solve(0.00, 0.00, 0.0045, 0.0, MomentCapacitySolver::SolutionMethod::FixedTensionStrain);
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == false);
   //std::cout << "Nominal moment at crack localization, Ml" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -222618.3053, 0.0001)); // -215664.0 (17972 k-ft) from FHWA example slides
   TRY_TESTME(IsZero(My));
   Float64 Ml = -Mx;

   ec = strainPlane.GetZ(0.00, -h);
   TRY_TESTME(IsEqual(ec, 0.0045));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis below top of girder
   TRY_TESTME(IsEqual(Yna, -10.00682)); // 25.5-10" = 15.5" from top of girder from FHWN example slides

   Float64 Yl = ec / (54 + Yna);
   TRY_TESTME(IsEqual(Yl, 0.0001022885, 0.00000001)); // 0.0001167 from FHWA example slides

   // Nominal moment capacity at compression strain limit in UHPC section, Mc, Yc
   // fixed strain at top of girder (not top of section/top of deck)
   solution = solver.Solve(0.00, 0.00, -0.0035, 0.0, MomentCapacitySolver::SolutionMethod::FixedStrain);  // strain in deck concrete exceeds crushing limit of -0.003
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);
   //std::cout << "Nominal moment at compression strain limit in UHPC section, Mc" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -237920.689141));
   TRY_TESTME(IsZero(My));
   Float64 Mc = -Mx;

   ec = strainPlane.GetZ(0.00, 10.00); // strain at top of deck
   TRY_TESTME(IsEqual(ec, -0.00824344));

   ec = strainPlane.GetZ(0.00, -h); // strain at bottom of beam
   TRY_TESTME(IsEqual(ec, 0.0221146));

   ec = strainPlane.GetZ(0.00, 0.00);
   TRY_TESTME(IsEqual(ec, -0.0035));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis
   TRY_TESTME(IsEqual(Yna, -7.378604));

   Float64 Yc = ec / Yna;
   TRY_TESTME(IsEqual(Yc, 0.00047434446, 0.000000001));

   // Nominal moment capacity at compression strain limit in deck, Md, Yd
   solution = solver.Solve(0.00, 0.00, -0.003, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); // exceeds tensile capacity of UHPC
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);
   //std::cout << "Nominal moment at compression strain limit in deck, Md" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -228894.0801, 0.0001));
   TRY_TESTME(IsZero(My));
   Float64 Md = -Mx;

   ec = strainPlane.GetZ(0.00, 10.0);
   TRY_TESTME(IsEqual(ec, -0.003));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis
   TRY_TESTME(IsEqual(Yna, -9.73137));

   Float64 Yd = ec / (Yna - 10);
   TRY_TESTME(IsEqual(Yd, 0.000152042, 0.000000001));

   // Nominal moment capacity at maximum usable strain of reinforcement, Mt, Yt
   solution = solver.Solve(0.00, 0.00, 0.035 - ei, Y, MomentCapacitySolver::SolutionMethod::FixedStrain); // strain exceeds crushing strain of UHPC
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);
   //std::cout << "Nominal moment at maximum usable strain in reinforcement, Mt" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -239340.350413));
   TRY_TESTME(IsZero(My));
   Float64 Mt = -Mx;

   ec = strainPlane.GetZ(0.00, Y);
   TRY_TESTME(IsEqual(ec, 0.035 - ei));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis
   TRY_TESTME(IsEqual(Yna, -6.588564));

   Float64 Yt = ec / (Yna - Y);
   TRY_TESTME(IsEqual(Yt, 0.000642314, 0.000000001));

   // Ductility ratio
   IndexType i = MinIndex(fabs(Yl), fabs(Yc), fabs(Yd), fabs(Yt)); // want least curvature
   Float64 Mn = (i == 0 ? Ml : i == 1 ? Mc : i == 2 ? Md : Mt);
   Float64 Yn = (i == 0 ? Yl : i == 1 ? Yc : i == 2 ? Yd : Yt);
   Float64 mu = Yn / Ysl;
   TRY_TESTME(IsEqual(mu, 2.8473, 0.0001)); // 2.14 from FHWA example slides

   Float64 phi = 0.75 + 0.15 * (mu - 1.0) / (3.0 - 1.0);
   phi = ::ForceIntoRange(0.75, phi, 0.90);
   TRY_TESTME(IsEqual(phi, 0.8885, 0.0001)); // 0.83 from FHWA example slides
   //Float64 Mr = phi*Mn;

   TESTME_EPILOG("MomentCapacitySolver::Test3");
}

bool MomentCapacitySolver::Test4(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("MomentCapacitySolver::Test4");

   // Source: Design Example of a UHPC Girder Bridge with a Conventional Concrete Deck (Graybeal, et.al.)
   // Presented April 5, 2021 to AASHTO T-10
   //
   // This differs from the test above in that it applies initial strains to the concrete

   WBFL::Units::AutoSystem au;
   WBFL::Units::System::SetSystemUnits(WBFL::Units::Measure::_12KSlug, WBFL::Units::Measure::Inch, WBFL::Units::Measure::Second, WBFL::Units::Measure::Fahrenheit, WBFL::Units::Measure::Degree);

   WBFL::Geometry::NUBeam beam;
   beam.SetD1(3.8125);
   beam.SetD2(1.6875);
   beam.SetD3(35.5);
   beam.SetD4(5.5);
   beam.SetD5(7.5);
   beam.SetR1(8);
   beam.SetR2(8);
   beam.SetR3(0);
   beam.SetR4(2);
   beam.SetT(3.5/*6.5*/);
   beam.SetW1(21/*34*/);
   beam.SetW2(27/*30*/);
   beam.SetC1(0/*0.75*/);

   Float64 h = beam.GetHeight();

   // put top center of beam at (0,0)
   beam.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter, WBFL::Geometry::Point2d(0, 0));

   // haunch
   WBFL::Geometry::Rectangle haunch;
   haunch.SetHeight(1.5);
   haunch.SetWidth(21);
   haunch.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, beam.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter));

   // deck
   WBFL::Geometry::Rectangle deck;
   deck.SetHeight(8.5);
   deck.SetWidth(108);
   deck.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, haunch.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter));

   // strands
   WBFL::Geometry::GenericShape layer1(13 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 2)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer2(13 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 4)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer3(13 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 6)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer4(9 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 8)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer5(5 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 10)); // 54" down from top + up 2" per layer to the center of layer
   WBFL::Geometry::GenericShape layer6(1 * 0.294, WBFL::Geometry::Point2d(0.0, -h + 12)); // 54" down from top + up 2" per layer to the center of layer

   //
   // materials
   //

   // deck
   std::shared_ptr<WBFL::Materials::UnconfinedConcreteModel> deck_concrete(std::make_shared<WBFL::Materials::UnconfinedConcreteModel>(_T("Deck"), 4.0));

   // girder
   std::shared_ptr<WBFL::Materials::UHPCModel> girder_concrete(std::make_shared<WBFL::Materials::UHPCModel>(_T("Girder")));
   girder_concrete->SetFc(22.0);
   girder_concrete->SetCompressiveStrainLimit(-0.0035);
   girder_concrete->SetFtcr(1.2);
   girder_concrete->SetFtloc(1.2);
   girder_concrete->Set_etloc(0.0045);

   // strand
   std::shared_ptr<WBFL::Materials::PSPowerFormulaModel> strand(std::make_shared<WBFL::Materials::PSPowerFormulaModel>(_T("Strand")));
   strand->SetStrandGrade(WBFL::Materials::StrandGrade::Grade270);

   // Initial Strains
   WBFL::Geometry::Point3d p1, p2, p3; // use three points to define a plane

   // Deck
   // 4 ksi conventional concrete, E = 2500*f'c^0.33 = 3950 ksi
   p1.Move(-10000, 0, -0.88 / 3950); // 0.88 ksi compression bottom of deck at Service III (see slide 33)
   p2.Move(10000, 0, -0.88 / 3950);
   p3.Move(0, 10, -1.33 / 3950); // 1.33 ksi compression at top of deck at Service III (see slide 33)
   std::shared_ptr<WBFL::Geometry::Plane3d> deck_initial_strain(std::make_shared<WBFL::Geometry::Plane3d>(p1, p2, p3));

   // Girder
   p1.Move(-10000, 0, -11.02 / 6933); // 11.02 ksi compression at top of girder at Service III (see slide 33)
   p2.Move(10000, 0, -11.02 / 6933);
   p3.Move(0, -h, 0.99 / 6933); // 0.99 ksi tension at bottom of girder at Service III (see slide 33)
   std::shared_ptr<WBFL::Geometry::Plane3d> girder_initial_strain(std::make_shared<WBFL::Geometry::Plane3d>(p1, p2, p3));

   Float64 ei = 166.2 / 28500; // fps = 166.2 ksi at Service III
   std::shared_ptr<WBFL::Geometry::Plane3d> initialStrain(std::make_shared<WBFL::Geometry::Plane3d>(ei));

   // Get a general section
   std::shared_ptr<GeneralSection> section(std::make_shared<GeneralSection>());
   section->AddShape(_T("Deck"), deck, deck_concrete, nullptr, deck_initial_strain, 1.0);
   section->AddShape(_T("Haunch"), haunch, deck_concrete, nullptr, deck_initial_strain, 1.0);
   section->AddShape(_T("Girder"), beam, girder_concrete, nullptr, girder_initial_strain, 1.0, true);
   section->AddShape(_T("Strand Layer 1"), layer1, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 2"), layer2, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 3"), layer3, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 4"), layer4, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 5"), layer5, strand, nullptr, initialStrain, 1.0);
   section->AddShape(_T("Strand Layer 6"), layer6, strand, nullptr, initialStrain, 1.0);

   MomentCapacitySolver solver;
   solver.SetSlices(50);
   solver.SetSliceGrowthFactor(1);
   solver.SetTolerance(0.0001);
   solver.SetSection(section);

   // Compute nominal capacity when the stress in the bottom layer of reinforcement is equal to 0.80fy (slide 50)
   // Msl and Ysl
   Float64 fu = 270;
   Float64 fy = 0.9 * fu;
   Float64 esl = 0.8 * fy / 28500;
   Float64 Y = layer1.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::CenterCenter).Y();

   auto solution = solver.Solve(0.00, 0.00, esl - ei, Y, MomentCapacitySolver::SolutionMethod::FixedStrain);
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == false);
   //std::cout << "Nominal moment when stress in bottom layer of strand is equal to 0.80fy, Msl" << std::endl;
   //DumpSolution(section,solution);

   Float64 Fz, Mx, My;
   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   auto strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -161496.2591 /*13458 k-ft*/, 0.0001)); //-164292.0 = 13691 k-ft from FHWA example slides
   TRY_TESTME(IsZero(My));
   Float64 Msl = -Mx;

   Float64 ec = strainPlane.GetZ(0.00, Y);
   Float64 _ec = girder_initial_strain->GetZ(0, Y); // initial strain in girder concrete
   TRY_TESTME(IsEqual(ec - _ec, esl - ei));

   const auto& general_solution = solution->GetGeneralSectionSolution();
   auto slices = general_solution->FindSlices(3); // shape 3 is the lowest strand
   auto nSlices = slices.size();
   TRY_TESTME(nSlices == 1);
   const auto* slice = slices[0];
   ec = slice->GetTotalStrain();
   TRY_TESTME(IsEqual(ec, esl));

   Float64 Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis below top of girder
   TRY_TESTME(IsEqual(Yna, -31.85653)); // 40.7-10" = 30.7" from top of girder from FHWN example slides

   ec = strainPlane.GetZ(0.00, -h); // strain at bottom of girder
   TRY_TESTME(IsEqual(ec, 0.00117416, 0.00000001)); // 0.00127 from FHWA example slides

   Float64 Ysl = ec / (h + Yna); // curvature
   TRY_TESTME(IsEqual(Ysl, 0.00005302513, 0.00000001)); // 0.0000545 from FHWA example slides

   // Nominal moment capacity at crack localization, Ml and Yl (slide 51)
   solution = solver.Solve(0.00, 0.00, 0.0045 - 0.99 / 6933, 0.0, MomentCapacitySolver::SolutionMethod::FixedTensionStrain);
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == false);
   //std::cout << "Nominal moment at crack localization, Ml" << std::endl;
   //DumpSolution(section, solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -215602.84328 /*17966.9 k-ft*/)); // -215664.0 = 17972 k-ft from FHWA example slides
   TRY_TESTME(IsZero(My));
   Float64 Ml = -Mx;

   ec = strainPlane.GetZ(0.00, -h);
   TRY_TESTME(IsEqual(ec, 0.0045));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis below top of girder
   TRY_TESTME(IsEqual(Yna, -15.61227)); // 25.5-10" = 15.5" from top of girder from FHWN example slides

   Float64 Yl = ec / (h + Yna);
   TRY_TESTME(IsEqual(Yl, 0.00011722496, 0.00000001)); // 0.0001167 from FHWN example slides

   // Nominal moment capacity at compression strain limit in UHPC section, Mc, Yc
   // fixed strain at top of girder (not top of section/top of deck)
   // this case not in FHWA example
   solution = solver.Solve(0.00, 0.00, -0.0035 + 11.02 / 6933, 0.0, MomentCapacitySolver::SolutionMethod::FixedStrain);  // strain in deck concrete exceeds crushing limit of -0.003
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);
   //std::cout << "Nominal moment at compression strain limit in UHPC section, Mc" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -238146.804912));
   TRY_TESTME(IsZero(My));
   Float64 Mc = -Mx;

   ec = strainPlane.GetZ(0.00, 10.00); // strain at top of deck
   TRY_TESTME(IsEqual(ec, -0.008287341));

   ec = strainPlane.GetZ(0.00, -h); // strain at bottom of beam
   TRY_TESTME(IsEqual(ec, 0.02235164));

   ec = strainPlane.GetZ(0.00, 0.00);
   TRY_TESTME(IsEqual(ec, -0.0035));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis
   TRY_TESTME(IsEqual(Yna, -7.310948));

   Float64 Yc = ec / Yna;
   TRY_TESTME(IsEqual(Yc, 0.000478734, 0.000000001));

   // Nominal moment capacity at compression strain limit in deck, Md, Yd
   // this case not in FHWA example
   solution = solver.Solve(0.00, 0.00, -0.003 + 1.33 / 3950, 0.0, MomentCapacitySolver::SolutionMethod::FixedCompressionStrain); // exceeds tensile capacity of UHPC
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);
   //std::cout << "Nominal moment at compression strain limit in deck, Md" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -234918.2945, 0.0001));
   TRY_TESTME(IsZero(My));
   Float64 Md = -Mx;

   ec = strainPlane.GetZ(0.00, 10.0); // this is at top of deck, using girder strain plane
   _ec = girder_initial_strain->GetZ(0, 10.0); // initial strain in girder concrete
   ec -= _ec; // remove effect of girder initial strain
   _ec = deck_initial_strain->GetZ(0, 10.);
   ec += _ec; // add effect of deck initial strain
   TRY_TESTME(IsEqual(ec, -0.003));


   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis
   TRY_TESTME(IsEqual(Yna, -8.029146));

   Float64 Yd = ec / (Yna - 10);
   TRY_TESTME(IsEqual(Yd, 0.000166397, 0.000000001));

   // Nominal moment capacity at maximum usable strain of reinforcement, Mt, Yt
   // this case not in FHWA example
   solution = solver.Solve(0.00, 0.00, 0.035 - ei, Y, MomentCapacitySolver::SolutionMethod::FixedStrain); // strain exceeds crushing strain of UHPC
   TRY_TESTME(solution->GetGeneralSectionSolution()->ExceededStrainLimits() == true);
   //std::cout << "Nominal moment at maximum usable strain in reinforcement, Mt" << std::endl;
   //DumpSolution(section,solution);

   Fz = solution->GetFz();
   Mx = solution->GetMx();
   My = solution->GetMy();
   strainPlane = solution->GetStrainPlane();

   TRY_TESTME(IsZero(Fz, 0.001));
   TRY_TESTME(IsEqual(Mx, -239345.576092));
   TRY_TESTME(IsZero(My));
   Float64 Mt = -Mx;

   ec = strainPlane.GetZ(0.00, Y); // strain in girder concrete
   _ec = girder_initial_strain->GetZ(0, Y); // initial strain in girder concrete
   TRY_TESTME(IsEqual(ec - _ec, 0.035 - ei));

   const auto& general_solution2 = solution->GetGeneralSectionSolution();
   slices = general_solution2->FindSlices(3); // shape 3 is the lowest strand
   nSlices = slices.size();
   TRY_TESTME(nSlices == 1);
   slice = slices[0];
   ec = slice->GetTotalStrain();
   TRY_TESTME(IsEqual(ec, 0.035));

   Yna = strainPlane.GetY(0.0, 0.0); // location of neutral axis
   TRY_TESTME(IsEqual(Yna, -6.582841));

   Float64 Yt = ec / (Yna - Y);
   TRY_TESTME(IsEqual(Yt, 0.000770634, 0.000000001));

   // Ductility ratio
   IndexType i = MinIndex(fabs(Yl), fabs(Yc), fabs(Yd), fabs(Yt)); // want least curvature
   Float64 Mn = (i == 0 ? Ml : i == 1 ? Mc : i == 2 ? Md : Mt);
   Float64 Yn = (i == 0 ? Yl : i == 1 ? Yc : i == 2 ? Yd : Yt);
   Float64 mu = Yn / Ysl;
   TRY_TESTME(IsEqual(mu, 2.2107, 0.0001)); // 2.14 from FHWA example slides

   Float64 phi = 0.75 + 0.15 * (mu - 1.0) / (3.0 - 1.0);
   phi = ::ForceIntoRange(0.75, phi, 0.90);
   TRY_TESTME(IsEqual(phi, 0.8408, 0.0001)); // 0.83 from FHWA example
   //Float64 Mr = phi*Mn;

   TESTME_EPILOG("MomentCapacitySolver::Test4");
}


#endif // _UNITTEST
