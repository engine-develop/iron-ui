#ifndef IRUI_NODE_HPP
#define IRUI_NODE_HPP

// Copyright (C) 2015 Engine Development
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Note: Source code incorporated from 'QNodesEditor':
//     http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//
// Copyright (c) 2012, STANISLAW ADASZEWSKI
//

//------------------------------------------------------------------------------
//

// Qt
#include <QGraphicsPathItem>

// Engine
#include "types.hpp"

//------------------------------------------------------------------------------
//

class Graph;
class Variable;
class Port;

//------------------------------------------------------------------------------
//

class Node
    : public QGraphicsPathItem
{

public:

	enum { Type = QGraphicsItem::UserType + 3 };

    Node( QGraphicsItem* parent = 0 );

    ~Node();

    int type() const;

    void setId( const uint32_t& id );

    const uint32_t& id() const;

    void setTypeId( const uint32_t& typeId );

    const uint32_t& typeId() const;

    void setName( const QString& name );

    QString name() const;

    double codePos() const;

    QList< Variable* > attributes() const;

    Variable* attribute( const uint8_t& id ) const;

    void updateAttributeIds();

    void update();

    void paint( QPainter* painter,
                const QStyleOptionGraphicsItem* option,
                QWidget* widget );

protected:

    QVariant itemChange( GraphicsItemChange change,
                         const QVariant &value );

    uint32_t m_id;
    uint32_t m_typeId;
    QGraphicsTextItem* m_nameText;
    QGraphicsPathItem* m_codeAnchor;

};

#endif // IRUI_NODE_HPP
