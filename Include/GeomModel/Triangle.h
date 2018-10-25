///////////////////////////////////////////////////////////////////////
// GeomModel - Geometric Modeling of shapes and sections
// Copyright � 1999-2017  Washington State Department of Transportation
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

#ifndef INCLUDED_GEOMMODEL_TRIANGLE_H_
#define INCLUDED_GEOMMODEL_TRIANGLE_H_
#pragma once

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <GeomModel\GeomModelExp.h>
#include <GeomModel\ShapeImp.h>

// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//
class gmPolygon;
class gpPolygon2d;

// MISCELLANEOUS
//

/*****************************************************************************
CLASS 
   gmTriangle

   Derived from gmShapeImp,  this class represents a Triangle primitive. 


DESCRIPTION
 The triangle is described by its height,  width,  offset,  rotation angle, 
 and is located by its hook point.  The rotation angle is measured from the 
 positive x-axis counter-clockwise.  The rotation angle defines the orientation
 of the base of the triangle (edge W). Rotation takes place about the hook point.
 The hook point of a triangle is at the lower left vertex.

LOG
   rdp : 12.22.1997 : Created file
*****************************************************************************/

class GEOMMODELCLASS gmTriangle : public gmShapeImp
{
public:
   // GROUP: LIFECYCLE

   //------------------------------------------------------------------------
   // Default constructor.  Creates a Triangle with height and width equal 
   // to zero,  with its hook point at (0,0).
   gmTriangle();

   //------------------------------------------------------------------------
   // gmTriangle
   // Explicit constructor.  Explicit constructor.  Creates a triangle with 
   // height h, width w, and offset c.  The hook point is positioned at 
   // hookPnt.  The triangle can be rotated by rotAngle..
   gmTriangle(const gpPoint2d& hookPnt,Float64 h, Float64 w,  Float64 c = 0.,
              Float64 rotAngle = 0.);

   //------------------------------------------------------------------------
   // Destructor
   virtual ~gmTriangle();

   // GROUP: OPERATORS
   // GROUP: OPERATIONS

   //------------------------------------------------------------------------
   // SetHookPoint
   // Sets the location of the hook point to hookPnt.  Returns the previous 
   // hook point location.
   gpPoint2d SetHookPoint(const gpPoint2d& hookPnt);

   //------------------------------------------------------------------------
   // GetHookPoint
   // Returns the location of the hook point.
   gpPoint2d GetHookPoint() const;

   //------------------------------------------------------------------------
   // SetWidth
   // Sets the width of the Triangle.  Returns the previous width.
   Float64 SetWidth(Float64 w);

   //------------------------------------------------------------------------
   // GetWidth
   // Returns the width of the Triangle.
   Float64 GetWidth() const;

   //------------------------------------------------------------------------
   // SetHeight
   // Sets the height of the Triangle.  Returns the previous height.
   Float64 SetHeight(Float64 h);

   //------------------------------------------------------------------------
   // GetHeight
   // Returns the height of the Triangle.
   Float64 GetHeight() const;

   //------------------------------------------------------------------------
   // SetOffset
   // Sets the offset of the triangle.
   Float64 SetOffset(Float64 c);

   //------------------------------------------------------------------------
   // GetOffset
   // Returns the offset of the triangle.
   Float64 GetOffset() const;

   //------------------------------------------------------------------------
   // SetRotationAngle
   // Sets the rotation angle of the triangle. The triangle is rotated about its hook point.
   Float64 SetRotationAngle(Float64 rotAngle);

   //------------------------------------------------------------------------
   // GetRotationAngle
   // Returns the rotation angle of the triangle.
   Float64 GetRotationAngle() const;


   //------------------------------------------------------------------------
   // CreateClone
   // Creates a clone of this broadcaster.  If bRegisterListeners is true the 
   // listeners are registered with the clone.  This is a factory method,  
   // you are responsible for freeing the memory allocated by this method.
   virtual gmIShape* CreateClone(bool bRegisterListeners = false) const;

   //------------------------------------------------------------------------
   // GetProperties
   // Assigns a gmProperties object to the object pointed to by pProperties. 
   // The origin of the shape properties object is the centroid of this shape
   // with a rotation of zero.
   virtual void GetProperties(gmProperties* pProperties) const;

   //------------------------------------------------------------------------
   // GetBoundingBox
   // Returns the smallest rectangle that bounds the entire shape.
   virtual gpRect2d GetBoundingBox() const;

   //------------------------------------------------------------------------
   // CreateClippedShape
   // Clips this shape against line.  Clips away the portion of the shape on the
   // side of the line defined by side.  This is a factory method.  You are 
   // responsible for freeing the memory allocated by this method.  If the shape
   // lies entirely on the clipping side of the line 0 is returned. Any listeners
   // to the original section are not transferred to this new section.
   virtual gmIShape* CreateClippedShape(const gpLine2d& line, 
                                       gpLine2d::Side side) const;

   //------------------------------------------------------------------------
   // CreateClippedShape
   // Clips this shape against Triangle r.  Clips in or out of the Triangle
   // as specified by region.  This is a factory method.  You are responsible 
   // for freeing memory allocated by this method.  This method returns 0 if, 
   // the shape lies entirely within the clipping Triangle and region is set 
   // to clip out, or the shape and the Triangle do not intersect and region 
   // is to clip in. Any listeners to the original section are not transferred
   // to this new section.
   virtual gmIShape* CreateClippedShape(const gpRect2d& r,
                                        gmShapeImp::ClipRegion region
                                        ) const;

   //------------------------------------------------------------------------
   // GetFurthestDistance
   // Returns the distance to a line that is parallel to line, on specified 
   // side of line,  that passes through the furthest point on the shape 
   // from line.
   virtual Float64 GetFurthestDistance(const gpLine2d& line, gpLine2d::Side side) const;

   //------------------------------------------------------------------------
   // Draw
   // Draws the shape on the given device context.  Mapping of the coordinates
   // to the device space should be done using the supplied point mapper.
   // Draw is for static displays only.  The drawing of this analytical 
   // model is not intended for interactive, graphical editing.  Interactive 
   // graphical editing is best left for a package specifically designed for 
   // this purpose, such as jKit/GO.  In a package like jKit/GO,  a GO object
   // would most likely delegate its drawing responsibility to the gmShapeImp 
   // object it represents.
   // Subject to removal if we can ever figure out the MVC stuff
   virtual void Draw(HDC hDC, const grlibPointMapper& mapper) const;

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
   virtual void Dump(dbgDumpContext& os) const;
#endif // _DEBUG

#if defined _UNITTEST

   //------------------------------------------------------------------------
   // Self-diagnostic test function.  Returns <b>true</b> if the test passes,
   // otherwise return <b>false</b>.
   static bool TestMe(dbgLog& rlog);

#endif // _UNITTEST

protected:
   // GROUP: DATA MEMBERS
   // GROUP: LIFECYCLE

   // Prevent accidental copying and assignment
   gmTriangle(const gmTriangle&);
   gmTriangle& operator=(const gmTriangle&);

   // GROUP: OPERATORS
   // GROUP: OPERATIONS

   //------------------------------------------------------------------------
   // DoTranslate
   // Called by the framework went the shape is to be translated.
   virtual void DoTranslate(const gpSize2d& delta);

   //------------------------------------------------------------------------
   // DoRotate
   // Called by the framework went the shape is to be rotated.
   virtual void DoRotate(const gpPoint2d& center, Float64 angle);

   //------------------------------------------------------------------------
   void MakeCopy(const gmTriangle& rOther);

   //------------------------------------------------------------------------
   virtual void MakeAssignment(const gmTriangle& rOther);

   // GROUP: ACCESS
   // GROUP: INQUIRY

private:
   // GROUP: DATA MEMBERS

   Float64   m_Height;
   Float64   m_Width;
   Float64   m_Offset;
   Float64   m_Rotation;
   gpPoint2d m_HookPoint;

   // GROUP: LIFECYCLE


   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   void Init();
   gmPolygon*   CreatePolygon() const;
   gpPolygon2d* CreategpPolygon() const;

   // GROUP: ACCESS
   // GROUP: INQUIRY
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif // INCLUDED_GEOMMODEL_Triangle_H_
