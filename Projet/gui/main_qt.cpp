#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "piocheitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Cascadia - Pioche");

    // Create graphics scene and view
    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsView* view = new QGraphicsView(scene);

    // Create and add pioche to scene
    PiocheItem* pioche = new PiocheItem;
    //pioche = std::make_unique<Pioche>(nbJoueur);
    scene->addItem(pioche);

    // Set up view
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::RubberBandDrag);

    window.setCentralWidget(view);
    window.resize(400, 300);
    window.show();

    return app.exec();
}
