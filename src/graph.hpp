#ifndef IRUI_Graph_HPP
#define IRUI_Graph_HPP

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
// Note: Source code incorporated from 'QNodesGraph':
//     http://algoholic.eu/qnodesGraph-qt-nodesports-based-data-processing-flow-Graph/
//
// Copyright (c) 2012, STANISLAW ADASZEWSKI
//

//------------------------------------------------------------------------------
//

#include <QDebug>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

//------------------------------------------------------------------------------
//

class QGraphicsSceneDragDropEvent;
class QDropEvent;
class QDragEnterEvent;
class QDragLeaveEvent;
class QDragMoveEvent;
class QGraphicsView;
class Connection;
class QGraphicsItem;
class QPointF;
class Node;

//------------------------------------------------------------------------------
//

class GraphScene
    : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit GraphScene( QObject* parent = 0x0 );

    virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
    virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent* event );
    virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
    virtual void dropEvent( QDropEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );

    void removeNode( QGraphicsItem* item );
    void removeSelected();

};

//------------------------------------------------------------------------------
//

class GraphView
    : public QGraphicsView
{
    Q_OBJECT

public:

    explicit GraphView( QGraphicsScene* scene, QWidget* parent = 0x0 );

    virtual void dragEnterEvent( QDragEnterEvent* event );
    virtual void dragLeaveEvent( QDragLeaveEvent* event );
    virtual void dragMoveEvent( QDragMoveEvent* event );
    virtual void dropEvent( QDropEvent* event );
    virtual void wheelEvent( QWheelEvent* event);
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );

protected:

    bool m_pan;
    int m_panStartX, m_panStartY;
    double m_panPosX, m_panPosY;

};

//------------------------------------------------------------------------------
//

class Graph
    : public QObject
{
	Q_OBJECT

public:

    explicit Graph( QObject* parent = 0x0 );

    void install( GraphScene* scene );

    bool eventFilter( QObject*, QEvent* );

    void save( QDataStream &ds );
    void load( QDataStream &ds );

private:

    QGraphicsItem* itemAt( const QPointF& );

    GraphScene* scene;
    GraphView* view;
    Connection* conn;

};

#endif // IRUI_Graph_HPP
