#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "numeral.h"

using namespace NumeralQt;

class Test : public QObject
{
    Q_OBJECT

public:
    Test();

private slots:
    void testStorying_data();
    void testStorying();
    void testFormat_data();
    void testFormat();
    void testDifferentTypes();
    void testNumeralLocale();
};

Test::Test()
{
    NumeralFormat::setDefaultNumeralLocale(QLocale(QLocale::C));
    NumeralFormat::setDefaultNanStub("n\\a");
}

void Test::testStorying_data()
{
    QTest::addColumn<QString>("st");
    QTest::addColumn<bool>("sign");
    QTest::addColumn<bool>("thousandSeparate");
    QTest::addColumn<int>("minPrecision");
    QTest::addColumn<int>("maxPrecision");
    QTest::addColumn<bool>("percent");

    QTest::newRow("1") << "" << false << true << 0 << 6 << false;
    QTest::newRow("2") << "0,0.*" << false << true << 0 << 1 << false;
    QTest::newRow("3") << "0,0.000****" << false << true << 3 << 7 << false;
    QTest::newRow("4") << "+0,0.000*%" << true << true << 3 << 4 << true;
    QTest::newRow("5") << "+0.0" << true << false << 1 << 1 << false;
    QTest::newRow("6") << "0.00%" << false << false << 2 << 2 << true;
}

void Test::testStorying()
{
    QFETCH(QString, st);
    QFETCH(bool, sign);
    QFETCH(bool, thousandSeparate);
    QFETCH(int, minPrecision);
    QFETCH(int, maxPrecision);
    QFETCH(bool, percent);
    NumeralFormat l(st);
    NumeralFormat r(sign, thousandSeparate, minPrecision, maxPrecision, percent);
    QCOMPARE(l, r);
    if (st.isEmpty())
    {
        // Test also default constructor. Thus, NumeralFormat() and NumeralFormat("") should be the same.
        QCOMPARE(NumeralFormat(st), NumeralFormat(sign, thousandSeparate, minPrecision, maxPrecision, percent));
    }
}

void Test::testFormat_data()
{
    QTest::addColumn<double>("number");
    QTest::addColumn<QString>("st");
    QTest::addColumn<QString>("result");

    QTest::newRow("1") << 1234.5678 << QString() << "1,234.5678";
    QTest::newRow("2") << 1234.5678 << "0,0" << "1,235";
    QTest::newRow("3") << 1.2345 << "+0.00%" << "+123.45%";
    QTest::newRow("4") << 12345.678 << "0,0.*" << "12,345.7";
    QTest::newRow("5") << 12345.678 << "0,0.0*" << "12,345.68";
    QTest::newRow("6") << 12345.678 << "0,0.0**" << "12,345.678";
    QTest::newRow("7") << 12345.6789 << "0,0.0**" << "12,345.679";
    QTest::newRow("8") << 12345.678 << "0,0.0000*" << "12,345.6780";
    QTest::newRow("9") << 12345.678 << "0.0000*" << "12345.6780";
    QTest::newRow("10") << qQNaN() << "0.0" << "n\\a";
    QTest::newRow("11") << 1.2345678 << "0.00****" << "1.234568";
    QTest::newRow("12") << 1.2345 << "0.00****" << "1.2345";
    QTest::newRow("13") << 1.2 << "0.00****" << "1.20";
}

void Test::testFormat()
{
    QFETCH(double, number);
    QFETCH(QString, st);
    QFETCH(QString, result);
    QCOMPARE(NumeralFormat::format(number, st), result);
}

void Test::testDifferentTypes()
{
    QVERIFY(NumeralFormat::format(static_cast<float>(1.222222222222222222), "0.000000000000000000000") == "1.222222");
    QVERIFY(NumeralFormat::format(static_cast<double>(1.222222222222222222), "0.000000000000000000000") == "1.222222222222222");
}

void Test::testNumeralLocale()
{
    NumeralLocale nl(QLocale::C, " ");
    QVERIFY(NumeralFormat::format(1234.5678, "0,0.*", "-", nl) == "1 234.6");

    NumeralFormat::setDefaultNumeralLocale(NumeralLocale(QLocale::C, " "));
    QVERIFY(NumeralFormat::format(1234.5678) == "1 234.5678");
}


QTEST_MAIN(Test)

#include "test.moc"
