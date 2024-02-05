///////////////////////////////////////////////////////////////////////
// DManip - Direct Manipulation Framework
// Copyright � 1999-2024  Washington State Department of Transportation
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

#include <DManip/DManipExp.h>
#include <DManip/DrawLineStrategy.h>
#include <DManip/DManipTypes.h>

namespace WBFL
{
   namespace DManip
   {
      /// @brief A strategy that draws a line with multiple subordinate strategies
      /// An example is drawing a simple line as well as a rectangular representation of the line
      class DMANIPCLASS CompoundDrawLineStrategy : public iDrawLineStrategy
      {
      private:
         CompoundDrawLineStrategy() = default;
      public:
         static std::shared_ptr<CompoundDrawLineStrategy> Create() { return std::shared_ptr<CompoundDrawLineStrategy>(new CompoundDrawLineStrategy()); }
	      virtual ~CompoundDrawLineStrategy() = default;

         void AddStrategy(std::shared_ptr<iDrawLineStrategy> pStrategy);
         void RemoveStrategy(IndexType index);
         std::shared_ptr<iDrawLineStrategy> GetStrategy(IndexType index);
         std::shared_ptr<const iDrawLineStrategy> GetStrategy(IndexType index) const;
         IndexType Count() const;

         // iLineDrawStrategy
         virtual void Draw(std::shared_ptr<const iLineDisplayObject> pDO, CDC* pDC) const override;
         virtual void DrawDragImage(std::shared_ptr<const iLineDisplayObject> pDO, CDC* pDC, std::shared_ptr<const iCoordinateMap> map, const POINT& dragStart, const POINT& dragPoint) const override;
         virtual void DrawHighlight(std::shared_ptr<const iLineDisplayObject> pDO, CDC* pDC, bool bHighlight) const override;
         virtual WBFL::Geometry::Rect2d GetBoundingBox(std::shared_ptr<const iLineDisplayObject> pDO) const override;

      private:
         using Strategies = std::vector<std::shared_ptr<iDrawLineStrategy>> ;
         Strategies m_Strategies;
      };
   };
};
