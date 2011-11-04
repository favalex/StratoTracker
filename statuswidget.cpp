#include "statuswidget.h"

#include <QTime>
#include <QFont>


StatusWidget::StatusWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setFont(QFont("Monospace"));
    setReadOnly(true);
    setCenterOnScroll(true);
}

void StatusWidget::addMessage(const QTime &time, const QString &message)
{
    QString text = QString("%1: %2").arg(time.toString()).arg(message);

    appendPlainText(text);
}
