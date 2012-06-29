/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_PositionGuideFigure_h
#define TrenchBroom_PositionGuideFigure_h

#include "Renderer/Figures/Figure.h"
#include "Utilities/VecMath.h"

namespace TrenchBroom {
    namespace Renderer {
        class RenderContext;
        class Vbo;
        
        class PositioningGuideFigure : public Figure {
        private:
            BBox m_bounds;
            Vec4f m_color;
            Vec4f m_hiddenColor;
            
            void renderLine(RenderContext& context, const Vec4f& color, const Vec3f& anchor, float size, const Vec3f& axis);
            void renderGuides(RenderContext& context, const Vec4f& color);
        public:
            PositioningGuideFigure(const BBox& bounds, const Vec4f& color, const Vec4f& hiddenColor);
            void updateBounds(const BBox& bounds);
            void render(RenderContext& context, Vbo& vbo);
        };
    }
}

#endif
