#include "map.h"

#include <QGraphicsSceneEvent>
#include <QCursor>

#include "marker.h"

#include <QDebug>

QTM_USE_NAMESPACE

Map::Map(QGeoMappingManager *manager) :
    QGraphicsGeoMap(manager), pressed(false)
{
    setCursor(Qt::OpenHandCursor);
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGeoMapObject *> objects = mapObjectsAtScreenPosition(event->pos());
    if (!objects.isEmpty())
    {
        Marker *marker = qobject_cast<Marker *>(objects.at(0));
        if (marker)
        {
            QStringList metadata = marker->metadata();
            metadata.prepend(marker->time().toString());
            emit markerClicked(metadata);
        }
    }

    pressed = true;
    last_pos = event->screenPos();
    setCursor(Qt::ClosedHandCursor);
}

void Map::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (pressed) {
        QPoint delta = last_pos;
        delta -= event->screenPos();
        pan(delta.x(), delta.y());
        last_pos = event->screenPos();
    }
}

void Map::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    pressed = false;
    setCursor(Qt::OpenHandCursor);
}

void Map::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    setZoomLevel(zoomLevel() + numSteps);

    event->accept();
}
