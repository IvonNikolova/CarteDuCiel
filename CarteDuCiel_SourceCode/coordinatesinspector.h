// CoordViewer.h

#ifndef COORDINATESINSPECTOR_H
#define COORDINATESINSPECTOR_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>


class CoordinatesInspector : public QGraphicsView
{
    public:
        CoordinatesInspector(QWidget* parent = nullptr, const QImage &image= QImage()) : QGraphicsView(parent)
        {
            // Create the scene and set it to the view
            m_scene = new QGraphicsScene(this);
            setScene(m_scene);

            m_imageItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
            m_scene->addItem(m_imageItem);

            // Add the coordinate text item to the scene
            m_coordItem = new QGraphicsTextItem();
            m_coordItem->setHtml("<font size='5'>x: 0, y: 0</font>");
            m_scene->addItem(m_coordItem);

            setupScene();
            setSceneRect(0, 0, image.width(), image.height());
        }
        void setupScene();
        QPointF mapToScene(const QPoint& point) const;

    protected:
        void mouseMoveEvent(QMouseEvent* event) override;

    private:
        QGraphicsPixmapItem* m_imageItem;
        QGraphicsTextItem* m_coordItem;
        QGraphicsScene* m_scene;

        void wheelEvent(QWheelEvent* event) override;
        void scrollContentsBy(int dx, int dy) override;
        void scaleBy(double scaleFactor);
        void zoomIn();
        void zoomOut();
};



#endif // COORDINATESINSPECTOR_H
