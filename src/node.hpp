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

#include <QDebug>
#include <QGraphicsPathItem>

//------------------------------------------------------------------------------
//

class Port;

//------------------------------------------------------------------------------
//

class Node
    : public QGraphicsPathItem
{

public:

	enum { Type = QGraphicsItem::UserType + 3 };

    Node( QGraphicsItem *parent = 0 );
    ~Node();

    Port* addPort( const QString &name, bool isOutput, int flags = 0, int ptr = 0 );
    void addInputPort( const QString &name );
    void addOutputPort( const QString &name );
    void addInputPorts( const QStringList &names );
    void addOutputPorts( const QStringList &names );
    void save( QDataStream& );
    void load( QDataStream&, QMap< quint64, Port* > &portMap );
    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
    Node* clone();
    QVector< Port* > ports();

	int type() const { return Type; }

protected:

    QVariant itemChange( GraphicsItemChange change, const QVariant &value );

private:

	int horzMargin;
	int vertMargin;
	int width;
    int height;

};

#endif // IRUI_NODE_HPP
