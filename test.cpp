#include <QString>
#include <QtTest>
#include <QCoreApplication>

class Test : public QObject
{
    Q_OBJECT

public:
    Test();

private Q_SLOTS:
    void simpleCase1();
};

Test::Test()
{
}

void Test::simpleCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(Test)

#include "test.moc"
