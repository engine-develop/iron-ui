//------------------------------------------------------------------------------

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

#include "utility.hpp"
#include "port.hpp"

#include "connection.hpp"

//------------------------------------------------------------------------------
//

Connection::Connection( QGraphicsItem *parent )
    : QGraphicsPathItem( parent )
{
    color = QColor::fromHsv( 14, 255, randRange( 180, 255 ) );
	m_port1 = 0;
	m_port2 = 0;

    setPen( QPen( color, 2 ) );
    setBrush( Qt::NoBrush );
    setZValue( -1 );
    setFlag( QGraphicsItem::ItemIsSelectable );
}

//------------------------------------------------------------------------------
//

Connection::~Connection()
{
    if ( m_port1 )
        m_port1->connections().remove( m_port1->connections().indexOf( this ) );
    if ( m_port2 )
        m_port2->connections().remove( m_port2->connections().indexOf( this ) );
}

//------------------------------------------------------------------------------
//

void Connection::setPos1( const QPointF &p )
{
	pos1 = p;
}

//------------------------------------------------------------------------------
//

void Connection::setPos2( const QPointF &p )
{
	pos2 = p;
}

//------------------------------------------------------------------------------
//

void Connection::setPort1( Port *p )
{
	m_port1 = p;

    m_port1->connections().append( this );
}

//------------------------------------------------------------------------------
//

void Connection::setPort2( Port *p )
{
	m_port2 = p;

    m_port2->connections().append( this );
}

//------------------------------------------------------------------------------
//

void Connection::updatePosFromPorts()
{
	pos1 = m_port1->scenePos();
	pos2 = m_port2->scenePos();
}

//------------------------------------------------------------------------------
//

void Connection::updatePath()
{
	QPainterPath p;

	//QPointF pos1(m_port1->scenePos());
	//QPointF pos2(m_port2->scenePos());

    p.moveTo( pos1 );

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

    QPointF ctr1( pos1.x() + dx * 0.25, pos1.y() + dy * 0.1 );
    QPointF ctr2( pos1.x() + dx * 0.75, pos1.y() + dy * 0.9 );

    p.cubicTo( ctr1, ctr2, pos2 );

    setPath( p );
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

void Connection::save( QDataStream &ds )
{
	ds << (quint64) m_port1;
	ds << (quint64) m_port2;
}

//------------------------------------------------------------------------------
//

void Connection::load( QDataStream &ds, const QMap< quint64, Port* > &portMap )
{
	quint64 ptr1;
	quint64 ptr2;
	ds >> ptr1;
	ds >> ptr2;

    setPort1( portMap[ ptr1 ] );
    setPort2( portMap[ ptr2 ] );
	updatePosFromPorts();
	updatePath();
}
