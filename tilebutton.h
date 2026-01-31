#pragma once

#include <QToolButton>
#include <QMouseEvent>

class TileButton : public QToolButton
{
    Q_OBJECT

public:
    explicit TileButton(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void tileClicked(Qt::MouseButton button);
};
