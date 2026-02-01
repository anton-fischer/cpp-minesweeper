#include "tilebutton.h"

TileButton::TileButton(QWidget* parent)
    : QToolButton(parent)
{
    this->setMinimumSize(32, 32);
    this->setToolButtonStyle(Qt::ToolButtonTextOnly);
    this->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);

    QFont f = this->font();
    f.setPointSize(13);
    this->setFont(f);
}

void TileButton::mousePressEvent(QMouseEvent* event) {
    emit tileClicked(event->button());
}
