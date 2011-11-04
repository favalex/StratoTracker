#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QPlainTextEdit>

class QTime;


class StatusWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent = 0);

public slots:
    void addMessage(const QTime &time, const QString &message);

};

#endif // STATUSWIDGET_H
