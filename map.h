#ifndef MAP_H
#define MAP_H

#include <QGraphicsGeoMap>
#include <QPoint>

class Map : public QtMobility::QGraphicsGeoMap
{
    Q_OBJECT

public:
    explicit Map(QtMobility::QGeoMappingManager *manager);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

signals:
    void markerClicked(const QStringList &metadata);

private:
    bool pressed;
    QPoint last_pos;
};

#endif // MAP_H
