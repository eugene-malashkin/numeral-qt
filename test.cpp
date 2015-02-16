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
    void testStorying();
    void testDoing();
    void testNumeralLocale();

private:
    void testNumeralFormat(
            const NumeralFormat &numeral,
            bool sign,
            bool thousandSeparate,
            int precision,
            bool extraPrecision,
            bool percent,
            const QString &caseDescription);
    QString bts(bool b);
};

Test::Test()
{
    NumeralFormat::setDefaultNumeralLocale(QLocale(QLocale::C));
    NumeralFormat::setDefaultNanStub("n\\a");
}

void Test::testStorying()
{
    testNumeralFormat(NumeralFormat(), false, true, 0, true, false, "Numeral format default");
    testNumeralFormat(NumeralFormat("0,0.*"), false, true, 0, true, false, "Numeral format 0,0.*");
    testNumeralFormat(NumeralFormat("+0,0.000*%"), true, true, 3, true, true, "Numeral format +0,0.000*%");
    testNumeralFormat(NumeralFormat("+0.0"), true, false, 1, false, false, "Numeral format +0.0");
    testNumeralFormat(NumeralFormat("0.00%"), false, false, 2, false, true, "Numeral format 0.00%");
}

void Test::testDoing()
{
    QVERIFY(NumeralFormat::format(1234.5678) == "1,234.5678");
    QVERIFY(NumeralFormat::format(1234.5678, "0,0") == "1,235");
    QVERIFY(NumeralFormat::format(1.2345, "+0.00%") == "+123.45%");
    QVERIFY(NumeralFormat::format(12345.678, "0,0.*") == "12,345.678");
    QVERIFY(NumeralFormat::format(12345.678, "0,0.0*") == "12,345.678");
    QVERIFY(NumeralFormat::format(12345.678, "0,0.0000*") == "12,345.6780");
    QVERIFY(NumeralFormat::format(12345.678, "0.0000*") == "12345.6780");
    QVERIFY(NumeralFormat::format(qQNaN(), "0.0", "-") == "-");
}

void Test::testNumeralLocale()
{
    NumeralLocale nl(QLocale::C, " ");
    QVERIFY(NumeralFormat::format(1234.5678, "0,0.*", "-", nl) == "1 234.5678");

    NumeralFormat::setDefaultNumeralLocale(NumeralLocale(QLocale::C, " "));
    QVERIFY(NumeralFormat::format(1234.5678) == "1 234.5678");
}

void Test::testNumeralFormat(const NumeralFormat &numeral,
        bool sign,
        bool thousandSeparate,
        int precision,
        bool extraPrecision,
        bool percent,
        const QString &caseDescription)
{
    QVERIFY2(numeral.sign() == sign, QString("%1. Sign must be %2").arg(caseDescription).arg(bts(sign)).toUtf8());
    QVERIFY2(numeral.thousandSeparate() == thousandSeparate, QString("%1. Thousand separate must be %2").arg(caseDescription).arg(bts(thousandSeparate)).toUtf8());
    QVERIFY2(numeral.extraPrecision() == extraPrecision, QString("%1. Extra precision must be %2").arg(caseDescription).arg(bts(extraPrecision)).toUtf8());
    QVERIFY2(numeral.precision() == precision, QString("%1. Precision must be %2").arg(caseDescription).arg(precision).toUtf8());
    QVERIFY2(numeral.percent() == percent, QString("%1. Percent must be %2").arg(caseDescription).arg(bts(percent)).toUtf8());
}

QString Test::bts(bool b)
{
    return (b)? "true" : "false";
}

QTEST_MAIN(Test)

#include "test.moc"
