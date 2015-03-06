//------------------------------------------------------------------------------
//

// Qt
#include <QListWidget>

// Engine
#include <iron.hpp>

#include "typestore.hpp"

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

QListWidget* TypeStoreView::createTab( const std::string& category )
{
    QListWidget* list = new QListWidget( this );
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

void TypeStoreView::addCategoryTab( const std::string& category )
{
    std::vector< type_t* > types
        = typestore_t::get().typesByCategory( category );

    if ( types.empty() ) return;

    std::sort( types.begin(), types.end() );

    QListWidget* catList = createTab( category );

    for ( auto it = types.begin(); it != types.end(); ++it )
    {
        catList->addItem( new QListWidgetItem( QIcon( ":/icons/resources/icons/node.png" ),
                                               QString( (*it)->name.c_str() ) ) );
    }
}

//------------------------------------------------------------------------------
//

void TypeStoreView::addCategoryTabs()
{
    createTab( "User" );

    std::vector< std::string > categories
        = typestore_t::get().categories();

    for ( auto it = categories.begin();
          it != categories.end(); ++it )
    {
        addCategoryTab( *it );
    }
}
