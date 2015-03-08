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

class Node;
class QGraphicsScene;

//------------------------------------------------------------------------------
//

class TypeStoreTypesList
    : public QListWidget
{
    Q_OBJECT

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

class TypeStoreView
    : public QTabWidget
{
    Q_OBJECT

public:

    explicit TypeStoreView( QWidget* parent = 0x0 );

//    static Variable* createVariable( QGraphicsScene* scene,
//                                     const uint32_t& id );

    static Node* createNode( QGraphicsScene* scene,
                             const uint32_t& id );

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
