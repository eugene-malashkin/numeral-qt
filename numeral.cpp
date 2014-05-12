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
