//------------------------------------------------------------------------------
//

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>

#include "utility.hpp"
#include "port.hpp"
#include "connection.hpp"
#include "variable.hpp"
#include "node.hpp"
#include "typestore.hpp"

#include "graph.hpp"

//------------------------------------------------------------------------------
//

GraphScene::GraphScene( QObject *parent )
    : QGraphicsScene( parent )
    , m_graph( 0x0 )
{
    setBackgroundBrush( QImage( ":/images/resources/images/scene_bkg_tile.png" ) );
}

//------------------------------------------------------------------------------
//

void GraphScene::setGraph( Graph* graph )
{
    m_graph = graph;
}

//------------------------------------------------------------------------------
//

Graph* GraphScene::graph() const
{
    return m_graph;
}

//------------------------------------------------------------------------------
//

void GraphScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    //if ( event->mimeData()->hasFormat( "text/plain" ) )
        event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphScene::dropEvent( QDropEvent* event )
{
    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphScene::keyPressEvent( QKeyEvent* event )
{
    if ( event->matches( QKeySequence::Delete ) )
    {
        removeSelected();
    }

    QGraphicsScene::keyPressEvent( event );
}

//------------------------------------------------------------------------------
//

void GraphScene::removeSelected()
{
    QList< QGraphicsItem* > items = selectedItems();

    for ( int i = 0; i < items.size(); ++i )
    {
        m_graph->removeComponent( items[ i ] );
    }
}

//------------------------------------------------------------------------------
//

GraphView::GraphView( QGraphicsScene* scene, QWidget* parent )
    : QGraphicsView( scene, parent )
    , m_graph( 0x0 )
{
    setRenderHint( QPainter::Antialiasing, true );
    setRenderHint( QPainter::SmoothPixmapTransform, true );
    setDragMode( QGraphicsView::RubberBandDrag );
    setAcceptDrops( true );
}

//------------------------------------------------------------------------------
//

void GraphView::setGraph( Graph* graph )
{
    m_graph = graph;
}

//------------------------------------------------------------------------------
//

Graph* GraphView::graph() const
{
    return m_graph;
}

//------------------------------------------------------------------------------
//

void GraphView::dragEnterEvent( QDragEnterEvent* event )
{
    //if ( event->mimeData()->hasFormat( "text/plain" ) )
        event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphView::dragLeaveEvent( QDragLeaveEvent* event )
{
    event->accept();
}

//------------------------------------------------------------------------------
//

void GraphView::dragMoveEvent( QDragMoveEvent* event )
{
    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphView::dropEvent( QDropEvent* event )
{
    const QMimeData* mdata = event->mimeData();
    if ( !mdata->hasText() ) return;

    QString typeData = mdata->text();
    QStringList typeDataList = typeData.split( ":" );

    if ( typeDataList[ 0 ] == "variable" )
    {
        Variable* v = m_graph->createVariable( typeDataList[ 1 ].toUInt() );
        v->setPos( mapToScene( event->pos() ) );
    }

    else if ( typeDataList[ 0 ] == "node" )
    {
        Node* n = m_graph->createNode( typeDataList[ 1 ].toUInt() );
        n->setPos( mapToScene( event->pos() ) );
    }

    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
//

void GraphView::wheelEvent( QWheelEvent* event)
{
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

    // Scale the view / do the zoom
    //
    static const double scaleFactor = 1.25;

    // Zoom in
    //
    if ( event->delta() > 0 )
    {
        scale( scaleFactor, scaleFactor );
    }

    // Zoom out
    //
    else
    {
        scale( 1.0 / scaleFactor, 1.0 / scaleFactor );
    }
}

//------------------------------------------------------------------------------
//

void GraphView::mousePressEvent( QMouseEvent* event )
{
    if ( event->button() == Qt::MiddleButton )
    {
        m_pan = true;
        m_panStartX = event->x();
        m_panStartY = event->y();
        setCursor( Qt::ClosedHandCursor );
        setDragMode( QGraphicsView::NoDrag );
    }

    event->accept();

    QGraphicsView::mousePressEvent( event );
}

//------------------------------------------------------------------------------
//

void GraphView::mouseReleaseEvent( QMouseEvent* event )
{
    if ( event->button() == Qt::MiddleButton )
    {
        m_pan = false;
        setCursor( Qt::ArrowCursor );
        setDragMode( QGraphicsView::RubberBandDrag );
    }

    event->accept();

    QGraphicsView::mouseReleaseEvent( event );
}

//------------------------------------------------------------------------------
//

void GraphView::mouseMoveEvent( QMouseEvent* event )
{
    if ( m_pan )
    {
        horizontalScrollBar()->setValue( horizontalScrollBar()->value() - ( event->x() - m_panStartX ) );
        verticalScrollBar()->setValue(     verticalScrollBar()->value() - ( event->y() - m_panStartY ) );

        double factor = 1.0;//transform().m11();
        double panPosX = event->pos().x();
        double panPosY = event->pos().y();
        translate( ( panPosX - m_panPosX ) / factor,
                   ( panPosY - m_panPosY ) / factor );

        m_panStartX = event->x();
        m_panStartY = event->y();
        m_panPosX = panPosX;
        m_panPosY = panPosY;
    }

    event->accept();

    QGraphicsView::mouseMoveEvent( event );
}

//------------------------------------------------------------------------------
//

Graph::Graph( Type type,
              QWidget* parent )
    : QObject( parent )
    , m_type( type )
    , m_scene( 0x0 )
    , m_view( 0x0 )
    , m_conn( 0x0 )
    , m_components()
{
    // Create graphics scene
    //
    m_scene = new GraphScene( this );
    m_scene->setGraph( this );
    m_scene->installEventFilter( this );

    // Create graphics view
    //
    m_view = new GraphView( (QGraphicsScene*) m_scene, parent );
    m_view->setGraph( this );
}

//------------------------------------------------------------------------------
//

const Graph::Type& Graph::type() const
{
    return m_type;
}

//------------------------------------------------------------------------------
//

GraphScene* Graph::scene()
{
    return m_scene;
}

//------------------------------------------------------------------------------
//

GraphView* Graph::view()
{
    return m_view;
}

//------------------------------------------------------------------------------
//

void Graph::clear()
{
    m_scene->clear();
    m_components.clear();
}

//------------------------------------------------------------------------------
//

uint32_t Graph::getComponentId()
{
    uint32_t id = 0;

    do
    {
        id = randRange( 0u, uint32_t( -1 ) - 1 );
    }

    while ( m_components.count( id ) );

    return id;
}

//------------------------------------------------------------------------------
//

void Graph::addComponent( QGraphicsItem* component )
{
    m_scene->addItem( component );

    uint32_t id = getComponentId();

    switch ( component->type() )
    {
        case Variable::Type:
        {
            Variable* v = static_cast< Variable* >( component );
            v->setId( id );
            break;
        }

        case Node::Type:
        {
            Node* n = static_cast< Node* >( component );
            n->setId( id );
            break;
        }

        default:
            break;
    }

    m_components[ id ] = component;
}

//------------------------------------------------------------------------------
//

void Graph::addComponents( QList< QGraphicsItem* >& components )
{
    foreach( QGraphicsItem* component, components )
    {
        addComponent( component );
    }
}

//------------------------------------------------------------------------------
//

void Graph::removeComponent( QGraphicsItem* component )
{
    uint32_t id = 0;

    switch ( component->type() )
    {
        case Variable::Type:
        {
            Variable* v = static_cast< Variable* >( component );
            id = v->id();

            QVector< Connection* >& connections
                = v->port()->connections();

            for ( int i = 0; i < connections.size(); ++i )
            {
                if ( connections[ i ] && connections[ i ]->scene() )
                {
                    m_scene->removeItem( connections[ i ] );
                }
            }

            break;
        }

        case Node::Type:
        {
            Node* n = static_cast< Node* >( component );
            id = n->id();

            QList< Variable* > attributes = n->attributes();

            for ( int i = 0; i < attributes.size(); ++i )
            {
                QVector< Connection* >& connections
                    = attributes[ i ]->port()->connections();

                for ( int j = 0; j < connections.size(); ++j )
                {
                    if ( connections[ j ] && connections[ j ]->scene() )
                    {
                        m_scene->removeItem( connections[ j ] );
                    }
                }
            }

            break;
        }

        default:
        {
            break;
        }
    }

    m_scene->removeItem( component );

    m_components.erase( id );
}

//------------------------------------------------------------------------------
//

void Graph::removeComponents( QList< QGraphicsItem* >& components )
{
    foreach( QGraphicsItem* component, components )
    {
        removeComponent( component );
    }
}

//------------------------------------------------------------------------------
//

QList< QGraphicsItem* > Graph::components() const
{
    QList< QGraphicsItem* > items = m_scene->items();
    QList< QGraphicsItem* > components;

    foreach( QGraphicsItem* item, items )
    {
        if ( item->type() > QGraphicsItem::UserType )
        {
            components.append( item );
        }
    }

    return components;
}

//------------------------------------------------------------------------------
//

QList< QGraphicsItem* > Graph::componentsByType( const int& type ) const
{
    QList< QGraphicsItem* > items = m_scene->items();
    QList< QGraphicsItem* > components;

    foreach( QGraphicsItem* item, items )
    {
        if ( item->type() == type )
        {
            components.append( item );
        }
    }

    return components;
}

//------------------------------------------------------------------------------
//

QGraphicsItem* Graph::component( const uint32_t& id )
{
    if ( !m_components.count( id ) ) return 0x0;

    return m_components[ id ];
}

//------------------------------------------------------------------------------
//

bool Graph::switchComponentId( const uint32_t& oldId,
                               const uint32_t& newId )
{
    if ( !m_components.count( oldId )
         || m_components.count( newId ) )
    {
        return false;
    }

    QGraphicsItem* component = m_components[ oldId ];
    m_components.erase( oldId );

    m_components[ newId ] = component;

    return true;
}

//------------------------------------------------------------------------------
//

Variable* Graph::createVariable( const uint32_t& typeId )
{
    return TypeStore::createVariable( this, typeId );
}

//------------------------------------------------------------------------------
//

Node* Graph::createNode( const uint32_t& typeId )
{
    return TypeStore::createNode( this, typeId );
}

//------------------------------------------------------------------------------
//

Connection* Graph::createConnection( Port* port1,
                                     Port* port2 )
{
    Connection* c = new Connection( 0x0 );
    m_scene->addItem( c );
    c->setPort1( port1 );
    c->setPos1( port1->scenePos() );
    c->setPort2( port2 );
    c->setPos2( port2->scenePos() );
    c->update();

    return c;
}

//------------------------------------------------------------------------------
//

Variable* Graph::addAttribute( Node* node,
                               const uint32_t& typeId,
                               const QString& name,
                               Direction direction )
{
    Variable* variable = createVariable( typeId );
    variable->setParentItem( node );
    variable->setFlag( QGraphicsItem::ItemIsMovable, false );
    variable->setFlag( QGraphicsItem::ItemIsSelectable, false );
    variable->setName( name );
    variable->setDirection( direction );

    node->update();

    return variable;
}

//------------------------------------------------------------------------------
//

QGraphicsItem* Graph::itemAt( const QPointF& pos )
{
    QList< QGraphicsItem* > items = m_scene->items( QRectF( pos - QPointF( 1, 1 ), QSize( 3, 3 ) ) );

    foreach( QGraphicsItem* item, items )
    {
        if ( item->type() > QGraphicsItem::UserType )
        {
            return item;
        }
    }

    return 0x0;
}

//------------------------------------------------------------------------------
//

bool Graph::eventFilter( QObject* o, QEvent* e )
{
    QGraphicsSceneMouseEvent* me = (QGraphicsSceneMouseEvent*) e;

    switch ( (int) e->type() )
	{
        case QEvent::GraphicsSceneMousePress:
        {
            switch ( (int) me->button() )
            {
                case Qt::LeftButton:
                {
                    QGraphicsItem* item = itemAt( me->scenePos() );

                    if ( item && item->type() == Port::Type )
                    {
                        m_view->setDragMode( QGraphicsView::NoDrag );

                        m_conn = new Connection( 0x0 );
                        m_scene->addItem( m_conn );
                        m_conn->setPort1( static_cast< Port* >( item ) );
                        m_conn->setPos1( item->scenePos() );
                        m_conn->setPos2( me->scenePos() );
                        m_conn->update();

                        return true;
                    }

                    break;
                }
            }
        }

        case QEvent::GraphicsSceneMouseMove:
        {
            if ( m_conn )
            {
                m_conn->setPos2( me->scenePos() );
                m_conn->update();

                return true;
            }

            break;
        }

        case QEvent::GraphicsSceneMouseRelease:
        {
            if ( m_conn && me->button() == Qt::LeftButton )
            {
                m_view->setDragMode( QGraphicsView::RubberBandDrag );

                QGraphicsItem* item = itemAt( me->scenePos() );

                if ( item && item->type() == Port::Type )
                {
                    Port* port1 = m_conn->port1();
                    Port* port2 = static_cast< Port* >( item );

                    if ( port1->variable()->node() != port2->variable()->node()
                         && port1->variable()->direction() != port2->variable()->direction()
                         && !port1->isConnected( port2 ) )
                    {
                        m_conn->setPos2( port2->scenePos() );
                        m_conn->setPort2( port2 );
                        m_conn->update();
                        m_conn = 0x0;

                        return true;
                    }
                }

                delete m_conn;
                m_conn = 0x0;

                return true;
            }

            break;
        }
	}

    return QObject::eventFilter( o, e );
}
