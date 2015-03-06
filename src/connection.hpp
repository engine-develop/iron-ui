#ifndef IRUI_CONNECTION_HPP
#define IRUI_CONNECTION_HPP

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

#include <QGraphicsPathItem>

//------------------------------------------------------------------------------
//

class Port;

//------------------------------------------------------------------------------
//

class Connection
    : public QGraphicsPathItem
{

public:

	enum { Type = QGraphicsItem::UserType + 2 };

    Connection( QGraphicsItem *parent = 0 );
    ~Connection();

    void setPos1( const QPointF &p );
    void setPos2( const QPointF &p );
    void setPort1( Port *p );
    void setPort2( Port *p );
	void updatePosFromPorts();
	void updatePath();
    Port* port1() const;
    Port* port2() const;

    void save( QDataStream& );
    void load( QDataStream&, const QMap< quint64, Port* > &portMap );

	int type() const { return Type; }

private:

    QColor color;
	QPointF pos1;
	QPointF pos2;
    Port* m_port1;
    Port* m_port2;

};

#endif // IRUI_CONNECTION_HPP
