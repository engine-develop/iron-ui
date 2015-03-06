//------------------------------------------------------------------------------
//

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "node.hpp"
#include "connection.hpp"

#include "port.hpp"

//------------------------------------------------------------------------------
//

Node::Node( QGraphicsItem *parent )
    : QGraphicsPathItem( parent )
{
	QPainterPath p;
	p.addRoundedRect(-50, -15, 100, 30, 5, 5);
    setPath( p );
    setFlag( QGraphicsItem::ItemIsMovable );
    setFlag( QGraphicsItem::ItemIsSelectable );
	horzMargin = 20;
	vertMargin = 5;
	width = horzMargin;
	height = vertMargin;
}

//------------------------------------------------------------------------------
//

Node::~Node()
{
}

//------------------------------------------------------------------------------
//

Port* Node::addPort( const QString &name, bool isOutput, int flags, int ptr )
{
    Port *port = new Port( this );
    port->setName( name );
    port->setIsOutput( isOutput );
    port->setNode( this );
    port->setPortFlags( flags );
    port->setPtr( ptr );

    QFontMetrics fm( scene()->font() );
    int w = fm.width( name );
	int h = fm.height();
	// port->setPos(0, height + h/2);
    if ( w > width - horzMargin )
		width = w + horzMargin;
	height += h;

	QPainterPath p;
    p.addRoundedRect( -width/2, -height/2, width, height, 5, 5 );
	setPath(p);

	int y = -height / 2 + vertMargin + port->radius();
    foreach( QGraphicsItem *port_, childItems() ) {
        if ( port_->type() != Port::Type )
			continue;

        Port *port = (Port*) port_;
        if ( port->isOutput() )
            port->setPos( width/2 + port->radius(), y );
		else
            port->setPos( -width/2 - port->radius(), y );
		y += h;
	}

	return port;
}

//------------------------------------------------------------------------------
//

void Node::addInputPort( const QString &name )
{
    addPort( name, false );
}

//------------------------------------------------------------------------------
//

void Node::addOutputPort( const QString &name )
{
    addPort( name, true );
}

//------------------------------------------------------------------------------
//

void Node::addInputPorts( const QStringList &names )
{
    foreach( QString n, names )
        addInputPort( n );
}

//------------------------------------------------------------------------------
//

void Node::addOutputPorts( const QStringList &names )
{
    foreach( QString n, names )
        addOutputPort( n );
}

//------------------------------------------------------------------------------
//

void Node::save( QDataStream &ds )
{
	ds << pos();

	int count(0);

    foreach( QGraphicsItem *port_, childItems() )
	{
        if ( port_->type() != Port::Type )
			continue;

		count++;
	}

	ds << count;

    foreach( QGraphicsItem *port_, childItems() )
	{
        if ( port_->type() != Port::Type )
			continue;

        Port *port = (Port*) port_;
		ds << (quint64) port;
		ds << port->portName();
		ds << port->isOutput();
		ds << port->portFlags();
	}
}

//------------------------------------------------------------------------------
//

void Node::load( QDataStream &ds, QMap< quint64, Port* > &portMap )
{
	QPointF p;
	ds >> p;
	setPos(p);
	int count;
	ds >> count;
    for ( int i = 0; i < count; i++ )
	{
		QString name;
		bool output;
		int flags;
		quint64 ptr;

		ds >> ptr;
		ds >> name;
		ds >> output;
		ds >> flags;
        portMap[ptr] = addPort( name, output, flags, ptr );
	}
}

//------------------------------------------------------------------------------
//

void Node::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Q_UNUSED( option )
    Q_UNUSED( widget )

    if ( isSelected() ) {
        painter->setPen( Qt::NoPen );
        painter->setBrush( QColor( 145, 145, 145 ) );
	} else {
        painter->setPen( Qt::NoPen );
        painter->setBrush( QColor( 130, 130, 130 ) );
	}

    painter->drawPath( path() );
}

//------------------------------------------------------------------------------
//

Node* Node::clone()
{
    Node *b = new Node( 0 );
    this->scene()->addItem( b );

    foreach( QGraphicsItem *port_, childItems() )
	{
        if ( port_->type() == Port::Type )
		{
            Port *port = (Port*) port_;
            b->addPort( port->portName(), port->isOutput(), port->portFlags(), port->ptr() );
		}
	}

	return b;
}

//------------------------------------------------------------------------------
//

QVector< Port* > Node::ports()
{
    QVector< Port* > res;

    foreach( QGraphicsItem *port_, childItems() )
	{
        if ( port_->type() == Port::Type )
            res.append( (Port*) port_ );
	}

	return res;
}

//------------------------------------------------------------------------------
//

QVariant Node::itemChange( GraphicsItemChange change, const QVariant &value )
{
    Q_UNUSED( change );

	return value;
}
