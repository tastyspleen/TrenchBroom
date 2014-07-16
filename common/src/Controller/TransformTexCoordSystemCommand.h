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

#ifndef __TrenchBroom__TransformTexCoordSystemCommand__
#define __TrenchBroom__TransformTexCoordSystemCommand__

#include "TrenchBroom.h"
#include "VecMath.h"
#include "SharedPointer.h"
#include "StringUtils.h"
#include "Controller/Command.h"
#include "Model/ModelTypes.h"
#include "Model/Snapshot.h"
#include "View/ViewTypes.h"

namespace TrenchBroom {
    namespace Controller {
        class TransformTexCoordSystemCommand : public Command {
        public:
            static const CommandType Type;
            typedef std::tr1::shared_ptr<TransformTexCoordSystemCommand> Ptr;
        private:
            typedef enum {
                Action_Rotate,
                Action_Scale,
                Action_Shear
            } Action;
            
            View::MapDocumentWPtr m_document;
            const Model::BrushFaceList m_faces;
            Model::Snapshot m_snapshot;
            
            Action m_action;
            Mat2x2 m_transformation;
            Vec3 m_invariant;
        public:
            static Ptr rotateTextures(View::MapDocumentWPtr document, const Model::BrushFaceList& faces, FloatType angle, const Vec3& invariant);
        private:
            TransformTexCoordSystemCommand(View::MapDocumentWPtr document, const Model::BrushFaceList& faces, Action action, const Mat2x2& transformation, const Vec3& invariant);
            static String makeName(Action action);

            bool doPerformDo();
            bool doPerformUndo();
            bool doCollateWith(Command::Ptr command);
        };
    }
}

#endif /* defined(__TrenchBroom__TransformTexCoordSystemCommand__) */
