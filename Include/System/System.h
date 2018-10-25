///////////////////////////////////////////////////////////////////////
// System - WBFL low level system services
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

#ifndef INCLUDED_SYSTEM_SYSTEM_H_
#define INCLUDED_SYSTEM_SYSTEM_H_
#pragma once


// This is a master include file all the packages contained in the system dll.
#if defined(BUILDSYSLIB)
	#error Do not use this header file in the System dll source files.
   #error It is for external users only
#endif

#include <WBFLTypes.h>
#include <WBFLDebug.h>
#include <System\Checks.h>
#include <System\Date.h>
#include <System\DllTest.h>
#include <System\DumpContext.h>
#include <System\EngNotation.h>
#include <System\Exception.h>
#include <System\FileDumpContext.h>
#include <System\LogDumpContext.h>
#include <System\Flags.h>
#include <System\IStructuredLoad.h>
#include <System\IStructuredSave.h>
#include <System\LineParseXml.h>
#include <System\Log.h>
#include <System\MacroTxn.h>
#include <System\NextAvailableId.h>
#include <System\NumericFormatTool.h>
#include <System\SectionValue.h>
#include <System\SectionAction.h>
#include <System\SingletonKiller.h>
#include <System\StructuredLoadXml.h>
#include <System\StructuredSaveXml.h>
#include <System\SubjectT.h>
#include <System\Tokenizer.h>
#include <System\Time.h>
#include <System\Transaction.h>
#include <System\TxnManager.h>
#include <SYstem\XProgrammingError.h>
#include <System\XStructuredLoad.h>
#include <System\XStructuredSave.h>
#include <System\ComCatMgr.h>

#endif // INCLUDED_SYSTEM_SYSTEM_H_