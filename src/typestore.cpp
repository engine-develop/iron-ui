//------------------------------------------------------------------------------
//

// Qt
#include <QDebug>
#include <QListWidget>
#include <QMimeData>
#include <QGraphicsScene>

// Engine
#include <iron.hpp>

#include "variable.hpp"
#include "port.hpp"
#include "node.hpp"
#include "graph.hpp"

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

TypeStore::TypeStore( QWidget* parent )
    : QTabWidget( parent )
{
    reload();
}

//------------------------------------------------------------------------------
//

void TypeStore::reload()
{
    clear();
    addCategoryTabs();
}

//------------------------------------------------------------------------------
//

template< int T >
TypeStoreTypesList* TypeStore::createTab( const std::string& category )
{
    TypeStoreTypesList* list = new TypeStoreTypesList( engine::Types( T ), this );
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
void TypeStore::addCategoryTab( const std::string& category )
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
            = new QListWidgetItem( QIcon( TTypeStoreTypesListItem< T >::icon() ),
                                   QString( (*it)->name.c_str() ) );
        item->setData( Qt::UserRole + 1, QVariant( (*it)->id ) );
        item->setToolTip( QString::fromStdString( (*it)->description ) );

        typesList->addItem( item );
    }
}

//------------------------------------------------------------------------------
//

template< int T >
void TypeStore::addCategoryTabsType()
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

void TypeStore::addCategoryTabs()
{
    // Add variable category tabs
    //
    addCategoryTabsType< engine::Types_Variable >();

    // Add node category tabs
    //
    addCategoryTabsType< engine::Types_Node >();
}

//------------------------------------------------------------------------------
//

Variable* TypeStore::createVariable( Graph* graph,
                                     const uint32_t& typeId )
{
    typedef engine::TypeStore< engine::Types_Variable > v_typestore_t;
    typedef engine::Type< engine::Types_Variable > v_type_t;

    v_type_t* type = v_typestore_t::get().type( typeId );
    if ( !type ) return 0x0;

    Variable* variable = new Variable( 0x0 );
    graph->addComponent( variable );
    variable->setTypeId( type->id );
    variable->setName( QString::fromStdString( type->name ) );
    variable->setData( Qt::UserRole + 1, QVariant( type->id ) );

    return variable;
}

//------------------------------------------------------------------------------
//

Node* TypeStore::createNode( Graph* graph,
                             const uint32_t& typeId )
{
    typedef engine::TypeStore< engine::Types_Variable > v_typestore_t;
    typedef engine::Type< engine::Types_Variable > v_type_t;

    typedef engine::TypeStore< engine::Types_Node > n_typestore_t;
    typedef engine::Type< engine::Types_Node > n_type_t;

    n_type_t* type = n_typestore_t::get().type( typeId );
    if ( !type ) return 0x0;

    Node* node = new Node( 0x0 );
    graph->addComponent( node );
    node->setTypeId( type->id );
    node->setName( QString::fromStdString( type->name ) );
    node->setData( Qt::UserRole + 1, QVariant( type->id ) );

    // Add attributes
    //
    for ( auto it = type->attributes.begin();
          it != type->attributes.end(); ++it )
    {
        std::vector< v_type_t* > types
            = v_typestore_t::get().typesByName( (*it).variable );

        if ( types.empty() ) continue;

        QString name = QString::fromStdString( (*it).name );

        if ( (*it).type == "Input"
          || (*it).type == "Internal" )
        {
            graph->addAttribute( node, types[ 0 ]->id, name, Input );
        }

        else if ( (*it).type == "Output" )
        {
            graph->addAttribute( node, types[ 0 ]->id, name, Output );
        }
    }

    return node;
}
