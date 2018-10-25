///////////////////////////////////////////////////////////////////////
// Stability
// Copyright � 1999-2018  Washington State Department of Transportation
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

#include <Stability\StabilityLib.h>
#include <Stability\LiftingStabilityReporter.h>

#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

stbLiftingStabilityReporter::stbLiftingStabilityReporter()
{
}

void stbLiftingStabilityReporter::BuildSpecCheckChapter(const stbIGirder* pGirder,const stbILiftingStabilityProblem* pStabilityProblem,const stbLiftingCheckArtifact* pArtifact,rptChapter* pChapter,LPCTSTR lpszLocColumnLabel,Float64 offset)
{
   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   *pChapter << pPara;
   *pPara << _T("Check for Lifting in Casting Yard [5.5.4.3][5.9.4.1]") << rptNewLine;
   *pPara << _T("Lifting Stresses and Factor of Safety Against Cracking") << rptNewLine;

   pPara = new rptParagraph;
   *pChapter << pPara;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   INIT_SCALAR_PROTOTYPE(rptRcScalar, scalar, pDisplayUnits->Scalar);
   INIT_UV_PROTOTYPE( rptStressUnitValue,   stress,       pDisplayUnits->Stress, true);
   INIT_UV_PROTOTYPE( rptLengthUnitValue,  location,  pDisplayUnits->SpanLength,   false);
   INIT_UV_PROTOTYPE( rptSqrtPressureValue, tension_coeff, pDisplayUnits->SqrtPressure, false);

   const stbLiftingResults& results = pArtifact->GetLiftingResults();
   const stbLiftingCriteria& criteria = pArtifact->GetCriteria();

   CComPtr<ISegment> segment;
   pGirder->GetSegment(&segment);

   rptCDRatio cdRatio;

   LPCTSTR strCorner[] = {_T("Top Left"),_T("Top Right"),_T("Bottom Left"),_T("Bottom Right")};

   LPCTSTR strImpact[3];
   stbTypes::ImpactDirection impactDir[3];
   Float64 impactFactor[3];
   IndexType impactIndex[3];

   Float64 ImpactUp, ImpactDown;
   pStabilityProblem->GetImpact(&ImpactUp,&ImpactDown);
   IndexType nImpactCases = 0;
   strImpact[nImpactCases] = _T("No impact");
   impactDir[nImpactCases] = stbTypes::NoImpact;
   impactFactor[nImpactCases] = 1.0;
   impactIndex[stbTypes::NoImpact] = nImpactCases;

   if (!IsZero(ImpactUp))
   {
      nImpactCases++;
      strImpact[nImpactCases] = _T("Impact Up");
      impactDir[nImpactCases] = stbTypes::ImpactUp;
      impactFactor[nImpactCases] = 1.0 - ImpactUp;
      impactIndex[stbTypes::ImpactUp] = nImpactCases;
   }

   if (!IsZero(ImpactDown))
   {
      nImpactCases++;
      strImpact[nImpactCases] = _T("Impact Down");
      impactDir[nImpactCases] = stbTypes::ImpactDown;
      impactFactor[nImpactCases] = 1.0 + ImpactDown;
      impactIndex[stbTypes::ImpactDown] = nImpactCases;
   }

   stbTypes::WindType windLoadType;
   Float64 windLoad;
   pStabilityProblem->GetWindLoading(&windLoadType,&windLoad);
   IndexType nWindCases = IsZero(windLoad) ? 0 : 1;
   LPCTSTR strWindDir[] = {_T("Left"), _T("Right")};

   bool bLabelImpact = (0 < nImpactCases ? true : false);
   bool bLabelWind   = (0 < nWindCases ? true : false);

   for ( IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++ )
   {
      if ( !results.bIsStable[impactDir[impactCase]] )
      {
         if ( 0 < nImpactCases )
         {
            *pPara << color(Red) << _T("WARNING: Girder is unstable for the ") << strImpact[impactCase] << _T(" case. The center of gravity is above the roll axis.") << color(Black) << rptNewLine;
         }
         else
         {
            *pPara << color(Red) << _T("WARNING: Girder is unstable. The center of gravity is above the roll axis.") << color(Black) << rptNewLine;
         }
      }
   }

   bool bLambda = (lrfdVersionMgr::SeventhEditionWith2016Interims <= lrfdVersionMgr::GetVersion() ? true : false);

   *pPara << _T("Compression stress limit = -") << criteria.CompressionCoefficient << RPT_FCI << _T(" = ") << stress.SetValue(criteria.AllowableCompression) << rptNewLine;
   
   *pPara << _T("Tension stress limit = ") << tension_coeff.SetValue(criteria.TensionCoefficient);
   if ( bLambda )
   {
      *pPara << symbol(lambda);
   }
   *pPara << symbol(ROOT) << RPT_FCI;
   if ( criteria.bMaxTension )
   {
      *pPara << _T(" but not more than ") << stress.SetValue(criteria.MaxTension);
   }
   *pPara << _T(" = " ) << stress.SetValue(criteria.AllowableTension) << rptNewLine;

   if ( segment )
   {
      *pPara << _T("Tension stress limit = ") << tension_coeff.SetValue(criteria.TensionCoefficientWithRebar);
      if ( bLambda )
      {
         *pPara << symbol(lambda);
      }
      *pPara << symbol(ROOT) << RPT_FCI;
      *pPara << _T(" if bonded reinforcement sufficient to resist the tensile force in the concrete is provided = ") << stress.SetValue(criteria.AllowableTensionWithRebar) << rptNewLine;
   }
   else
   {
      *pPara << _T("Tensile stress limit with bonded reinforcement sufficient to resist tension force in concrete was not evaluated") << rptNewLine;
   }

   *pPara << _T("Minimum factor of safety against cracking = ") << scalar.SetValue(criteria.MinFScr) << rptNewLine;
   
   Float64 fcReqd = pArtifact->RequiredFcCompression();
   *pPara << RPT_FCI << _T(" required for compression stress = ");
   if ( 0 < fcReqd )
   {
      *pPara << stress.SetValue(pArtifact->RequiredFcCompression()) << rptNewLine;
   }
   else
   {
      *pPara << symbol(infinity) << rptNewLine;
   }

   fcReqd = pArtifact->RequiredFcTension();
   *pPara << RPT_FCI << _T(" required for tensile stress = ");
   if ( fcReqd < 0 )
   {
      ATLASSERT(fcReqd == -99999);
      *pPara << _T("Regardless of the concrete strength, the stress requirements will not be satisfied.") << rptNewLine;
   }
   else
   {
      *pPara << stress.SetValue(fcReqd) << rptNewLine;
   }

   if ( segment )
   {
      fcReqd = pArtifact->RequiredFcTensionWithRebar();
      *pPara << RPT_FCI << _T(" required for tensile stress with bonded reinforcement sufficient to resist the tensile force in the concrete = ");
      if ( fcReqd < 0 )
      {
         ATLASSERT(fcReqd == -99999);
         *pPara << _T("Regardless of the concrete strength, the stress requirements will not be satisfied.") << rptNewLine;
      }
      else
      {
         *pPara << stress.SetValue(fcReqd) << rptNewLine;
      }
   }

   *pPara << _T("The tensile stress case with the minimum C/D ratio governs") << rptNewLine;

   stress.ShowUnitTag(false);

   ColumnIndexType nColumns = 10;
   ColumnIndexType nColSpan = 2;
   if ( bLabelImpact )
   {
      nColumns += 2;
      nColSpan++;
   }

   if ( bLabelWind )
   {
      nColumns += 2;
      nColSpan++;
   }



   rptRcTable* pStressTable = rptStyleManager::CreateDefaultTable(nColumns,_T(""));
   *pPara << pStressTable << rptNewLine;

   ColumnIndexType col1 = 0;
   ColumnIndexType col2 = 0;
   pStressTable->SetRowSpan(0,col1,2);
   pStressTable->SetRowSpan(1,col2++,SKIP_CELL);

   if ( lpszLocColumnLabel )
   {
      (*pStressTable)(0,col1++) << COLHDR(rptRcStringLiteral(lpszLocColumnLabel),rptLengthUnitTag,pDisplayUnits->SpanLength);
   }
   else
   {
      (*pStressTable)(0,col1++) << COLHDR(_T("Dist from left") << rptNewLine << _T("end of girder"), rptLengthUnitTag, pDisplayUnits->SpanLength );
   }

   pStressTable->SetColumnSpan(0,col1,nColSpan);
   (*pStressTable)(0,col1++) << _T("Maximum Stress");
   (*pStressTable)(1,col2++) << COLHDR(RPT_STRESS(_T("")), rptStressUnitTag, pDisplayUnits->Stress );
   (*pStressTable)(1,col2++) << _T("Location");
   if ( bLabelImpact )
   {
      (*pStressTable)(1,col2++) << _T("Impact") << rptNewLine << _T("Direction");
   }
   if ( bLabelWind )
   {
      (*pStressTable)(1,col2++) << _T("Wind") << rptNewLine << _T("Direction");
   }

   pStressTable->SetRowSpan(0,col1,2);
   pStressTable->SetRowSpan(1,col2++,SKIP_CELL);
   (*pStressTable)(0,col1++) << COLHDR(RPT_STRESS(_T("Limit")), rptStressUnitTag, pDisplayUnits->Stress );

   pStressTable->SetRowSpan(0,col1,2);
   pStressTable->SetRowSpan(1,col2++,SKIP_CELL);
   (*pStressTable)(0,col1++) << _T("Tension") << rptNewLine << _T("Status") << rptNewLine << _T("(C/D)");

   pStressTable->SetColumnSpan(0,col1,nColSpan);
   (*pStressTable)(0,col1++) << _T("Minimum Stress");
   (*pStressTable)(1,col2++) << COLHDR(RPT_STRESS(_T("")), rptStressUnitTag, pDisplayUnits->Stress );
   (*pStressTable)(1,col2++) << _T("Location");
   if ( bLabelImpact )
   {
      (*pStressTable)(1,col2++) << _T("Impact") << rptNewLine << _T("Direction");
   }
   if ( bLabelWind )
   {
      (*pStressTable)(1,col2++) << _T("Wind") << rptNewLine << _T("Direction");
   }

   pStressTable->SetRowSpan(0,col1,2);
   pStressTable->SetRowSpan(1,col2++,SKIP_CELL);
   (*pStressTable)(0,col1++) << _T("Compression") << rptNewLine << _T("Status") << rptNewLine << _T("(C/D)");

   pStressTable->SetRowSpan(0,col1,2);
   pStressTable->SetRowSpan(1,col2++,SKIP_CELL);
   (*pStressTable)(0,col1++) << Sub2(_T("FS"),_T("cr"));

   pStressTable->SetRowSpan(0,col1,2);
   pStressTable->SetRowSpan(1,col2++,SKIP_CELL);
   (*pStressTable)(0,col1++) << _T("FS") << rptNewLine << _T("Status");

   pStressTable->SetNumberOfHeaderRows(2);
   for ( ColumnIndexType i = col1; i < pStressTable->GetNumberOfColumns(); i++ )
   {
      pStressTable->SetColumnSpan(0,i,SKIP_CELL);
   }

   RowIndexType row = pStressTable->GetNumberOfHeaderRows();
   for( const auto& sectionResult : results.vSectionResults)
   {
      ColumnIndexType col = 0;

      const stbIAnalysisPoint* pAnalysisPoint = pStabilityProblem->GetAnalysisPoint(sectionResult.AnalysisPointIndex);
      (*pStressTable)(row,col++) << rptRcStringLiteral(pAnalysisPoint->AsString(pDisplayUnits->SpanLength,offset,false));

      stbTypes::ImpactDirection impact;
      stbTypes::WindDirection wind;
      stbTypes::Corner corner;
      Float64 fAllow;
      bool bPassed;
      Float64 cd;
      pArtifact->GetControllingTensionCase(sectionResult,&impact,&wind,&corner,&fAllow,&bPassed,&cd);

      Float64 f;
      if ( pStabilityProblem->EvaluateStressesAtEquilibriumAngle() )
      {
         f = sectionResult.f[impact][wind][corner];
      }
      else
      {
         f = sectionResult.fDirect[impact][wind][corner];
      }
      (*pStressTable)(row,col++) << stress.SetValue(f);
      (*pStressTable)(row,col++) << strCorner[corner];
      if ( bLabelImpact )
      {
         (*pStressTable)(row,col++) << strImpact[impactIndex[impact]];
      }
      if ( bLabelWind )
      {
         (*pStressTable)(row,col++) << strWindDir[wind];
      }

      if ( f < 0 )
      {
         (*pStressTable)(row,col++) << _T("-");
      }
      else
      {
         (*pStressTable)(row,col++) << stress.SetValue(fAllow);
      }

      if ( bPassed )
      {
         (*pStressTable)(row,col++) << CD_PASS(cdRatio,cd);
      }
      else
      {
         (*pStressTable)(row,col++) << CD_FAIL(cdRatio,cd);
      }

      // compression status
      pArtifact->GetControllingCompressionCase(sectionResult,&impact,&wind,&corner,&fAllow,&bPassed,&cd);

      if ( pStabilityProblem->EvaluateStressesAtEquilibriumAngle() )
      {
         f = sectionResult.f[impact][wind][corner];
      }
      else
      {
         f = sectionResult.fDirect[impact][wind][corner];
      }
      (*pStressTable)(row,col++) << stress.SetValue(f);
      (*pStressTable)(row,col++) << strCorner[corner];
      if ( bLabelImpact )
      {
         (*pStressTable)(row,col++) << strImpact[impactIndex[impact]];
      }
      if ( bLabelWind )
      {
         (*pStressTable)(row,col++) << strWindDir[wind];
      }

      if ( bPassed )
      {
         (*pStressTable)(row,col++) << CD_PASS(cdRatio,cd);
      }
      else
      {
         (*pStressTable)(row,col++) << CD_FAIL(cdRatio,cd);
      }


      Float64 FScr = sectionResult.FScrMin;
      (*pStressTable)(row,col++) << scalar.SetValue(FScr);

      if ( FScr <= criteria.MinFScr )
      {
         (*pStressTable)(row,col++) << RPT_FAIL;
      }
      else
      {
         (*pStressTable)(row,col++) << RPT_PASS;
      }

      row++;
   }

   pPara = new rptParagraph;
   *pChapter << pPara;

   rptRcTable* pTable = rptStyleManager::CreateTableNoHeading(2,_T("Factor of Safety Against Failure"));
   *pPara << pTable << rptNewLine;

   row = pTable->GetNumberOfHeaderRows();
   (*pTable)(row,0) << _T("Factor of Safety Against Failure (") << Sub2(_T("FS"),_T("f")) << _T(")");
   (*pTable)(row,1) << scalar.SetValue(results.MinAdjFsFailure);
   row++;

   (*pTable)(row,0) << _T("Minimum Factor of Safety Against Failure");
   (*pTable)(row,1) << scalar.SetValue(criteria.MinFSf);
   row++;

   (*pTable)(row,0) << _T("Status");
   if ( pArtifact->PassedFailureCheck() )
   {
      (*pTable)(row,1) << RPT_PASS;
   }
   else
   {
      (*pTable)(row,1) << RPT_FAIL;
   }
}

void stbLiftingStabilityReporter::BuildDetailsChapter(const stbIGirder* pGirder, const stbILiftingStabilityProblem* pStabilityProblem, const stbLiftingResults* pResults, rptChapter* pChapter, LPCTSTR lpszLocColumnLabel, Float64 offset)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   CComPtr<ISegment> segment;
   pGirder->GetSegment(&segment);

   std::_tstring strFlange[] = { _T("Top Left"),_T("Top Right"),_T("Bottom Left"),_T("Bottom Right") };

   LPCTSTR strImpact[3];
   stbTypes::ImpactDirection impactDir[3];
   Float64 impactFactor[3] = { -1,-1,-1 };
   IndexType impactIndex[3] = { INVALID_INDEX,INVALID_INDEX,INVALID_INDEX };

   Float64 ImpactUp, ImpactDown;
   pStabilityProblem->GetImpact(&ImpactUp, &ImpactDown);
   IndexType nImpactCases = 0;
   strImpact[nImpactCases] = _T("No impact");
   impactDir[nImpactCases] = stbTypes::NoImpact;
   impactFactor[nImpactCases] = 1.0;
   impactIndex[stbTypes::NoImpact] = nImpactCases;

   if (!IsZero(ImpactUp))
   {
      nImpactCases++;
      strImpact[nImpactCases] = _T("Impact Up");
      impactDir[nImpactCases] = stbTypes::ImpactUp;
      impactFactor[nImpactCases] = 1.0 - ImpactUp;
      impactIndex[stbTypes::ImpactUp] = nImpactCases;
   }

   if (!IsZero(ImpactDown))
   {
      nImpactCases++;
      strImpact[nImpactCases] = _T("Impact Down");
      impactDir[nImpactCases] = stbTypes::ImpactDown;
      impactFactor[nImpactCases] = 1.0 + ImpactDown;
      impactIndex[stbTypes::ImpactDown] = nImpactCases;
   }

   stbTypes::WindType windLoadType;
   Float64 windLoad;
   pStabilityProblem->GetWindLoading(&windLoadType, &windLoad);
   IndexType nWindCases = IsZero(windLoad) ? 0 : 1;
   LPCTSTR strWindDir[] = { _T("Left"), _T("Right") };

   bool bLabelImpact = (0 < nImpactCases ? true : false);
   bool bLabelWind = (0 < nWindCases ? true : false);

   // If epxs = 0, lateral camber = 0, Ixy = 0, and Xleft = Max(Wtf,Wbf)/2, then use a simple formatting
   // otherwise do the full formatting
   bool bSimpleFormat = false;

   // this is the first check to see if epxs and lateral camber meet the criteria for simple formatting
   // we have to deal with the section propertes later
   bool bSimpleFormatTest1 = IsZero(pStabilityProblem->GetFpeLateralEccentricity()) && (!pStabilityProblem->IncludeLateralRollAxisOffset() || (pStabilityProblem->IncludeLateralRollAxisOffset() && IsZero(pStabilityProblem->GetLateralCamber())));

   Float64 Ag, Ixx, Iyy, Ixy, Xcg, Ycg, Hg, Wtop, Wbot;
   pGirder->GetSectionProperties(0, stbTypes::Start, &Ag, &Ixx, &Iyy, &Ixy, &Xcg, &Ycg, &Hg, &Wtop, &Wbot);
   if (bSimpleFormatTest1)
   {
      // we might be able to use simple formatting... check the section properties
      if (IsZero(Ixy) && IsEqual(Xcg, Max(Wtop, Wbot) / 2))
      {
         // we can use simple formatting
         bSimpleFormat = true;
      }
   }

   INIT_SCALAR_PROTOTYPE(rptRcScalar, scalar, pDisplayUnits->Scalar);
   INIT_UV_PROTOTYPE(rptForceUnitValue, force, pDisplayUnits->GeneralForce, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, longLength, pDisplayUnits->SpanLength, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, shortLength, pDisplayUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment, pDisplayUnits->Moment, false);
   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, true);
   INIT_UV_PROTOTYPE(rptStressUnitValue, modE, pDisplayUnits->ModE, true);
   INIT_UV_PROTOTYPE(rptAngleUnitValue, tiltAngle, pDisplayUnits->RadAngle, true);
   INIT_UV_PROTOTYPE(rptAngleUnitValue, crackAngle, pDisplayUnits->RadAngle, false);
   INIT_UV_PROTOTYPE(rptAngleUnitValue, angle, pDisplayUnits->Angle, true);
   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, false);
   INIT_UV_PROTOTYPE(rptLength4UnitValue, inertia, pDisplayUnits->MomentOfInertia, false);
   INIT_UV_PROTOTYPE(rptPressureUnitValue, pressure, pDisplayUnits->WindPressure, true);
   INIT_UV_PROTOTYPE(rptVelocityUnitValue, velocity, pDisplayUnits->Velocity, true);

   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   *pChapter << pPara;
   *pPara << _T("Lifting Analysis Details") << rptNewLine;
   *pPara << _T("Details for Lifting in Casting Yard [5.5.4.3][5.9.4.1]") << rptNewLine;

   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("General Parameters") << rptNewLine;
   pPara = new rptParagraph;
   *pChapter << pPara;

   *pPara << _T("Girder Length, ") << Sub2(_T("L"), _T("g")) << _T(" = ") << longLength.SetValue(pGirder->GetGirderLength()) << rptNewLine;
   *pPara << _T("Girder Weight, ") << Sub2(_T("W"), _T("g")) << _T(" = ") << force.SetValue(pResults->Wg) << rptNewLine;

   Float64 Ll, Lr;
   pStabilityProblem->GetSupportLocations(&Ll, &Lr);
   *pPara << _T("Left support overhang = ") << longLength.SetValue(Ll) << rptNewLine;
   *pPara << _T("Right support overhang = ") << longLength.SetValue(Lr) << rptNewLine;
   *pPara << _T("Clear span between lift points, ") << Sub2(_T("L"), _T("s")) << _T(" = ") << longLength.SetValue(pResults->Ls) << rptNewLine;

   *pPara << _T("Location of Roll Axis above top of girder, ") << Sub2(_T("y"), _T("rc")) << _T(" = ") << shortLength.SetValue(pStabilityProblem->GetYRollAxis()) << rptNewLine;

   Float64 impactUp, impactDown;
   pStabilityProblem->GetImpact(&impactUp, &impactDown);
   *pPara << _T("Upward Impact = ") << 100 * impactUp << _T("%") << rptNewLine;
   *pPara << _T("Downward Impact = ") << 100 * impactDown << _T("%") << rptNewLine;

   if (pApp->GetUnitsMode() == eafTypes::umUS)
   {
      Float64 sweepTolerance = pStabilityProblem->GetSweepTolerance();
      INT x = (INT)::RoundOff((1.0 / (sweepTolerance*120.0)), 1.0);
      *pPara << _T("Sweep Tolerance, ") << Sub2(_T("t"), _T("sweep")) << _T(" = ") << Super(_T("1")) << _T("/") << Sub(x) << _T(" in per 10 ft") << rptNewLine;
   }
   else
   {
      *pPara << _T("Sweep Tolerance, ") << Sub2(_T("t"), _T("sweep")) << _T(" = ") << 1000 * pStabilityProblem->GetSweepTolerance() << _T("mm/m") << rptNewLine;
   }
   *pPara << _T("Lifting Device Placement Tolerance, ") << Sub2(_T("e"), _T("lift")) << _T(" = ") << shortLength.SetValue(pStabilityProblem->GetSupportPlacementTolerance()) << rptNewLine;

   const matConcreteEx& concrete = pStabilityProblem->GetConcrete();
   *pPara << RPT_FCI << _T(" = ") << stress.SetValue(concrete.GetFc()) << rptNewLine;
   *pPara << Sub2(_T("E"), _T("ci")) << _T(" = Modulus of Elasticity = ") << modE.SetValue(concrete.GetE()) << rptNewLine;
   *pPara << RPT_STRESS(_T("r")) << _T(" = Modulus of Rupture = ") << stress.SetValue(concrete.GetFlexureFr()) << rptNewLine;

   IndexType nSections = pGirder->GetSectionCount();
   bool bPrismaticBeam = false;
   if (nSections == 1)
   {
      Float64 Ag1, Ixx1, Iyy1, Ixy1, Xcg1, Ycg1, Hg1, Wtop1, Wbot1;
      pGirder->GetSectionProperties(0, stbTypes::Start, &Ag1, &Ixx1, &Iyy1, &Ixy1, &Xcg1, &Ycg1, &Hg1, &Wtop1, &Wbot1);
      Float64 Ag2, Ixx2, Iyy2, Ixy2, Xcg2, Ycg2, Hg2, Wtop2, Wbot2;
      pGirder->GetSectionProperties(0, stbTypes::End, &Ag2, &Ixx2, &Iyy2, &Ixy2, &Xcg2, &Ycg2, &Hg2, &Wtop2, &Wbot2);
      if (IsEqual(Ag1, Ag2) && IsEqual(Ixx1, Ixx2) && IsEqual(Iyy1, Iyy2) && IsEqual(Ixy1, Ixy2) && IsEqual(Xcg1, Xcg2) && IsEqual(Ycg1, Ycg2) && IsEqual(Hg1, Hg2) && IsEqual(Wtop1, Wtop2) && IsEqual(Wbot1, Wbot2))
      {
         bPrismaticBeam = true;

         pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
         *pChapter << pPara;
         *pPara << _T("Section Properties") << rptNewLine;
         pPara = new rptParagraph;
         *pChapter << pPara;

         area.ShowUnitTag(true);
         inertia.ShowUnitTag(true);
         shortLength.ShowUnitTag(true);

         *pPara << Sub2(_T("A"), _T("g")) << _T(" = ") << area.SetValue(Ag1) << rptNewLine;
         *pPara << Sub2(_T("I"), _T("x")) << _T(" = ") << inertia.SetValue(Ixx1) << rptNewLine;
         *pPara << Sub2(_T("I"), _T("y")) << _T(" = ") << inertia.SetValue(Iyy1) << rptNewLine;
         *pPara << Sub2(_T("I"), _T("xy")) << _T(" = ") << inertia.SetValue(Ixy1) << rptNewLine;
         if (pStabilityProblem->IncludeLateralRollAxisOffset())
         {
            *pPara << Sub2(_T("X"), _T("left")) << _T(" = ") << shortLength.SetValue(Xcg1) << rptNewLine;
         }
         *pPara << Sub2(_T("Y"), _T("top")) << _T(" = ") << shortLength.SetValue(-Ycg1) << rptNewLine;
         *pPara << Sub2(_T("H"), _T("g")) << _T(" = ") << shortLength.SetValue(Hg1) << rptNewLine;
         *pPara << Sub2(_T("W"), _T("top")) << _T(" = ") << shortLength.SetValue(Wtop1) << rptNewLine;
         *pPara << Sub2(_T("W"), _T("bot")) << _T(" = ") << shortLength.SetValue(Wbot1) << rptNewLine;

         if (!bSimpleFormat)
         {
            pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
            *pChapter << pPara;
            *pPara << _T("Stress Points") << rptNewLine;
            pPara = new rptParagraph;
            *pChapter << pPara;

            gpPoint2d pntTL, pntTR, pntBL, pntBR;
            pGirder->GetStressPoints(0, stbTypes::Start, &pntTL, &pntTR, &pntBL, &pntBR);
            *pPara << _T("Top Left") << rptNewLine;
            *pPara << _T("X = ") << shortLength.SetValue(pntTL.X()) << rptNewLine;
            *pPara << _T("Y = ") << shortLength.SetValue(pntTL.Y()) << rptNewLine << rptNewLine;

            *pPara << _T("Top Right") << rptNewLine;
            *pPara << _T("X = ") << shortLength.SetValue(pntTR.X()) << rptNewLine;
            *pPara << _T("Y = ") << shortLength.SetValue(pntTR.Y()) << rptNewLine << rptNewLine;

            *pPara << _T("Bottom Left") << rptNewLine;
            *pPara << _T("X = ") << shortLength.SetValue(pntBL.X()) << rptNewLine;
            *pPara << _T("Y = ") << shortLength.SetValue(pntBL.Y()) << rptNewLine << rptNewLine;

            *pPara << _T("Bottom Right") << rptNewLine;
            *pPara << _T("X = ") << shortLength.SetValue(pntBR.X()) << rptNewLine;
            *pPara << _T("Y = ") << shortLength.SetValue(pntBR.Y()) << rptNewLine << rptNewLine;
         }
      }
   }

   if (!bPrismaticBeam)
   {
      ColumnIndexType nCol = 17;
      if (pStabilityProblem->IncludeLateralRollAxisOffset())
      {
         nCol += 2;
      }

      rptRcTable* pSectPropTable = rptStyleManager::CreateDefaultTable(nCol, _T("Section Properties"));
      *pPara << pSectPropTable << rptNewLine;

      ColumnIndexType col = 0;
      pSectPropTable->SetNumberOfHeaderRows(2);

      pSectPropTable->SetRowSpan(0, col, 2);
      pSectPropTable->SetRowSpan(1, col, SKIP_CELL);
      (*pSectPropTable)(0, col++) << COLHDR(_T("Section") << rptNewLine << _T("Length"), rptLengthUnitTag, pDisplayUnits->SpanLength);

      ColumnIndexType nColSpan = 8;
      if (pStabilityProblem->IncludeLateralRollAxisOffset())
      {
         nColSpan++;
      }
      pSectPropTable->SetColumnSpan(0, col, nColSpan);
      (*pSectPropTable)(0, col) << _T("Start of Section");
      for (ColumnIndexType i = 1; i < nColSpan; i++)
      {
         pSectPropTable->SetColumnSpan(0, col + i, SKIP_CELL);
      }
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("A"), _T("g")), rptAreaUnitTag, pDisplayUnits->Area);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("I"), _T("x")), rptLength4UnitTag, pDisplayUnits->MomentOfInertia);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("I"), _T("y")), rptLength4UnitTag, pDisplayUnits->MomentOfInertia);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("I"), _T("xy")), rptLength4UnitTag, pDisplayUnits->MomentOfInertia);
      if (pStabilityProblem->IncludeLateralRollAxisOffset())
      {
         (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("X"), _T("left")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      }
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("Y"), _T("top")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("H"), _T("g")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("W"), _T("top")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("W"), _T("bot")), rptLengthUnitTag, pDisplayUnits->ComponentDim);

      pSectPropTable->SetColumnSpan(0, col, nColSpan);
      (*pSectPropTable)(0, col) << _T("End of Section");
      for (ColumnIndexType i = 1; i < nColSpan; i++)
      {
         pSectPropTable->SetColumnSpan(0, col + i, SKIP_CELL);
      }
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("A"), _T("g")), rptAreaUnitTag, pDisplayUnits->Area);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("I"), _T("x")), rptLength4UnitTag, pDisplayUnits->MomentOfInertia);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("I"), _T("y")), rptLength4UnitTag, pDisplayUnits->MomentOfInertia);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("I"), _T("xy")), rptLength4UnitTag, pDisplayUnits->MomentOfInertia);
      if (pStabilityProblem->IncludeLateralRollAxisOffset())
      {
         (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("X"), _T("left")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      }
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("Y"), _T("top")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("H"), _T("g")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("W"), _T("top")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pSectPropTable)(1, col++) << COLHDR(Sub2(_T("W"), _T("bot")), rptLengthUnitTag, pDisplayUnits->ComponentDim);


      rptRcTable* pStressPointTable = nullptr;
      if (!bSimpleFormat)
      {
         pStressPointTable = rptStyleManager::CreateDefaultTable(17, _T("Stress Points"));
         *pPara << pStressPointTable << rptNewLine;

         pStressPointTable->SetNumberOfHeaderRows(3);
         col = 0;
         pStressPointTable->SetRowSpan(0, col, 3);
         pStressPointTable->SetRowSpan(1, col, SKIP_CELL);
         pStressPointTable->SetRowSpan(2, col, SKIP_CELL);
         (*pStressPointTable)(0, col++) << COLHDR(_T("Section") << rptNewLine << _T("Length"), rptLengthUnitTag, pDisplayUnits->SpanLength);

         pStressPointTable->SetColumnSpan(0, col, 8);
         pStressPointTable->SetColumnSpan(0, col + 1, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 2, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 3, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 4, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 5, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 6, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 7, SKIP_CELL);
         (*pStressPointTable)(0, col) << _T("Start of Section");

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Top Left");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Top Right");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Bottom Left");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Bottom Right");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(0, col, 8);
         pStressPointTable->SetColumnSpan(0, col + 1, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 2, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 3, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 4, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 5, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 6, SKIP_CELL);
         pStressPointTable->SetColumnSpan(0, col + 7, SKIP_CELL);
         (*pStressPointTable)(0, col) << _T("End of Section");

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Top Left");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Top Right");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Bottom Left");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pStressPointTable->SetColumnSpan(1, col, 2);
         pStressPointTable->SetColumnSpan(1, col + 1, SKIP_CELL);
         (*pStressPointTable)(1, col) << _T("Bottom Right");
         (*pStressPointTable)(2, col++) << COLHDR(_T("X"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
         (*pStressPointTable)(2, col++) << COLHDR(_T("Y"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      }

      RowIndexType row = pSectPropTable->GetNumberOfHeaderRows();
      longLength.ShowUnitTag(false);
      shortLength.ShowUnitTag(false);
      for (IndexType sectIdx = 0; sectIdx < nSections; sectIdx++, row++)
      {
         col = 0;
         Float64 L = pGirder->GetSectionLength(sectIdx);
         (*pSectPropTable)(row, col++) << longLength.SetValue(L);

         Float64 Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtop, Wbot;
         pGirder->GetSectionProperties(sectIdx, stbTypes::Start, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtop, &Wbot);
         (*pSectPropTable)(row, col++) << area.SetValue(Ag);
         (*pSectPropTable)(row, col++) << inertia.SetValue(Ixx);
         (*pSectPropTable)(row, col++) << inertia.SetValue(Iyy);
         (*pSectPropTable)(row, col++) << inertia.SetValue(Ixy);
         if (pStabilityProblem->IncludeLateralRollAxisOffset())
         {
            (*pSectPropTable)(row, col++) << shortLength.SetValue(Xleft);
         }
         (*pSectPropTable)(row, col++) << shortLength.SetValue(-Ytop);
         (*pSectPropTable)(row, col++) << shortLength.SetValue(Hg);
         (*pSectPropTable)(row, col++) << shortLength.SetValue(Wtop);
         (*pSectPropTable)(row, col++) << shortLength.SetValue(Wbot);

         pGirder->GetSectionProperties(sectIdx, stbTypes::End, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtop, &Wbot);
         (*pSectPropTable)(row, col++) << area.SetValue(Ag);
         (*pSectPropTable)(row, col++) << inertia.SetValue(Ixx);
         (*pSectPropTable)(row, col++) << inertia.SetValue(Iyy);
         (*pSectPropTable)(row, col++) << inertia.SetValue(Ixy);
         if (pStabilityProblem->IncludeLateralRollAxisOffset())
         {
            (*pSectPropTable)(row, col++) << shortLength.SetValue(Xleft);
         }
         (*pSectPropTable)(row, col++) << shortLength.SetValue(-Ytop);
         (*pSectPropTable)(row, col++) << shortLength.SetValue(Hg);
         (*pSectPropTable)(row, col++) << shortLength.SetValue(Wtop);
         (*pSectPropTable)(row, col++) << shortLength.SetValue(Wbot);

         if (!bSimpleFormat)
         {
            col = 0;
            row++;
            (*pStressPointTable)(row, col++) << longLength.SetValue(L);

            gpPoint2d pntTL, pntTR, pntBL, pntBR;
            pGirder->GetStressPoints(sectIdx, stbTypes::Start, &pntTL, &pntTR, &pntBL, &pntBR);
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTL.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTL.Y());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTR.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTR.Y());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBL.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBL.Y());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBR.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBR.Y());

            pGirder->GetStressPoints(sectIdx, stbTypes::End, &pntTL, &pntTR, &pntBL, &pntBR);
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTL.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTL.Y());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTR.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntTR.Y());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBL.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBL.Y());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBR.X());
            (*pStressPointTable)(row, col++) << shortLength.SetValue(pntBR.Y());
         }
      }

      *pPara << rptNewLine;
   }

   longLength.ShowUnitTag(true);
   shortLength.ShowUnitTag(true);

   // Inclined Lifting Cable Parameters
   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("Inclined Lifting Cable Parameters") << rptNewLine;
   pPara = new rptParagraph;
   *pChapter << pPara;
   *pPara << _T("Lift Cable Angle, ") << Sub2(symbol(theta), _T("lift")) << _T(" = ") << angle.SetValue(pStabilityProblem->GetLiftAngle()) << rptNewLine;
   *pPara << _T("Horizontal component of lifting force, without impact, ") << Sub2(_T("P"), _T("lift")) << _T(" = ") << force.SetValue(-pResults->Plift) << rptNewLine;

   if (bSimpleFormat)
   {
      *pPara << _T("Critical compression load, ") << Sub2(_T("P"), _T("crit")) << _T(" = ") << Super2(symbol(pi), _T("2")) << Sub2(_T("E"), _T("ci")) << Sub2(_T("I"), _T("yy")) << _T("/") << Super2(_T("L"), _T("2")) << _T(" = ") << force.SetValue(pResults->Pcrit) << rptNewLine;
   }
   else
   {
      *pPara << _T("Critical compression load, ") << Sub2(_T("P"), _T("crit")) << _T(" = ") << Super2(symbol(pi), _T("2")) << Sub2(_T("E"), _T("ci")) << _T("(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")/(") << Sub2(_T("I"), _T("xx")) << Super2(_T("L"), _T("2")) << _T(") = ") << force.SetValue(pResults->Pcrit) << rptNewLine;
   }

   *pPara << _T("Vertical deflection due to horizontal component of lifting force, without impact, ") << Sub2(symbol(DELTA), _T("lift")) << _T(" = ") << shortLength.SetValue(pResults->dLift) << rptNewLine;
   *pPara << _T("Lateral deflection magnification factor, ") << Sub2(_T("m"), _T("e")) << _T(" = ") << _T("1/(1 - ((IM)") << Sub2(_T("P"), _T("lift")) << _T(")/") << Sub2(_T("P"), _T("crit")) << _T(")");
   for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
   {
      *pPara << _T(" = ") << scalar.SetValue(pResults->emag[impactDir[impactCase]]);
      if (0 < nImpactCases)
      {
         *pPara << _T(" (") << strImpact[impactCase] << _T(")");
      }
   }

   *pPara << rptNewLine;

   // Vertical location of center of gravity
   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("Vertical Location of Center of Gravity") << rptNewLine;
   pPara = new rptParagraph;
   *pChapter << pPara;

   *pPara << _T("Offset Factor, ") << Sub2(_T("F"), _T("o")) << _T(" = (") << Sub2(_T("L"), _T("s")) << _T("/") << Sub2(_T("L"), _T("g")) << _T(")") << Super(_T("2")) << _T(" - 1/3 = ") << scalar.SetValue(pResults->OffsetFactor) << rptNewLine;

   bool bDirectCamber;
   Float64 camber;
   pStabilityProblem->GetCamber(&bDirectCamber, &camber);
   Float64 precamber = pGirder->GetPrecamber();
   if (bDirectCamber)
   {
      *pPara << _T("Camber Multipler, m = ") << pStabilityProblem->GetCamberMultiplier() << rptNewLine;
      *pPara << _T("Camber, ") << Sub2(symbol(DELTA), _T("camber")) << _T(" = ") << shortLength.SetValue(camber) << rptNewLine;
      *pPara << _T("Precamber, ") << Sub2(symbol(DELTA), _T("precamber")) << _T(" = ") << shortLength.SetValue(precamber) << rptNewLine;
      *pPara << _T("Location of center of gravity below roll axis, ") << Sub2(_T("y"), _T("r")) << _T(" = ") << Sub2(_T("Y"), _T("top")) << _T(" - ") << Sub2(_T("F"), _T("o")) << _T("(") << _T("m") << Sub2(symbol(DELTA), _T("camber")) << _T(" + (IM)") << Sub2(symbol(DELTA), _T("lift")) << _T(" + ") << Sub2(symbol(DELTA), _T("precamber")) << _T(")") << _T(" + ") << Sub2(_T("y"), _T("rc"));
      for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
      {
         *pPara << _T(" = ") << shortLength.SetValue(pResults->Dra[impactDir[impactCase]]);
         if (0 < nImpactCases)
         {
            *pPara << _T(" (") << strImpact[impactCase] << _T(")");
         }

         if (pResults->Dra[impactDir[impactCase]] < 0)
         {
            *pPara << color(Red) << _T(" WARNING: Girder is unstable. The center of gravity is above the roll axis.") << color(Black) << rptNewLine;
         }
      }
      *pPara << rptNewLine;
   }
   else
   {
      *pPara << _T("Camber offset factor, ") << Sub2(_T("F"), _T("co")) << _T(" = ") << scalar.SetValue(pResults->CamberOffsetFactor) << rptNewLine;
      *pPara << _T("Precamber, ") << Sub2(symbol(DELTA), _T("precamber")) << _T(" = ") << shortLength.SetValue(precamber) << rptNewLine;
      *pPara << _T("Location of center of gravity below roll axis, ") << Sub2(_T("y"), _T("r")) << _T(" = ") << Sub2(_T("F"), _T("co")) << _T("(") << Sub2(_T("Y"), _T("top")) << _T(" + ") << Sub2(_T("y"), _T("rc")) << _T(")") << _T(" - ") << Sub2(_T("F"), _T("o")) << _T("((IM)") << Sub2(symbol(DELTA), _T("lift")) << _T(" + ") << Sub2(symbol(DELTA), _T("precamber")) << _T(")");
      for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
      {
         *pPara << _T(" = ") << shortLength.SetValue(pResults->Dra[impactDir[impactCase]]);
         if (0 < nImpactCases)
         {
            *pPara << _T(" (") << strImpact[impactCase] << _T(")");
         }

         if (pResults->Dra[impactDir[impactCase]] < 0)
         {
            *pPara << color(Red) << _T(" WARNING: ") << _T("Girder is unstable. The center of gravity is above the roll axis.") << color(Black) << rptNewLine;
         }
      }
      *pPara << rptNewLine;
   }

   *pPara << rptNewLine;

   // Lateral Deflection Parameters
   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("Lateral Deflection Parameters") << rptNewLine;
   if (pStabilityProblem->IncludeLateralRollAxisOffset())
   {
      pPara = new rptParagraph;
      *pChapter << pPara;

      *pPara << _T("Lateral Camber, ") << Sub2(symbol(DELTA), _T("lc")) << _T(" = ") << shortLength.SetValue(pStabilityProblem->GetLateralCamber()) << rptNewLine;

      if (pResults->XcgMethod == stbTypes::Exact)
      {
         Float64 Ag1, Ixx1, Iyy1, Ixy1, Xcg1, Ycg1, Hg1, Wtop1, Wbot1;
         pGirder->GetSectionProperties(0, stbTypes::Start, &Ag1, &Ixx1, &Iyy1, &Ixy1, &Xcg1, &Ycg1, &Hg1, &Wtop1, &Wbot1);
         if (Wbot1 < Wtop1)
         {
            *pPara << _T("Eccentricity of CG from roll axis, ") << Sub2(_T("e"), _T("cg")) << _T(" = ") << _T("|") << Sub2(_T("X"), _T("left")) << _T(" - ") << Sub2(_T("W"), _T("top")) << _T("/2| = ") << shortLength.SetValue(pResults->Xleft) << rptNewLine;
         }
         else
         {
            *pPara << _T("Eccentricity of CG from roll axis, ") << Sub2(_T("e"), _T("cg")) << _T(" = ") << _T("|") << Sub2(_T("X"), _T("left")) << _T(" - ") << Sub2(_T("W"), _T("bot")) << _T("/2| = ") << shortLength.SetValue(pResults->Xleft) << rptNewLine;
         }
      }
      else
      {
         *pPara << _T("Eccentricity of CG from roll axis, ") << Sub2(_T("e"), _T("cg")) << _T(" = ") << symbol(SUM) << _T("(0.5(") << Super2(Sub2(_T("X"), _T("left")), _T("i")) << _T(" + ") << Super2(Sub2(_T("X"), _T("left")), _T("i+1")) << _T(")") << _T("(Section Length)") << _T(")/") << Sub2(_T("L"), _T("g")) << _T(" = ") << shortLength.SetValue(pResults->Xleft) << rptNewLine;
      }
   }

   pPara = new rptParagraph;
   *pChapter << pPara;
   *pPara << _T("Lateral Sweep, ") << Sub2(_T("e"), _T("sweep")) << _T(" = ") << Sub2(_T("L"), _T("g")) << Sub2(_T("t"), _T("sweep")) << _T(" = ") << shortLength.SetValue(pResults->LateralSweep) << rptNewLine;


   *pPara << _T("Initial lateral eccentricity of center of gravity of girder due to lateral sweep and eccentricity of lifting devices from centerline of girder, ") << rptNewLine;
   if (pStabilityProblem->IncludeLateralRollAxisOffset())
   {
      *pPara << Sub2(_T("e"), _T("i")) << _T(" = ") << Sub2(_T("m"), _T("e")) << _T("[") << Sub2(_T("F"), _T("o")) << _T("(") << Sub2(_T("e"), _T("sweep")) << _T(" + ") << Sub2(symbol(DELTA), _T("lc")) << _T(")") << _T(" + ") << Sub2(_T("e"), _T("lift")) << _T(" + ") << Sub2(_T("e"), _T("cg")) << _T(")");
   }
   else
   {
      *pPara << Sub2(_T("e"), _T("i")) << _T(" = ") << Sub2(_T("m"), _T("e")) << _T("(") << Sub2(_T("F"), _T("o")) << Sub2(_T("e"), _T("sweep")) << _T(" + ") << Sub2(_T("e"), _T("lift")) << _T(")");
   }
   for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
   {
      *pPara << _T(" = ") << shortLength.SetValue(pResults->EccLateralSweep[impactDir[impactCase]]);
      if (0 < nImpactCases)
      {
         *pPara << _T(" (") << strImpact[impactCase] << _T(")");
      }
   }
   *pPara << rptNewLine;

   *pPara << _T("Lateral Deflection of center of gravity due to total girder weight applied to weak axis, ") << Sub2(_T("z"), _T("o")) << rptNewLine;
   if (pResults->ZoMethod == stbTypes::Exact)
   {
      if (bSimpleFormat)
      {
         *pPara << Sub2(_T("z"),_T("o")) << _T(" = (") << Sub2(_T("W"), _T("g")) << _T("/12E") << Sub2(_T("I"), _T("yy")) << Sub2(_T("L"), _T("g")) << Super(_T("2")) << _T(")(")
            << Sub2(_T("L"), _T("s")) << Super(_T("5")) << _T("/10") << _T(" - ")
            << Super2(_T("a"), _T("2")) << Sub2(_T("L"), _T("s")) << Super(_T("3")) << _T(" + ")
            << _T("3") << Super2(_T("a"), _T("4")) << Sub2(_T("L"), _T("s")) << _T(" + ")
            << _T("6") << Super2(_T("a"), _T("5")) << _T("/5") << _T(")") << rptNewLine;
      }
      else
      {
         *pPara << Sub2(_T("z"), _T("o")) << _T(" = (") << Sub2(_T("W"), _T("g")) << Sub2(_T("I"), _T("xx")) << _T("/(12E(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T("-") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")") << Sub2(_T("L"), _T("g")) << Super(_T("2")) << _T(")(")
            << Sub2(_T("L"), _T("s")) << Super(_T("5")) << _T("/10") << _T(" - ")
            << Super2(_T("a"), _T("2")) << Sub2(_T("L"), _T("s")) << Super(_T("3")) << _T(" + ")
            << _T("3") << Super2(_T("a"), _T("4")) << Sub2(_T("L"), _T("s")) << _T(" + ")
            << _T("6") << Super2(_T("a"), _T("5")) << _T("/5") << _T(")") << rptNewLine;
      }
   }

   for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
   {
      if (0 < nImpactCases)
      {
         *pPara << strImpact[impactCase] << _T(" ");
      }
      *pPara << Sub2(_T("z"),_T("o")) << _T(" = ") << shortLength.SetValue(pResults->Zo[impactDir[impactCase]]);
   }
   *pPara << rptNewLine;

   // Wind Load Parameters
   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("Wind Load Parameters") << rptNewLine;
   pPara = new rptParagraph;
   *pChapter << pPara;
   if (windLoadType == stbTypes::Speed)
   {
      *pPara << _T("Wind Speed, V = ") << velocity.SetValue(windLoad) << rptNewLine;
      *pPara << _T("Pressure exposure and elevation coefficient, ") << Sub2(_T("K"), _T("z")) << _T(" = 1.0 for Service I (LRFD 3.8.1.2)") << rptNewLine;
      *pPara << _T("Gust effect factor, G = 1.0 for Service I (LRFD 3.8.1.2)") << rptNewLine;
      *pPara << _T("Drag Coefficient, ") << Sub2(_T("C"), _T("d")) << _T(" = ") << scalar.SetValue(pGirder->GetDragCoefficient()) << rptNewLine;
      *pPara << _T("Wind Pressure, ") << Sub2(_T("P"), _T("z")) << _T(" = ") << Sub2(_T("w"), _T("wind")) << _T(" = 2.56x") << Super2(_T("10"), _T("-6")) << Super2(_T("V"), _T("2")) << Sub2(_T("K"), _T("z")) << _T("G") << Sub2(_T("C"), _T("d")) << _T(" = ") << pressure.SetValue(pResults->WindPressure) << rptNewLine;
   }
   else
   {
      *pPara << _T("Lateral wind pressure, ") << Sub2(_T("w"), _T("wind")) << _T(" = ") << pressure.SetValue(pResults->WindPressure) << rptNewLine;
   }
   *pPara << _T("Total Wind Load, ") << Sub2(_T("W"), _T("wind")) << _T(" = ") << force.SetValue(pResults->Wwind) << rptNewLine;
   *pPara << _T("Location of resultant wind force below roll axis, ") << Sub2(_T("y"), _T("wind")) << rptNewLine;
   if (bDirectCamber)
   {
      *pPara << Sub2(_T("y"), _T("wind")) << _T(" = ") << Sub2(_T("H"), _T("g")) << _T("/2 + ") << Sub2(_T("y"), _T("rc")) << _T(" - ") << Sub2(_T("F"), _T("o")) << _T("(") << _T("(m)") << Sub2(symbol(DELTA), _T("camber")) << _T(" + (IM)") << Sub2(symbol(DELTA), _T("lift")) << _T(")") << rptNewLine;
   }
   else
   {
      *pPara << Sub2(_T("y"), _T("wind")) << _T(" = ") << Sub2(_T("F"), _T("co")) << _T("(") << Sub2(_T("H"), _T("g")) << _T("/2 + ") << Sub2(_T("y"), _T("rc")) << _T(") - ") << Sub2(_T("F"), _T("o")) << _T("(IM)") << Sub2(symbol(DELTA), _T("lift")) << rptNewLine;
   }
   for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
   {
      if (0 < nImpactCases)
      {
         *pPara << strImpact[impactCase] << _T(" ");
      }
      *pPara << Sub2(_T("y"),_T("wind")) << _T(" = ") << shortLength.SetValue(pResults->Ywind[impactDir[impactCase]]) << rptNewLine;
   }
   *pPara << rptNewLine;

   *pPara << _T("Lateral Deflection due to wind applied toward the left, ") << Sub2(_T("z"), _T("wind")) << rptNewLine;
   *pPara << Sub2(_T("z"), _T("wind")) << _T(" = ") << Sub2(_T("W"), _T("wind")) << Sub2(_T("z"), _T("o")) << _T("/") << Sub2(_T("W"), _T("g")) << rptNewLine;
   for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
   {
      if (0 < nImpactCases)
      {
         *pPara << strImpact[impactCase] << _T(" ");
      }
      *pPara << Sub2(_T("z"),_T("wind")) << _T(" = ") << shortLength.SetValue(pResults->ZoWind[impactDir[impactCase]]);
   }
   *pPara << rptNewLine;

   *pPara << _T("Lateral eccentricity of Girder Self Weight due to Wind Load towards the left, ") << Sub2(_T("e"), _T("wind")) << rptNewLine;
   *pPara << Sub2(_T("e"),_T("wind")) << _T(" = ") << Sub2(_T("W"), _T("wind")) << Sub2(_T("y"), _T("wind")) << _T("/") << Sub2(_T("W"), _T("g")) << rptNewLine;
   for (IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++)
   {
      if (0 < nImpactCases)
      {
         *pPara << strImpact[impactCase] << _T(" ");
      }
      *pPara << Sub2(_T("e"),_T("wind")) << _T(" = ") << shortLength.SetValue(pResults->EccWind[impactDir[impactCase]]) << rptNewLine;
   }
   *pPara << rptNewLine;

   rptRcTable* pPrestressTable = rptStyleManager::CreateDefaultTable(bSimpleFormat ? 9 : 14, _T("Stress due to Effective Prestress"));
   *pPara << pPrestressTable << rptNewLine;

   ColumnIndexType col = 0;
   pPrestressTable->SetNumberOfHeaderRows(2);
   pPrestressTable->SetRowSpan(0, col, 2);
   pPrestressTable->SetRowSpan(1, col, SKIP_CELL);
   if (lpszLocColumnLabel)
   {
      (*pPrestressTable)(0, col++) << COLHDR(rptRcStringLiteral(lpszLocColumnLabel), rptLengthUnitTag, pDisplayUnits->SpanLength);
   }
   else
   {
      (*pPrestressTable)(0, col++) << COLHDR(_T("Dist from") << rptNewLine << _T("left end"), rptLengthUnitTag, pDisplayUnits->SpanLength);
   }

   ColumnIndexType nColSpan = (bSimpleFormat ? 2 : 3);
   pPrestressTable->SetColumnSpan(0, col, nColSpan);
   (*pPrestressTable)(0, col) << _T("Straight");
   for (ColumnIndexType i = 1; i < nColSpan; i++)
   {
      pPrestressTable->SetColumnSpan(0, col + i, SKIP_CELL);
   }
   (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("F"), _T("pe")), rptForceUnitTag, pDisplayUnits->GeneralForce);
   if (bSimpleFormat)
   {
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("ps")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   }
   else
   {
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("psx")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("psy")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   }

   pPrestressTable->SetColumnSpan(0, col, nColSpan);
   (*pPrestressTable)(0, col) << _T("Harped");
   for (ColumnIndexType i = 1; i < nColSpan; i++)
   {
      pPrestressTable->SetColumnSpan(0, col + i, SKIP_CELL);
   }
   (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("F"), _T("pe")), rptForceUnitTag, pDisplayUnits->GeneralForce);
   if (bSimpleFormat)
   {
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("ps")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   }
   else
   {
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("psx")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("psy")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   }

   pPrestressTable->SetColumnSpan(0, col, nColSpan);
   (*pPrestressTable)(0, col) << _T("Temporary");
   for (ColumnIndexType i = 1; i < nColSpan; i++)
   {
      pPrestressTable->SetColumnSpan(0, col + i, SKIP_CELL);
   }
   (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("F"), _T("pe")), rptForceUnitTag, pDisplayUnits->GeneralForce);
   if (bSimpleFormat)
   {
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("ps")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   }
   else
   {
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("psx")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
      (*pPrestressTable)(1, col++) << COLHDR(Sub2(_T("e"), _T("psy")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   }

   nColSpan = (bSimpleFormat ? 2 : 4);
   pPrestressTable->SetColumnSpan(0, col, nColSpan);
   (*pPrestressTable)(0, col) << RPT_STRESS(_T("ps"));
   for (ColumnIndexType i = 1; i < nColSpan; i++)
   {
      pPrestressTable->SetColumnSpan(0, col + i, SKIP_CELL);
   }

   if (bSimpleFormat)
   {
      (*pPrestressTable)(1, col++) << COLHDR(_T("Top"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pPrestressTable)(1, col++) << COLHDR(_T("Bottom"), rptStressUnitTag, pDisplayUnits->Stress);
   }
   else
   {
      (*pPrestressTable)(1, col++) << COLHDR(_T("Top Left"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pPrestressTable)(1, col++) << COLHDR(_T("Top Right"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pPrestressTable)(1, col++) << COLHDR(_T("Bottom Left"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pPrestressTable)(1, col++) << COLHDR(_T("Bottom Right"), rptStressUnitTag, pDisplayUnits->Stress);
   }

   longLength.ShowUnitTag(false);
   shortLength.ShowUnitTag(false);
   force.ShowUnitTag(false);
   stress.ShowUnitTag(false);
   area.ShowUnitTag(false);

   Float64 epsx = pStabilityProblem->GetFpeLateralEccentricity();

   RowIndexType row = pPrestressTable->GetNumberOfHeaderRows();
   for (const auto& sectionResult : pResults->vSectionResults)
   {
      col = 0;

      const stbIAnalysisPoint* pAnalysisPoint = pStabilityProblem->GetAnalysisPoint(sectionResult.AnalysisPointIndex);

      (*pPrestressTable)(row, col++) << rptRcStringLiteral(pAnalysisPoint->AsString(pDisplayUnits->SpanLength, offset, false));

      Float64 Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtop, Wbot;
      pGirder->GetSectionProperties(pAnalysisPoint->GetLocation(), &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtop, &Wbot);

      Float64 Fpe, Yps;
      pStabilityProblem->GetFpe(stbTypes::Straight, pAnalysisPoint->GetLocation(), &Fpe, &Yps);
      (*pPrestressTable)(row, col++) << force.SetValue(Fpe);
      if (!bSimpleFormat)
      {
         (*pPrestressTable)(row, col++) << shortLength.SetValue(epsx);
      }
      (*pPrestressTable)(row, col++) << shortLength.SetValue(Ytop - Yps);

      pStabilityProblem->GetFpe(stbTypes::Harped, pAnalysisPoint->GetLocation(), &Fpe, &Yps);
      (*pPrestressTable)(row, col++) << force.SetValue(Fpe);
      if (!bSimpleFormat)
      {
         (*pPrestressTable)(row, col++) << shortLength.SetValue(epsx);
      }
      (*pPrestressTable)(row, col++) << shortLength.SetValue(Ytop - Yps);

      pStabilityProblem->GetFpe(stbTypes::Temporary, pAnalysisPoint->GetLocation(), &Fpe, &Yps);
      (*pPrestressTable)(row, col++) << force.SetValue(Fpe);
      if (!bSimpleFormat)
      {
         (*pPrestressTable)(row, col++) << shortLength.SetValue(epsx);
      }
      (*pPrestressTable)(row, col++) << shortLength.SetValue(Ytop - Yps);

      if (bSimpleFormat)
      {
         ATLASSERT(IsEqual(sectionResult.fps[stbTypes::TopLeft], sectionResult.fps[stbTypes::TopRight], 0.001));
         ATLASSERT(IsEqual(sectionResult.fps[stbTypes::BottomLeft], sectionResult.fps[stbTypes::BottomRight], 0.001));
         (*pPrestressTable)(row, col++) << stress.SetValue(sectionResult.fps[stbTypes::TopLeft]);
         (*pPrestressTable)(row, col++) << stress.SetValue(sectionResult.fps[stbTypes::BottomLeft]);
      }
      else
      {
         (*pPrestressTable)(row, col++) << stress.SetValue(sectionResult.fps[stbTypes::TopLeft]);
         (*pPrestressTable)(row, col++) << stress.SetValue(sectionResult.fps[stbTypes::TopRight]);
         (*pPrestressTable)(row, col++) << stress.SetValue(sectionResult.fps[stbTypes::BottomLeft]);
         (*pPrestressTable)(row, col++) << stress.SetValue(sectionResult.fps[stbTypes::BottomRight]);
      }

      row++;
   }

   *pPara << Sub2(_T("F"), _T("pe")) << _T(" = effective prestress force") << rptNewLine;
   if (bSimpleFormat)
   {
      *pPara << Sub2(_T("e"), _T("ps")) << _T(" = eccentricity of the prestress force. Positive values are for ") << Sub2(_T("F"), _T("pe")) << _T(" below the centroidal axis") << rptNewLine;
      *pPara << RPT_STRESS(_T("ps")) << _T(" = ") << _T("stress due to prestressing") << rptNewLine;
      *pPara << _T("Top ") << RPT_STRESS(_T("ps")) << _T(" = -") << Sub2(_T("F"), _T("pe")) << _T("(1/") << Sub2(_T("A"), _T("g")) << _T(" - ") << Sub2(_T("e"), _T("ps")) << Sub2(_T("Y"), _T("top")) << _T("/") << Sub2(_T("I"), _T("x")) << _T(")") << rptNewLine;
      *pPara << _T("Bottom " ) << RPT_STRESS(_T("ps")) << _T(" = -") << Sub2(_T("F"), _T("pe")) << _T("(1/") << Sub2(_T("A"), _T("g")) << _T(" + ") << Sub2(_T("e"), _T("ps")) << _T("(") << Sub2(_T("H"), _T("g")) << _T(" - ") << Sub2(_T("Y"), _T("top")) << _T(")/") << Sub2(_T("I"), _T("x")) << _T(")") << rptNewLine;
   }
   else
   {
      *pPara << Sub2(_T("e"), _T("psx")) << _T(" = eccentricity of the prestress force. Positive values are for ") << Sub2(_T("F"), _T("pe")) << _T(" to the left of the centroidal axis") << rptNewLine;
      *pPara << Sub2(_T("e"), _T("psy")) << _T(" = eccentricity of the prestress force. Positive values are for ") << Sub2(_T("F"), _T("pe")) << _T(" below the centroidal axis") << rptNewLine;
      *pPara << Sub2(_T("M"), _T("psx")) << _T(" = ") << Sub2(_T("F"), _T("pe")) << Sub2(_T("e"), _T("psy")) << rptNewLine;
      *pPara << Sub2(_T("M"), _T("psy")) << _T(" = ") << Sub2(_T("F"), _T("pe")) << Sub2(_T("e"), _T("psx")) << rptNewLine;
      *pPara << RPT_STRESS(_T("ps")) << _T("(x,y) = ")
         << _T("(") << Sub2(_T("M"), _T("psy")) << Sub2(_T("I"), _T("xx")) << _T("+") << Sub2(_T("M"), _T("psx")) << Sub2(_T("I"), _T("xy")) << _T(")(x)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
         << _T(" - ")
         << _T("(") << Sub2(_T("M"), _T("psx")) << Sub2(_T("I"), _T("yy")) << _T("+") << Sub2(_T("M"), _T("psy")) << Sub2(_T("I"), _T("xy")) << _T(")(y)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
         << _T(" - ")
         << Sub2(_T("F"), _T("pe")) << _T("/") << Sub2(_T("A"), _T("g")) << rptNewLine;
   }

   rptRcTable* pStressTable = rptStyleManager::CreateDefaultTable(bSimpleFormat ? 11 : 15, _T("Stress due to External Loads"));
   *pPara << pStressTable << rptNewLine;

   col = 0;
   pStressTable->SetNumberOfHeaderRows(2);
   pStressTable->SetRowSpan(0, col, 2);
   pStressTable->SetRowSpan(1, col, SKIP_CELL);
   if (lpszLocColumnLabel)
   {
      (*pStressTable)(0, col++) << COLHDR(rptRcStringLiteral(lpszLocColumnLabel), rptLengthUnitTag, pDisplayUnits->SpanLength);
   }
   else
   {
      (*pStressTable)(0, col++) << COLHDR(_T("Dist from") << rptNewLine << _T("left end"), rptLengthUnitTag, pDisplayUnits->SpanLength);
   }

   pStressTable->SetRowSpan(0, col, 2);
   pStressTable->SetRowSpan(1, col, SKIP_CELL);
   (*pStressTable)(0, col++) << COLHDR(Sub2(_T("M"), _T("girder")), rptMomentUnitTag, pDisplayUnits->Moment);

   nColSpan = (bSimpleFormat ? 2 : 4);
   pStressTable->SetColumnSpan(0, col, nColSpan);
   for (ColumnIndexType i = 1; i < nColSpan; i++)
   {
      pStressTable->SetColumnSpan(0, col + i, SKIP_CELL);
   }
   (*pStressTable)(0, col) << RPT_STRESS(_T("g"));
   if (bSimpleFormat)
   {
      (*pStressTable)(1, col++) << COLHDR(_T("Top"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Bottom"), rptStressUnitTag, pDisplayUnits->Stress);
   }
   else
   {
      (*pStressTable)(1, col++) << COLHDR(_T("Top Left"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Top Right"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Bottom Left"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Bottom Right"), rptStressUnitTag, pDisplayUnits->Stress);
   }

   pStressTable->SetRowSpan(0, col, 2);
   pStressTable->SetRowSpan(1, col, SKIP_CELL);
   (*pStressTable)(0, col++) << COLHDR(Sub2(_T("M"), _T("wind")), rptMomentUnitTag, pDisplayUnits->Moment);

   pStressTable->SetColumnSpan(0, col, 4);
   (*pStressTable)(0, col) << RPT_STRESS(_T("w"));
   pStressTable->SetColumnSpan(0, col + 1, SKIP_CELL);
   pStressTable->SetColumnSpan(0, col + 2, SKIP_CELL);
   pStressTable->SetColumnSpan(0, col + 3, SKIP_CELL);
   (*pStressTable)(1, col++) << COLHDR(_T("Top Left"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pStressTable)(1, col++) << COLHDR(_T("Top Right"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pStressTable)(1, col++) << COLHDR(_T("Bottom Left"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pStressTable)(1, col++) << COLHDR(_T("Bottom Right"), rptStressUnitTag, pDisplayUnits->Stress);

   nColSpan = (bSimpleFormat ? 2 : 4);
   pStressTable->SetColumnSpan(0, col, nColSpan);
   for (ColumnIndexType i = 1; i < nColSpan; i++)
   {
      pStressTable->SetColumnSpan(0, col + i, SKIP_CELL);
   }
   (*pStressTable)(0, col) << RPT_STRESS(_T("h"));
   if (bSimpleFormat)
   {
      (*pStressTable)(1, col++) << COLHDR(_T("Top"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Bottom"), rptStressUnitTag, pDisplayUnits->Stress);
   }
   else
   {
      (*pStressTable)(1, col++) << COLHDR(_T("Top Left"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Top Right"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Bottom Left"), rptStressUnitTag, pDisplayUnits->Stress);
      (*pStressTable)(1, col++) << COLHDR(_T("Bottom Right"), rptStressUnitTag, pDisplayUnits->Stress);
   }

   longLength.ShowUnitTag(false);
   shortLength.ShowUnitTag(false);
   force.ShowUnitTag(false);
   stress.ShowUnitTag(false);
   area.ShowUnitTag(false);

   row = pStressTable->GetNumberOfHeaderRows();
   for (const auto& sectionResult : pResults->vSectionResults)
   {
      col = 0;

      const stbIAnalysisPoint* pAnalysisPoint = pStabilityProblem->GetAnalysisPoint(sectionResult.AnalysisPointIndex);

      (*pStressTable)(row, col++) << rptRcStringLiteral(pAnalysisPoint->AsString(pDisplayUnits->SpanLength, offset, false));

      (*pStressTable)(row, col++) << moment.SetValue(sectionResult.Mg);

      if (bSimpleFormat)
      {
         ATLASSERT(IsEqual(sectionResult.fg[stbTypes::TopLeft], sectionResult.fg[stbTypes::TopRight]));
         ATLASSERT(IsEqual(sectionResult.fg[stbTypes::BottomLeft], sectionResult.fg[stbTypes::BottomRight]));
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fg[stbTypes::TopLeft]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fg[stbTypes::BottomLeft]);
      }
      else
      {
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fg[stbTypes::TopLeft]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fg[stbTypes::TopRight]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fg[stbTypes::BottomLeft]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fg[stbTypes::BottomRight]);
      }

      (*pStressTable)(row, col++) << moment.SetValue(sectionResult.Mw);

      (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fw[stbTypes::TopLeft]);
      (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fw[stbTypes::TopRight]);
      (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fw[stbTypes::BottomLeft]);
      (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fw[stbTypes::BottomRight]);

      if (bSimpleFormat)
      {
         ATLASSERT(IsEqual(sectionResult.fcable[stbTypes::TopLeft], sectionResult.fcable[stbTypes::TopRight]));
         ATLASSERT(IsEqual(sectionResult.fcable[stbTypes::BottomLeft], sectionResult.fcable[stbTypes::BottomRight]));
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fcable[stbTypes::TopLeft]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fcable[stbTypes::BottomLeft]);
      }
      else
      {
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fcable[stbTypes::TopLeft]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fcable[stbTypes::TopRight]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fcable[stbTypes::BottomLeft]);
         (*pStressTable)(row, col++) << stress.SetValue(sectionResult.fcable[stbTypes::BottomRight]);
      }

      row++;
   }

   *pPara << RPT_STRESS(_T("g")) << _T(" = ") << _T("stress due to girder self weight applied without tilt or impact") << rptNewLine;
   if (bSimpleFormat)
   {
      *pPara << _T("Top ") << RPT_STRESS(_T("g")) << _T(" = -") << Sub2(_T("M"), _T("girder")) << Sub2(_T("Y"), _T("top")) << _T("/") << Sub2(_T("I"), _T("xx")) << rptNewLine;
      *pPara << _T("Bottom ") << RPT_STRESS(_T("g")) << _T(" = ") << Sub2(_T("M"), _T("girder")) << _T("(") << Sub2(_T("H"), _T("g")) << _T(" - ") << Sub2(_T("Y"), _T("top")) << _T(")/") << Sub2(_T("I"), _T("xx")) << rptNewLine;
   }
   else
   {
      *pPara << RPT_STRESS(_T("g")) << _T("(x,y)") << _T(" = ")
         << Sub2(_T("M"), _T("girder")) << Sub2(_T("I"), _T("xy")) << _T("(x)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
         << _T(" - ")
         << Sub2(_T("M"), _T("girder")) << Sub2(_T("I"), _T("yy")) << _T("(y)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")") << rptNewLine;
   }

   *pPara << RPT_STRESS(_T("w")) << _T(" = ") << _T("stress due to wind toward the left") << rptNewLine;
   if (bSimpleFormat)
   {
      *pPara << _T("Top Left ") << RPT_STRESS(_T("w")) << _T(" = ") << Sub2(_T("M"), _T("wind")) << Sub2(_T("W"), _T("top")) << _T("/(2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
      *pPara << _T("Top Right ") << RPT_STRESS(_T("w")) << _T(" = -") << Sub2(_T("M"), _T("wind")) << Sub2(_T("W"), _T("top")) << _T("/(2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
      *pPara << _T("Bottom Left ") << RPT_STRESS(_T("w")) << _T(" = ") << Sub2(_T("M"), _T("wind")) << Sub2(_T("W"), _T("bot")) << _T("/(2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
      *pPara << _T("Bottom Right ") << RPT_STRESS(_T("w")) << _T(" = -") << Sub2(_T("M"), _T("wind")) << Sub2(_T("W"), _T("bot")) << _T("/(2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
   }
   else
   {
      *pPara << RPT_STRESS(_T("w")) << _T("(x,y)") << _T(" = ")
         << Sub2(_T("M"), _T("wind")) << Sub2(_T("I"), _T("xy")) << _T("(x)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
         << _T(" - ")
         << Sub2(_T("M"), _T("wind")) << Sub2(_T("I"), _T("yy")) << _T("(y)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")") << rptNewLine;
   }

   *pPara << RPT_STRESS(_T("h")) << _T(" = ") << _T("stress due to horizontal component of lifting cable force without impact") << rptNewLine;
   if (bSimpleFormat)
   {
      *pPara << _T("Top ") << RPT_STRESS(_T("h")) << _T(" = -") << Sub2(_T("P"), _T("lift")) << _T("(1/") << Sub2(_T("A"), _T("g")) << _T(" + ") << Sub2(_T("y"), _T("r")) << Sub2(_T("Y"), _T("top")) << _T("/") << Sub2(_T("I"), _T("xx")) << _T(")") << rptNewLine;
      *pPara << _T("Bottom ") << RPT_STRESS(_T("h")) << _T(" = -") << Sub2(_T("P"), _T("lift")) << _T("(1/") << Sub2(_T("A"), _T("g")) << _T(" - ") << Sub2(_T("y"), _T("r")) << _T("(") << Sub2(_T("H"), _T("g")) << _T(" - ") << Sub2(_T("Y"), _T("top")) << _T(")/") << Sub2(_T("I"), _T("xx")) << _T(")") << rptNewLine;
   }
   else
   {
      *pPara << Sub2(_T("M"), _T("lift")) << _T(" = ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("y"), _T("r")) << rptNewLine;
      *pPara << RPT_STRESS(_T("h")) << _T("(x,y) = ")
         << _T("(") << Sub2(_T("M"), _T("lift")) << Sub2(_T("I"), _T("xy")) << _T(")(x)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
         << _T(" - ")
         << _T("(") << Sub2(_T("M"), _T("lift")) << Sub2(_T("I"), _T("yy")) << _T(")(y)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
         << _T(" - ")
         << Sub2(_T("P"), _T("lift")) << _T("/") << Sub2(_T("A"), _T("g")) << rptNewLine;
   }

   for ( IndexType impactCase = 0; impactCase <= nImpactCases; impactCase++ )
   {
      for ( IndexType windCase = 0; windCase <= nWindCases; windCase++ )
      {
         stbTypes::WindDirection wind = (stbTypes::WindDirection)windCase;

         CString strTitle;

         if ( bLabelImpact && !bLabelWind )
         {
            // more than one impact case but no wind
            strTitle.Format(_T("%s"),strImpact[impactCase]);
         }
         else if ( !bLabelImpact && bLabelWind )
         {
            // only one impact case and wind cases
            strTitle.Format(_T("Wind towards the %s"),strWindDir[wind]);
         }
         else if ( bLabelImpact && bLabelWind )
         {
            // more than one impact case and wind cases
            strTitle.Format(_T("%s, Wind towards the %s"),strImpact[impactCase],strWindDir[wind]);
         }
         else
         {
            strTitle = _T("");
         }

         Float64 windSign = (wind == stbTypes::Left ? 1 : -1);
         std::_tstring strWindSign(wind == stbTypes::Left ? _T("+") : _T("-"));
         std::_tstring strOppWindSign(wind == stbTypes::Left ? _T("-") : _T("+"));

         if (strTitle != _T(""))
         {
            pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
            *pChapter << pPara;
            *pPara << strTitle << rptNewLine;
            pPara = new rptParagraph;
            *pChapter << pPara;
         }

         if (strTitle == _T(""))
         {
            pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
            *pChapter << pPara;
            *pPara << _T("Equilibrium Tilt Angle") << rptNewLine;
            pPara = new rptParagraph;
            *pChapter << pPara;
            *pPara << Sub2(symbol(theta), _T("eq")) << _T(" = (") << Sub2(_T("e"), _T("i"));
         }
         else
         {
            *pPara << _T("Equilibrium Tilt Angle, ") << Sub2(symbol(theta), _T("eq")) << _T(" = (") << Sub2(_T("e"), _T("i"));
         }
         *pPara << _T(" ") << strWindSign.c_str() << _T(" ") << Sub2(_T("z"),_T("wind")) << _T(" ") << strOppWindSign.c_str() << _T(" ") << Sub2(_T("e"),_T("wind"));
         *pPara << _T(")/(") << Sub2(_T("y"),_T("r")) << _T(" - ") << Sub2(_T("z"),_T("o")) << _T(") = ");
         *pPara << tiltAngle.SetValue(pResults->ThetaEq[impactDir[impactCase]][wind]) << rptNewLine;

         *pPara << rptNewLine;

         if ( 0 < nImpactCases )
         {
            *pPara << _T("IM = ") << scalar.SetValue(impactFactor[impactCase]) << rptNewLine;

            *pPara << rptNewLine;
         }

         rptRcTable* pTotalStressTable = rptStyleManager::CreateDefaultTable(16,_T("Stresses"));
         (*pPara) << pTotalStressTable << rptNewLine;
         (*pPara) << Sub2(_T("F"),_T("so")) << _T(" = Section Offset Factor = ") << _T("(") << Sub2(_T("L"),_T("s")) << _T("/") << Sub2(_T("L"),_T("g")) << Super2(_T(")"),_T("2")) << _T(" - ") << _T("((") << Sub2(_T("L"),_T("g")) << _T(" - 2X)") << _T("/") << Sub2(_T("L"),_T("g")) << Super2(_T(")"),_T("2")) << rptNewLine;
         (*pPara) << Sub2(_T("e"),_T("h")) << _T(" = eccentricity of the horizontal component of the lift cable force = ") << Sub2(_T("m"),_T("e")) << Sub2(_T("e"),_T("lift")) << _T(" + ") << Sub2(_T("F"),_T("so")) << _T("(") << Sub2(_T("m"),_T("e")) << Sub2(_T("F"),_T("o")) << Sub2(_T("e"),_T("sweep")) << _T(" ") << strWindSign.c_str() << _T(" ") << Sub2(_T("z"),_T("wind")) << _T(") = ") << Sub2(_T("m"),_T("e")) << Sub2(_T("e"),_T("lift")) << _T("(1 - ") << Sub2(_T("F"),_T("so")) << _T(") + ") << Sub2(_T("F"),_T("so")) << _T("(") << Sub2(_T("e"),_T("i")) << _T(" ") << strWindSign.c_str() << _T(" ") << Sub2(_T("z"),_T("wind")) << _T(")") << rptNewLine;
         (*pPara) << Sub2(_T("M"),_T("h")) << _T(" = lateral moment due to horizontal component of the lift cable force = (IM)") << Sub2(_T("P"),_T("lift")) << Sub2(_T("e"),_T("h")) << rptNewLine;
         (*pPara) << RPT_STRESS(_T("direct")) << _T(" = ") << RPT_STRESS(_T("ps")) << _T(" + (IM)(") << RPT_STRESS(_T("g")) << _T(" + ") << RPT_STRESS(_T("h")) << _T(") ") << strWindSign.c_str() << _T(" ") << RPT_STRESS(_T("w")) << rptNewLine;
         (*pPara) << RPT_STRESS(_T("tilt")) << _T(" = stress induced by girder rotation to equilibrium position") << rptNewLine;
         if (bSimpleFormat)
         {
            (*pPara) << _T("Top Left ") << RPT_STRESS(_T("tilt")) << _T(" = ((IM)") << _T("(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) << _T(")") << Sub2(symbol(theta), _T("eq")) << _T(" + ") << Sub2(_T("M"), _T("h")) << _T(")") << Sub2(_T("W"), _T("top")) << _T("/2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
            (*pPara) << _T("Top Right ") << RPT_STRESS(_T("tilt")) << _T(" = -((IM)") << _T("(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) << _T(")") << Sub2(symbol(theta), _T("eq")) << _T(" + ") << Sub2(_T("M"), _T("h")) << _T(")") << Sub2(_T("W"), _T("top")) << _T("/2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
            (*pPara) << _T("Bottom Left ") << RPT_STRESS(_T("tilt")) << _T(" = ((IM)") << _T("(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) << _T(")") << Sub2(symbol(theta), _T("eq")) << _T(" + ") << Sub2(_T("M"), _T("h")) << _T(")") << Sub2(_T("W"), _T("bot")) << _T("/2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
            (*pPara) << _T("Bottom Right ") << RPT_STRESS(_T("tilt")) << _T(" = -((IM)") << _T("(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) << _T(")") << Sub2(symbol(theta), _T("eq")) << _T(" + ") << Sub2(_T("M"), _T("h")) << _T(")") << Sub2(_T("W"), _T("bot")) << _T("/2") << Sub2(_T("I"), _T("yy")) << _T(")") << rptNewLine;
         }
         else
         {
            (*pPara) << RPT_STRESS(_T("tilt")) << _T("(x,y) = ")
               << _T("(IM)(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) << _T(")") << Sub2(symbol(theta), _T("eq")) << Sub2(_T("I"), _T("xx")) << _T(")(x)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")")
               << _T(" - ")
               << _T("(IM)(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) <<_T(")") << Sub2(symbol(theta), _T("eq")) << Sub2(_T("I"), _T("xy")) << _T(")(y)/(") << Sub2(_T("I"), _T("xx")) << Sub2(_T("I"), _T("yy")) << _T(" - ") << Super2(Sub2(_T("I"), _T("xy")), _T("2")) << _T(")") << rptNewLine;
         }
         (*pPara) << RPT_STRESS(_T("total")) << _T(" = ") << RPT_STRESS(_T("direct")) << _T(" + ") << RPT_STRESS(_T("tilt")) << rptNewLine;

         col = 0;
         pTotalStressTable->SetNumberOfHeaderRows(2);
         pTotalStressTable->SetRowSpan(0,col,2);
         pTotalStressTable->SetRowSpan(1,col,SKIP_CELL);
         if ( lpszLocColumnLabel )
         {
            (*pTotalStressTable)(0,col++) << COLHDR(rptRcStringLiteral(lpszLocColumnLabel),rptLengthUnitTag,pDisplayUnits->SpanLength);
         }
         else
         {
            (*pTotalStressTable)(0,col++) << COLHDR(_T("Dist from") << rptNewLine << _T("left end"),rptLengthUnitTag,pDisplayUnits->SpanLength);
         }

         pTotalStressTable->SetRowSpan(0,col,2);
         pTotalStressTable->SetRowSpan(1,col,SKIP_CELL);
         (*pTotalStressTable)(0,col++) << Sub2(_T("F"),_T("so"));

         pTotalStressTable->SetRowSpan(0,col,2);
         pTotalStressTable->SetRowSpan(1,col,SKIP_CELL);
         (*pTotalStressTable)(0,col++) << COLHDR(Sub2(_T("e"),_T("h")), rptLengthUnitTag, pDisplayUnits->ComponentDim);

         pTotalStressTable->SetRowSpan(0,col,2);
         pTotalStressTable->SetRowSpan(1,col,SKIP_CELL);
         (*pTotalStressTable)(0,col++) << COLHDR(Sub2(_T("M"),_T("h")), rptMomentUnitTag, pDisplayUnits->Moment);

         pTotalStressTable->SetColumnSpan(0,col,4);
         pTotalStressTable->SetColumnSpan(0,col+1,SKIP_CELL);
         pTotalStressTable->SetColumnSpan(0,col+2,SKIP_CELL);
         pTotalStressTable->SetColumnSpan(0,col+3,SKIP_CELL);
         (*pTotalStressTable)(0,col)   << RPT_STRESS(_T("direct"));
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Top Left"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Top Right"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Bottom Left"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Bottom Right"),rptStressUnitTag,pDisplayUnits->Stress);

         pTotalStressTable->SetColumnSpan(0,col,4);
         pTotalStressTable->SetColumnSpan(0,col+1,SKIP_CELL);
         pTotalStressTable->SetColumnSpan(0,col+2,SKIP_CELL);
         pTotalStressTable->SetColumnSpan(0,col+3,SKIP_CELL);
         (*pTotalStressTable)(0,col)   << RPT_STRESS(_T("tilt"));
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Top Left"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Top Right"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Bottom Left"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Bottom Right"),rptStressUnitTag,pDisplayUnits->Stress);

         pTotalStressTable->SetColumnSpan(0,col,4);
         pTotalStressTable->SetColumnSpan(0,col+1,SKIP_CELL);
         pTotalStressTable->SetColumnSpan(0,col+2,SKIP_CELL);
         pTotalStressTable->SetColumnSpan(0,col+3,SKIP_CELL);
         (*pTotalStressTable)(0,col)   << RPT_STRESS(_T("total"));
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Top Left"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Top Right"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Bottom Left"),rptStressUnitTag,pDisplayUnits->Stress);
         (*pTotalStressTable)(1,col++) << COLHDR(_T("Bottom Right"),rptStressUnitTag,pDisplayUnits->Stress);

         rptRcTable* pCrackingTable = rptStyleManager::CreateDefaultTable(5,_T("Factor of Safety Against Cracking"));
         (*pPara) << pCrackingTable << rptNewLine;
         (*pPara) << Sub2(_T("M"),_T("cr")) << _T(" = Cracking Moment") << rptNewLine;
         if (bSimpleFormat)
         {
            (*pPara) << _T("Top Flange ") << Sub2(_T("M"), _T("cr")) << _T(" = (") << RPT_STRESS(_T("r")) << _T(" - ") << RPT_STRESS(_T("direct")) << _T(")2") << Sub2(_T("I"), _T("yy")) << _T("/") << Sub2(_T("W"), _T("top")) << _T(" - ") << Sub2(_T("M"), _T("h")) << rptNewLine;
            (*pPara) << _T("Bottom Flange ") << Sub2(_T("M"), _T("cr")) << _T(" = (") << RPT_STRESS(_T("r")) << _T(" - ") << RPT_STRESS(_T("direct")) << _T(")2") << Sub2(_T("I"), _T("yy")) << _T("/") << Sub2(_T("W"), _T("bot")) << _T(" - ") << Sub2(_T("M"), _T("h")) << rptNewLine;
         }
         else
         {
            (*pPara) << Sub2(_T("M"), _T("cr")) << _T(" = ") << _T("(") << RPT_STRESS(_T("r")) << _T(" - ") << RPT_STRESS(_T("direct")) << _T(")") 
               << _T("(") << Sub2(_T("I"),_T("xx")) << Sub2(_T("I"),_T("yy")) << _T(" - ") << Super2(Sub2(_T("I"),_T("xy")),_T("2")) << _T(")")
               << _T("(") << Sub2(_T("I"),_T("xx")) << _T("(x)") << _T(" - ") << Sub2(_T("I"),_T("xy")) << _T("(y)") << _T(")") 
               << _T(" - ") << Sub2(_T("M"),_T("h")) << rptNewLine;
         }
         (*pPara) << _T("Cracked Flange, indicates the flange that is first to crack") << rptNewLine;
         (*pPara) << Sub2(symbol(theta), _T("cr")) << _T(" = tilt angle at cracking") << rptNewLine;
         (*pPara) << Sub2(symbol(theta),_T("cr")) << _T(" = ") << Sub2(_T("M"), _T("cr")) << _T("/(") << Sub2(_T("M"), _T("girder")) << _T(" + ") << Sub2(_T("P"), _T("lift")) << Sub2(_T("z"), _T("o")) << _T(") ") << symbol(LTE) << _T(" 0.4 radians") << rptNewLine;
         (*pPara) << Sub2(_T("FS"), _T("cr")) << _T(" = Factor of Safety Against Cracking") << rptNewLine;
         (*pPara) << Sub2(_T("FS"),_T("cr")) << _T(" = (") << Sub2(_T("y"),_T("r")) << Sub2(symbol(theta),_T("cr")) << _T(") / (") << Sub2(_T("e"),_T("i")) << _T(" + ") << Sub2(_T("z"),_T("o")) << Sub2(symbol(theta),_T("cr")) << _T(" ") << strWindSign.c_str() << _T(" ") << Sub2(_T("z"),_T("wind")) << _T(" ") << strOppWindSign.c_str() << _T(" ") << Sub2(_T("e"),_T("wind")) << _T(")") << rptNewLine;

         col = 0;
         if ( lpszLocColumnLabel )
         {
            (*pCrackingTable)(0,col++) << COLHDR(rptRcStringLiteral(lpszLocColumnLabel),rptLengthUnitTag,pDisplayUnits->SpanLength);
         }
         else
         {
            (*pCrackingTable)(0,col++) << COLHDR(_T("Dist from") << rptNewLine << _T("left end"),rptLengthUnitTag,pDisplayUnits->SpanLength);
         }
         (*pCrackingTable)(0,col++) << COLHDR(Sub2(_T("M"),_T("cr")),rptMomentUnitTag,pDisplayUnits->Moment);
         (*pCrackingTable)(0,col++) << _T("Cracked Flange");
         (*pCrackingTable)(0,col++) << COLHDR(Sub2(symbol(theta),_T("cr")),rptAngleUnitTag,pDisplayUnits->RadAngle);
         (*pCrackingTable)(0,col++) << Sub2(_T("FS"),_T("cr"));


         rptRcTable* pRebarTable = nullptr;
         if ( segment )
         {
            pRebarTable = rptStyleManager::CreateDefaultTable(7,_T("Bonded reinforcement requirements [C5.9.4.1.2]"));
         
            col = 0;
            if ( lpszLocColumnLabel )
            {
               (*pRebarTable)(0,col++) << COLHDR(rptRcStringLiteral(lpszLocColumnLabel),rptLengthUnitTag,pDisplayUnits->SpanLength);
            }
            else
            {
               (*pRebarTable)(0,col++) << COLHDR(_T("Dist from") << rptNewLine << _T("left end"),rptLengthUnitTag,pDisplayUnits->SpanLength);
            }
            (*pRebarTable)(0,col++) << COLHDR(Sub2(_T("Y"),_T("na")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
            (*pRebarTable)(0,col++) << _T("Slope NA");
            (*pRebarTable)(0,col++) << COLHDR(Sub2(_T("A"),_T("t")), rptAreaUnitTag,pDisplayUnits->Area);
            (*pRebarTable)(0,col++) << COLHDR(_T("T"),rptForceUnitTag,pDisplayUnits->GeneralForce);
            (*pRebarTable)(0,col++) << COLHDR(Sub2(_T("A"),_T("s")) << rptNewLine << _T("Provided") << Super(_T("*")), rptAreaUnitTag,pDisplayUnits->Area);
            (*pRebarTable)(0,col++) << COLHDR(Sub2(_T("A"),_T("s")) << rptNewLine << _T("Required") << Super(_T("**")), rptAreaUnitTag,pDisplayUnits->Area);
         }

         
         RowIndexType srow = pTotalStressTable->GetNumberOfHeaderRows();
         RowIndexType crow = pCrackingTable->GetNumberOfHeaderRows();
         RowIndexType rrow = (pRebarTable ? pRebarTable->GetNumberOfHeaderRows() : 0);
         for( const auto& sectionResult : pResults->vSectionResults)
         {
            col = 0;

            const stbIAnalysisPoint* pAnalysisPoint = pStabilityProblem->GetAnalysisPoint(sectionResult.AnalysisPointIndex);
            (*pTotalStressTable)(srow,col++) << rptRcStringLiteral(pAnalysisPoint->AsString(pDisplayUnits->SpanLength,offset,false));

            (*pTotalStressTable)(srow,col++) << scalar.SetValue(sectionResult.OffsetFactor);
            (*pTotalStressTable)(srow,col++) << shortLength.SetValue(sectionResult.eh[impactDir[impactCase]][wind]);
            (*pTotalStressTable)(srow,col++) << moment.SetValue(sectionResult.Mh[impactDir[impactCase]][wind]);

            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fDirect[impactDir[impactCase]][wind][stbTypes::TopLeft]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fDirect[impactDir[impactCase]][wind][stbTypes::TopRight]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fDirect[impactDir[impactCase]][wind][stbTypes::BottomLeft]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fDirect[impactDir[impactCase]][wind][stbTypes::BottomRight]);

            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fTilt[impactDir[impactCase]][wind][stbTypes::TopLeft]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fTilt[impactDir[impactCase]][wind][stbTypes::TopRight]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fTilt[impactDir[impactCase]][wind][stbTypes::BottomLeft]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.fTilt[impactDir[impactCase]][wind][stbTypes::BottomRight]);

            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.f[impactDir[impactCase]][wind][stbTypes::TopLeft]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.f[impactDir[impactCase]][wind][stbTypes::TopRight]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.f[impactDir[impactCase]][wind][stbTypes::BottomLeft]);
            (*pTotalStressTable)(srow,col++) << stress.SetValue(sectionResult.f[impactDir[impactCase]][wind][stbTypes::BottomRight]);

            srow++;

            col = 0;
            (*pCrackingTable)(crow,col++) << rptRcStringLiteral(pAnalysisPoint->AsString(pDisplayUnits->SpanLength,offset,false));
            (*pCrackingTable)(crow,col++) << moment.SetValue(sectionResult.Mcr[impactDir[impactCase]][wind][sectionResult.CrackedFlange[impactDir[impactCase]][wind]]);
            (*pCrackingTable)(crow,col++) << strFlange[sectionResult.CrackedFlange[impactDir[impactCase]][wind]].c_str();
            (*pCrackingTable)(crow,col++) << crackAngle.SetValue(sectionResult.ThetaCrack[impactDir[impactCase]][wind][sectionResult.CrackedFlange[impactDir[impactCase]][wind]]);
            (*pCrackingTable)(crow,col++) << scalar.SetValue(sectionResult.FScr[impactDir[impactCase]][wind]);

            crow++;

            if ( segment )
            {
               col = 0;
               (*pRebarTable)(rrow,col++) << rptRcStringLiteral(pAnalysisPoint->AsString(pDisplayUnits->SpanLength,offset,false));
               if ( sectionResult.AsRequired[impactDir[impactCase]][wind] < 0 )
               {
                  (*pRebarTable)(rrow,col++) << _T("-");
                  (*pRebarTable)(rrow,col++) << _T("-");
                  (*pRebarTable)(rrow,col++) << _T("-");
                  (*pRebarTable)(rrow,col++) << _T("-");
                  (*pRebarTable)(rrow,col++) << _T("-");
                  (*pRebarTable)(rrow,col++) << _T("-");
               }
               else
               {
                  (*pRebarTable)(rrow,col++) << shortLength.SetValue(sectionResult.Yna[impactDir[impactCase]][wind]);
                  (*pRebarTable)(rrow,col++) << scalar.SetValue(sectionResult.NAslope[impactDir[impactCase]][wind]);
                  (*pRebarTable)(rrow,col++) << area.SetValue(sectionResult.AreaTension[impactDir[impactCase]][wind]);
                  (*pRebarTable)(rrow,col++) << force.SetValue(sectionResult.T[impactDir[impactCase]][wind]);
                  (*pRebarTable)(rrow,col++) << area.SetValue(sectionResult.AsProvided[impactDir[impactCase]][wind]);
                  (*pRebarTable)(rrow,col++) << area.SetValue(sectionResult.AsRequired[impactDir[impactCase]][wind]);
               }
               
               rrow++;
            }
         } // next section

         pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
         *pChapter << pPara;
         *pPara << _T("Factor of Safety Against Failure") << rptNewLine;
         pPara = new rptParagraph;
         *pChapter << pPara;

         *pPara << Sub2(symbol(theta), _T("max")) << _T(" = maximum tilt angle") << rptNewLine;
         *pPara << Sub2(symbol(theta), _T("max")) << _T(" = ") << symbol(ROOT) << _T("((") << Sub2(_T("e"),_T("i")) << _T(" ") << strWindSign.c_str() << _T(" ") << Sub2(_T("z"),_T("wind")) << _T(" ") << strOppWindSign.c_str() << _T(" ") << Sub2(_T("e"),_T("wind")) << _T(") / (2.5") << Sub2(_T("z"),_T("o")) << _T(")) ") << symbol(LTE) << _T(" 0.4 radians") << rptNewLine;
         *pPara << Sub2(symbol(theta),_T("max")) << _T(" = ") << tiltAngle.SetValue(pResults->ThetaMax[impactDir[impactCase]][wind]) << rptNewLine;
         *pPara << Sub2(_T("FS"), _T("f")) << _T(" = Factor of Safety Against Failure") << rptNewLine;
         *pPara << Sub2(_T("FS"),_T("f")) << _T(" = ") << _T("(") << Sub2(_T("y"),_T("r")) << Sub2(symbol(theta),_T("max")) << _T(") / (") << Sub2(_T("e"),_T("i")) << _T(" + (1 + 2.5") << Sub2(symbol(theta),_T("max")) << _T(")(") << Sub2(_T("z"),_T("o")) << Sub2(symbol(theta),_T("max")) << _T(" ") << strWindSign.c_str() << _T(" ") << Sub2(_T("z"),_T("wind")) << _T(" ") << strOppWindSign.c_str() << _T(" ") << Sub2(_T("e"),_T("wind")) << _T(")) = ") << scalar.SetValue(pResults->FsFailure[impactDir[impactCase]][wind]) << rptNewLine;
         *pPara << _T("If ") << Sub2(_T("FS"),_T("f")) << _T(" is less than ") << Sub2(_T("FS"),_T("cr")) << _T(" then ") << Sub2(_T("FS"),_T("f")) << _T(" = ") << Sub2(_T("FS"),_T("cr")) << _T(". ");
         *pPara << Sub2(_T("FS"),_T("f")) << _T(" = ") << scalar.SetValue(pResults->FsFailure[impactDir[impactCase]][wind]) << _T(", ") << Sub2(_T("FS"),_T("cr")) << _T(" = ") << scalar.SetValue(pResults->MinFScr) << _T(", therefore ") << Sub2(_T("FS"),_T("f")) << _T(" = ") << scalar.SetValue(pResults->AdjFsFailure[impactDir[impactCase]][wind]) << rptNewLine; 

         *pPara << rptNewLine;

         if ( segment )
         {
            (*pPara) << pRebarTable << rptNewLine;
            (*pPara) << _T("The neutral axis is defined by its location with respect to the top center of the girder (") << Sub2(_T("Y"),_T("na")) << _T(") and its slope (Slope NA)") << rptNewLine;
            (*pPara) << Super(_T("*")) << _T(" provided reinforcement must be fully developed and lie within the tension area of the section before they are considered") << rptNewLine;
            (*pPara) << Super(_T("**")) << _T(" required reinforcement is the minimum area of sufficiently bonded reinforcement needed to use the alternative tensile stress limit") << rptNewLine;

            *pPara << rptNewLine;
         }

      } // next wind direction
   } // next impact direction

   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("Controlling Factors of Safety") << rptNewLine;
   pPara = new rptParagraph;
   *pChapter << pPara;

   CString strTitle;

   if ( bLabelImpact && !bLabelWind )
   {
      // more than one impact case but no wind
      strTitle.Format(_T("%s"),strImpact[impactIndex[pResults->FScrImpactDirection]]);
   }
   else if ( !bLabelImpact && bLabelWind )
   {
      // only one impact case and wind cases
      strTitle.Format(_T("Wind towards the %s"),strWindDir[pResults->FScrWindDirection]);
   }
   else if ( bLabelImpact && bLabelWind )
   {
      // more than one impact case and wind cases
      strTitle.Format(_T("%s, Wind towards the %s"),strImpact[impactIndex[pResults->FScrImpactDirection]],strWindDir[pResults->FScrWindDirection]);
   }
   else
   {
      strTitle = _T("");
   }

   longLength.ShowUnitTag(true);
   *pPara << _T("The minimum factor of safety against cracking, ") << rptRcStringLiteral(pStabilityProblem->GetAnalysisPoint(pResults->vSectionResults[pResults->FScrAnalysisPointIndex].AnalysisPointIndex)->AsString(pDisplayUnits->SpanLength,offset,true)) << _T(" ");
   *pPara << strFlange[pResults->vSectionResults[pResults->FScrAnalysisPointIndex].CrackedFlange[pResults->FScrImpactDirection][pResults->FScrWindDirection]].c_str() << _T(" flange tip");
   if (strTitle.IsEmpty() )
   {
      *pPara << rptNewLine;
   }
   else
   {
      *pPara << _T(" with ") << strTitle << rptNewLine;
   }
   *pPara << Sub2(_T("FS"),_T("cr")) << _T(" Min = ") << scalar.SetValue(pResults->MinFScr) << rptNewLine;

   *pPara << rptNewLine;

   if ( bLabelImpact && !bLabelWind )
   {
      // more than one impact case but no wind
      strTitle.Format(_T("%s"),strImpact[impactIndex[pResults->FSfImpactDirection]]);
   }
   else if ( !bLabelImpact && bLabelWind )
   {
      // only one impact case and wind cases
      strTitle.Format(_T("Wind towards the %s"),strWindDir[pResults->FSfWindDirection]);
   }
   else if ( bLabelImpact && bLabelWind )
   {
      // more than one impact case and wind cases
      strTitle.Format(_T("%s, Wind towards the %s"),strImpact[impactIndex[pResults->FSfImpactDirection]],strWindDir[pResults->FSfWindDirection]);
   }
   else
   {
      strTitle = _T("");
   }

   *pPara << _T("The minimum factor of safety against failure, ") << strTitle << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("f")) << _T(" Min = ") << scalar.SetValue(pResults->MinAdjFsFailure) << rptNewLine;
}
