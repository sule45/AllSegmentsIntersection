#include "oblastcrtanja.h"

OblastCrtanja::OblastCrtanja(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);

    setAutoFillBackground(true);
    setPalette(Pal);

}

void OblastCrtanja::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

}
