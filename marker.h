#ifndef MARKER_H
#define MARKER_H

#include <QtLocation/QGeoMapPixmapObject>

#include <QGeoMapPixmapObject>
#include <QTime>
#include <QStringList>


class BalloonLogData;


QTM_USE_NAMESPACE


class Marker : public QGeoMapPixmapObject
{
    Q_OBJECT

public:
    Marker(const QTime &time, qreal lat, qreal lon, const QStringList &metadata);

    QTime time() const;
    QStringList metadata() const;

private:
    QTime t;
    QStringList data;
};

#endif // MARKER_H
