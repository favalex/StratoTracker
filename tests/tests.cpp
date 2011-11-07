#include <QtTest/QtTest>

class Tests: public QObject
{
    Q_OBJECT

private slots:
    void testPacketParsing();
};

void Tests::testPacketParsing()
{
}

QTEST_MAIN(Tests)
#include "tests.moc"
