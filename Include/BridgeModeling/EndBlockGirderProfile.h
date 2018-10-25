///////////////////////////////////////////////////////////////////////
// BMF - Bridge Modeling Framework - Product modeling of bridge structures
// Copyright � 1999-2016  Washington State Department of Transportation
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

#ifndef INCLUDED_BRIDGEMODELING_ENDBLOCKGIRDERPROFILE_H_
#define INCLUDED_BRIDGEMODELING_ENDBLOCKGIRDERPROFILE_H_
#pragma once

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <BridgeModeling\BridgeModelingExp.h>
#include <BridgeModeling\GirderProfile.h>

// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//

// MISCELLANEOUS
//

/*****************************************************************************
CLASS 
   bmfEndBlockGirderProfile

   Profile object representing an I-beam with end blocks.


DESCRIPTION
   Profile object representing an I-beam with end blocks.

COPYRIGHT
   Copyright (c) 1997-2002
   Washington State Department Of Transportation
   All Rights Reserved

LOG
   rab : 10.03.2002 : Created file
*****************************************************************************/

class BMFCLASS bmfEndBlockGirderProfile : public bmfGirderProfile
{
public:
   // GROUP: LIFECYCLE
   // GROUP: OPERATORS
   // GROUP: OPERATIONS

   //------------------------------------------------------------------------
   virtual Float64 GetEndBlockLength() const = 0;

   // GROUP: ACCESS
   // GROUP: INQUIRY

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
   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   // GROUP: ACCESS
   // GROUP: INQUIRY
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif // INCLUDED_BRIDGEMODELING_ENDBLOCKGIRDERPROFILE_H_

