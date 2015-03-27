//------------------------------------------------------------------------------
//

// Qt
#include <QtWidgets>
#include <QPalette>
#include <QFileDialog>
#include <QString>

// Engine
#include "sketch.hpp"
#include "typestore.hpp"
#include "node.hpp"
#include "graph.hpp"

#include "mainwindow.hpp"

//------------------------------------------------------------------------------
//

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
{
    // Set window title, geometry
    //
    QString windowTitle = "Iron " IRON_API_VERSION_S;
    setWindowTitle( windowTitle );
    resize( 1000, 700 );

    // Create sketch
    //
    m_sketch = new Sketch( this );

    // Create TypeStore view
    //
    m_typeStore = new TypeStore( this );

    // Create central widget, layouts
    //
    QWidget* centralWidget = new QWidget( this );
    QVBoxLayout* vLayout = new QVBoxLayout( this );
    QSplitter* vSplitter = new QSplitter( Qt::Vertical );
    vSplitter->addWidget( m_sketch );
    vSplitter->addWidget( m_typeStore );
    vSplitter->setStretchFactor( 0, 2 );
    vLayout->addWidget( vSplitter );
    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );

    // Create actions
    //
    QAction* newAct = new QAction( tr("&New"), this );
    newAct->setShortcuts( QKeySequence::New );
    newAct->setStatusTip( tr( "Create a new file" )  );
    connect( newAct, SIGNAL( triggered() ), this, SLOT( newFile() ) );

    QAction* openAct = new QAction( tr("&Open"), this );
    openAct->setShortcuts( QKeySequence::Open );
    openAct->setStatusTip( tr( "Open a file" )  );
    connect( openAct, SIGNAL( triggered() ), this, SLOT( openFile() ) );

    QAction* saveAct = new QAction( tr( "&Save" ), this );
    saveAct->setShortcuts( QKeySequence::Save );
    saveAct->setStatusTip( tr( "Save a file" ) );
    connect( saveAct, SIGNAL( triggered() ), this, SLOT( saveFile() ) );

    QAction* closeAct = new QAction( tr( "&Close" ), this );
    closeAct->setShortcuts( QKeySequence::Close );
    closeAct->setStatusTip( tr( "Save a file" ) );
    connect( closeAct, SIGNAL( triggered() ), this, SLOT( closeFile() ) );

    QAction* quitAct = new QAction( tr( "&Quit" ), this );
    quitAct->setShortcuts( QKeySequence::Quit );
    quitAct->setStatusTip( tr( "Quit the application" ) );
    connect( quitAct, SIGNAL( triggered() ), qApp, SLOT( quit() ) );

    // Add menu items
    //
    m_fileMenu = menuBar()->addMenu( tr( "&File" ) );
    m_fileMenu->addAction( newAct );
    m_fileMenu->addAction( openAct );
    m_fileMenu->addAction( saveAct );
    m_fileMenu->addAction( closeAct );
    m_fileMenu->addSeparator();
    m_fileMenu->addAction( quitAct );

    // Set UI colors
    //
    qApp->setStyle( QStyleFactory::create( "plastique" ) );

    QPalette palette = QApplication::palette();
    palette.setColor( QPalette::Window,        QColor( 99,  96,  92  ) );
    palette.setColor( QPalette::WindowText,    QColor( 230, 230, 230 ) );
    palette.setColor( QPalette::Base,          QColor( 89,  86,  82  ) );
    palette.setColor( QPalette::AlternateBase, QColor( 110, 106, 101 ) );
    palette.setColor( QPalette::Text,          QColor( 230, 230, 230 ) );
    qApp->setPalette( palette );

    qApp->setStyleSheet(
                        "QMenuBar {"
                            "background-color: rgb(99,96,92);"
                            "color: rgb(255,255,255);"
                            "border: 0px solid rgb(0,0,0);"
                        "}"
                        "QMenuBar::item {"
                            "background-color: rgb(99,96,92);"
                            "color: rgb(255,255,255);"
                        "}"
                        "QMenuBar::item::selected {"
                            "background-color: rgb(64,62,59);"
                        "}"
                        "QMenu {"
                            "background-color: rgb(99,96,92);"
                            "color: rgb(230,230,230);"
                            "border: 1px solid rgb(0,0,0);"
                        "}"
                        "QMenu::item::selected {"
                            "background-color: rgb(64,62,59);"
                        "}"
                        "QScrollBar::horizontal {"
                            "background-color: transparent;"
                        "}"
                         "QScrollBar::vertical {"
                             "background-color: transparent;"
                         "}"
                        "QTabBar { "
                            "background-color: rgb(99,96,92); "
                        "}"
                        "QTabBar::tab { "
                            "background-color: rgb(128,123,117); "
                        "}"
                         "QTabBar::tab::selected { "
                             "background-color: rgb(153,148,141); "
                         "}"
                    );

}

//------------------------------------------------------------------------------
//

MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
//

bool MainWindow::newFile()
{
    MainWindow* other = new MainWindow;
    other->show();

    return true;
}

//------------------------------------------------------------------------------
//

bool MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName( this );

    if ( fileName.isEmpty() )
    {
        return false;
    }

    return read( fileName.toStdString(), m_sketch );
}

//------------------------------------------------------------------------------
//

bool MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName( this );

    if ( fileName.isEmpty() )
    {
        return false;
    }

    return write( m_sketch, fileName.toStdString() );
}

//------------------------------------------------------------------------------
//

bool MainWindow::closeFile()
{
    m_sketch->clear();

    return true;
}
