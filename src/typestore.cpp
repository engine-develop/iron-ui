//------------------------------------------------------------------------------
//

// Qt
#include <QDebug>
#include <QListWidget>
#include <QMimeData>
#include <QGraphicsScene>

// Engine
#include <iron.hpp>

#include "node.hpp"
#include "port.hpp"

#include "typestore.hpp"

//------------------------------------------------------------------------------
//

TypeStoreTypesList::TypeStoreTypesList( engine::Types type,
                                        QWidget* parent )
    : QListWidget( parent )
    , m_type( type )
{
}

//------------------------------------------------------------------------------
//

QStringList TypeStoreTypesList::mimeTypes() const
{
    QStringList types;
    types.append( "text/plain" );

    return types;
}

//------------------------------------------------------------------------------
//

QMimeData* TypeStoreTypesList::mimeData( const QList< QListWidgetItem* > items ) const
{
    QVariant typeData = items[ 0 ]->data( Qt::UserRole + 1 );

    QString label;
    if      ( m_type == engine::Types_Variable ) { label = "variable"; }
    else if ( m_type == engine::Types_Node )     { label = "node";     }

    QMimeData* data = new QMimeData();
    data->setText( label + ":" + typeData.toString() );

    return data;
}

//------------------------------------------------------------------------------
//

TypeStoreView::TypeStoreView( QWidget* parent )
    : QTabWidget( parent )
{
    reload();
}

//------------------------------------------------------------------------------
//

void TypeStoreView::reload()
{
    clear();
    addCategoryTabs();
}

//------------------------------------------------------------------------------
//

template< int T >
TypeStoreTypesList* TypeStoreView::createTab( const std::string& category )
{
    TypeStoreTypesList* list = new TypeStoreTypesList( engine::Types( T ), this );
    //list->setViewMode( QListView::ListMode );
    //list->setUniformItemSizes( true );
    //list->setGridSize( QSize( 70, 32 ) );
    list->setFlow( QListView::LeftToRight );
    list->setWrapping( true );
    list->setSelectionMode( QAbstractItemView::SingleSelection );
    list->setDragEnabled( true );

    addTab( list, category.c_str() );

    return list;
}

//------------------------------------------------------------------------------
//

template< int T >
void TypeStoreView::addCategoryTab( const std::string& category )
{
    typedef engine::TypeStore< T > typestore_t;
    typedef engine::Type< T > type_t;

    std::vector< type_t* > types
        = typestore_t::get().typesByCategory( category );

    if ( types.empty() ) return;

    std::sort( types.begin(), types.end() );

    TypeStoreTypesList* typesList = createTab< T >( category );

    for ( auto it = types.begin(); it != types.end(); ++it )
    {
        QListWidgetItem* item
            = new QListWidgetItem( QIcon( ":/icons/resources/icons/node.png" ),
                                   QString( (*it)->name.c_str() ) );
        item->setData( Qt::UserRole + 1, QVariant( (*it)->id ) );
        item->setToolTip( QString::fromStdString( (*it)->description ) );

        typesList->addItem( item );
    }
}

//------------------------------------------------------------------------------
//

template< int T >
void TypeStoreView::addCategoryTabsType()
{
    // Create node category tabs
    //
    std::vector< std::string > categories
        = engine::TypeStore< T >::get().categories();

    for ( auto it = categories.begin();
          it != categories.end(); ++it )
    {
        addCategoryTab< T >( *it );
    }
}

//------------------------------------------------------------------------------
//

void TypeStoreView::addCategoryTabs()
{
    // Add variable category tabs
    //
    addCategoryTabsType< engine::Types_Variable >();

    // Add node category tabs
    //
    addCategoryTabsType< engine::Types_Node >();
}

////------------------------------------------------------------------------------
////

//Variable* TypeStoreView::createVariable( QGraphicsScene* scene,
//                                         const uint32_t& id )
//{
//}

//------------------------------------------------------------------------------
//

Node* TypeStoreView::createNode( QGraphicsScene* scene,
                                 const uint32_t& id )
{
    typedef engine::TypeStore< engine::Types_Node > typestore_t;
    typedef engine::Type< engine::Types_Node > type_t;

    if ( !scene ) return 0x0;

    type_t* type = typestore_t::get().type( id );
    if ( !type ) return 0x0;

    Node* n = new Node( 0x0 );
    scene->addItem( n );
    n->setData( Qt::UserRole + 1, QVariant( type->id ) );

    // Add name port
    //
    Port* port = 0x0;
    port = n->addPort( QString::fromStdString( type->name ), false, Port::NamePort );

    // Add attribute ports
    //
    for ( auto it = type->attributes.begin();
          it != type->attributes.end(); ++it )
    {
        QString name = QString::fromStdString( (*it).name );

        if ( (*it).type == "Input"
          || (*it).type == "Internal" )
        {
            port = n->addPort( name, false );
        }

        else if ( (*it).type == "Output" )
        {
            port = n->addPort( name, true );
        }
    }

    return n;
}
