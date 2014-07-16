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

#include "TransformTexCoordSystemCommand.h"

#include "Macros.h"
#include "Model/BrushFace.h"
#include "View/MapDocument.h"

namespace TrenchBroom {
    namespace Controller {
        const Command::CommandType TransformTexCoordSystemCommand::Type = Command::freeType();

        TransformTexCoordSystemCommand::Ptr TransformTexCoordSystemCommand::rotateTextures(View::MapDocumentWPtr document, const Model::BrushFaceList& faces, const FloatType angle, const Vec3& invariant) {
            const Mat2x2 transformation = rotationMatrix2(Math::radians(angle));
            return Ptr(new TransformTexCoordSystemCommand(document, faces, Action_Rotate, transformation, invariant));
        }

        TransformTexCoordSystemCommand::TransformTexCoordSystemCommand(View::MapDocumentWPtr document, const Model::BrushFaceList& faces, const Action action, const Mat2x2& transformation, const Vec3& invariant) :
        Command(Type, makeName(action), true, true),
        m_document(document),
        m_faces(faces),
        m_action(action),
        m_transformation(transformation),
        m_invariant(invariant) {}
        
        String TransformTexCoordSystemCommand::makeName(const Action action) {
            switch (action) {
                case Action_Rotate:
                    return "Rotate texture";
                case Action_Scale:
                    return "Scale texture";
                case Action_Shear:
                    return "Shear texture";
                DEFAULT_SWITCH()
            }
        }

        bool TransformTexCoordSystemCommand::doPerformDo() {
            View::MapDocumentSPtr document = lock(m_document);
            m_snapshot = Model::Snapshot(m_faces);
            
            Model::BrushFaceList::const_iterator it, end;
            for (it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
                Model::BrushFace* face = *it;
                document->faceWillChangeNotifier(face);
                face->transformTexture(m_transformation, m_invariant);
                document->faceDidChangeNotifier(face);
            }
            return true;
        }
        
        bool TransformTexCoordSystemCommand::doPerformUndo() {
            View::MapDocumentSPtr document = lock(m_document);
            
            document->faceWillChangeNotifier(m_faces.begin(), m_faces.end());
            m_snapshot.restore(document->worldBounds());
            document->faceDidChangeNotifier(m_faces.begin(), m_faces.end());
            return true;
        }
        
        bool TransformTexCoordSystemCommand::doCollateWith(Command::Ptr command) {
            Ptr other = Command::cast<TransformTexCoordSystemCommand>(command);
            if (other->m_action != m_action)
                return false;
            if (other->m_invariant != m_invariant)
                return false;
            m_transformation = other->m_transformation * m_transformation;
            return true;
        }
    }
}
