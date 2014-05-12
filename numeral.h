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

#endif // NUMERAL_H
