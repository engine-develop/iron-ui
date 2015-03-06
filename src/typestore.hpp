#ifndef IRUI_TYPESTORE_HPP
#define IRUI_TYPESTORE_HPP

//------------------------------------------------------------------------------
//

// Qt
#include <QTabWidget>

// Engine
#include <iron.hpp>

//------------------------------------------------------------------------------
//

class QListWidget;

//------------------------------------------------------------------------------
//

class TypeStoreView
    : public QTabWidget
{
    Q_OBJECT

public:

    typedef engine::TypeStore< engine::Types_Node > typestore_t;
    typedef engine::Type< engine::Types_Node > type_t;

    explicit TypeStoreView( QWidget* parent = 0x0 );

public slots:

    void reload();

protected:

    QListWidget* createTab( const std::string& category );

    void addCategoryTab( const std::string& category );

    void addCategoryTabs();

};

#endif // IRUI_TYPESTORE_HPP
