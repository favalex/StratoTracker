#include "directorymonitor.h"

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QRegExp>
#include <QTime>

#include <QDebug>

namespace
{
    qreal degMinTodec(int degrees, qreal minutes)
    {
        return degrees + minutes / 60.0;
    }

    qreal convertLatitude(const QString &latitude, const QString &NS)
    {
        int degrees = latitude.left(2).toInt();
        qreal minutes = latitude.right(5).toDouble();

        return NS == "S" ? -degMinTodec(degrees, minutes) : degMinTodec(degrees, minutes);
    }

    qreal convertLongitude(const QString &longitude, const QString &WE)
    {
        int degrees = longitude.left(3).toInt();
        qreal minutes = longitude.right(5).toDouble();

        return WE == "W" ? -degMinTodec(degrees, minutes) : degMinTodec(degrees, minutes);
    }
}


DirectoryMonitor::DirectoryMonitor(QObject *parent) :
    QObject(parent), watcher(new QFileSystemWatcher(this))
{
    connect(watcher, SIGNAL(directoryChanged(QString)), SLOT(manageDirectoryChanged(QString)));
}

bool DirectoryMonitor::setDirectory(const QString &path)
{
    removeDirectory(current_dir);
    addDirectory(path);
}

bool DirectoryMonitor::addDirectory(const QString &path)
{
    qDebug() << "Adding directory" << path;

    QFileInfo info(path);
    if (!info.isDir())
        return false;

    current_dir = info.absoluteFilePath();

    manageDirectoryChanged(current_dir);
    watcher->addPath(current_dir);

    return true;
}

bool DirectoryMonitor::removeDirectory(const QString &path)
{
    if (!watcher->directories().contains(path))
        return false;

    watcher->removePath(path);
    return true;
}

QStringList DirectoryMonitor::watchedDirectories() const
{
    return watcher->directories();
}

// FIXME: don't return on all errors
void DirectoryMonitor::manageDirectoryChanged(const QString &path)
{
    QDir dir(path);
    dir.setSorting(QDir::Name);

    foreach (const QFileInfo &info, dir.entryInfoList(QDir::Files, QDir::Name))
    {
        QString path = info.absoluteFilePath();
        QFile file(path);

        if (!logfiles.contains(path) && file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Processing file" << path;

            const QByteArray &data = file.readAll();
            if (data.isEmpty())
                continue;

            switch (data.at(0))
            {
            case '/':
            {
                QRegExp regexp("^/(\\d{6})h(\\d{4}\\.\\d{2})(\\w)/(\\d{5}\\.\\d{2})(\\w)>(.*)");
                if (regexp.indexIn(data) == -1) {
                    qWarning() << "Malformed '/' packet" << data;
                    continue;
                }

                QTime time = QTime::fromString(regexp.cap(1), "HHmmss");
                qreal latitude = convertLatitude(regexp.cap(2), regexp.cap(3));
                qreal longitude = convertLongitude(regexp.cap(4), regexp.cap(5));
                QStringList metadata = regexp.cap(6).split(';');

                if (latitude == 0 && longitude == 0)
                    continue;

                emit telemethryReceived(time, latitude, longitude, metadata);
            }
            break;
            case '>':
            {
                QRegExp regexp("^>(\\d{6})h(\\D+)");
                if (regexp.indexIn(data) == -1) {
                    qWarning() << "Malformed '>' packet" << data;
                    continue;
                }

                QTime time = QTime::fromString(regexp.cap(1), "HHmmss");
                QString message = regexp.cap(2);

                emit statusReceived(time, message);
            }
            break;
            default:
                qWarning() << "Unknown packet starting with '" << data.at(0) << "'";
                continue;
            }

            logfiles << path;
        }
    }
}
