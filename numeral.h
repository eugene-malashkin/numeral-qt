#ifndef NUMERAL_H
#define NUMERAL_H

#include <QString>
#include <QLocale>

class NumeralLocale
{
public:
    NumeralLocale();
    NumeralLocale(const QLocale &locale);
    NumeralLocale(const QLocale &locale, const QString &groupSeparator);
    NumeralLocale(const NumeralLocale &another);
    void setLocale(const QLocale &value);
    QLocale locale() const;
    void setGroupSeparator(const QString &value);
    QString groupSeparator() const;

private:
    QLocale m_locale;
    QString m_groupSeparator;
};

class Numeral
{
public:
    Numeral();
    Numeral(const QString &st);
    Numeral(bool sign, bool thousandSeparate, int precision, bool extraPrecision, bool percent);
    bool operator == (const Numeral &another);
    bool operator != (const Numeral &another);
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
    QString toString(double number, const QString &nanStub, const NumeralLocale &numeralLocale) const;
    QString toString(double number, const QString &nanStub) const;
    QString toString(double number) const;
    static QString format(double number, const QString &formatString, const QString &nanStub, const NumeralLocale &numeralLocale);
    static QString format(double number, const QString &formatString, const QString &nanStub);
    static QString format(double number, const QString &formatString);
    static QString format(double number);
    static void setDefaultNumeralLocale(const NumeralLocale &value);
    static NumeralLocale defaultNumeralLocale();
    static void setDefaultNanStub(const QString &value);
    static QString defaultNanStub();

private:
    bool m_sign;
    bool m_thousandSeparate;
    int m_precision;
    bool m_extraPrecision;
    bool m_percent;
    static NumeralLocale *m_defaultNumeralLocale;
    static QString *m_defaultNanStub;
    void parse(const QStringRef &st);
    void parseIntegerPart(const QStringRef &st);
    void parseFractionalPart(const QStringRef &st);
    QString decorateSign(const QString &formattedNumber, double number, const NumeralLocale &numeralLocale) const;
    QString decorateThousandSeparator(const QString &formattedNumber, const NumeralLocale &numeralLocale) const;
    QString decorateTrimmingZeros(const QString &formattedNumber, const NumeralLocale &numeralLocale) const;
    QString initialFormat(double positiveNumber, const NumeralLocale &numeralLocale) const;
    static void createDefaultNumeralLocaleIfNeeded();
    static void createDefaultNanStubIfNeeded();
};

#endif // NUMERAL_H
