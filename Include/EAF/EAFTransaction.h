///////////////////////////////////////////////////////////////////////
// EAF - Extensible Application Framework
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

#pragma once

#include <EAF\EAFExp.h>
#include <string>

/// Abstract base class of all transactions. A transaction is a unit of change in the system.
/// Transactions are excuted, undone, and redone
class EAFCLASS CEAFTransaction
{
public:
   CEAFTransaction() = default;
   virtual ~CEAFTransaction() = default; 

   CEAFTransaction(const CEAFTransaction&) = delete;
   CEAFTransaction& operator=(const CEAFTransaction&) = delete;

   /// Excutes the transation. Returns true if successful.
   virtual bool Execute();

   /// Causes the transaction to undo itself
   virtual void Undo();

   /// Creates a copy of the transaction
   virtual std::unique_ptr<CEAFTransaction> CreateClone() const = 0;

   /// Writes the name of the transaction to a stream
   virtual void Log(std::_tostream& os) const;

   /// Returns the name of the transaction. This is useful for Undo/Redo menu commands.
   virtual std::_tstring Name() const = 0;

   /// Returns true if the transaction can be undone
   virtual bool IsUndoable() const;

   /// Returns true if the transaction can be repeated
   virtual bool IsRepeatable() const;

#if defined _DEBUG
public:
   bool AssertValid() const;
   void Dump(WBFL::Debug::LogContext& os) const;
#endif // _DEBUG

#if defined _UNITTEST
public:
   static bool TestMe(WBFL::Debug::Log& rlog);
#endif // _UNITTEST
};
