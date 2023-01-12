///////////////////////////////////////////////////////////////////////
// Math - Utility library of mathematical services
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

#include <Math\MathLib.h>
#include <Math\Range.h>
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace WBFL::Math;


Range::BoundType intersect_bounds(const Range::BoundType& b1, const Range::BoundType& b2)
{
   return (b1!=b2)? Range::BoundType::Limit : b1;
}
Range::BoundType union_bounds(const Range::BoundType& b1, const Range::BoundType& b2)
{
   return (b1!=b2)? Range::BoundType::Bound : b1;
}

Range::Range(Float64 leftLocation, BoundType leftBound, Float64 rightLocation, BoundType rightBound):
m_LeftLocation(leftLocation),
m_LeftBound(leftBound),
m_RightLocation(rightLocation),
m_RightBound(rightBound)
{
}

bool Range::operator==(const Range& rhs) const
{
   return m_LeftLocation == rhs.m_LeftLocation &&
          m_LeftBound == rhs.m_LeftBound &&
          m_RightLocation == rhs.m_RightLocation &&
          m_RightBound == rhs.m_RightBound;
}

void Range::Set(Float64 leftLocation, BoundType leftBound, Float64 rightLocation, BoundType rightBound)
{
   m_LeftLocation  = leftLocation;
   m_LeftBound     = leftBound;
   m_RightLocation = rightLocation;
   m_RightBound    = rightBound;
   ASSERTVALID;
}

Float64 Range::GetLeftBoundLocation() const
{
   ASSERTVALID;
   return m_LeftLocation;
}

void Range::SetLeftBoundLocation(Float64 bound)
{
   m_LeftLocation = bound;
}

Range::BoundType Range::GetLeftBoundType() const
{
   ASSERTVALID;
   return m_LeftBound;
}

void Range::SetLeftBoundType(Range::BoundType type)
{
   m_LeftBound = type;
}

Float64 Range::GetRightBoundLocation() const
{
   ASSERTVALID;
   return m_RightLocation;
}

void Range::SetRightBoundLocation(Float64 bound)
{
   m_RightLocation = bound;
}

Range::BoundType Range::GetRightBoundType() const
{
   ASSERTVALID;
   return m_RightBound;
}

void Range::SetRightBoundType(Range::BoundType type)
{
   m_RightBound = type;
}


bool Range::IsInRange(Float64 value) const
{
   ASSERTVALID;
   if (m_LeftLocation < value && value < m_RightLocation)
      return true;

   if (value == m_LeftLocation && m_LeftBound == BoundType::Bound)
      return true;

   if (value == m_RightLocation && m_RightBound == BoundType::Bound)
      return true;

   return false;
}

bool Range::IsNull() const
{
   ASSERTVALID;
   return m_LeftLocation == m_RightLocation &&
      m_LeftBound == BoundType::Limit &&
      m_RightBound == BoundType::Limit;
}

void Range::SetNull()
{
   m_LeftLocation = 0.0;
   m_RightLocation = 0.0;
   m_LeftBound = BoundType::Limit;
   m_RightBound = BoundType::Limit;
}

bool Range::IsIntersection(const Range& rOther) const
{
   ASSERTVALID;
   if (!(IsNull() || rOther.IsNull()))
      return m_LeftLocation <= rOther.m_RightLocation &&
      rOther.m_LeftLocation <= m_RightLocation;
   else
      return false;
}

Range Range::Intersection(const Range& rOther) const
{
   ASSERTVALID;
   Range range;
   if (IsIntersection(rOther))
   {
      // left bound
      if (rOther.m_LeftLocation < m_LeftLocation)
      {
         range.m_LeftLocation = m_LeftLocation;
         range.m_LeftBound = m_LeftBound;
      }
      else if (m_LeftLocation < rOther.m_LeftLocation)
      {
         range.m_LeftLocation = rOther.m_LeftLocation;
         range.m_LeftBound = rOther.m_LeftBound;
      }
      else
      {
         // edges match - Limit wins in a contention
         range.m_LeftLocation = rOther.m_LeftLocation;
         range.m_LeftBound = intersect_bounds(m_LeftBound, rOther.m_LeftBound);
      }

      // Right bound
      if (m_RightLocation < rOther.m_RightLocation)
      {
         range.m_RightLocation = m_RightLocation;
         range.m_RightBound = m_RightBound;
      }
      else if (rOther.m_RightLocation < m_RightLocation)
      {
         range.m_RightLocation = rOther.m_RightLocation;
         range.m_RightBound = rOther.m_RightBound;
      }
      else
      {
         range.m_RightLocation = rOther.m_RightLocation;
         range.m_RightBound = intersect_bounds(m_RightBound, rOther.m_RightBound);
      }
   }

   return range;
}

Range Range::Union(const Range& rOther) const
{
   ASSERTVALID;
   Range range;
   // left bound
   if (m_LeftLocation < rOther.m_LeftLocation)
   {
      range.m_LeftLocation = m_LeftLocation;
      range.m_LeftBound = m_LeftBound;
   }
   else if (rOther.m_LeftLocation < m_LeftLocation)
   {
      range.m_LeftLocation = rOther.m_LeftLocation;
      range.m_LeftBound = rOther.m_LeftBound;
   }
   else
   {
      // edges match - Limit wins in a contention
      range.m_LeftLocation = rOther.m_LeftLocation;
      range.m_LeftBound = union_bounds(m_LeftBound, rOther.m_LeftBound);
   }

   // right bound
   if (rOther.m_RightLocation < m_RightLocation)
   {
      range.m_RightLocation = m_RightLocation;
      range.m_RightBound = m_RightBound;
   }
   else if (m_RightLocation < rOther.m_RightLocation)
   {
      range.m_RightLocation = rOther.m_RightLocation;
      range.m_RightBound = rOther.m_RightBound;
   }
   else
   {
      // edges match - Limit wins in a contention
      range.m_RightLocation = rOther.m_RightLocation;
      range.m_RightBound = union_bounds(m_RightBound, rOther.m_RightBound);
   }

   return range;
}

#if defined _DEBUG
bool Range::AssertValid() const
{
   if (m_LeftLocation>m_RightLocation)
      return false;

   return true;
}

void Range::Dump(WBFL::Debug::LogContext& os) const
{
   os << "Dump for Range" << WBFL::Debug::endl;
   os << "m_LeftLocation:  " << m_LeftLocation << WBFL::Debug::endl;
   os << "m_LeftBound:     " << (m_LeftBound == BoundType::Bound ? "Bound" : "Limit") << WBFL::Debug::endl;
   os << "m_RightLocation: " << m_RightLocation << WBFL::Debug::endl;
   os << "m_RightBound:    " << (m_RightBound == BoundType::Bound ? "Bound" : "Limit") << WBFL::Debug::endl;
}
#endif // _DEBUG

#if defined _UNITTEST
bool Range::TestMe(WBFL::Debug::Log& rlog)
{
   TESTME_PROLOGUE("Range");
   Range r1;
   TRY_TESTME(r1.IsNull());
   r1.Set(10,Range::BoundType::Bound, 150, Range::BoundType::Bound);
   Range r2(-220,Range::BoundType::Bound, 100, Range::BoundType::Limit);
   Range r3 = r1.Intersection(r2);
   Range r4 = r2.Intersection(r1);
   TRY_TESTME(r3==r4);
   Range r5;
   r5.SetLeftBoundLocation(10);
   r5.SetRightBoundLocation(100);
   r5.SetLeftBoundType(Range::BoundType::Bound);
   r5.SetRightBoundType(Range::BoundType::Limit);
   TRY_TESTME(r5==r4);
   r4 = r2.Union(r1);
   r5.SetLeftBoundLocation(-220);
   r5.SetRightBoundLocation(150);
   r5.SetLeftBoundType(Range::BoundType::Bound);
   r5.SetRightBoundType(Range::BoundType::Bound);
   TRY_TESTME(r5==r4);
   TRY_TESTME(r5.GetLeftBoundLocation()==-220);
   TRY_TESTME(r5.GetRightBoundLocation()==150);
   TRY_TESTME(r5.GetLeftBoundType()==Range::BoundType::Bound);
   TRY_TESTME(r5.GetRightBoundType()==Range::BoundType::Bound);
   r5.SetNull();
   TRY_TESTME(r5.IsNull());

   TESTME_EPILOG("Range");
}
#endif // _UNITTEST
