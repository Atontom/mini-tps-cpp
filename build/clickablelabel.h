#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#pragma once
#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = nullptr);
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void mouseMoved(int x,int y);

};

#endif // CLICKABLELABEL_H
