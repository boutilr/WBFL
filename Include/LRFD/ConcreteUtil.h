///////////////////////////////////////////////////////////////////////
// LRFD - Utility library to support equations, methods, and procedures
//        from the AASHTO LRFD Bridge Design Specification
// Copyright � 1999-2023  Washington State Department of Transportation
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

#ifndef INCLUDED_LRFD_CONCRETEUTIL_H_
#define INCLUDED_LRFD_CONCRETEUTIL_H_
#pragma once

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <Lrfd\LrfdExp.h>
#include <System\Exception.h>
#include <System\SectionValue.h>

#include <Materials/SimpleConcrete.h>

// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//

// MISCELLANEOUS
//

/*****************************************************************************
CLASS 
   lrfdConcreteUtil

   Utility class for concrete related calculations.


DESCRIPTION
   Utility class for concrete related calculations.

LOG
   rab : 11.14.1997 : Created file
*****************************************************************************/

class LRFDCLASS lrfdConcreteUtil
{
public:
   // GROUP: ENUMERATIONS

   // GROUP: LIFECYCLE

   //------------------------------------------------------------------------
   // Destructor
   ~lrfdConcreteUtil();

   // GROUP: OPERATORS
   // GROUP: OPERATIONS

   //------------------------------------------------------------------------
   // Returns the minimum density for concrete to be considered normal weight
   // per LRFD 5.2
   static Float64 GetNWCDensityLimit();

   //------------------------------------------------------------------------
   // Returns the maximum density for concrete to be considered light weight
   // per LRFD 5.2
   static Float64 GetLWCDensityLimit();

   // Gets the valid range of UHPC concrete strength
   static void GetPCIUHPCStrengthRange(Float64* pFcMin, Float64* pFcMax);
   static void GetPCIUHPCMinProperties(Float64* pfcMin, Float64* pffc, Float64* pfpeak, Float64* pfrr);

   //------------------------------------------------------------------------
   // Returns the modulus of elasticity of concrete.  The modulus of elasticity
   // is computed in accordance with equation 5.4.2.4-1.
   // If density is out of range, a lrfdXModE exception is thrown.
   static Float64 ModE(WBFL::Materials::ConcreteType type,Float64 fc,Float64 density,bool bCheckRange = true);

   //------------------------------------------------------------------------
   // Computes the concrete strength from the modulus of elasticy based on
   // LRFD Equation 5.2.4.2-1
   static Float64 FcFromEc(WBFL::Materials::ConcreteType type, Float64 ec,Float64 density);

   //------------------------------------------------------------------------
   // Returns the modulus of rupture.  The modulus of rupture is computed in
   // accordnace with Article 5.4.2.6
   static Float64 ModRupture(Float64 fc, WBFL::Materials::ConcreteType concreteType);
   static Float64 ModRupture(Float64 fc, Float64 k);

   //------------------------------------------------------------------------
   // Returns the beta1 factor.  The beta1 factor is computed in accordance
   // with Article 5.7.2.2
   static Float64 Beta1(Float64 fc);

   static void InterfaceShearParameters(bool isRoughened, WBFL::Materials::ConcreteType girderConcType, WBFL::Materials::ConcreteType deckConcType, Float64* pC, Float64* pU, Float64* pK1, Float64* pK2);

   //------------------------------------------------------------------------
   // Return nominal horizontal shear resistances per 5.8.4.1
   // Three resistances are returned for eqn's 1-3.
   // The governing resistance is determined by taking the min of these three
   static void InterfaceShearResistances(Float64 c, Float64 u, Float64 K1, Float64 K2,
                                          Float64 Acv, Float64 Avf, Float64 Pc,
                                          Float64 fc, Float64 fy,
                                          Float64* penqn1, Float64* penqn2, Float64* penqn3);

   //------------------------------------------------------------------------
   // Get lower limit of average shear strength Vn/Acv where steel is required by
   // 5.8.4.1-4.
   static Float64 LowerLimitOfShearStrength(bool isRoughened, bool doAllStirrupsEngageDeck);

   //------------------------------------------------------------------------
   // Get upper limit of bv for 5.8.4.1-4.
   static Float64 UpperLimitForBv();
   
   //------------------------------------------------------------------------
   // Get Min number of legs for a given bv  5.8.4.1-4.
   static Uint16 MinLegsForBv(Float64 bv);

   //------------------------------------------------------------------------
   // Calculate minimum Avf using 5.7.4.2 (pre2017: 5.8.4.4)
   // Use a struct to contain control information
   struct HsAvfOverSMinType
   {
      enum ValidEqnsType {eq41only, eqBoth}; // 
      ValidEqnsType ValidEqns; // Which equations were valid: 5.7.4.2-1, 5.7.4.2-2, or both
      Float64 res5_7_4_2_1;
      Float64 res5_7_4_2_3;
      Float64 AvfOverSMin; // Min of two values above, or zero if negative

      HsAvfOverSMinType(): // initialize with default constructor
      ValidEqns(eq41only), res5_7_4_2_1(0.0), res5_7_4_2_3(0.0), AvfOverSMin(0.0)
      {;}
   };

   static HsAvfOverSMinType AvfOverSMin(Float64 bv, Float64 fy,const WBFL::System::SectionValue& Vuh,Float64 phi,Float64 c,Float64 u,Float64 pc);

   //------------------------------------------------------------------------
   // Max bar spacing 5.8.4.1/2
   static Float64 MaxStirrupSpacingForHoriz(Float64 Hg);

   //------------------------------------------------------------------------
   // Calculate required Avf for horizontal shear
   static Float64 AvfRequiredForHoriz(const WBFL::System::SectionValue& Vuh, Float64 phi, Float64 AvfOverSMin,
                                      Float64 c, Float64 u, Float64 K1, Float64 K2,
                                      Float64 bv, Float64 Acv, Float64 Avf, Float64 Pc, 
                                      Float64 fc, Float64 fy);

   //------------------------------------------------------------------------
   // Calculates the concrete density modification factor (lambda)
   // per LRFD 5.4.2.8 (added to LRFD in 2016)
   static Float64 ComputeConcreteDensityModificationFactor(WBFL::Materials::ConcreteType type,Float64 density,bool bHasFct,Float64 fct,Float64 fc);

   static std::_tstring GetTypeName(WBFL::Materials::ConcreteType type,bool bFull);
   static WBFL::Materials::ConcreteType GetTypeFromTypeName(LPCTSTR strName);

   // GROUP: ACCESS
   // GROUP: INQUIRY
   // GROUP: DEBUG
#if defined _UNITTEST
   static bool TestMe(WBFL::Debug::Log& rlog);
#endif // _UNITTEST

protected:
   // GROUP: DATA MEMBERS
   // GROUP: LIFECYCLE
   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   // GROUP: ACCESS
   // GROUP: INQUIRY

private:
   // GROUP: DATA MEMBERS
   // GROUP: LIFECYCLE

   //------------------------------------------------------------------------
   // Default constructor
   lrfdConcreteUtil();

   // Prevent accidental copying and assignment
   lrfdConcreteUtil(const lrfdConcreteUtil&) = delete;
   lrfdConcreteUtil& operator=(const lrfdConcreteUtil&) = delete;

   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   // GROUP: ACCESS
   // GROUP: INQUIRY
   static Float64 ModRupture(Float64 fc, WBFL::Materials::ConcreteType concreteType,Float64 k);
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif // INCLUDED_LRFD_CONCRETEUTIL_H_
