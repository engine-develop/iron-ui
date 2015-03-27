//------------------------------------------------------------------------------
//

#include <QtWidgets>
#include <QApplication>
#include "mainwindow.hpp"

//------------------------------------------------------------------------------
//

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );

    MainWindow w;
    w.show();

//    QGraphicsScene scene;
//    QGraphicsView view(&scene);
//    QGraphicsTextItem* item  = new QGraphicsTextItem("QGraphicsTextItem Text Color");
//    item->setDefaultTextColor(Qt::black);
//    item ->setTextInteractionFlags(Qt::TextEditorInteraction);
//    scene.addItem(item);
//    view.setFixedSize(250,250);
//    view.setWindowTitle("QGraphicsTextItem Colorize and Rotate Example");
//    view.show();

    return a.exec();
}
