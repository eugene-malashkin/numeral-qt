#include "numeral.h"
#include <float.h>
#include <math.h>

#include <QDebug>

static bool isNan(double x)
{
#if defined(_MSC_VER)
    return (_isnan(x) != 0);
#else
    return (isnan(x) != 0);
#endif
}

//******************************************************************************************************
/*!
 *\class NumeralFormat
*/
//******************************************************************************************************

NumeralFormat::NumeralFormat()
{
    clear();
}

NumeralFormat::NumeralFormat(const QString &st)
{
    setFormatString(st);
}

NumeralFormat::NumeralFormat(bool sign, bool thousandSeparate, int precision, bool extraPrecision, bool percent)
{
    clear();
    setSign(sign);
    setThousandSeparate(thousandSeparate);
    setPrecision(precision);
    setExtraPrecision(extraPrecision);
    setPercent(percent);
}

bool NumeralFormat::operator == (const NumeralFormat &another)
{
    return
            (m_sign == another.m_sign) &&
            (m_thousandSeparate == another.m_thousandSeparate) &&
            (m_precision == another.m_precision) &&
            (m_extraPrecision == another.m_extraPrecision) &&
            (m_percent == another.m_percent);
}

bool NumeralFormat::operator != (const NumeralFormat &another)
{
    return !(operator ==(another));
}

void NumeralFormat::clear()
{
    m_sign = false;
    m_thousandSeparate = true;
    m_precision = 0;
    m_extraPrecision = true;
    m_percent = false;
}

void NumeralFormat::setFormatString(const QString &st)
{
    clear();
    QStringRef remainSt(&st);
    remainSt = remainSt.trimmed();
    parse(remainSt);
}

QString NumeralFormat::formatString() const
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

void NumeralFormat::setSign(bool value)
{
    m_sign = value;
}

bool NumeralFormat::sign() const
{
    return m_sign;
}

void NumeralFormat::setThousandSeparate(bool value)
{
    m_thousandSeparate = value;
}

bool NumeralFormat::thousandSeparate() const
{
    return m_thousandSeparate;
}

void NumeralFormat::setPrecision(int value)
{
    m_precision = value;
}

int NumeralFormat::precision() const
{
    return m_precision;
}

void NumeralFormat::setExtraPrecision(bool value)
{
    m_extraPrecision = value;
}

bool NumeralFormat::extraPrecision() const
{
    return m_extraPrecision;
}

void NumeralFormat::setPercent(bool value)
{
    m_percent = value;
}

bool NumeralFormat::percent() const
{
    return m_percent;
}

void NumeralFormat::parse(const QStringRef &st)
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

void NumeralFormat::parseIntegerPart(const QStringRef &st)
{
    m_thousandSeparate = st.contains(",");
}

void NumeralFormat::parseFractionalPart(const QStringRef &st)
{
    m_precision = st.count('0');
}


//******************************************************************************************************
/*!
 *\class Numeral
*/
//******************************************************************************************************

QLocale *Numeral::m_defaultLocale = NULL;
QString *Numeral::m_defaultNanStub = NULL;

Numeral::Numeral()
    : m_numeralFormat()
    , m_locale(defaultLocale())
    , m_nanStub(defaultNanStub())
{
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

Numeral::Numeral(const NumeralFormat &format)
    : m_numeralFormat(format)
    , m_locale(defaultLocale())
    , m_nanStub(defaultNanStub())
{
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

Numeral::Numeral(const NumeralFormat &format, const QLocale &locale)
    : m_numeralFormat(format)
    , m_locale(locale)
    , m_nanStub(defaultNanStub())
{
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

void Numeral::setNumeralFormat(const NumeralFormat &value)
{
    m_numeralFormat = value;
}

NumeralFormat Numeral::numeralFormat() const
{
    return m_numeralFormat;
}

void Numeral::setlocale(const QLocale &value)
{
    m_locale = value;
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

QLocale Numeral::locale() const
{
    return m_locale;
}

void Numeral::setNanStub(const QString &value)
{
    m_nanStub = value;
}

QString Numeral::nanStub() const
{
    return m_nanStub;
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

QString Numeral::toString(double number) const
{
    if (isNan(number))
    {
        return m_nanStub;
    }
    else
    {
        double cnumber = (m_numeralFormat.percent())? number*100.0 : number;
        QString result = initialFormat(fabs(cnumber));
        result = decorateTrimmingZeros(result);
        result = decorateThousandSeparator(result);
        result = decorateSign(result, cnumber);
        if (m_numeralFormat.percent())
        {
            result = result + m_locale.percent();
        }
        return result;
    }
}

QString Numeral::format(double number, const NumeralFormat &numeralFormat)
{
    Numeral n(numeralFormat);
    return n.toString(number);
}

QString Numeral::decorateSign(const QString &formattedNumber, double number) const
{
    QString sign;
    bool isNegative = (number < -DBL_EPSILON);
    bool isPositive = (number > +DBL_EPSILON);
    if (isNegative)
    {
        sign = m_locale.negativeSign();
    }
    else if ((isPositive) && (m_numeralFormat.sign()))
    {
        sign = m_locale.positiveSign();
    }
    return sign + formattedNumber;
}

QString Numeral::decorateThousandSeparator(const QString &formattedNumber) const
{
    if (!m_numeralFormat.thousandSeparate())
    {
        return formattedNumber;
    }
    else
    {
        QString result = formattedNumber;
        int startPosition = result.indexOf(QRegExp("[0-9]"));
        if (startPosition >= 0)
        {
            int endPosition = result.indexOf(m_locale.decimalPoint());
            if (endPosition < 0)
            {
                endPosition = result.length();
            }
            for (int i = endPosition - 3; i >= startPosition + 1; i -= 3)
            {
                result.insert(i, m_locale.groupSeparator());
            }
        }
        return result;
    }
}

QString Numeral::decorateTrimmingZeros(const QString &formattedNumber) const
{
    QString result = formattedNumber;
    if (m_numeralFormat.extraPrecision())
    {
        int decimalIndex = result.indexOf(m_locale.decimalPoint());
        if (decimalIndex >= 0)
        {
            int stopIndex = decimalIndex + m_numeralFormat.precision();
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

QString Numeral::initialFormat(double positiveNumber) const
{
    int precision = 6;
    if (!m_numeralFormat.extraPrecision())
    {
        precision = m_numeralFormat.precision();
    }
    return m_locale.toString(positiveNumber + DBL_EPSILON, 'f', qMax(precision, 0));
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
