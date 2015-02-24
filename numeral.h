#ifndef NUMERAL_H
#define NUMERAL_H

#include <QString>
#include <QLocale>

namespace NumeralQt
{
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

class NumeralFormat
{
public:
    NumeralFormat();
    NumeralFormat(const QString &st);
    NumeralFormat(bool sign, bool thousandSeparate, int minPrecision, int maxPrecision, bool percent);
    bool operator == (const NumeralFormat &another) const;
    bool operator != (const NumeralFormat &another) const;
    void clear();
    void setFormatString(const QString &st);
    QString formatString() const;
    void setSign(bool value);
    bool sign() const;
    void setThousandSeparate(bool value);
    bool thousandSeparate() const;
    void setPrecisionRange(int minPrecision, int maxPrecision);
    int minPrecision() const;
    int maxPrecision() const;
    void setPercent(bool value);
    bool percent() const;
    QString toString(double number, const QString &nanStub, const NumeralLocale &numeralLocale) const;
    QString toString(float number, const QString &nanStub, const NumeralLocale &numeralLocale) const;
    template<typename T> QString toString(T number, const QString &nanStub, const NumeralLocale &numeralLocale) const
    {
        return toString(static_cast<double>(number), nanStub, numeralLocale);
    }
    QString toString(double number, const QString &nanStub) const;
    QString toString(float number, const QString &nanStub) const;
    template<typename T> QString toString(T number, const QString &nanStub) const
    {
        return toString(static_cast<double>(number), nanStub);
    }
    QString toString(double number) const;
    QString toString(float number) const;
    template<typename T> QString toString(T number) const
    {
        return toString(static_cast<double>(number));
    }
    static QString format(double number, const QString &formatString, const QString &nanStub, const NumeralLocale &numeralLocale);
    static QString format(float number, const QString &formatString, const QString &nanStub, const NumeralLocale &numeralLocale);
    template<typename T> static QString format(T number, const QString &formatString, const QString &nanStub, const NumeralLocale &numeralLocale)
    {
        return format(static_cast<double>(number), formatString, nanStub, numeralLocale);
    }
    static QString format(double number, const QString &formatString, const QString &nanStub);
    static QString format(float number, const QString &formatString, const QString &nanStub);
    template<typename T> static QString format(T number, const QString &formatString, const QString &nanStub)
    {
        return format(static_cast<double>(number), formatString, nanStub);
    }
    static QString format(double number, const QString &formatString);
    static QString format(float number, const QString &formatString);
    template<typename T> static QString format(T number, const QString &formatString)
    {
        return format(static_cast<double>(number), formatString);
    }
    static QString format(double number);
    static QString format(float number);
    template<typename T> static QString format(T number)
    {
        return format(static_cast<double>(number));
    }
    static void setDefaultNumeralLocale(const NumeralLocale &value);
    static NumeralLocale defaultNumeralLocale();
    static void setDefaultNanStub(const QString &value);
    static QString defaultNanStub();

private:
    bool m_sign;
    bool m_thousandSeparate;
    int m_minPrecision;
    int m_maxPrecision;
    bool m_percent;
    static NumeralLocale *m_defaultNumeralLocale;
    static QString *m_defaultNanStub;
    void parse(const QStringRef &st);
    void parseIntegerPart(const QStringRef &st);
    void parseFractionalPart(const QStringRef &st);
    bool hasExtraPrecision() const;
    QString decoratePercent(const QString &formattedNumber, const NumeralLocale &numeralLocale) const;
    QString decorateSign(const QString &formattedNumber, double number, const NumeralLocale &numeralLocale) const;
    QString decorateThousandSeparator(const QString &formattedNumber, const NumeralLocale &numeralLocale) const;
    QString decorateTrimmingZeros(const QString &formattedNumber, const NumeralLocale &numeralLocale) const;
    QString initialFormat(double positiveNumber, const NumeralLocale &numeralLocale) const;
    QString initialFormat(float positiveNumber, const NumeralLocale &numeralLocale) const;
    static void createDefaultNumeralLocaleIfNeeded();
    static void createDefaultNanStubIfNeeded();
};
}

#endif // NUMERAL_H
