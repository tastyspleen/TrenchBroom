/*
 Copyright (C) 2010-2014 Kristian Duske
 
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
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include "TrenchBroom.h"
#include "VecMath.h"
#include "TestUtils.h"
#include "Model/BrushFace.h"
#include "Model/ParaxialTexCoordSystem.h"

namespace TrenchBroom {
    namespace Model {
        TEST(ParaxialTexCoordSystemTest, initWithPoints) {
            ParaxialTexCoordSystem posX(Vec3::Null, Vec3::PosZ, Vec3::PosY);
            ASSERT_VEC_EQ(Vec3::PosY, posX.xAxis());
            ASSERT_VEC_EQ(Vec3::NegZ, posX.yAxis());
            
            ParaxialTexCoordSystem negX(Vec3::Null, Vec3::PosZ, Vec3::NegY);
            ASSERT_VEC_EQ(Vec3::PosY, negX.xAxis());
            ASSERT_VEC_EQ(Vec3::NegZ, negX.yAxis());

            ParaxialTexCoordSystem posY(Vec3::Null, Vec3::PosZ, Vec3::NegX);
            ASSERT_VEC_EQ(Vec3::PosX, posY.xAxis());
            ASSERT_VEC_EQ(Vec3::NegZ, posY.yAxis());
            
            ParaxialTexCoordSystem negY(Vec3::Null, Vec3::PosZ, Vec3::PosX);
            ASSERT_VEC_EQ(Vec3::PosX, negY.xAxis());
            ASSERT_VEC_EQ(Vec3::NegZ, negY.yAxis());

            ParaxialTexCoordSystem posZ(Vec3::Null, Vec3::PosY, Vec3::PosX);
            ASSERT_VEC_EQ(Vec3::PosX, posZ.xAxis());
            ASSERT_VEC_EQ(Vec3::NegY, posZ.yAxis());
            
            ParaxialTexCoordSystem negZ(Vec3::Null, Vec3::NegY, Vec3::PosX);
            ASSERT_VEC_EQ(Vec3::PosX, negZ.xAxis());
            ASSERT_VEC_EQ(Vec3::NegY, negZ.yAxis());
        }
        
        TEST(ParaxialTexCoordSystemTest, getTexCoords) {
            ParaxialTexCoordSystem posX(Vec3::Null, Vec3::PosZ, Vec3::PosY);
            BrushFaceAttribs attribs("textureName");
            ASSERT_EQ(Vec2f::Null, posX.getTexCoords(Vec3::Null, attribs));
            
            attribs.setOffset(Vec2f(2.0f, -3.0f));
            ASSERT_EQ(Vec2f(2.0f, -3.0f), posX.getTexCoords(Vec3::Null, attribs));
            
            attribs.setOffset(Vec2f::Null);
            attribs.setScale(Vec2f(1.5f, 0.5f));
            ASSERT_EQ(Vec2f::Null, posX.getTexCoords(Vec3::Null, attribs));
            ASSERT_EQ(Vec2f(2.0f, -8.0f), posX.getTexCoords(Vec3(9.0, 3.0, 4.0), attribs));
        }
        
        TEST(ParaxialTexCoordSystemTest, transformTexCoordSystem) {
            ParaxialTexCoordSystem posZ(Vec3::Null, Vec3::PosY, Vec3::PosX);
            
            Plane3 boundary(0.0, Vec3::PosZ);
            BrushFaceAttribs attribs("textureName");
            posZ.transform(boundary, rotationMatrix3(Math::radians(15.0)), attribs);
            
            ASSERT_VEC_EQ(Vec2f::Null, attribs.offset());
            ASSERT_VEC_EQ(Vec2f::One, attribs.scale());
            ASSERT_FLOAT_EQ(15.0f, attribs.rotation());
        }
    }
}
