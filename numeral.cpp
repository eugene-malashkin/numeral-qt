#include "numeral.h"
#include <QtNumeric>
#include <float.h>
#include <math.h>

//******************************************************************************************************
/*!
 *\class Numeral
*/
//******************************************************************************************************

QLocale *Numeral::m_defaultLocale = NULL;
QString *Numeral::m_defaultNanStub = NULL;

Numeral::Numeral()
{
    clear();
}

Numeral::Numeral(const QString &st)
{
    setFormatString(st);
}

Numeral::Numeral(bool sign, bool thousandSeparate, int precision, bool extraPrecision, bool percent)
{
    clear();
    setSign(sign);
    setThousandSeparate(thousandSeparate);
    setPrecision(precision);
    setExtraPrecision(extraPrecision);
    setPercent(percent);
}

bool Numeral::operator == (const Numeral &another)
{
    return
            (m_sign == another.m_sign) &&
            (m_thousandSeparate == another.m_thousandSeparate) &&
            (m_precision == another.m_precision) &&
            (m_extraPrecision == another.m_extraPrecision) &&
            (m_percent == another.m_percent);
}

bool Numeral::operator != (const Numeral &another)
{
    return !(operator ==(another));
}

void Numeral::clear()
{
    m_sign = false;
    m_thousandSeparate = true;
    m_precision = 0;
    m_extraPrecision = true;
    m_percent = false;
}

void Numeral::setFormatString(const QString &st)
{
    clear();
    QStringRef remainSt(&st);
    remainSt = remainSt.trimmed();
    parse(remainSt);
}

QString Numeral::formatString() const
{
    QStringList result;
    if (m_sign)
    {
        result << "+";
    }
    if (!m_thousandSeparate)
    {
        result << "0";
    }
    else
    {
        result << "0,0";
    }
    if (m_precision > 0)
    {
        result << ".";
        for (int i = 0; i < m_precision; i++)
        {
            result << "0";
        }
    }
    if (m_extraPrecision)
    {
        result << "*";
    }
    if (m_percent)
    {
        result << "%";
    }
    return result.join("");
}

void Numeral::setSign(bool value)
{
    m_sign = value;
}

bool Numeral::sign() const
{
    return m_sign;
}

void Numeral::setThousandSeparate(bool value)
{
    m_thousandSeparate = value;
}

bool Numeral::thousandSeparate() const
{
    return m_thousandSeparate;
}

void Numeral::setPrecision(int value)
{
    m_precision = value;
}

int Numeral::precision() const
{
    return m_precision;
}

void Numeral::setExtraPrecision(bool value)
{
    m_extraPrecision = value;
}

bool Numeral::extraPrecision() const
{
    return m_extraPrecision;
}

void Numeral::setPercent(bool value)
{
    m_percent = value;
}

bool Numeral::percent() const
{
    return m_percent;
}

QString Numeral::toString(double number, const QString &nanStub, const QLocale &locale) const
{
    if (qIsNaN(number))
    {
        return nanStub;
    }
    else
    {
        QLocale clocale(locale);
        clocale.setNumberOptions(QLocale::OmitGroupSeparator);
        double cnumber = (percent())? number*100.0 : number;
        QString result = initialFormat(fabs(cnumber), clocale);
        result = decorateTrimmingZeros(result, clocale);
        result = decorateThousandSeparator(result, clocale);
        result = decorateSign(result, cnumber, clocale);
        if (percent())
        {
            result = result + clocale.percent();
        }
        return result;
    }
}

QString Numeral::toString(double number, const QString &nanStub) const
{
    return toString(number, nanStub, defaultLocale());
}

QString Numeral::toString(double number) const
{
    return toString(number, defaultNanStub());
}

QString Numeral::format(double number, const QString &formatString, const QString &nanStub, const QLocale &locale)
{
    Numeral obj(formatString);
    return obj.toString(number, nanStub, locale);
}

QString Numeral::format(double number, const QString &formatString, const QString &nanStub)
{
    Numeral obj(formatString);
    return obj.toString(number, nanStub);
}

QString Numeral::format(double number, const QString &formatString)
{
    Numeral obj(formatString);
    return obj.toString(number);
}

QString Numeral::format(double number)
{
    Numeral obj;
    return obj.toString(number);
}

void Numeral::setDefaultLocale(const QLocale &value)
{
    createDefaultLocaleIfNeeded();
    *m_defaultLocale = value;
}

QLocale Numeral::defaultLocale()
{
    createDefaultLocaleIfNeeded();
    return *m_defaultLocale;
}

void Numeral::setDefaultNanStub(const QString &value)
{
    createDefaultNanStubIfNeeded();
    *m_defaultNanStub = value;
}

QString Numeral::defaultNanStub()
{
    createDefaultNanStubIfNeeded();
    return *m_defaultNanStub;
}

void Numeral::parse(const QStringRef &st)
{
    QStringRef remainSt = st;

    // Sign
    if (remainSt.startsWith('+'))
    {
        m_sign = true;
        remainSt = remainSt.mid(1);
    }
    else
    {
        m_sign = false;
    }

    // Percent
    if (remainSt.endsWith('%'))
    {
        m_percent = true;
        remainSt = remainSt.left(remainSt.length()-1);
    }
    else
    {
        m_percent = false;
    }

    // Extra precision
    if (remainSt.endsWith('*'))
    {
        m_extraPrecision = true;
        remainSt = remainSt.left(remainSt.length()-1);
    }
    else
    {
        m_extraPrecision = false;
    }

    // Split integer and fractional parts
    int dotIndex = remainSt.indexOf('.');
    if (dotIndex < 0)
    {
        // Integer part only
        parseIntegerPart(remainSt);
    }
    else
    {
        parseIntegerPart(remainSt.left(dotIndex));
        parseFractionalPart(remainSt.mid(dotIndex + 1));
    }
}

void Numeral::parseIntegerPart(const QStringRef &st)
{
    m_thousandSeparate = st.contains(",");
}

void Numeral::parseFractionalPart(const QStringRef &st)
{
    m_precision = st.count('0');
}

QString Numeral::decorateSign(const QString &formattedNumber, double number, const QLocale &locale) const
{
    QString s;
    bool isNegative = (number < -DBL_EPSILON);
    bool isPositive = (number > +DBL_EPSILON);
    if (isNegative)
    {
        s = locale.negativeSign();
    }
    else if ((isPositive) && (sign()))
    {
        s = locale.positiveSign();
    }
    return s + formattedNumber;
}

QString Numeral::decorateThousandSeparator(const QString &formattedNumber, const QLocale &locale) const
{
    if (!thousandSeparate())
    {
        return formattedNumber;
    }
    else
    {
        QString result = formattedNumber;
        int startPosition = result.indexOf(QRegExp("[0-9]"));
        if (startPosition >= 0)
        {
            int endPosition = result.indexOf(locale.decimalPoint());
            if (endPosition < 0)
            {
                endPosition = result.length();
            }
            for (int i = endPosition - 3; i >= startPosition + 1; i -= 3)
            {
                result.insert(i, locale.groupSeparator());
            }
        }
        return result;
    }
}

QString Numeral::decorateTrimmingZeros(const QString &formattedNumber, const QLocale &locale) const
{
    QString result = formattedNumber;
    if (extraPrecision())
    {
        int decimalIndex = result.indexOf(locale.decimalPoint());
        if (decimalIndex >= 0)
        {
            int stopIndex = decimalIndex + precision();
            int truncateAfterIndex = stopIndex;
            for (int i = result.length()-1; i >= stopIndex; i--)
            {
                if (result[i] != '0')
                {
                    truncateAfterIndex = i;
                    break;
                }
            }
            if (truncateAfterIndex == decimalIndex)
            {
                // chop off decimal splitter too
                truncateAfterIndex--;
            }
            result = result.left(truncateAfterIndex+1);
        }
    }
    return result;
}

QString Numeral::initialFormat(double positiveNumber, const QLocale &locale) const
{
    int p = 6;
    if (!extraPrecision())
    {
        p = precision();
    }
    return locale.toString(positiveNumber + DBL_EPSILON, 'f', qMax(p, 0));
}

void Numeral::createDefaultLocaleIfNeeded()
{
    if (m_defaultLocale == NULL)
    {
        m_defaultLocale = new QLocale;
    }
}

void Numeral::createDefaultNanStubIfNeeded()
{
    if (m_defaultNanStub == NULL)
    {
        m_defaultNanStub = new QString;
    }
}
