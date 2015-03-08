//------------------------------------------------------------------------------
//

#include "port.hpp"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "connection.hpp"

//------------------------------------------------------------------------------
//

Port::Port( QGraphicsItem* parent )
    : QGraphicsPathItem( parent )
{
    label = new QGraphicsTextItem( this );

    radius_ = 4;
	margin = 2;

	QPainterPath p;
    p.addEllipse( -radius_, -radius_, 2*radius_, 2*radius_ );
    setPath( p );
    setPen( Qt::NoPen );
    setBrush( QBrush( QColor( 255, 61, 0 ) ) );
    setFlag( QGraphicsItem::ItemSendsScenePositionChanges );

	m_portFlags = 0;
}

//------------------------------------------------------------------------------
//

Port::~Port()
{
    foreach( Connection *conn, m_connections )
		delete conn;
}

//------------------------------------------------------------------------------
//

void Port::setNode( Node* b )
{
    m_node = b;
}

//------------------------------------------------------------------------------
//

void Port::setName( const QString &n )
{
	name = n;
    label->setPlainText( n );
}

//------------------------------------------------------------------------------
//

void Port::setIsOutput( bool o )
{
	isOutput_ = o;

    if ( isOutput_ )
        label->setPos( -radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2 );
	else
        label->setPos( radius_ + margin, -label->boundingRect().height()/2 );
}

//------------------------------------------------------------------------------
//

int Port::radius()
{
	return radius_;
}

//------------------------------------------------------------------------------
//

bool Port::isOutput()
{
	return isOutput_;
}

//------------------------------------------------------------------------------
//

QVector< Connection* >& Port::connections()
{
	return m_connections;
}

//------------------------------------------------------------------------------
//

void Port::setPortFlags( int f )
{
	m_portFlags = f;

    if ( m_portFlags & TypePort )
	{
        QFont font( scene()->font() );
        font.setItalic( true );
        label->setFont( font );
        setPath( QPainterPath() );
    }

    else if ( m_portFlags & NamePort )
    {
        QFont font( scene()->font() );
        font.setBold( true );
        label->setFont( font );
        setPath( QPainterPath() );
    }
}

//------------------------------------------------------------------------------
//

Node* Port::node() const
{
    return m_node;
}

//------------------------------------------------------------------------------
//

quint64 Port::ptr()
{
	return m_ptr;
}

//------------------------------------------------------------------------------
//

void Port::setPtr( quint64 p )
{
	m_ptr = p;
}

//------------------------------------------------------------------------------
//

bool Port::isConnected( Port* other )
{
    foreach( Connection* conn, m_connections )
        if ( conn->port1() == other || conn->port2() == other )
			return true;

	return false;
}

//------------------------------------------------------------------------------
//

QVariant Port::itemChange( GraphicsItemChange change, const QVariant &value )
{
    if ( change == ItemScenePositionHasChanged )
	{
        foreach( Connection *conn, m_connections )
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}

	return value;
}
