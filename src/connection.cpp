//------------------------------------------------------------------------------

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

#include "utility.hpp"
#include "port.hpp"

#include "connection.hpp"

//------------------------------------------------------------------------------
//

Connection::Connection( QGraphicsItem* parent )
    : QGraphicsPathItem( parent )
    , m_pos1()
    , m_pos2()
    , m_port1( 0x0 )
    , m_port2( 0x0 )
{
    m_color = QColor::fromHsv( 14, 255, randRange( 180, 255 ) );
    setPen( QPen( m_color, 2 ) );
    setBrush( Qt::NoBrush );
    setZValue( -1 );
    setFlag( QGraphicsItem::ItemIsSelectable );
}

//------------------------------------------------------------------------------
//

Connection::~Connection()
{
    if ( m_port1 )
    {
        m_port1->connections().remove( m_port1->connections().indexOf( this ) );
    }

    if ( m_port2 )
    {
        m_port2->connections().remove( m_port2->connections().indexOf( this ) );
    }
}

//------------------------------------------------------------------------------
//

int Connection::type() const
{
    return Type;
}

//------------------------------------------------------------------------------
//

void Connection::setPos1( const QPointF& p )
{
    m_pos1 = p;
}

//------------------------------------------------------------------------------
//

void Connection::setPos2( const QPointF& p )
{
    m_pos2 = p;
}

//------------------------------------------------------------------------------
//

void Connection::setPort1( Port* p )
{
	m_port1 = p;
    m_port1->connections().append( this );
}

//------------------------------------------------------------------------------
//

void Connection::setPort2( Port* p )
{
	m_port2 = p;
    m_port2->connections().append( this );
}

//------------------------------------------------------------------------------
//

Port* Connection::port1() const
{
	return m_port1;
}

//------------------------------------------------------------------------------
//

Port* Connection::port2() const
{
	return m_port2;
}

//------------------------------------------------------------------------------
//

void Connection::updatePosFromPorts()
{
    m_pos1 = m_port1->scenePos();
    m_pos2 = m_port2->scenePos();
}

//------------------------------------------------------------------------------
//

void Connection::update()
{
    QPainterPath p;
    p.moveTo( m_pos1 );

    qreal dx = m_pos2.x() - m_pos1.x();
    qreal dy = m_pos2.y() - m_pos1.y();

    QPointF ctr1( m_pos1.x() + dx * 0.25, m_pos1.y() + dy * 0.1 );
    QPointF ctr2( m_pos1.x() + dx * 0.75, m_pos1.y() + dy * 0.9 );

    p.cubicTo( ctr1, ctr2, m_pos2 );

    setPath( p );
}
