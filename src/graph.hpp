#ifndef IRUI_GRAPH_HPP
#define IRUI_GRAPH_HPP

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

// STD
#include <unordered_map>

// Qt
#include <QDebug>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

// Engine
#include "types.hpp"

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
class Variable;
class Node;
class Graph;
class Port;

//------------------------------------------------------------------------------
//

class GraphScene
    : public QGraphicsScene
{
    Q_OBJECT

public:

    GraphScene( QObject* parent = 0x0 );

    void setGraph( Graph* graph );

    Graph* graph() const;

    virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
    virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent* event );
    virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
    virtual void dropEvent( QDropEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );

    void removeSelected();

protected:

    Graph* m_graph;

};

//------------------------------------------------------------------------------
//

class GraphView
    : public QGraphicsView
{
    Q_OBJECT

public:

    GraphView( QGraphicsScene* scene, QWidget* parent = 0x0 );

    void setGraph( Graph* graph );

    Graph* graph() const;

    virtual void dragEnterEvent( QDragEnterEvent* event );
    virtual void dragLeaveEvent( QDragLeaveEvent* event );
    virtual void dragMoveEvent( QDragMoveEvent* event );
    virtual void dropEvent( QDropEvent* event );
    virtual void wheelEvent( QWheelEvent* event);
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );

protected:

    Graph* m_graph;
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

    enum Type
    {
        Type_Setup = 0,
        Type_Loop  = 1
    };

    Graph( Type type,
           QWidget* parent = 0x0 );

    const Type& type() const;

    //----------
    //

    GraphScene* scene();

    GraphView* view();

    void clear();

    //----------
    //

    void addComponent( QGraphicsItem* component );

    void addComponents( QList< QGraphicsItem* >& components );

    void removeComponent( QGraphicsItem* component );

    void removeComponents( QList< QGraphicsItem* >& components );

    QList< QGraphicsItem* > components() const;

    QList< QGraphicsItem* > componentsByType( const int& type ) const;

    QGraphicsItem* component( const uint32_t& id );

    //----------
    //

    Variable* createVariable( const uint32_t& typeId );

    Node* createNode( const uint32_t& typeId );

    Variable* addAttribute( Node* node,
                            const uint32_t& typeId,
                            const QString& name,
                            Direction direction );

    bool eventFilter( QObject*, QEvent* );

private:

    QGraphicsItem* itemAt( const QPointF& );

    uint32_t getComponentId();

    Type m_type;
    GraphScene* m_scene;
    GraphView* m_view;
    Connection* m_conn;

    std::unordered_map< uint32_t, QGraphicsItem* > m_components;

};

#endif // IRUI_GRAPH_HPP
