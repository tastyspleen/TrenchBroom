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

#include "Selection.h"
#include "BrushGeometry.h"
#include <cmath>

namespace TrenchBroom {
    namespace Model {
        Selection::Selection() : Observable(), m_mode(SM_NONE) {}
        
        ESelectionMode Selection::mode() const {
            return m_mode;
        }
        
        bool Selection::isPartial(Brush& brush) const {
            return find(m_partialBrushes.begin(), m_partialBrushes.end(), &brush) != m_partialBrushes.end();
        }
        
        bool Selection::empty() const {
            return m_entities.empty() && m_brushes.empty() && m_faces.empty();
        }
        
        const vector<Texture*>& Selection::mruTextures() const {
            return m_mruTextures;
        }
        
        const vector<Face*>& Selection::faces() const {
            return m_faces;
        }
        
        const vector<Face*> Selection::brushFaces() const {
            vector<Face*> faces;
            for (int i = 0; i < m_brushes.size(); i++) {
                vector<Face*> brushFaces = m_brushes[i]->faces();
                for (int j = 0; j < brushFaces.size(); j++)
                    faces.push_back(brushFaces[j]);
            }
            return faces;
        }
        
        const vector<Brush*>& Selection::brushes() const {
            return m_brushes;
        }
        
        const vector<Brush*>& Selection::partialBrushes() const {
            return m_partialBrushes;
        }
        
        const vector<Entity*>& Selection::entities() const {
            return m_entities;
        }
        
        const Entity* Selection::brushSelectionEntity() const {
            if (m_mode != SM_BRUSHES)
                return NULL;
            
            Entity* entity = m_brushes[0]->entity();
            for (int i = 1; i < m_brushes.size(); i++) {
                if (m_brushes[i]->entity() != entity)
                    return NULL;
            }
            
            return entity;
        }
        
        Vec3f Selection::center() const {
            Vec3f center;
            switch (m_mode) {
                case SM_FACES:
                    center = m_faces[0]->center();
                    for (int i = 1; i < m_faces.size(); i++) {
                        Vec3f temp = m_faces[i]->center();
                        addV3f(&center, &temp, &center);
                    }
                    scaleV3f(&center, 1.0f / m_faces.size(), &center);
                    break;
                case SM_BRUSHES:
                    center = m_brushes[0]->center();
                    for (int i = 1; i < m_brushes.size(); i++) {
                        Vec3f temp = m_brushes[i]->center();
                        addV3f(&center, &temp, &center);
                    }
                    scaleV3f(&center, 1.0f / m_brushes.size(), &center);
                    break;
                case SM_ENTITIES:
                    center = m_entities[0]->center();
                    for (int i = 1; i < m_entities.size(); i++) {
                        Vec3f temp = m_entities[i]->center();
                        addV3f(&center, &temp, &center);
                    }
                    scaleV3f(&center, 1.0f / m_entities.size(), &center);
                    break;
                case SM_BRUSHES_ENTITIES:
                    center = m_brushes[0]->center();
                    for (int i = 1; i < m_brushes.size(); i++) {
                        Vec3f temp = m_brushes[i]->center();
                        addV3f(&center, &temp, &center);
                    }
                    for (int i = 0; i < m_entities.size(); i++) {
                        Vec3f temp = m_entities[i]->center();
                        addV3f(&center, &temp, &center);
                    }
                    scaleV3f(&center, 1.0f / m_brushes.size() + m_entities.size(), &center);
                    break;
                default:
                    center = NanVector;
                    break;
            }
            
            return center;
        }
        
        BBox Selection::bounds() const {
            BBox bounds;
            switch (m_mode) {
                case SM_FACES:
                    bounds = m_faces[0]->brush()->bounds();
                    for (int i = 1; i < m_faces.size(); i++) {
                        BBox temp = m_faces[i]->brush()->bounds();
                        mergeBoundsWithBounds(&bounds, &temp, &bounds);
                    }
                    break;
                case SM_BRUSHES:
                    bounds = m_brushes[0]->bounds();
                    for (int i = 1; i < m_brushes.size(); i++) {
                        BBox temp = m_brushes[i]->bounds();
                        mergeBoundsWithBounds(&bounds, &temp, &bounds);
                    }
                    break;
                case SM_ENTITIES:
                    bounds = m_entities[0]->bounds();
                    for (int i = 1; i < m_entities.size(); i++) {
                        BBox temp = m_entities[i]->bounds();
                        mergeBoundsWithBounds(&bounds, &temp, &bounds);
                    }
                    break;
                case SM_BRUSHES_ENTITIES:
                    bounds = m_brushes[0]->bounds();
                    for (int i = 1; i < m_brushes.size(); i++) {
                        BBox temp = m_brushes[i]->bounds();
                        mergeBoundsWithBounds(&bounds, &temp, &bounds);
                    }
                    for (int i = 0; i < m_entities.size(); i++) {
                        BBox temp = m_entities[i]->bounds();
                        mergeBoundsWithBounds(&bounds, &temp, &bounds);
                    }
                    break;
                default:
                    bounds.min = NanVector;
                    bounds.max = NanVector;
                    break;
            }
            return bounds;
        }
        
        void Selection::addTexture(Texture& texture) {
            if (texture.dummy)
                return;
            
            vector<Texture*>::iterator it = find(m_mruTextures.begin(), m_mruTextures.end(), &texture);
            if (it != m_mruTextures.end())
                m_mruTextures.erase(it);
            m_mruTextures.push_back(&texture);
        }
        
        void Selection::addFace(Face& face) {
            if (m_mode != SM_FACES) removeAll();
            
            m_faces.push_back(&face);
            face.setSelected(true);
            
            if (find(m_partialBrushes.begin(), m_partialBrushes.end(), face.brush()) == m_partialBrushes.end())
                m_partialBrushes.push_back(face.brush());
            
            addTexture(*face.texture());
            m_mode = SM_FACES;
            
            vector<Face*> faces;
            faces.push_back(&face);
            postNotification(SelectionAdded, &faces);
        }
        
        void Selection::addFaces(const vector<Face*>& faces) {
            if (faces.empty()) return;
            if (m_mode != SM_FACES) removeAll();
            
            for (int i = 0; i < faces.size(); i++) {
                Face* face = faces[i];
                m_faces.push_back(face);
                face->setSelected(true);
                if (find(m_partialBrushes.begin(), m_partialBrushes.end(), face->brush()) == m_partialBrushes.end())
                    m_partialBrushes.push_back(face->brush());
            }
            
            addTexture(*faces[faces.size() - 1]->texture());
            m_mode = SM_FACES;
            
            postNotification(SelectionAdded, &faces);
        }
        
        void Selection::addBrush(Brush& brush) {
            if (m_mode == SM_FACES) removeAll();
            
            m_brushes.push_back(&brush);
            brush.setSelected(true);
            
            if (m_mode == SM_ENTITIES) m_mode = SM_BRUSHES_ENTITIES;
            else m_mode = SM_BRUSHES;
            
            vector<Brush*> brushes;
            brushes.push_back(&brush);
            postNotification(SelectionAdded, &brushes);
        }
        
        void Selection::addBrushes(const vector<Brush*>& brushes) {
            if (brushes.empty()) return;
            if (m_mode == SM_FACES) removeAll();
            
            for (int i = 0; i < brushes.size(); i++) {
                Brush* brush = brushes[i];
                m_brushes.push_back(brush);
                brush->setSelected(true);
            }
            
            if (m_mode == SM_ENTITIES) m_mode = SM_BRUSHES_ENTITIES;
            else m_mode = SM_BRUSHES;
            
            postNotification(SelectionAdded, &brushes);
        }
        
        void Selection::addEntity(Entity& entity) {
            if (m_mode == SM_FACES) removeAll();
            
            m_entities.push_back(&entity);
            entity.setSelected(true);
            
            if (m_mode == SM_BRUSHES) m_mode = SM_BRUSHES_ENTITIES;
            else m_mode = SM_ENTITIES;
            
            vector<Entity*> entities;
            entities.push_back(&entity);
            postNotification(SelectionAdded, &entities);
        }
        
        void Selection::addEntities(const vector<Entity*>& entities) {
            if (entities.empty()) return;
            if (m_mode == SM_FACES) removeAll();
            
            for (int i = 0; i < entities.size(); i++) {
                Entity* entity = entities[i];
                m_entities.push_back(entity);
                entity->setSelected(true);
            }
            
            if (m_mode == SM_BRUSHES) m_mode = SM_BRUSHES_ENTITIES;
            else m_mode = SM_ENTITIES;
            
            postNotification(SelectionAdded, &entities);
        }
        
        void Selection::removeFace(Face& face) {
            vector<Face*>::iterator it = find(m_faces.begin(), m_faces.end(), &face);
            if (it == m_faces.end()) return;
            
            m_faces.erase(it);
            face.setSelected(false);
            
            if (m_faces.size() == 0) {
                m_mode = SM_NONE;
                m_partialBrushes.clear();
            } else {
                const vector<Face*> siblings = face.brush()->faces();
                bool keep = false;
                for (int i = 0; i < siblings.size() && !keep; i++)
                    keep = find(m_faces.begin(), m_faces.end(), siblings[i]) != m_faces.end();
                if (!keep)
                    m_partialBrushes.erase(find(m_partialBrushes.begin(), m_partialBrushes.end(), face.brush()));
            }
            
            vector<Face*> faces;
            faces.push_back(&face);
            postNotification(SelectionRemoved, &faces);
        }
        
        void Selection::removeFaces(const vector<Face*>& faces) {
            if (faces.empty()) return;
            
            vector<Face*> removedFaces;
            for (int i = 0; i < faces.size(); i++) {
                Face* face = faces[i];
                vector<Face*>::iterator it = find(m_faces.begin(), m_faces.end(), face);
                if (it != m_faces.end()) {
                    m_faces.erase(it);
                    face->setSelected(false);
                    removedFaces.push_back(face);
                    
                    const vector<Face*> siblings = face->brush()->faces();
                    bool keep = false;
                    for (int j = 0; j < siblings.size() && !keep; j++)
                        keep = find(m_faces.begin(), m_faces.end(), siblings[j]) != m_faces.end();
                    if (!keep)
                        m_partialBrushes.erase(find(m_partialBrushes.begin(), m_partialBrushes.end(), face->brush()));
                }
            }
            
            if (m_faces.size() == 0)
                m_mode = SM_NONE;
            
            postNotification(SelectionRemoved, &removedFaces);
        }
        
        void Selection::removeBrush(Brush& brush) {
            vector<Brush*>::iterator it = find(m_brushes.begin(), m_brushes.end(), &brush);
            if (it == m_brushes.end()) return;
            
            m_brushes.erase(it);
            brush.setSelected(false);
            
            if (m_brushes.empty()) {
                if (m_entities.empty()) m_mode = SM_NONE;
                else m_mode = SM_ENTITIES;
            }
            
            vector<Brush*> brushes;
            brushes.push_back(&brush);
            postNotification(SelectionRemoved, &brushes);
        }
        
        void Selection::removeBrushes(const vector<Brush*>& brushes) {
            if (brushes.empty()) return;
            
            vector<Brush*> removedBrushes;
            for (int i = 0; i < brushes.size(); i++) {
                Brush* brush = brushes[i];
                vector<Brush*>::iterator it = find(m_brushes.begin(), m_brushes.end(), brush);
                if (it != m_brushes.end()) {
                    m_brushes.erase(it);
                    brush->setSelected(false);
                    removedBrushes.push_back(brush);
                }
            }
            
            if (m_brushes.empty()) {
                if (m_entities.empty()) m_mode = SM_NONE;
                else m_mode = SM_ENTITIES;
            }
            
            postNotification(SelectionRemoved, &removedBrushes);
        }
        
        void Selection::removeEntity(Entity& entity) {
            vector<Entity*>::iterator it = find(m_entities.begin(), m_entities.end(), &entity);
            if (it == m_entities.end()) return;
            
            m_entities.erase(it);
            entity.setSelected(false);
            
            if (m_entities.empty()) {
                if (m_brushes.empty()) m_mode = SM_NONE;
                else m_mode = SM_BRUSHES;
            }
            
            vector<Entity*> entities;
            entities.push_back(&entity);
            postNotification(SelectionRemoved, &entities);
        }
        
        void Selection::removeEntities(const vector<Entity*>& entities) {
            if (entities.empty()) return;
            
            vector<Entity*> removedEntities;
            for (int i = 0; i < entities.size(); i++) {
                Entity* entity = entities[i];
                vector<Entity*>::iterator it = find(m_entities.begin(), m_entities.end(), entity);
                if (it != m_entities.end()) {
                    m_entities.erase(it);
                    entity->setSelected(false);
                    removedEntities.push_back(entity);
                }
            }
            
            if (m_entities.empty()) {
                if (m_brushes.empty()) m_mode = SM_NONE;
                else m_mode = SM_BRUSHES;
            }
            
            postNotification(SelectionRemoved, &removedEntities);
        }
        
        void Selection::removeAll() {
            if (!m_faces.empty()) {
                vector<Face*> copy = m_faces;
                for (int i = 0; i < m_faces.size(); i++)
                    m_faces[i]->setSelected(false);
                m_faces.clear();
                m_mode = SM_NONE;
                postNotification(SelectionRemoved, &copy);
            }
            
            if (!m_brushes.empty()) {
                vector<Brush*> copy = m_brushes;
                for (int i = 0; i < m_brushes.size(); i++)
                    m_brushes[i]->setSelected(false);
                m_brushes.clear();
                m_mode = SM_NONE;
                postNotification(SelectionRemoved, &copy);
            }
            
            if (!m_entities.empty()) {
                vector<Entity*> copy = m_entities;
                for (int i = 0; i < m_entities.size(); i++)
                    m_entities[i]->setSelected(false);
                m_entities.clear();
                m_mode = SM_NONE;
                postNotification(SelectionRemoved, &copy);
            }
        }
    }
}