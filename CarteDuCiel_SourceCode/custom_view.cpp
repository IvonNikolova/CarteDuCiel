#include "custom_view.h"

custom_view::custom_view(QWidget *parent)
    : QGraphicsView(parent) {
}

// Sources: https://www.youtube.com/watch?v=4dq7n8S9AHw -- the template
// Sources: https://www.youtube.com/watch?v=7LOykGfklY0&t=408s
void custom_view::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.05; // how much it will zoom each time the wheel's rotated

    if (event->angleDelta().y() > 0) // Sources: https://stackoverflow.com/questions/66268136/how-to-replace-the-deprecated-function-qwheeleventdelta-in-the-zoom-in-z
    {
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        scale(1/scaleFactor, 1/scaleFactor); //the scaleFactor should be different from 1, otherwise, we'll not be able to zoom with the mouse's wheel, because 1/1 is 1 i.e. no change
    }
} // wheelEvent
