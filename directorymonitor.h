#ifndef DIRECTORYMONITOR_H
#define DIRECTORYMONITOR_H

#include <QObject>
#include <QSet>

class QFileSystemWatcher;
class QTime;

class DirectoryMonitor : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryMonitor(QObject *parent = 0);

    bool setDirectory(const QString &path);

    bool addDirectory(const QString &path);
    bool removeDirectory(const QString &path);

    QStringList watchedDirectories() const;

signals:
    void telemethryReceived(const QTime &time, qreal lat, qreal lon, const QStringList &metadata);
    void statusReceived(const QTime &time, const QString &message);

private slots:
    void manageDirectoryChanged(const QString &path);

private:
    QString current_dir;
    QFileSystemWatcher *watcher;
    QSet<QString> logfiles;
};

#endif // DIRECTORYMONITOR_H
