#include "coordinatesinspector.h"

void CoordinatesInspector::setupScene()
{
    // Set the mouse tracking to true so that the mouseMoveEvent is triggered
    setMouseTracking(true);

    // Set the minimum and maximum zoom levels
    setRenderHint(QPainter::SmoothPixmapTransform);

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setMinimumSize(600, 600);
    setMinimumSize(4000, 4000);

    // Set the alignment to top-left
      setAlignment(Qt::AlignTop | Qt::AlignLeft);
};


QPointF CoordinatesInspector::mapToScene(const QPoint& point) const
{
    return QGraphicsView::mapToScene(point);
}

void CoordinatesInspector::mouseMoveEvent(QMouseEvent* event)
{
    // Call the base class implementation
    QGraphicsView::mouseMoveEvent(event);

    // Get the cursor position in scene coordinates
    QPointF cursorPos = mapToScene(event->pos());

    // Check if the cursor is within the image bounds
    if (m_imageItem->contains(cursorPos))
    {

        // Get the pixel color at the cursor position
        QRgb pixelColor = m_imageItem->pixmap().toImage().pixel(cursorPos.toPoint());

        // Extract the RGB values
        int red = qRed(pixelColor);
        int green = qGreen(pixelColor);
        int blue = qBlue(pixelColor);

        // Update the coordinate text item with the cursor position and pixel color
        QString coordText = QString("x: %1, y: %2") //\nColor: %3, %4, %5")
                               .arg(cursorPos.x())
                               .arg(cursorPos.y());
//                                   .arg(red)
//                                   .arg(green)
//                                   .arg(blue);
        m_coordItem->setHtml("<font size='5'>" + coordText + "</font>");

        // Position the coordinate text item to the cursor position in scene coordinates
        m_coordItem->setPos(cursorPos + QPointF(10, 10));

        // Adjust the font size based on the current zoom level
        qreal fontPointSize = 10.0 / transform().m11(); // 10 is the default font size
        QFont font = m_coordItem->font();
        font.setPointSizeF(fontPointSize);
        m_coordItem->setFont(font);
    }

}

void CoordinatesInspector::wheelEvent(QWheelEvent* event)
{
    // Calculate the scale factor based on the mouse wheel delta
    double scaleFactor = pow(2.0, event->angleDelta().y() / 240.0);

    // Scale the view by the calculated factor
    scaleBy(scaleFactor);

      QGraphicsView::wheelEvent(event);
}

void CoordinatesInspector::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx, dy);

    // Adjust the position of the coordinate label when the image is scrolled
    QPointF coordPos = m_coordItem->pos();
    coordPos -= QPointF(dx, dy);
    m_coordItem->setPos(coordPos);

    QGraphicsView::scrollContentsBy(dx, dy);
}

void CoordinatesInspector::scaleBy(double scaleFactor)
{
    // Calculate the new scale factor based on the current scale and the desired factor
    double newScale = transform().m11() * scaleFactor;

    // Check if the new scale is within the acceptable range
    if (newScale > 0.05 && newScale < 10)
    {
        // Scale the view by the calculated factor
        scale(scaleFactor, scaleFactor);
    }
}

void CoordinatesInspector::zoomIn()
{
    scaleBy(1.2);
}

void CoordinatesInspector::zoomOut()
{
    scaleBy(1 / 1.2);
}

