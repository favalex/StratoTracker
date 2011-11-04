#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QWidget>

class QLabel;
class QGridLayout;

class MetadataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MetadataWidget(const QStringList &fields, QWidget *parent = 0);

    void setFields(const QStringList &fields);

public slots:
    void setMetadata(const QStringList &metadata);

private:
    QGridLayout *metadata_layout;
    QList<QLabel *> key_list;
    QList<QLabel *> value_list;
};

#endif // METADATAWIDGET_H
