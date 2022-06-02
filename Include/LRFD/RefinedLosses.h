///////////////////////////////////////////////////////////////////////
// LRFD - Utility library to support equations, methods, and procedures
//        from the AASHTO LRFD Bridge Design Specification
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

#ifndef INCLUDED_LRFD_REFINEDLOSSES_H_
#define INCLUDED_LRFD_REFINEDLOSSES_H_
#pragma once

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <Lrfd\LrfdExp.h>
#include <Lrfd\PsStrand.h>
#include <Lrfd\VersionMgrListener.h>
#include <Lrfd\Losses.h>

// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//

// MISCELLANEOUS
//

/*****************************************************************************
CLASS 
   lrfdRefinedLosses

   Utility class for computing prestress losses using the LRFD Refined Estimate
   Method.


DESCRIPTION
   Utility class for computing prestress losses using the LRFD Refined Estimate
   Method. Encapsulates Articles 5.9.5.2.3 and 5.9.5.4.

LOG
   rab : 12.01.1997 : Created file
*****************************************************************************/

class LRFDCLASS lrfdRefinedLosses : public lrfdLosses
{
public:

   // GROUP: LIFECYCLE

   //------------------------------------------------------------------------
   // Default constructor.
   // Strand type = LowRelaxation
   // Relative Humidity = 70%
   // Ep = 197000 MPa
   // Eci = 25000 MPa
   // fpu = 1860 MPa
   // fpj = 0.80*fpu
   // fpy = 0.90*fpu
   // Time to prestress transfer = 1 day
   // Aps = 1mm^2
   // A = 1mm^2
   // I = 1mm^4
   // e = 0 mm
   // Mg = 0 N-m
   // Mdl = 0 N-m
   lrfdRefinedLosses();

   //------------------------------------------------------------------------
   // Constructor.  Initializes the object with the give values.
   // fpy is initialized to 0.85fpu for StressRelieved strands and
   // 0.90fpu for LowRelaxation strands.
   lrfdRefinedLosses(Float64 x, // location along girder where losses are computed
                         Float64 Lg,    // girder length
                         lrfdLosses::SectionPropertiesType sectionProperties,
                         matPsStrand::Grade gradePerm, // strand grade
                         matPsStrand::Type typePerm, // strand type
                         matPsStrand::Coating coatingPerm, // strand coating (none, epoxy)
                         matPsStrand::Grade gradeTemp, // strand grade
                         matPsStrand::Type typeTemp, // strand type
                         matPsStrand::Coating coatingTemp, // strand coating (none, epoxy)
                         Float64 fpjPerm, // fpj permanent strands
                         Float64 fpjTemp, // fpj of temporary strands
                         Float64 ApsPerm,  // area of permanent strand
                         Float64 ApsTemp,  // area of TTS 
                         Float64 aps,      // area of one strand
                         const gpPoint2d& epermRelease, // eccentricty of permanent ps strands with respect to CG of girder at release
                         const gpPoint2d& epermFinal, // eccentricty of permanent ps strands with respect to CG of girder at final
                         const gpPoint2d& etemp, // eccentricty of temporary strands with respect to CG of girder
                         TempStrandUsage usage,
                         Float64 anchorSet,
                         Float64 wobble,
                         Float64 friction,
                         Float64 angleChange,

                         Float64 Fc,   // 28 day strength of girder concrete
                         Float64 Fci,  // Release strength
                         Float64 FcSlab,   
                         Float64 Ec,   // Modulus of elasticity of girder
                         Float64 Eci,  // Modulus of elasticity of girder at transfer
                         Float64 Ecd,  // Modulus of elasticity of deck

                         Float64 Mdlg,  // Dead load moment of girder only
                         const std::vector<std::pair<Float64, Float64>>& Madlg,  // Additional dead load on girder section (first value is moment, second is elastic gain reduction factor)
                         const std::vector<std::pair<Float64, Float64>>& Msidl1, // Superimposed dead loads, stage 1
                         const std::vector<std::pair<Float64, Float64>>& Msidl2, // Superimposed dead loads, stage 2

                         Float64 Ag,    // Area of girder
                         Float64 Ixx,    // Moment of inertia of girder
                         Float64 Iyy,
                         Float64 Ixy,
                         Float64 Ybg,   // Centroid of girder measured from bottom
                         Float64 Ac1,    // Area of the composite girder and deck
                         Float64 Ic1,    // Moment of inertia of composite
                         Float64 Ybc1,   // Centroid of composite measured from bottom
                         Float64 Ac2,    // Area of the composite girder and deck
                         Float64 Ic2,    // Moment of inertia of composite
                         Float64 Ybc2,   // Centroid of composite measured from bottom

                         Float64 An,    // Area of girder
                         Float64 Ixxn,    // Moment of inertia of girder
                         Float64 Iyyn,
                         Float64 Ixyn,
                         Float64 Ybn,   // Centroid of girder measured from bottom
                         Float64 Acn,    // Area of the composite girder and deck
                         Float64 Icn,    // Moment of inertia of composite
                         Float64 Ybcn,   // Centroid of composite measured from bottom

                         Float64 rh, // relative humidity

                         Float64 ti,   // Time until prestress transfer
                         Float64 shippingLosses, // time dependent loss at shipping or % of final time dependent loss if < 0
                         bool bValidateParameters
                         );


   //------------------------------------------------------------------------
   // Destructor
   ~lrfdRefinedLosses();


   // GROUP: ACCESS
   Float64 ShrinkageLosses() const;
   Float64 CreepLosses() const;
   Float64 TemporaryStrand_RelaxationLossesAtXfer() const;
   Float64 PermanentStrand_RelaxationLossesAtXfer() const;
   Float64 RelaxationLossesAfterXfer() const;
   Float64 TemporaryStrand_ImmediatelyBeforeXferLosses() const;
   Float64 PermanentStrand_ImmediatelyBeforeXferLosses() const;
   Float64 TemporaryStrand_ImmediatelyAfterXferLosses() const;
   Float64 PermanentStrand_ImmediatelyAfterXferLosses() const;

   //-------------------------------------------------------------
   virtual Float64 TemporaryStrand_TimeDependentLossesAtShipping() const override;
   virtual Float64 PermanentStrand_TimeDependentLossesAtShipping() const override;
   virtual Float64 TimeDependentLossesBeforeDeck() const override;
   virtual Float64 TimeDependentLossesAfterDeck() const override;
   virtual Float64 TimeDependentLosses() const override;

   // GROUP: INQUIRY
   // GROUP: DEBUG
   #if defined _UNITTEST
   static bool TestMe(dbgLog& rlog);
   #endif // _UNITTEST

protected:
   // GROUP: DATA MEMBERS
   // GROUP: LIFECYCLE
   // GROUP: OPERATORS
   // GROUP: OPERATIONS

   virtual void ValidateParameters() const override;
   virtual void UpdateLongTermLosses() const override;
   virtual void UpdateHaulingLosses() const override;

   mutable Float64 m_dfpSR;
   mutable Float64 m_dfpCR;
   mutable Float64 m_dfpR2;

   Float64 m_Shipping;
};


#endif // INCLUDED_LRFD_REFINEDLOSSES_H_
