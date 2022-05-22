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

#ifndef INCLUDED_LRFD_XCODEVERSION_H_
#define INCLUDED_LRFD_XCODEVERSION_H_
#pragma once

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <Lrfd\LrfdExp.h>
#include <System\Exception.h>

// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//

// MISCELLANEOUS
//

/*****************************************************************************
CLASS 
   lrfdXCodeVersion

   Exception object thrown when the current code version is not supprted by a
   function or a class.


DESCRIPTION
   Exception object thrown when the current code version is not supprted by a
   function or a class.

LOG
   rab : 11.14.1997 : Created file
*****************************************************************************/

class LRFDCLASS lrfdXCodeVersion : public WBFL::System::XBase
{
public:
   // GROUP: ENUMERATION
   enum Reason { BadVersion };

   // GROUP: LIFECYCLE

   //------------------------------------------------------------------------
   // Default constructor
   lrfdXCodeVersion(Reason reason,
                    LPCTSTR file,
                    Int16 line);

   //------------------------------------------------------------------------
   // Copy constructor
   lrfdXCodeVersion(const lrfdXCodeVersion& rOther) = default;

   //------------------------------------------------------------------------
   // Destructor
   virtual ~lrfdXCodeVersion() = default;

   // GROUP: OPERATORS
   //------------------------------------------------------------------------
   // Assignment operator
   lrfdXCodeVersion& operator = (const lrfdXCodeVersion& rOther) = default;

   // GROUP: OPERATIONS
   virtual void Throw() const override;
   virtual Int32 GetReason() const noexcept override;
   Reason GetReasonCode() const noexcept;

   // GROUP: ACCESS

   // GROUP: INQUIRY
   // GROUP: DEBUG
#if defined _DEBUG
   //------------------------------------------------------------------------
   // Returns <b>true</b> if the class is in a valid state, otherwise returns
   // <b>false</b>.
   virtual bool AssertValid() const;

   //------------------------------------------------------------------------
   // Dumps the contents of the class to the given stream.
   virtual void Dump(WBFL::Debug::LogContext& os) const;
#endif // _DEBUG

private:
   // GROUP: DATA MEMBERS
   Reason m_Reason;

   // GROUP: LIFECYCLE
   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   // GROUP: ACCESS
   // GROUP: INQUIRY
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif // INCLUDED_LRFD_XCODEVERSION_H_
