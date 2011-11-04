#include "marker.h"

#include <QPixmapCache>
#include <QPainter>


Marker::Marker(const QTime &time, qreal lat, qreal lon, const QStringList &md)
    : t(time), data(md)
{
    setCoordinate(QGeoCoordinate(lat, lon));

    QPixmap pixmap(13, 13);
    if (!QPixmapCache::find("marker", &pixmap))
    {
        // TODO: substitude with something decent.
        pixmap.fill(Qt::transparent);

        QPainter p(&pixmap);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::blue);
        p.setBrush(Qt::blue);
        p.drawEllipse(pixmap.rect());

        QPixmapCache::insert("marker", pixmap);
    }
    setPixmap(pixmap);
    setOffset(QPoint(-6, -6));
}

QTime Marker::time() const
{
    return t;
}

QStringList Marker::metadata() const
{
    return data;
}
