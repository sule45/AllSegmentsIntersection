#ifndef OBLASTCRTANJA_H
#define OBLASTCRTANJA_H

#include <QWidget>
#include <QPainter>

class OblastCrtanja : public QWidget
{
    Q_OBJECT
public:
    explicit OblastCrtanja(QWidget *parent = 0);

signals:

public slots:


protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

#endif // OBLASTCRTANJA_H
