#include "mapview.h"

#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGraphicsGeoMap>
#include <QtLocation/QGeoMappingManager>
#include <QtLocation/QGeoCoordinate>
#include <QtLocation/QGeoMapPolylineObject>

#include "map.h"
#include "marker.h"

#include <QDebug>

QTM_USE_NAMESPACE

namespace
{
    bool markerLessThan(const Marker *m1, const Marker *m2)
    {
        return m1->time() < m2->time();
    }
}


MapView::MapView(QWidget *parent) :
    QGraphicsView(parent)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(true);

    QGeoServiceProvider *serviceProvider = new QGeoServiceProvider("nokia");
    QGeoMappingManager *manager = serviceProvider->mappingManager();

    geo_map = new Map(manager);
    connect(geo_map, SIGNAL(markerClicked(QStringList)), SIGNAL(markerClicked(QStringList)));
    geo_map->setMapType(QGraphicsGeoMap::StreetMap);
    geo_map->setCenter(QGeoCoordinate(0.0, 0.0));
    geo_map->setZoomLevel(5);
    scene->addItem(geo_map);

    track = new QGeoMapPolylineObject;
    QPen pen(Qt::red);
    pen.setWidth(3);
    track->setPen(pen);
    track->setZValue(1);
    setMouseTracking(true);
}

void MapView::resizeEvent(QResizeEvent *event)
{
    if (geo_map)
        geo_map->resize(event->size());
}

void MapView::zoomIn()
{
    geo_map->setZoomLevel(geo_map->zoomLevel() + 1);
}

void MapView::zoomOut()
{
    geo_map->setZoomLevel(geo_map->zoomLevel() - 1);
}

void MapView::addData(const QTime &time, qreal lat, qreal lon, const QStringList &metadata)
{
    // Add marker
    Marker *marker = new Marker(time, lat, lon, metadata);
    marker->setZValue(2);

    markers.append(marker);
    qSort(markers.begin(), markers.end(), markerLessThan);

    geo_map->addMapObject(marker);
    geo_map->setCenter(marker->coordinate());
    QStringList md = marker->metadata();
    md.prepend(marker->time().toString());
    emit markerClicked(md);

    // Add point to track
    QList<QGeoCoordinate> path;
    foreach (const Marker *marker, markers)
        path.append(marker->coordinate());

    track->setPath(path);

    // QGeoMapPolylineObject cannot be added to map if it's empty
    if (!path.isEmpty() && !geo_map->mapObjects().contains(track))
        geo_map->addMapObject(track);
}
