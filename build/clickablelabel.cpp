#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent)
{
    setMouseTracking(true);

}
void ClickableLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event->pos().x(), event->pos().y());
}
