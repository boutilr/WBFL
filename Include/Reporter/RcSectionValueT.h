///////////////////////////////////////////////////////////////////////
// Reporter - Report Creation and Representation Library
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

#ifndef INCLUDED_REPORTER_RCSECTIONVALUET_H_
#define INCLUDED_REPORTER_RCSECTIONVALUET_H_
#pragma once

#include <Reporter\ReporterExp.h>
#include <Reporter\RcSectionValue.h>
#include <System\SectionValue.h>
#include <Units\PhysicalT.h>
#include <Units\SysUnits.h>
#include <MathEx.h>


#if defined BUILDREPORTERLIB
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif // BUILDREPORTERLIB


/// Template class the implements the rptSectionValue abstract interface.
template <class T>
class rptRcSectionValueT : public rptRcSectionValue
{
public:
   rptRcSectionValueT(const sysSectionValue& value,  ///< unit value in system units
                      const T* pUnitOfMeasure,       ///< pointer to unit of measure
                      Float64 zeroTolerance = 0.,    ///< tolerance for zeroness
                      bool bShowUnitTag = true) :    ///< indicates if the unit tag is to be included in with the output string
   rptRcSectionValue( bShowUnitTag ),
   m_Value( value ),
   m_pUnitOfMeasure( pUnitOfMeasure ),
   m_ZeroTolerance( zeroTolerance )
   {
   }

   rptRcSectionValueT(const T* pUnitOfMeasure,       ///< pointer to unit of measure
                      Float64 zeroTolerance = 0.,    ///< tolerance for zeroness
                      bool bShowUnitTag = true) :    ///< indicates if the unit tag is to be included in with the output string
   rptRcSectionValue( bShowUnitTag ),
   m_Value( 0.00 ),
   m_pUnitOfMeasure( pUnitOfMeasure ),
   m_ZeroTolerance( zeroTolerance )
   {
   }

   rptRcSectionValueT(const rptRcSectionValueT& rOther) :
   rptRcSectionValue( rOther )
   {
      MakeCopy( rOther );
   }

   virtual ~rptRcSectionValueT()
   {
   }

   rptRcSectionValueT& operator=(const rptRcSectionValueT& rOther)
   {
      if ( this != &rOther )
         MakeAssignment( rOther );

      return *this;
   }

   /// Creates a clone
   virtual rptReportContent* CreateClone() const override
   {
      return new rptRcSectionValueT( *this );
   }


   /// Assings a new section value and returns a reference to this
   virtual rptReportContent& SetValue(const sysSectionValue& value) override
   {
      m_Value = value;
      return *this;
   }

   /// Returns the section value.
   ///
   /// \param bConvert If true, the returned value is converted into the specifed unit of measure
   virtual sysSectionValue GetValue(bool bConvert = false) const override
   {
      return sysSectionValue( GetLeftValue(bConvert), GetRightValue(bConvert) );
   }

   /// Returns the left section value.
   ///
   /// \param bConvert If true, the returned value is converted into the specifed unit of measure
   virtual Float64 GetLeftValue(bool bConvert = false) const override
   {
      return GetValue( m_Value.Left(), bConvert );
   }

   /// Returns the right section value.
   ///
   /// \param bConvert If true, the returned value is converted into the specifed unit of measure
   virtual Float64 GetRightValue(bool bConvert = false) const override
   {
      return GetValue( m_Value.Right(), bConvert );
   }

   /// Returns the unit tag for the output unit of measure.
   virtual const std::_tstring& GetUnitTag() const override
   {
      return m_pUnitOfMeasure->UnitTag();
   }

protected:
   /// Copies the content from rOther to this object
   void MakeCopy(const rptRcSectionValueT& rOther)
   {
      m_Value          = rOther.m_Value;
      m_pUnitOfMeasure = rOther.m_pUnitOfMeasure;
      m_ZeroTolerance  = rOther.m_ZeroTolerance;
   }

   /// Assigns the content from oOther to this object
   virtual void MakeAssignment(const rptRcSectionValueT& rOther)
   {
      rptRcSectionValue::MakeAssignment( rOther );
      MakeCopy( rOther );
   }

private:
   sysSectionValue m_Value;
   const T* m_pUnitOfMeasure;
   Float64 m_ZeroTolerance;

   Float64 GetValue(Float64 v,bool bConvert = false) const
   {
      Float64 value = bConvert ? ::ConvertFromSysUnits(v, *m_pUnitOfMeasure) : v;

      if ( IsZero( value, m_ZeroTolerance ) )
         value = 0.;

      return value;
   }
};


#define DECLARE_RC_SECTION_VALUE(u,t) \
   REPORTERTPL rptRcSectionValueT<u>; \
   typedef rptRcSectionValueT<u> t;

DECLARE_RC_SECTION_VALUE( unitMass,           rptMassSectionValue           );
DECLARE_RC_SECTION_VALUE( unitMassPerLength,  rptMassPerLEngthSectionValue  );
DECLARE_RC_SECTION_VALUE( unitLength,         rptLengthSectionValue         );
DECLARE_RC_SECTION_VALUE( unitTime,           rptTimeSectionValue           );
DECLARE_RC_SECTION_VALUE( unitTemperature,    rptTemperatureSectionValue    );
DECLARE_RC_SECTION_VALUE( unitAngle,          rptAngleSectionValue          );
DECLARE_RC_SECTION_VALUE( unitLength2,        rptLength2SectionValue        );
DECLARE_RC_SECTION_VALUE( unitLength3,        rptLength3SectionValue        );
DECLARE_RC_SECTION_VALUE( unitLength4,        rptLength4SectionValue        );
DECLARE_RC_SECTION_VALUE( unitPressure,       rptPressureSectionValue       );
DECLARE_RC_SECTION_VALUE( unitDensity,        rptDensitySectionValue        );
DECLARE_RC_SECTION_VALUE( unitForce,          rptForceSectionValue          );
DECLARE_RC_SECTION_VALUE( unitForcePerLength, rptForcePerLengthSectionValue );
DECLARE_RC_SECTION_VALUE( unitMomentPerAngle, rptMomentPerAngleSectionValue );
DECLARE_RC_SECTION_VALUE( unitMoment,         rptMomentSectionValue         );
DECLARE_RC_SECTION_VALUE( unitSqrtPressure,   rptSqrtPressureValue          );

typedef rptPressureSectionValue rptStressSectionValue;
typedef rptLength2SectionValue  rptAreaSectionValue;
typedef rptLength3SectionValue  rptVolumeSectionValue;

#endif // INCLUDED_REPORTER_RCSECTIONVALUET_H_
