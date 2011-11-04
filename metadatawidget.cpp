#include "metadatawidget.h"

#include <QGridLayout>
#include <QLabel>

MetadataWidget::MetadataWidget(const QStringList &fields, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *main_layout = new QVBoxLayout;
    metadata_layout = new QGridLayout;
    main_layout->addLayout(metadata_layout);
    main_layout->addStretch();

    setLayout(main_layout);
    setFields(fields);
}

void MetadataWidget::setFields(const QStringList &fields)
{
    qDeleteAll(key_list);
    qDeleteAll(value_list);

    for (int i = 0; i < fields.size(); ++i)
    {
        QLabel *key = new QLabel("<b>" + fields.at(i) + "</b>");
        key_list.append(key);
        metadata_layout->addWidget(key, i, 0);

        QLabel *value = new QLabel;
        value->setAlignment(Qt::AlignRight);
        value_list.append(value);
        metadata_layout->addWidget(value, i, 1);
    }
}

void MetadataWidget::setMetadata(const QStringList &metadata)
{
    Q_ASSERT_X(metadata.size() == value_list.size(), "MetadataWidget::setMetadata", "metadata and fields mismatch");

    for (int i = 0; i < metadata.size(); ++i)
        value_list.at(i)->setText(metadata.at(i));
}
