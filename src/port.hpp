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

#include <QGraphicsPathItem>

//------------------------------------------------------------------------------
//

class Node;
class Connection;

//------------------------------------------------------------------------------
//

class Port
    : public QGraphicsPathItem
{

public:

	enum { Type = QGraphicsItem::UserType + 1 };
	enum { NamePort = 1, TypePort = 2 };

    Port( QGraphicsItem *parent = 0 );
    ~Port();

    void setNode( Node* );
    void setName( const QString &n );
    void setIsOutput( bool o );
	int radius();
	bool isOutput();
    QVector< Connection* >& connections();
    void setPortFlags( int );

	const QString& portName() const { return name; }
	int portFlags() const { return m_portFlags; }

	int type() const { return Type; }

    Node* node() const;

	quint64 ptr();
    void setPtr( quint64 );

    bool isConnected( Port* );

protected:

    QVariant itemChange( GraphicsItemChange change, const QVariant &value );

private:

    Node* m_node;
	QString name;
	bool isOutput_;
    QGraphicsTextItem* label;
	int radius_;
	int margin;
    QVector< Connection* > m_connections;
	int m_portFlags;
	quint64 m_ptr;

};

#endif // IRUI_PORT_HPP
