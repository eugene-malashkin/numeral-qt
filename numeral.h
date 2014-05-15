#ifndef NUMERAL_H
#define NUMERAL_H

#include <QString>
#include <QLocale>

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
    QString toString(double number, const QString &nanStub, const QLocale &locale) const;
    QString toString(double number, const QString &nanStub) const;
    QString toString(double number) const;
    static QString format(double number, const QString &formatString, const QString &nanStub, const QLocale &locale);
    static QString format(double number, const QString &formatString, const QString &nanStub);
    static QString format(double number, const QString &formatString);
    static QString format(double number);
    static void setDefaultLocale(const QLocale &value);
    static QLocale defaultLocale();
    static void setDefaultNanStub(const QString &value);
    static QString defaultNanStub();

private:
    bool m_sign;
    bool m_thousandSeparate;
    int m_precision;
    bool m_extraPrecision;
    bool m_percent;
    static QLocale *m_defaultLocale;
    static QString *m_defaultNanStub;
    void parse(const QStringRef &st);
    void parseIntegerPart(const QStringRef &st);
    void parseFractionalPart(const QStringRef &st);
    QString decorateSign(const QString &formattedNumber, double number, const QLocale &locale) const;
    QString decorateThousandSeparator(const QString &formattedNumber, const QLocale &locale) const;
    QString decorateTrimmingZeros(const QString &formattedNumber, const QLocale &locale) const;
    QString initialFormat(double positiveNumber, const QLocale &locale) const;
    static void createDefaultLocaleIfNeeded();
    static void createDefaultNanStubIfNeeded();
};

#endif // NUMERAL_H
