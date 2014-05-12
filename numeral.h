#ifndef NUMERAL_H
#define NUMERAL_H

#include <QString>
#include <QLocale>

class NumeralFormat
{
public:
    NumeralFormat();
    NumeralFormat(const QString &st);
    NumeralFormat(bool sign, bool thousandSeparate, int precision, bool extraPrecision, bool percent);
    bool operator == (const NumeralFormat &another);
    bool operator != (const NumeralFormat &another);
    void clear();
    void setFormatString(const QString &st);
    QString formatString() const;
    void setSign(bool value);
    bool sign() const;
    void setThousandSeparate(bool value);
    bool thousandSeparate() const;
    void setPrecision(int value);
    int precision() const;
    void setExtraPrecision(bool value);
    bool extraPrecision() const;
    void setPercent(bool value);
    bool percent() const;

private:
    bool m_sign;
    bool m_thousandSeparate;
    int m_precision;
    bool m_extraPrecision;
    bool m_percent;
    void parse(const QStringRef &st);
    void parseIntegerPart(const QStringRef &st);
    void parseFractionalPart(const QStringRef &st);
};

class Numeral
{
public:
    Numeral();
    Numeral(const NumeralFormat &numeralFormat);
    Numeral(const NumeralFormat &numeralFormat, const QLocale &locale);
    void setNumeralFormat(const NumeralFormat &value);
    NumeralFormat numeralFormat() const;
    void setlocale(const QLocale &value);
    QLocale locale() const;
    void setNanStub(const QString &value);
    QString nanStub() const;
    static void setDefaultLocale(const QLocale &value);
    static QLocale defaultLocale();
    static void setDefaultNanStub(const QString &value);
    static QString defaultNanStub();
    QString toString(double number) const;
    static QString format(double number, const NumeralFormat &numeralFormat = NumeralFormat());

private:
    NumeralFormat m_numeralFormat;
    QLocale m_locale;
    QString m_nanStub;
    static QLocale *m_defaultLocale;
    static QString *m_defaultNanStub;

public:
    QString decorateSign(const QString &formattedNumber, double number) const;
    QString decorateThousandSeparator(const QString &formattedNumber) const;
    QString decorateTrimmingZeros(const QString &formattedNumber) const;
    QString initialFormat(double positiveNumber) const;
    static void createDefaultLocaleIfNeeded();
    static void createDefaultNanStubIfNeeded();
};


#endif // NUMERAL_H
