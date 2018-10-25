///////////////////////////////////////////////////////////////////////
// DManipTools - Direct Manipulation Tools
// Copyright � 1999-2011  Washington State Department of Transportation
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

// HorzCurveDisplayObjectImpl.h : Declaration of the CHorzCurveDisplayObjectImpl

#ifndef __HORZCURVEDISPLAYOBJECT_H_
#define __HORZCURVEDISPLAYOBJECT_H_
#pragma once

#include "resource.h"       // main symbols
#include <DManipTools\HorzCurveDisplayObject.h>
#include <DManip\DisplayObjectDefaultImpl.h>
#include <WBFLCogo.h>

/////////////////////////////////////////////////////////////////////////////
// CHorzCurveDisplayObjectImpl
class ATL_NO_VTABLE CHorzCurveDisplayObjectImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
//   public CComRefCountTracer<CHorzCurveDisplayObjectImpl,CComObjectRootEx<CComSingleThreadModel> >,
	public CComCoClass<CHorzCurveDisplayObjectImpl, &CLSID_HorzCurveDisplayObject>,
    public CDisplayObjectDefaultImpl,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CHorzCurveDisplayObjectImpl>,
	public iHorzCurveDisplayObject
{
public:
   CHorzCurveDisplayObjectImpl();

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_HORZCURVEDISPLAYOBJECT)

DECLARE_PROTECT_FINAL_CONSTRUCT()
DECLARE_GET_CONTROLLING_UNKNOWN()

BEGIN_COM_MAP(CHorzCurveDisplayObjectImpl)
	COM_INTERFACE_ENTRY(iHorzCurveDisplayObject)
	COM_INTERFACE_ENTRY(iDisplayObject)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP() 

BEGIN_CONNECTION_POINT_MAP(CHorzCurveDisplayObjectImpl)
END_CONNECTION_POINT_MAP()

   // clipboard format
   static UINT ms_cfFormat;
   LOGFONT  m_Font;
   HorzCurveDrawMode m_DrawMode;
   CComPtr<IHorzCurve> m_HorzCurve;

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// iHorzCurveDisplayObject
public:
   STDMETHOD_(void,putref_HorzCurve)(IHorzCurve* hc,BOOL bRedraw,BOOL bFireEvent);
   STDMETHOD_(void,get_HorzCurve)(IHorzCurve* *hc);

   STDMETHOD_(void,put_DrawMode)(HorzCurveDrawMode mode);
   STDMETHOD_(void,get_DrawMode)(HorzCurveDrawMode* mode);

   // Font for labels. Note that font height is in 10th of points.
   // Note that Title is drawn in bold version of same font
   STDMETHOD_(void,put_Font)(const LOGFONT& Font);
   STDMETHOD_(void,get_Font)(LOGFONT* pFont);

// iDisplayObject Implementation
public:
   STDMETHOD_(void,SetID)(IDType id) { Do_SetID(id); }
   STDMETHOD_(IDType,GetID)() { return Do_GetID(); }

   STDMETHOD_(void,SetItemData)(void* pItemData,bool bDelete) { Do_SetItemData(pItemData,bDelete); }
   STDMETHOD_(void,GetItemData)(void** ppItemData) { Do_GetItemData(ppItemData); }

   STDMETHOD_(void,SetDisplayList)(iDisplayList * pDL) { Do_SetDisplayList(pDL); }
   STDMETHOD_(void,GetDisplayList)(iDisplayList** list) { Do_GetDisplayList(list); }

   STDMETHOD_(void,Visible)(BOOL bVisible) { Do_Visible(bVisible); }
   STDMETHOD_(BOOL,IsVisible)() { return Do_IsVisible(); }

   // Drawing
   STDMETHOD_(void,Draw)(CDC* pDC);
   STDMETHOD_(void,Highlite)(CDC* pDC,BOOL bHighlite);
#if defined(_DEBUG)
   STDMETHOD_(void,DrawGravityWell)(CDC* pDC) { Do_DrawGravityWell(pDC); }
#endif 

   // Size and Hit Testing
   STDMETHOD_(void,SetGravityWellStrategy)(iGravityWellStrategy* pStrategy) { Do_SetGravityWellStrategy(pStrategy); }
   STDMETHOD_(void,GetGravityWellStrategy)(iGravityWellStrategy** pStrategy){ Do_GetGravityWellStrategy(pStrategy); }
   STDMETHOD_(BOOL,HitTest)(CPoint point) { return Do_HitTest(point); }
   STDMETHOD_(CRect,GetBoundingBox)(){ return Do_GetBoundingBox(); }
   STDMETHOD_(void,GetBoundingBox)(IRect2d** rect); 
   STDMETHOD_(BOOL,TouchesRect)(CRect r) { return Do_TouchesRect(r); }

   // Selection
   STDMETHOD_(void,Select)(BOOL bSelect) {Do_Select(bSelect);}
   STDMETHOD_(BOOL,IsSelected)() {return Do_IsSelected();}
   STDMETHOD_(void,SetSelectionType)(SelectionType st) { Do_SetSelectionType(st); }
   STDMETHOD_(SelectionType,GetSelectionType)() { return Do_GetSelectionType(); }

   // Interface Events
   STDMETHOD_(bool,OnLButtonDown)(UINT nFlags,CPoint point) { return Do_OnLButtonDown(nFlags,point); }
   STDMETHOD_(bool,OnLButtonUp)(UINT nFlags,CPoint point) { return Do_OnLButtonUp(nFlags,point); }
   STDMETHOD_(bool,OnLButtonDblClk)(UINT nFlags,CPoint point) { return Do_OnLButtonDblClk(nFlags,point); }
   STDMETHOD_(bool,OnRButtonDown)(UINT nFlags,CPoint point) { return Do_OnRButtonDown(nFlags,point); }
   STDMETHOD_(bool,OnRButtonUp)(UINT nFlags,CPoint point) { return Do_OnRButtonUp(nFlags,point); }
   STDMETHOD_(bool,OnRButtonDblClk)(UINT nFlags,CPoint point) { return Do_OnRButtonDblClk(nFlags,point); }
   STDMETHOD_(bool,OnMouseMove)(UINT nFlags,CPoint point) { return Do_OnMouseMove(nFlags,point); }
   STDMETHOD_(bool,OnMouseWheel)(UINT nFlags,short zDelta,CPoint point) { return Do_OnMouseWheel(nFlags,zDelta,point); }
   STDMETHOD_(bool,OnKeyDown)(UINT nChar, UINT nRepCnt, UINT nFlags) { return Do_OnKeyDown(nChar,nRepCnt,nFlags); }
   STDMETHOD_(bool,OnContextMenu)(CWnd* pWnd,CPoint point) { return Do_OnContextMenu(pWnd,point); }

   // Tool Tips
   STDMETHOD_(void,SetToolTipText)(LPCTSTR lpszToolTipText) {Do_SetToolTipText(lpszToolTipText);}
   STDMETHOD_(CString,GetToolTipText)();
   STDMETHOD_(void,SetMaxTipWidth)(INT maxWidth) { Do_SetMaxTipWidth(maxWidth); }
   STDMETHOD_(INT,GetMaxTipWidth)() { return Do_GetMaxTipWidth(); }
   STDMETHOD_(void,SetTipDisplayTime)(INT iTime) { Do_SetTipDisplayTime(iTime); }
   STDMETHOD_(INT,GetTipDisplayTime)() { return Do_GetTipDisplayTime(); }

   // Event Sink
   STDMETHOD_(void,RegisterEventSink)(iDisplayObjectEvents* pEventSink)
   { Do_RegisterEventSink(pEventSink); }
   STDMETHOD_(void,UnregisterEventSink)()
   { Do_UnregisterEventSink(); }
   STDMETHOD_(void,GetEventSink)(iDisplayObjectEvents** pEventSink)
   { Do_GetEventSink(pEventSink); }

   // Drag Drop
   STDMETHOD_(void,SetDropSite)(iDropSite* pDropSite) { Do_SetDropSite(pDropSite); }
   STDMETHOD_(void,GetDropSite)(iDropSite** dropSite) { Do_GetDropSite(dropSite); }
};

#endif //__HORZCURVEDISPLAYOBJECT_H_
