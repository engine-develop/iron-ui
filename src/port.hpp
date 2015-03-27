#ifndef IRUI_PORT_HPP
#define IRUI_PORT_HPP

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
#include <QGraphicsTextItem>
#include <QGraphicsPathItem>

// Engine
#include "types.hpp"

//------------------------------------------------------------------------------
//

class Variable;
class Node;
class Connection;
class QPainter;

//------------------------------------------------------------------------------
//

class PortValue
    : public QGraphicsTextItem
{

public:

    PortValue( QGraphicsItem* parent = 0x0 );

    void keyPressEvent( QKeyEvent* event );

    void paint( QPainter* p,
                const QStyleOptionGraphicsItem* o,
                QWidget* w );

};

//------------------------------------------------------------------------------
//

class Port
    : public QGraphicsPathItem
{

public:

	enum { Type = QGraphicsItem::UserType + 1 };

    enum State
    {
        Expanded = 0x1
    };

    Port( QGraphicsItem* parent = 0x0 );

    ~Port();

    int type() const { return Type; }

    void setState( int state );

    const int& state() const;

    void setDirection( Direction direction );

    const Direction& direction() const;

    Variable* variable();

    QString value() const;

    QVector< Connection* >& connections();

    bool isConnected( Port* );

    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event );

    void update();

protected:

    QVariant itemChange( GraphicsItemChange change, const QVariant &value );

    Direction m_direction;
    QVector< Connection* > m_connections;
    int m_state;
    PortValue* m_valueText;

};

#endif // IRUI_PORT_HPP
