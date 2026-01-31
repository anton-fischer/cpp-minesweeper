#include "tilebutton.h"

TileButton::TileButton(QWidget* parent)
    : QToolButton(parent)
{
}

void TileButton::mousePressEvent(QMouseEvent* event) {
    emit tileClicked(event->button());
}
