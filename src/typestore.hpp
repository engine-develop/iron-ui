#ifndef IRUI_TYPESTORE_HPP
#define IRUI_TYPESTORE_HPP

//------------------------------------------------------------------------------
//

// Qt
#include <QListWidget>
#include <QTabWidget>

// Engine
#include <iron.hpp>

//------------------------------------------------------------------------------
//

class Variable;
class Node;
class Graph;
class QGraphicsScene;

//------------------------------------------------------------------------------
//

template< int T >
struct TTypeStoreTypesListItem
{
    static EN_INLINE QString icon() { return ""; }
};

template<>
struct TTypeStoreTypesListItem< engine::Types_Variable >
{
    static EN_INLINE QString icon() { return ":/icons/resources/icons/variable.png"; }
};

template<>
struct TTypeStoreTypesListItem< engine::Types_Node >
{
    static EN_INLINE QString icon() { return ":/icons/resources/icons/node.png"; }
};

//------------------------------------------------------------------------------
//

class TypeStoreTypesList
    : public QListWidget
{

public:

    explicit TypeStoreTypesList( engine::Types type,
                                 QWidget* parent = 0x0 );

protected:

    QStringList mimeTypes() const;

    QMimeData* mimeData( const QList< QListWidgetItem* > items ) const;

    engine::Types m_type;

};

//------------------------------------------------------------------------------
//

class TypeStore
    : public QTabWidget
{
    Q_OBJECT

public:

    TypeStore( QWidget* parent = 0x0 );

    static Variable* createVariable( Graph* graph,
                                     const uint32_t& typeId );

    static Node* createNode( Graph* graph,
                             const uint32_t& typeId );

public slots:

    void reload();

protected:

    template< int T >
    TypeStoreTypesList* createTab( const std::string& category );

    template< int T >
    void addCategoryTab( const std::string& category );

    template< int T >
    void addCategoryTabsType();

    void addCategoryTabs();

};

#endif // IRUI_TYPESTORE_HPP
