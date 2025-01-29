#ifndef CUSTOM_VIEW_H
#define CUSTOM_VIEW_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QWidget>

class custom_view : public QGraphicsView /* imageView needs to inherit a QGraphicsView */
{
    public:
        custom_view(QWidget *parent = nullptr); /* QGraphicsView requires a parant as a constructor */
    protected:
        virtual void wheelEvent(QWheelEvent *event);
};

#endif // CUSTOM_VIEW_H










