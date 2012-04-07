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

#ifndef TrenchBroom_Entity_h
#define TrenchBroom_Entity_h

#include <string>
#include <vector>
#include <map>
#include "MapObject.h"
#include "Brush.h"
#include "VecMath.h"
#include "Vbo.h"
#include "Map.h"
#include "EntityDefinition.h"

using namespace std;

namespace TrenchBroom {
    namespace Model {
        static string const ClassnameKey        = "classname";
        static string const SpawnFlagsKey       = "spawnflags";
        static string const WorldspawnClassname = "worldspawn";
        static string const GroupClassname      = "func_group";
        static string const GroupNameKey        = "__tb_group_name";
        static string const GroupVisibilityKey  = "__tb_group_visible";
        static string const OriginKey           = "origin";
        static string const AngleKey            = "angle";
        static string const MessageKey          = "message";
        static string const ModsKey             = "__tb_mods";
        
        class Map;
        class Brush;
        class Entity : public MapObject {
        private:
            EntityDefinition* m_entityDefinition;
            Vec3f m_center;
            Vec3f m_origin;
            float m_angle;
            BBox m_bounds;
            BBox m_maxBounds;
            
            Map* m_map;
            vector<Brush*> m_brushes;
            
            map<string, string> m_properties;
            
            int m_filePosition;
            bool m_selected;
            VboBlock* m_vboBlock;
            
            void init();
            void rebuildGeometry();
            void rotate90(EAxis axis, Vec3f rotationCenter, bool clockwise);
        public:
            Entity();
            Entity(const map<string, string> properties);
            ~Entity();
            
            const EntityDefinition* entityDefinition() const;
            void setEntityDefinition(EntityDefinition* entityDefinition);
            Vec3f center() const;
            Vec3f origin() const;
            BBox bounds() const;
            BBox maxBounds() const;
            
            Map* quakeMap() const;
            void setMap(Map* quakeMap);
            const vector<Brush*>& brushes() const;
            
            const map<string, string> properties() const;
            const string* propertyForKey(const string& key) const;
            bool propertyWritable(const string& key) const;
            bool propertyDeletable(const string& key) const;
            void setProperty(const string& key, const string& value);
            void setProperty(const string& key, const string* value);
            void setProperty(const string& key, Vec3f value, bool round);
            void setProperty(const string& key, float value, bool round);
            void setProperties(map<string, string> properties, bool replace);
            void deleteProperty(const string& key);
            
            const string* classname() const;
            const int angle() const;
            bool worldspawn() const;
            bool group() const;
            
            void addBrush(Brush* brush);
            void addBrushes(const vector<Brush*>& brushes);
            void brushChanged(Brush* brush);
            void removeBrush(Brush* brush);
            void removeBrushes(vector<Brush*>& brushes);
            
            void translate(Vec3f delta);
            void rotate90CW(EAxis axis, Vec3f rotationCenter);
            void rotate90CCW(EAxis axis, Vec3f rotationCenter);
            void rotate(TQuaternion rotation, Vec3f rotationCenter);
            void flip(EAxis axis, Vec3f flipCenter);
            
            int filePosition() const;
            void setFilePosition(int filePosition);
            bool selected() const;
            void setSelected(bool selected);
            VboBlock* vboBlock() const;
            void setVboBlock(VboBlock* vboBlock);
        };
    }
}
#endif