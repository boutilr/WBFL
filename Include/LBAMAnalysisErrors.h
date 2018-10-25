///////////////////////////////////////////////////////////////////////
// LBAM Analysis - Longitindal Bridge Analysis Model
// Copyright � 1999-2013  Washington State Department of Transportation
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

#ifndef __LBAMAnalysisError_H_
#define __LBAMAnalysisError_H_

// HRESULT's for the Fem2d System


#define LBAMA_E_BLANK_STAGE_NAMES_NOT_ALLOWED  		     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,801)
#define LBAMA_E_BLANK_LOADGROUP_NAMES_NOT_ALLOWED  	     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,802)
#define LBAMA_E_DUPLICATE_STAGE_NAMES     			     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,804)
#define LBAMA_E_DUPLICATE_LOADGROUP_NAMES   			     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,805)
#define LBAMA_E_SPAN_LENGTH_MUST_BE_GT_ZERO               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,806)
#define LBAMA_E_SPAN_LENGTH_EXCEEDS_SUPERSTRUCTUREMEMBERS MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,807)
#define LBAMA_E_MEMBER_LENGTH_MUST_BE_GT_ZERO             MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,808)
#define LBAMA_E_SEGMENT_LENGTH_MUST_BE_GT_ZERO            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,809)
#define LBAMA_E_NO_SEGMENT_FOR_MEMBER                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,810)
#define LBAMA_E_NO_REM_STAGE_FOR_TS                       MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,812)
#define LBAMA_E_INVALID_TS_LOCATION                       MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,815)
#define LBAMA_E_INVALID_SECTION_PROP                      MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,816)
#define LBAMA_E_MUST_HAVE_ONE_SPAN                        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,817)
#define LBAMA_E_INVALID_SUPPORTS                          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,818)
#define LBAMA_E_SUPPORT_ROLLER_RELEASE                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,819)
#define LBAMA_E_NO_SEGMENT_FOR_SUPPORT                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,820)
#define LBAMA_E_SUPPORT_OFFSET_GT_LENGTH                  MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,821)
#define LBAMA_E_SUPPORT_LENGTH_MUST_BE_GT_ZERO            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,822)
#define LBAMA_E_INVALID_TS_ID_SUPPORT                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,823)
#define LBAMA_E_TEMPSUPPORT_OFFSET_GT_LENGTH              MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,825)
#define LBAMA_E_TEMPSUPPORT_LENGTH_MUST_BE_GT_ZERO        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,826)
#define LBAMA_E_INVALID_SEGMENT_LENGTH                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,827)
#define LBAMA_E_TEMPORARY_SUPPORT_ID                      MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,829)
#define LBAMA_E_MUST_HAVE_ONE_SSM                         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,830)
#define LBAMA_E_POI_SPAN_OOR                              MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,831)
#define LBAMA_E_POI_SSM_OOR                               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,832)
#define LBAMA_E_POI_SUPPORT_OOR                           MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,833)
#define LBAMA_E_POI_TEMP_SUPPORT_OOR                      MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,834)
#define LBAMA_E_INVALID_POI_LOC                           MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,835)
#define LBAMA_E_DUPLICATE_POI_ID                          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,836)
#define LBAMA_E_TEMPSUPPORT_DIST                          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,837)
#define LBAMA_E_POI_ZERO_LENGTH_SUPPORT                   MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,838)
#define LBAMA_E_TEMPSUPPORT_NOT_EXIST                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,839)
#define LBAMA_E_SPAN_NOT_EXIST                            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,840)
#define LBAMA_E_SUPPORT_NOT_EXIST                         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,841)
#define LBAMA_E_SSM_NOT_EXIST                             MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,842)
#define LBAMA_E_LOCATION_OOR                              MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,843)
#define LBAMA_E_LOC_4_ZERO_LENGTH_ZERO                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,844)
#define LBAMA_E_POINT_LOAD_LOC                            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,845)
#define LBAMA_E_POINT_LOAD_UNKNOWN                        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,846)
#define LBAMA_E_LOADGROUP_NOT_FOUND                       MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,847)
#define LBAMA_E_STAGE_NOT_FOUND                           MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,848)
#define LBAMA_E_MODEL_DOES_NOT_EXIST                      MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,849)
#define LBAMA_E_INVALID_MEMBER_TYPE                       MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,850)
#define LBAMA_E_DIST_LOAD_ZERO_LENGTH                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,851)
#define LBAMA_E_INVALID_DISTR_LOAD_LOCATION               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,852)
#define LBAMA_E_INVALID_LOAD_ORIENTATION                  MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,853)
#define LBAMA_E_INVALID_LOAD_DIRECTION                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,854)
#define LBAMA_E_INVALID_STRAIN_LOC                        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,855)
#define LBAMA_E_INVALID_TEMPERATURE_LOC                   MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,856)
#define LBAMA_E_GENERATING_TEMPERATURE_LOAD               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,857)
#define LBAMA_E_INVALID_DIST_LOC                          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,858)
#define LBAMA_E_TEMPERATURE_ZERO_DEPTH                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,859)
#define LBAMA_E_SETTLEMENT_LOC                            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,860)
#define LBAMA_E_INVALID_HINGE_REMOVAL_STAGE               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,861)
#define LBAMA_E_TEMPORARY_SUPPORTS_TOO_CLOSE              MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,862)
#define LBAMA_E_INVALID_POI_MEMBER_TYPE                   MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,863)
#define LBAMA_E_POI_NOT_FOUND                             MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,864)
#define LBAMA_E_STRESS_NC                                 MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,865)
#define LBAMA_E_NO_INFLUENCE_LOCATIONS                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,868)
#define LBAMA_E_TEMP_POIS_CUMMULATIVE                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,870)
#define LBAMA_E_INVALID_REGION                            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,871)
#define LBAMA_E_MM_STRESSPOINTS                           MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,872)
#define LBAMA_E_INCOMP_STRESSRESULTS                      MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,873)
#define LBAMA_E_INSTABILITY                               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,874)
#define LBAMA_E_FEM2D                                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,875)
#define LBAMA_E_POINT_LOAD_ZERO_LENGTH                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,877)

#endif 