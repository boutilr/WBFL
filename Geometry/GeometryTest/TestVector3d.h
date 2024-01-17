///////////////////////////////////////////////////////////////////////
// Geometry - Geometric Modeling Library
// Copyright � 1999-2024  Washington State Department of Transportation
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

// TestVector3d.h: interface for the CTestVector3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVECTOR3D_H__19582346_B7D5_11D3_8A0B_006097C68A9C__INCLUDED_)
#define AFX_TESTVECTOR3D_H__19582346_B7D5_11D3_8A0B_006097C68A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTestVector3d  
{
public:
	static void Test();
	CTestVector3d();
	virtual ~CTestVector3d();

private:
	static void TestISupportErrorInfo();
};

#endif // !defined(AFX_TESTVECTOR3D_H__19582346_B7D5_11D3_8A0B_006097C68A9C__INCLUDED_)
