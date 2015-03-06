//------------------------------------------------------------------------------
//

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>

#include "port.hpp"
#include "connection.hpp"
#include "node.hpp"

#include "graph.hpp"

//------------------------------------------------------------------------------
//

GraphScene::GraphScene( QObject *parent )
    : QGraphicsScene( parent )
{
    //setBackgroundBrush( QBrush( QColor( 89, 86, 82 ) ) );
    setBackgroundBrush( QImage( ":/images/resources/images/scene_bkg_tile.png" ) );
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
}

//------------------------------------------------------------------------------
//

void GraphScene::removeNode( QGraphicsItem* item )
{
    if ( !item ) return;

    Node* n = static_cast< Node* >( item );

    QVector< Port* > ports = n->ports();

    for ( int j = 0; j < ports.size(); ++j )
    {
        QVector< Connection* >& connections = ports[ j ]->connections();

        for ( int k = 0; k < connections.size(); ++k )
        {
            if ( connections[ k ] && connections[ k ]->scene() )
            {
                removeItem( connections[ k ] );
            }
        }
    }

    removeItem( item );
}

//------------------------------------------------------------------------------
//

void GraphScene::removeSelected()
{
    QList< QGraphicsItem* > items = selectedItems();

    for ( int i = 0; i < items.size(); ++i )
    {
        if ( items[ i ]->type() == Node::Type )
        {
            removeNode( items[ i ] );
        }

        else
        {
            removeItem( items[ i ] );
        }
    }
}

//------------------------------------------------------------------------------
//

GraphView::GraphView( QGraphicsScene* scene, QWidget* parent )
    : QGraphicsView( scene, parent )
{
    setRenderHint( QPainter::Antialiasing, true );
    setRenderHint( QPainter::SmoothPixmapTransform, true );
    setDragMode( QGraphicsView::RubberBandDrag );
    setAcceptDrops( true );
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
    Node* n = new Node( 0x0 );
    scene()->addItem( n );

    static const char* names[] = { "Vin", "Voutsadfasdf", "Imin", "Imax", "mul", "add", "sub", "div", "Conv", "FFT" };
    for ( int i = 0; i < 4 + rand() % 3; i++ )
    {
        n->addPort( names[ rand() % 10 ], rand() % 2, 0, 0 );
    }

    n->setPos( mapToScene( event->pos() ) );

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

Graph::Graph( QObject *parent )
    : QObject( parent )
    , scene( 0x0 )
    , view( 0x0 )
    , conn( 0x0 )
{
}

//------------------------------------------------------------------------------
//

void Graph::install( GraphScene* s )
{
    s->installEventFilter( this );
	scene = s;

    QList< QGraphicsView* > views = scene->views();

    if ( !views.empty() )
    {
        view = static_cast< GraphView* >( views[ 0 ] );
    }
}

//------------------------------------------------------------------------------
//

QGraphicsItem* Graph::itemAt( const QPointF &pos )
{
    QList< QGraphicsItem* > items = scene->items( QRectF( pos - QPointF( 1, 1 ), QSize( 3, 3 ) ) );

    foreach( QGraphicsItem *item, items )
        if ( item->type() > QGraphicsItem::UserType )
			return item;

	return 0;
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
                    QGraphicsItem *item = itemAt( me->scenePos() );

                    if ( item && item->type() == Port::Type )
                    {
                        view->setDragMode( QGraphicsView::NoDrag );

                        conn = new Connection( 0 );
                        scene->addItem( conn );
                        conn->setPort1( (Port*) item );
                        conn->setPos1( item->scenePos() );
                        conn->setPos2( me->scenePos() );
                        conn->updatePath();

                        return true;
                    }

                    else if ( item && item->type() == Node::Type )
                    {
                    }

                    break;
                }

                case Qt::RightButton:
                {
                    // TODO: context menu
                    //
                    break;
                }
            }
        }

        case QEvent::GraphicsSceneMouseMove:
        {
            if ( conn )
            {
                conn->setPos2( me->scenePos() );
                conn->updatePath();

                return true;
            }

            break;
        }

        case QEvent::GraphicsSceneMouseRelease:
        {
            if ( conn && me->button() == Qt::LeftButton )
            {
                view->setDragMode( QGraphicsView::RubberBandDrag );

                QGraphicsItem *item = itemAt( me->scenePos() );

                if ( item && item->type() == Port::Type )
                {
                    Port *port1 = conn->port1();
                    Port *port2 = (Port*) item;

                    if ( port1->node() != port2->node() && port1->isOutput() != port2->isOutput() && !port1->isConnected( port2 ) )
                    {
                        conn->setPos2( port2->scenePos() );
                        conn->setPort2( port2 );
                        conn->updatePath();
                        conn = 0x0;

                        return true;
                    }
                }

                delete conn;
                conn = 0x0;

                return true;
            }

            break;
        }
	}

    return QObject::eventFilter( o, e );
}

//------------------------------------------------------------------------------
//

void Graph::save( QDataStream &ds )
{
    foreach( QGraphicsItem *item, scene->items() )
        if ( item->type() == Node::Type )
		{
			ds << item->type();
            ( (Node*) item )->save( ds );
		}

    foreach( QGraphicsItem *item, scene->items() )
        if ( item->type() == Connection::Type )
		{
			ds << item->type();
            ( (Connection*) item )->save( ds );
		}
}

//------------------------------------------------------------------------------
//

void Graph::load( QDataStream &ds )
{
	scene->clear();

    QMap< quint64, Port* > portMap;

    while ( !ds.atEnd() )
	{
		int type;
		ds >> type;
        if ( type == Node::Type )
		{
            Node *node = new Node( 0 );
            scene->addItem( node );
            node->load( ds, portMap );
        } else if ( type == Connection::Type )
		{
            Connection *conn = new Connection( 0 );
            scene->addItem( conn );
            conn->load( ds, portMap );
		}
	}
}
