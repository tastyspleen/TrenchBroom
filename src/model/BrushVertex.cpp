/*
 Copyright (C) 2010-2013 Kristian Duske
 
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

#include "BrushVertex.h"

namespace TrenchBroom {
    namespace Model {
        BrushVertex::BrushVertex(const Vec3& position) :
        m_position(position),
        m_mark(New) {}

        void BrushVertex::updateMark(const Plane3& plane) {
            const PointStatus::Type status = plane.pointStatus(m_position);
            switch (status) {
                case PointStatus::PSAbove:
                    m_mark = Drop;
                    break;
                case PointStatus::PSBelow:
                    m_mark = Keep;
                    break;
                default:
                    m_mark = Undecided;
                    break;
            }
        }
    }
}