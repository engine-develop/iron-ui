//------------------------------------------------------------------------------
//

// Qt
#include <QDebug>
#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

// Engine
#include "graph.hpp"
#include "typestore.hpp"
#include "variable.hpp"
#include "connection.hpp"

#include "node.hpp"

//------------------------------------------------------------------------------
//

Node::Node( QGraphicsItem *parent )
    : QGraphicsPathItem( parent )
    , m_id( 0 )
    , m_typeId( 0 )
    , m_nameText( 0x0 )
    , m_codeAnchor( 0x0 )
{
    setFlags( QGraphicsItem::ItemIsMovable
              | QGraphicsItem::ItemIsSelectable );

    m_nameText = new QGraphicsTextItem( this );
    m_codeAnchor = new QGraphicsPathItem( this );

    update();
}

//------------------------------------------------------------------------------
//

Node::~Node()
{
}

//------------------------------------------------------------------------------
//

int Node::type() const
{
    return Type;
}

//------------------------------------------------------------------------------
//

void Node::setId( const uint32_t& id )
{
    m_id = id;
}

//------------------------------------------------------------------------------
//

const uint32_t& Node::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
//

void Node::setTypeId( const uint32_t& typeId )
{
    m_typeId = typeId;
}

//------------------------------------------------------------------------------
//

const uint32_t& Node::typeId() const
{
    return m_typeId;
}

//------------------------------------------------------------------------------
//

void Node::setName( const QString& name )
{
    if ( !scene() ) return;

    QFont font( scene()->font() );
    font.setBold( true );
    m_nameText->setFont( font );
    m_nameText->setPlainText( name );

    update();
}

//------------------------------------------------------------------------------
//

QString Node::name() const
{
    return m_nameText->toPlainText();
}

//------------------------------------------------------------------------------
//

double Node::codePos() const
{
    return m_codeAnchor->boundingRect().bottomLeft().x();
}

//------------------------------------------------------------------------------
//

QList< Variable* > Node::attributes() const
{
    QList< Variable* > result;

    foreach( QGraphicsItem* child, childItems() )
    {
        if ( child->type() == Variable::Type )
        {
            result.append( static_cast< Variable* >( child ) );
        }
    }

    return result;
}

//------------------------------------------------------------------------------
//

void Node::update()
{
    int nInputs = 0;
    int nOutputs = 0;
    double wMaxInput = 0;
    double wMaxOutput = 0;
    double yInput = 0;
    double yOutput = 0;

    // Setup input variables
    //
    foreach( QGraphicsItem* child, childItems() )
    {
        if ( child->type() != Variable::Type )
            continue;

        Variable* variable = static_cast< Variable* >( child );

        if ( variable->direction() != Input )
        {
            continue;
        }

        ++nInputs;
        wMaxInput = std::max( wMaxInput, variable->boundingRect().width() );
        variable->setPos( 0, yInput );
        yInput += variable->boundingRect().height();
    }

    // Setup output variables
    //
    foreach( QGraphicsItem* child, childItems() )
    {
        if ( child->type() != Variable::Type )
            continue;

        Variable* variable = static_cast< Variable* >( child );

        if ( variable->direction() != Output )
        {
            continue;
        }

        ++nOutputs;
        wMaxOutput = std::max( wMaxOutput, variable->boundingRect().width() );
        variable->setPos( wMaxInput, yOutput );
        yOutput += variable->boundingRect().height();
    }

    double width = wMaxInput + wMaxOutput - 10;
    if ( nOutputs ) width -= 10;
    double height = std::max( yInput, yOutput ) - 1;

    // Add outline rectangle
    //
    QPainterPath path;
    path.addRoundedRect( 9, 0, width, height, 10, 10 );
    setPath( path );

    // Setup name text
    //
    double lHeight = m_nameText->boundingRect().height();
    m_nameText->setPos( 10, -lHeight );

    // Add code anchor
    //
    QPainterPath codeAnchorPath;
    codeAnchorPath.addRect( 9, height, 2, 1e6 );
    m_codeAnchor->setPath( codeAnchorPath );
    m_codeAnchor->setPen( Qt::NoPen );
    m_codeAnchor->setBrush( QColor( 255, 255, 255, 15 ) );
}

//------------------------------------------------------------------------------
//

void Node::paint( QPainter* painter,
                  const QStyleOptionGraphicsItem* option,
                  QWidget* widget )
{
    Q_UNUSED( option )
    Q_UNUSED( widget )

    if ( isSelected() )
    {
        painter->setBrush( QColor( 150, 150, 150 ) );
    }

    else
    {
        painter->setBrush( QColor( 130, 130, 130 ) );
	}

    painter->setPen( Qt::NoPen );
    painter->drawPath( path() );
}

//------------------------------------------------------------------------------
//

QVariant Node::itemChange( GraphicsItemChange change,
                           const QVariant &value )
{
    Q_UNUSED( change );

	return value;
}
