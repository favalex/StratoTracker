#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>

class Map;
class BalloonLogData;
class Marker;

namespace QtMobility
{
    class QGeoMapPolylineObject;
}


class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = 0);

    void resizeEvent(QResizeEvent *event);

public slots:
    void zoomIn();
    void zoomOut();
    void addData(const QTime &time, qreal lat, qreal lon, const QStringList &metadata);

signals:
    void markerClicked(const QStringList &metadata);

private:
    Map *geo_map;
    QtMobility::QGeoMapPolylineObject *track;
    QList<const Marker *> markers;
};

#endif // MAPVIEW_H
