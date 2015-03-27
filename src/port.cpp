//------------------------------------------------------------------------------
//

// Qt
#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QLinearGradient>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QKeyEvent>

// Engine
#include "connection.hpp"

#include "port.hpp"

//------------------------------------------------------------------------------
//

PortValue::PortValue( QGraphicsItem* parent )
    : QGraphicsTextItem( parent )
{
}

//------------------------------------------------------------------------------
//

void PortValue::keyPressEvent( QKeyEvent* event )
{
    if (    event->key() != Qt::Key_Enter
         && event->key() != Qt::Key_Return
         && event->key() != Qt::Key_Space )
    {
        QGraphicsTextItem::keyPressEvent( event );
    }

    else
    {
        event->ignore();
    }
}

//------------------------------------------------------------------------------
//

void PortValue::paint( QPainter* p,
                       const QStyleOptionGraphicsItem* o,
                       QWidget* w )
{
    p->setPen( Qt::NoPen );
    p->setBackgroundMode( Qt::TransparentMode );
    p->drawRect( boundingRect() );

    QGraphicsTextItem::paint( p, o, w );
}

//------------------------------------------------------------------------------
//

Port::Port( QGraphicsItem* parent )
    : QGraphicsPathItem( parent )
    , m_state( 0 )
{
    setFlag( QGraphicsItem::ItemSendsScenePositionChanges );
    setPen( Qt::NoPen );

    m_valueText = new PortValue( this );
    m_valueText->setTextInteractionFlags( Qt::TextEditorInteraction );
    m_valueText->setTextWidth( 60 );
    m_valueText->setDefaultTextColor( QColor( 0, 0, 0 ) );
    m_valueText->setPlainText( "0" );
}

//------------------------------------------------------------------------------
//

Port::~Port()
{
    foreach( Connection *conn, m_connections )
    {
		delete conn;
    }
}

//------------------------------------------------------------------------------
//

void Port::setState( int state )
{
    m_state = state;
}

//------------------------------------------------------------------------------
//

const int& Port::state() const
{
    return m_state;
}

//------------------------------------------------------------------------------
//

void Port::setDirection( Direction direction )
{
    m_direction = direction;

    update();
}

//------------------------------------------------------------------------------
//

const Direction& Port::direction() const
{
    return m_direction;
}

//------------------------------------------------------------------------------
//

Variable* Port::variable()
{
    return (Variable*) parentItem();
}

//------------------------------------------------------------------------------
//

QString Port::value() const
{
    return m_valueText->toPlainText();
}

//------------------------------------------------------------------------------
//

QVector< Connection* >& Port::connections()
{
	return m_connections;
}

//------------------------------------------------------------------------------
//

bool Port::isConnected( Port* other )
{
    foreach( Connection* conn, m_connections )
    {
        if ( conn->port1() == other || conn->port2() == other )
        {
			return true;
        }
    }

	return false;
}

//------------------------------------------------------------------------------
//

void Port::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event )
{
    m_state ^= Expanded;
    update();

    event->accept();
}

//------------------------------------------------------------------------------
//

QVariant Port::itemChange( GraphicsItemChange change, const QVariant &value )
{
    if ( change == ItemScenePositionHasChanged )
	{
        foreach( Connection* conn, m_connections )
		{
			conn->updatePosFromPorts();
            conn->update();
		}
	}

	return value;
}

//------------------------------------------------------------------------------
//

void Port::update()
{    
    static const int widthPad = 5;
    int widthExpand = ( m_state & Expanded ) ? 70 : 0;

    int pWidth  = parentItem()->boundingRect().width();
    int pHeight = parentItem()->boundingRect().height();

    int hScale = 1;
    int hOffset = pWidth - widthPad;
    int gradientId = 0;

    if ( m_direction == Input )
    {
        hScale = -1;
        hOffset = widthPad;
        gradientId = 1;
    }

    QPolygon poly;
    poly << QPoint(  (widthPad+widthExpand) * hScale,  pHeight/2 )
         << QPoint(  (widthPad+widthExpand) * hScale, -(pHeight-1)/2 )
         << QPoint( -widthPad * hScale, -(pHeight-1)/2 )
         << QPoint(  0,                  0         )
         << QPoint( -widthPad * hScale,  pHeight/2 );

    QPainterPath path;
    path.addPolygon( poly );
    setPath( path );

    setPos( hOffset - 1, (pHeight-1)/2 );

    QLinearGradient gradient( boundingRect().bottomRight(),
                              boundingRect().bottomLeft() );
    gradient.setColorAt(  gradientId, QColor( 255, 61, 0 ) );
    gradient.setColorAt( !gradientId, QColor( 179, 43, 0 ) );

    if ( m_state & Expanded )
    {
        if ( m_direction == Input )
        {
            gradient.setFinalStop( -10.0, 10.0 );
            m_valueText->setPos( -70, -(pHeight-1)/2 );
        }

        else if ( m_direction == Output )
        {
            gradient.setStart( 10.0, 10.0 );
            m_valueText->setPos( 10, -(pHeight-1)/2 );
        }

        m_valueText->show();
    }

    else
    {
        m_valueText->hide();
    }

    setBrush( gradient );
}
