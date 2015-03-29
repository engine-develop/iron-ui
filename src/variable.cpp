//------------------------------------------------------------------------------
//

// Qt
#include <QDebug>
#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

// Engine
#include "graph.hpp"
#include "port.hpp"
#include "node.hpp"

#include "variable.hpp"

//------------------------------------------------------------------------------
//

Variable::Variable( QGraphicsItem* parent )
    : QGraphicsPathItem( parent )
    , m_id( 0 )
    , m_typeId( 0 )
    , m_label( 0x0 )
    , m_codeAnchor( 0x0 )
    , m_direction( Output )
    , m_port( 0x0 )
    , m_attributeId( 0 )
{
    setFlags(   QGraphicsItem::ItemIsMovable
              | QGraphicsItem::ItemIsSelectable
              | QGraphicsItem::ItemIsFocusable );

    m_label = new QGraphicsTextItem( this );
    m_codeAnchor = new QGraphicsPathItem( this );
    m_port = new Port( this );

    setDirection( Output );
}

//------------------------------------------------------------------------------
//

Variable::~Variable()
{
}

//------------------------------------------------------------------------------
//

int Variable::type() const
{
    return Type;
}

//------------------------------------------------------------------------------
//

void Variable::setId( const uint32_t& id )
{
    m_id = id;
}

//------------------------------------------------------------------------------
//

const uint32_t& Variable::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
//

void Variable::setTypeId( const uint32_t& typeId )
{
    m_typeId = typeId;
}

//------------------------------------------------------------------------------
//

const uint32_t& Variable::typeId() const
{
    return m_typeId;
}

//------------------------------------------------------------------------------
//

Port* Variable::port()
{
    return m_port;
}

//------------------------------------------------------------------------------
//

Node* Variable::node()
{
    return (Node*) parentItem();
}

//------------------------------------------------------------------------------
//

void Variable::setAttributeId( const uint8_t& id )
{
    m_attributeId = id;
}

//------------------------------------------------------------------------------
//

const uint8_t& Variable::attributeId() const
{
    return m_attributeId;
}

//------------------------------------------------------------------------------
//

void Variable::setDirection( const Direction& direction )
{
    m_direction = direction;

    update();
}

//------------------------------------------------------------------------------
//

const Direction& Variable::direction() const
{
    return m_direction;
}

//------------------------------------------------------------------------------
//

void Variable::setName( const QString& name )
{
    if ( !scene() ) return;

    QFont font( scene()->font() );
    font.setItalic( true );
    m_label->setFont( font );
    m_label->setPlainText( name );

    update();
}

//------------------------------------------------------------------------------
//

QString Variable::name() const
{
    return m_label->toPlainText();
}

//------------------------------------------------------------------------------
//

double Variable::codePos() const
{
    return m_codeAnchor->boundingRect().bottomLeft().x();
}

//------------------------------------------------------------------------------
//

void Variable::paint( QPainter* painter,
                      const QStyleOptionGraphicsItem* option,
                      QWidget* widget )
{
    Q_UNUSED( option )
    Q_UNUSED( widget )

    if ( isSelected()
         || ( parentItem() && parentItem()->isSelected() ) )
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

void Variable::update()
{
    static const int widthPad = 30;

    int lWidth  = m_label->boundingRect().width();
    int lHeight = m_label->boundingRect().height();
    int width = lWidth + widthPad;

    QPainterPath path;
    path.addRoundedRect( 0, 0, width, lHeight, 10, 10 );
    setPath( path );

    m_label->setPos( width/2 - lWidth/2, -1 );

    // Add code anchor
    //
    QPainterPath codeAnchorPath;
    codeAnchorPath.addRect( 9, lHeight, 2, 1e6 );
    m_codeAnchor->setPath( codeAnchorPath );
    m_codeAnchor->setPen( Qt::NoPen );
    m_codeAnchor->setBrush( QColor( 255, 255, 255, 15 ) );

    if ( node() != 0x0 )
    {
        m_codeAnchor->hide();
    }

    m_port->update();
}
