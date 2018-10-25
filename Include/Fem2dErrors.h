///////////////////////////////////////////////////////////////////////
// Fem2D - Two-dimensional Beam Analysis Engine
// Copyright � 1999-2016  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This library is a part of the Washington Bridge Foundation Libraries
// and was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Library Open Source License as 
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// PURPOSE.  See the Alternate Route Library Open Source License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#ifndef __Fem2dError_H_
#define __Fem2dError_H_
#pragma once

// HRESULT's for the Fem2d System

#define FEM2D_E_MEMBER_HAS_SAME_JOINTS				    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,512)
#define FEM2D_E_MEMBER_JOINT_NOT_EXISTS				    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,513)
#define FEM2D_E_MEMBER_WITH_ID_ALREADY_EXISTS		    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,514)
#define FEM2D_E_JOINT_WITH_ID_ALREADY_EXISTS			    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,515)
#define FEM2D_E_STIFFNESS_MUST_BE_POSITIVE			    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,516)
#define FEM2D_E_JOINT_LOAD_WITH_ID_ALREADY_EXISTS	    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,517)
#define FEM2D_E_POINT_LOAD_WITH_ID_ALREADY_EXISTS	    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,518)
#define FEM2D_E_LOADING_WITH_ID_ALREADY_EXISTS	        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,519)
#define FEM2D_E_INVALID_POINT_LOAD_LOCATION	            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,520)
#define FEM2D_E_INVALID_POI_LOCATION	                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,521)
#define FEM2D_E_POI_WITH_ID_ALREADY_EXISTS			    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,522)
#define FEM2D_E_POINT_LOAD_OFF_GIRDER_END			    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,523)
#define FEM2D_E_FRACTIONAL_VALUE_OUT_OF_RANGE		    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,524)
#define FEM2D_E_ZERO_LENGTH_MEMBER		                MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,525)
#define FEM2D_E_JOINT_EQUILIBRIUM_NOT_SATISFIED          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,526)
#define FEM2D_E_MEMBER_EQUILIBRIUM_NOT_SATISFIED         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,527)
#define FEM2D_E_POI_REFERENCES_MEMBER_NOT_EXISTS         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,528)
#define FEM2D_E_POI_FRACTIONAL_OUT_OF_RANGE              MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,529)
#define FEM2D_E_POI_LOCATED_OFF_MEMBER_END               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,530)
#define FEM2D_E_JOINT_NOT_FOUND                          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,531)
#define FEM2D_E_LOADING_NOT_FOUND                        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,532)
#define FEM2D_E_MEMBER_NOT_FOUND                         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,533)
#define FEM2D_E_POI_NOT_FOUND                            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,534)
#define FEM2D_E_SOLVING_GLOBAL_STIFFNESS                 MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,535)
#define FEM2D_E_INVALID_GLOBAL_DOF                       MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,536)
#define FEM2D_E_INVALID_CONDENSED_DOF                    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,537)
#define FEM2D_E_JOINT_LOAD_REF_JOINT_NOT_EXISTS          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,538)
#define FEM2D_E_POINT_LOAD_REF_MEMBER_NOT_EXISTS         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,539)
#define FEM2D_E_JOINT_DISP_WITH_ID_ALREADY_EXISTS	    MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,540)
#define FEM2D_E_JOINT_DISP_REF_JOINT_NOT_EXISTS          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,541)
#define FEM2D_E_JOINT_DISP_TO_FIXED_DOF_ONLY             MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,542)
#define FEM2D_E_MATRIX_BACK_SUBSTITUTION                 MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,543)
#define FEM2D_E_MATRIX_FACTORING                         MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,544)
#define FEM2D_E_DIST_LOAD_OFF_GIRDER_END		            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,545)
#define FEM2D_E_DIST_LOAD_B_BEFORE_A    		            MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,547)
#define FEM2D_E_DIST_LOAD_REF_MEMBER_NOT_EXISTS          MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,548)
#define FEM2D_E_INVALID_DIST_LOAD_LOCATION               MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,549)
#define FEM2D_E_DIST_LOAD_WITH_ID_ALREADY_EXISTS	        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,550)
#define FEM2D_E_INVALID_LOAD_ORIENTATION        	        MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,551)
#define FEM2D_E_MEMBER_STRAIN_WITH_ID_ALREADY_EXISTS     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,552)
#define FEM2D_E_MODEL_HAS_NO_JOINTS                      MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,553)
#define FEM2D_E_MODEL_HAS_NO_MEMBERS                     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_ITF,554)

#endif