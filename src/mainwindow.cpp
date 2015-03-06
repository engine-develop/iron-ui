//------------------------------------------------------------------------------
//

// Qt
#include <QPalette>
#include <QFileDialog>

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

    // Create graphics scene
    //
    scene = new GraphScene( this );

    // Create graphics view
    //
    view = new GraphView( scene, this );

    // Create graph and install graphics scene
    //
    graph = new Graph( this );
    graph->install( scene );

    // Create TypeStore view
    //
    typeStoreView = new TypeStoreView( this );

    // Create central widget, layouts
    //
    QWidget* centralWidget = new QWidget( this );
    QVBoxLayout* vLayout = new QVBoxLayout( this );
    QSplitter* vSplitter = new QSplitter( Qt::Vertical );
    vSplitter->addWidget( view );
    vSplitter->addWidget( typeStoreView );
    vSplitter->setStretchFactor( 0, 2 );
    vLayout->addWidget( vSplitter );
    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );

    // Create actions
    //
    QAction* loadAct = new QAction( tr("&Open"), this );
    loadAct->setShortcuts( QKeySequence::Open );
    loadAct->setStatusTip( tr( "Open a file" )  );
    connect( loadAct, SIGNAL( triggered() ), this, SLOT( openFile() ) );

    QAction* saveAct = new QAction( tr( "&Save" ), this );
    saveAct->setShortcuts( QKeySequence::Save );
    saveAct->setStatusTip( tr( "Save a file" ) );
    connect( saveAct, SIGNAL( triggered() ), this, SLOT( saveFile() ) );

    QAction* quitAct = new QAction( tr( "&Quit" ), this );
    quitAct->setShortcuts( QKeySequence::Quit );
    quitAct->setStatusTip( tr( "Quit the application" ) );
    connect( quitAct, SIGNAL( triggered() ), qApp, SLOT( quit() ) );

    // Add menu items
    //
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( loadAct );
    fileMenu->addAction( saveAct );
    fileMenu->addSeparator();
    fileMenu->addAction( quitAct );

    setColors();
}

//------------------------------------------------------------------------------
//

MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
//

void MainWindow::setColors()
{
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

void MainWindow::saveFile()
{
	QString fname = QFileDialog::getSaveFileName();

    if ( fname.isEmpty() )
    {
		return;
    }

    QFile f( fname );
    f.open( QFile::WriteOnly );
    QDataStream ds( &f );
    graph->save( ds );
}

//------------------------------------------------------------------------------
//

void MainWindow::openFile()
{
	QString fname = QFileDialog::getOpenFileName();

    if ( fname.isEmpty() )
    {
		return;
    }

    QFile f( fname );
    f.open( QFile::ReadOnly );
    QDataStream ds( &f );
    graph->load( ds );
}
