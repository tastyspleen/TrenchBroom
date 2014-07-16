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
        
        static void assertRotation(const float rotation, const BrushFaceAttribs& attribs) {
            ASSERT_FLOAT_EQ(Math::mod(rotation + 360.0f, 360.0f), Math::mod(attribs.rotation() + 360.0f, 360.0f));
        }
        
        TEST(ParaxialTexCoordSystemTest, rotateTexCoordSystem) {
            ParaxialTexCoordSystem posZ(Vec3::Null, Vec3::PosY, Vec3::PosX);
            BrushFaceAttribs attribs("textureName");

            // rotate 15 degs
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            posZ.transform(Plane3(0.0, Vec3::PosZ), rotationMatrix2(Math::radians(15.0)), attribs, Vec3::Null);
            ASSERT_VEC_EQ(Vec2f::Null, attribs.offset());
            ASSERT_VEC_EQ(Vec2f::One, attribs.scale());
            assertRotation(15.0f, attribs);
            
            // rotate 210 degs
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            posZ.transform(Plane3(0.0, Vec3::PosZ), rotationMatrix2(Math::radians(210.0)), attribs, Vec3::Null);
            ASSERT_VEC_EQ(Vec2f::Null, attribs.offset());
            ASSERT_VEC_EQ(Vec2f::One, attribs.scale());
            assertRotation(210.0f, attribs);
            
            // rotate -15 degs
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            posZ.transform(Plane3(0.0, Vec3(0.5, 0.2, 0.9).normalized()), rotationMatrix2(Math::radians(-15.0)), attribs, Vec3::Null);
            ASSERT_VEC_EQ(Vec2f::Null, attribs.offset());
            ASSERT_VEC_EQ(Vec2f::One, attribs.scale());
            assertRotation(-15.0f, attribs);
            
            const Vec3 point(5.0, 9.0, 0.0);
            Vec2f oldCoords, newCoords;
            
            // rotate 90 degs, but keep point invariant
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            oldCoords = posZ.getTexCoords(point, attribs);

            posZ.transform(Plane3(0.0, Vec3::PosZ), rotationMatrix2(Math::radians(90.0)), attribs, point);
            ASSERT_VEC_EQ(Vec2f(-4.0f, -14.0f), attribs.offset());
            ASSERT_VEC_EQ(Vec2f::One, attribs.scale());
            assertRotation(90.0f, attribs);
            
            newCoords = posZ.getTexCoords(point, attribs);
            ASSERT_VEC_EQ(oldCoords, newCoords);
            
            // rotate 264 degs, but keep point invariant
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            oldCoords = posZ.getTexCoords(point, attribs);
            
            posZ.transform(Plane3(0.0, Vec3::PosZ), rotationMatrix2(Math::radians(264.0)), attribs, point);
            ASSERT_VEC_EQ(Vec2f::One, attribs.scale());
            assertRotation(264.0f, attribs);
            
            newCoords = posZ.getTexCoords(point, attribs);
            ASSERT_VEC_EQ(oldCoords, newCoords);

            // rotate 15 degs when scale is set
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            attribs.setScale(Vec2f(1.1f, 0.75f));
            posZ.transform(Plane3(0.0, Vec3::PosZ), rotationMatrix2(Math::radians(15.0)), attribs, Vec3::Null);
            ASSERT_VEC_EQ(Vec2f::Null, attribs.offset());
            ASSERT_VEC_EQ(Vec2f(1.1f, 0.75f), attribs.scale());
            assertRotation(15.0f, attribs);
            
            // rotate 171 degs when scale is set, but keep point invariant
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            attribs.setScale(Vec2f(-0.91f, 1.8f));
            oldCoords = posZ.getTexCoords(point, attribs);
            
            posZ.transform(Plane3(0.0, Vec3::PosZ), rotationMatrix2(Math::radians(171.0)), attribs, point);
            ASSERT_VEC_EQ(Vec2f(-0.91f, 1.8f), attribs.scale());
            assertRotation(171.0, attribs);
            
            newCoords = posZ.getTexCoords(point, attribs);
            ASSERT_VEC_EQ(oldCoords, newCoords);
        }
        
        TEST(ParaxialTexCoordSystemTest, scaleTexCoordSystem) {
            ParaxialTexCoordSystem posZ(Vec3::Null, Vec3::PosY, Vec3::PosX);
            BrushFaceAttribs attribs("textureName");

            // scale X by 1.15
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            posZ.transform(Plane3(0.0, Vec3::PosZ), scalingMatrix(Vec2(1.15, 1.0)), attribs, Vec3::Null);
            ASSERT_VEC_EQ(Vec2f::Null, attribs.offset());
            ASSERT_VEC_EQ(Vec2f(1.15f, 1.0f), attribs.scale());
            assertRotation(0.0f, attribs);

            const Vec3 point(5.0, 9.0, 0.0);
            Vec2f oldCoords, newCoords;
            
            // scale X by 1.15 when rotation is set to 15 degs
            posZ = ParaxialTexCoordSystem(Vec3::Null, Vec3::PosY, Vec3::PosX);
            attribs = BrushFaceAttribs("textureName");
            attribs.setRotation(15.0f);
            posZ.setRotation(Vec3::PosZ, 0.0f, 15.0f);
            oldCoords = posZ.getTexCoords(point, attribs);
            
            posZ.transform(Plane3(0.0, Vec3::PosZ), scalingMatrix(Vec2(1.15, 1.0)), attribs, point);
            ASSERT_VEC_EQ(Vec2f(1.15f, 1.0f), attribs.scale());
            assertRotation(15.0f, attribs);

            newCoords = posZ.getTexCoords(point, attribs);
            ASSERT_VEC_EQ(oldCoords, newCoords);
        }
    }
}
