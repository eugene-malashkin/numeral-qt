#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "numeral.h"

class Test : public QObject
{
    Q_OBJECT

public:
    Test();

private slots:
    void numeralFormatFromStrings();

private:
    void testNumeralFormat(
            const NumeralFormat &numeralFormat,
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
}

void Test::numeralFormatFromStrings()
{
    testNumeralFormat(NumeralFormat(), false, true, 0, true, false, "Numeral format default");
    testNumeralFormat(NumeralFormat("0,0.*"), false, true, 0, true, false, "Numeral format 0,0.*");
    testNumeralFormat(NumeralFormat("+0,0.000*%"), true, true, 3, true, true, "Numeral format +0,0.000*%");
    testNumeralFormat(NumeralFormat("+0.0"), true, false, 1, false, false, "Numeral format +0.0");
    testNumeralFormat(NumeralFormat("0.00%"), false, false, 2, false, true, "Numeral format 0.00%");
}

void Test::testNumeralFormat(
        const NumeralFormat &numeralFormat,
        bool sign,
        bool thousandSeparate,
        int precision,
        bool extraPrecision,
        bool percent,
        const QString &caseDescription)
{
    QVERIFY2(numeralFormat.sign() == sign, QString("%1. Sign must be %2").arg(caseDescription).arg(bts(sign)).toUtf8());
    QVERIFY2(numeralFormat.thousandSeparate() == thousandSeparate, QString("%1. Thousand separate must be %2").arg(caseDescription).arg(bts(thousandSeparate)).toUtf8());
    QVERIFY2(numeralFormat.extraPrecision() == extraPrecision, QString("%1. Extra precision must be %2").arg(caseDescription).arg(bts(extraPrecision)).toUtf8());
    QVERIFY2(numeralFormat.precision() == precision, QString("%1. Precision must be %2").arg(caseDescription).arg(precision).toUtf8());
    QVERIFY2(numeralFormat.percent() == percent, QString("%1. Percent must be %2").arg(caseDescription).arg(bts(percent)).toUtf8());
}

QString Test::bts(bool b)
{
    return (b)? "true" : "false";
}

QTEST_MAIN(Test)

#include "test.moc"
