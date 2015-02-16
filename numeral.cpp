#include "numeral.h"
#include <QtNumeric>
#include <float.h>
#include <math.h>

//******************************************************************************************************
/*!
 *\class NumeralLocale
*/
//******************************************************************************************************

NumeralQt::NumeralLocale::NumeralLocale()
    : m_locale()
    , m_groupSeparator(m_locale.groupSeparator())
{

}

NumeralQt::NumeralLocale::NumeralLocale(const QLocale &locale)
    : m_locale(locale)
    , m_groupSeparator(m_locale.groupSeparator())
{

}

NumeralQt::NumeralLocale::NumeralLocale(const QLocale &locale, const QString &groupSeparator)
    : m_locale(locale)
    , m_groupSeparator(groupSeparator)
{

}

NumeralQt::NumeralLocale::NumeralLocale(const NumeralLocale &another)
    : m_locale(another.m_locale)
    , m_groupSeparator(another.m_groupSeparator)
{

}

void NumeralQt::NumeralLocale::setLocale(const QLocale &value)
{
    m_locale = value;
}

QLocale NumeralQt::NumeralLocale::locale() const
{
    return m_locale;
}

void NumeralQt::NumeralLocale::setGroupSeparator(const QString &value)
{
    m_groupSeparator = value;
}

QString NumeralQt::NumeralLocale::groupSeparator() const
{
    return m_groupSeparator;
}


//******************************************************************************************************
/*!
 *\class Numeral
*/
//******************************************************************************************************

NumeralQt::NumeralLocale *NumeralQt::NumeralFormat::m_defaultNumeralLocale = nullptr;
QString *NumeralQt::NumeralFormat::m_defaultNanStub = nullptr;

NumeralQt::NumeralFormat::NumeralFormat()
{
    clear();
}

NumeralQt::NumeralFormat::NumeralFormat(const QString &st)
{
    setFormatString(st);
}

NumeralQt::NumeralFormat::NumeralFormat(bool sign, bool thousandSeparate, int minPrecision, int maxPrecision, bool percent)
{
    clear();
    setSign(sign);
    setThousandSeparate(thousandSeparate);
    setMinPrecision(minPrecision);
    setMaxPrecision(maxPrecision);
    setPercent(percent);
}

bool NumeralQt::NumeralFormat::operator == (const NumeralFormat &another)
{
    return
            (m_sign == another.m_sign) &&
            (m_thousandSeparate == another.m_thousandSeparate) &&
            (m_minPrecision == another.m_minPrecision) &&
            (m_minPrecision == another.m_maxPrecision) &&
            (m_percent == another.m_percent);
}

bool NumeralQt::NumeralFormat::operator != (const NumeralFormat &another)
{
    return !(operator ==(another));
}

void NumeralQt::NumeralFormat::clear()
{
    m_sign = false;
    m_thousandSeparate = true;
    m_minPrecision = 0;
    m_maxPrecision = 6;
    m_percent = false;
}

void NumeralQt::NumeralFormat::setFormatString(const QString &st)
{
    clear();
    QStringRef remainSt(&st);
    remainSt = remainSt.trimmed();
    parse(remainSt);
}

QString NumeralQt::NumeralFormat::formatString() const
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
    if (m_minPrecision > 0)
    {
        result << ".";
        for (int i = 0; i < m_minPrecision; ++i)
        {
            result << "0";
        }
    }
    if (m_maxPrecision > m_minPrecision)
    {
        for (int i = m_minPrecision; i < m_maxPrecision; ++i)
        {
            result << "*";
        }
    }
    if (m_percent)
    {
        result << "%";
    }
    return result.join("");
}

void NumeralQt::NumeralFormat::setSign(bool value)
{
    m_sign = value;
}

bool NumeralQt::NumeralFormat::sign() const
{
    return m_sign;
}

void NumeralQt::NumeralFormat::setThousandSeparate(bool value)
{
    m_thousandSeparate = value;
}

bool NumeralQt::NumeralFormat::thousandSeparate() const
{
    return m_thousandSeparate;
}

void NumeralQt::NumeralFormat::setMinPrecision(int value)
{
    m_minPrecision = value;
}

int NumeralQt::NumeralFormat::minPrecision() const
{
    return m_minPrecision;
}

void NumeralQt::NumeralFormat::setMaxPrecision(int value)
{
    m_maxPrecision = value;
}

int NumeralQt::NumeralFormat::maxPrecision() const
{
    return m_maxPrecision;
}

void NumeralQt::NumeralFormat::setPercent(bool value)
{
    m_percent = value;
}

bool NumeralQt::NumeralFormat::percent() const
{
    return m_percent;
}

QString NumeralQt::NumeralFormat::toString(double number, const QString &nanStub, const NumeralLocale &numeralLocale) const
{
    if (qIsNaN(number))
    {
        return nanStub;
    }
    else
    {
        NumeralLocale clocale(numeralLocale);
        QLocale tmp = clocale.locale();
        tmp.setNumberOptions(QLocale::OmitGroupSeparator);
        clocale.setLocale(tmp);

        double cnumber = (percent())? number*100.0 : number;
        QString result = initialFormat(fabs(cnumber), clocale);
        result = decorateTrimmingZeros(result, clocale);
        result = decorateThousandSeparator(result, clocale);
        result = decorateSign(result, cnumber, clocale);
        if (percent())
        {
            result = result + clocale.locale().percent();
        }
        return result;
    }
}

QString NumeralQt::NumeralFormat::toString(double number, const QString &nanStub) const
{
    return toString(number, nanStub, defaultNumeralLocale());
}

QString NumeralQt::NumeralFormat::toString(double number) const
{
    return toString(number, defaultNanStub());
}

QString NumeralQt::NumeralFormat::format(double number, const QString &formatString, const QString &nanStub, const NumeralLocale &numeralLocale)
{
    NumeralFormat obj(formatString);
    return obj.toString(number, nanStub, numeralLocale);
}

QString NumeralQt::NumeralFormat::format(double number, const QString &formatString, const QString &nanStub)
{
    NumeralFormat obj(formatString);
    return obj.toString(number, nanStub);
}

QString NumeralQt::NumeralFormat::format(double number, const QString &formatString)
{
    NumeralFormat obj(formatString);
    return obj.toString(number);
}

QString NumeralQt::NumeralFormat::format(double number)
{
    NumeralFormat obj;
    return obj.toString(number);
}

void NumeralQt::NumeralFormat::setDefaultNumeralLocale(const NumeralLocale &value)
{
    createDefaultNumeralLocaleIfNeeded();
    *m_defaultNumeralLocale = value;
}

NumeralQt::NumeralLocale NumeralQt::NumeralFormat::defaultNumeralLocale()
{
    createDefaultNumeralLocaleIfNeeded();
    return *m_defaultNumeralLocale;
}

void NumeralQt::NumeralFormat::setDefaultNanStub(const QString &value)
{
    createDefaultNanStubIfNeeded();
    *m_defaultNanStub = value;
}

QString NumeralQt::NumeralFormat::defaultNanStub()
{
    createDefaultNanStubIfNeeded();
    return *m_defaultNanStub;
}

void NumeralQt::NumeralFormat::parse(const QStringRef &st)
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

    // Split integer and fractional parts
    int dotIndex = remainSt.indexOf('.');
    if (dotIndex < 0)
    {
        // Integer part only
        parseIntegerPart(remainSt);

        // No fractional part
        m_minPrecision = 0;
        m_maxPrecision = 0;
    }
    else
    {
        parseIntegerPart(remainSt.left(dotIndex));
        parseFractionalPart(remainSt.mid(dotIndex + 1));
    }
}

void NumeralQt::NumeralFormat::parseIntegerPart(const QStringRef &st)
{
    m_thousandSeparate = st.contains(",");
}

void NumeralQt::NumeralFormat::parseFractionalPart(const QStringRef &st)
{
    m_minPrecision = st.count('0');
    m_maxPrecision = m_minPrecision + st.count('*');
}

bool NumeralQt::NumeralFormat::hasExtraPrecision() const
{
    return (m_maxPrecision > m_minPrecision);
}

QString NumeralQt::NumeralFormat::decorateSign(const QString &formattedNumber, double number, const NumeralLocale &numeralLocale) const
{
    QString s;
    bool isNegative = (number < -DBL_EPSILON);
    bool isPositive = (number > +DBL_EPSILON);
    if (isNegative)
    {
        s = numeralLocale.locale().negativeSign();
    }
    else if ((isPositive) && (sign()))
    {
        s = numeralLocale.locale().positiveSign();
    }
    return s + formattedNumber;
}

QString NumeralQt::NumeralFormat::decorateThousandSeparator(const QString &formattedNumber, const NumeralLocale &numeralLocale) const
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
            int endPosition = result.indexOf(numeralLocale.locale().decimalPoint());
            if (endPosition < 0)
            {
                endPosition = result.length();
            }
            for (int i = endPosition - 3; i >= startPosition + 1; i -= 3)
            {
                result.insert(i, numeralLocale.groupSeparator());
            }
        }
        return result;
    }
}

QString NumeralQt::NumeralFormat::decorateTrimmingZeros(const QString &formattedNumber, const NumeralLocale &numeralLocale) const
{
    QString result = formattedNumber;
    if (hasExtraPrecision())
    {
        int decimalIndex = result.indexOf(numeralLocale.locale().decimalPoint());
        if (decimalIndex >= 0)
        {
            int stopIndex = decimalIndex + minPrecision();
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

QString NumeralQt::NumeralFormat::initialFormat(double positiveNumber, const NumeralLocale &numeralLocale) const
{
    return numeralLocale.locale().toString(positiveNumber + DBL_EPSILON, 'f', qMax(maxPrecision(), 0));
}

void NumeralQt::NumeralFormat::createDefaultNumeralLocaleIfNeeded()
{
    if (m_defaultNumeralLocale == nullptr)
    {
        m_defaultNumeralLocale = new NumeralLocale;
    }
}

void NumeralQt::NumeralFormat::createDefaultNanStubIfNeeded()
{
    if (m_defaultNanStub == nullptr)
    {
        m_defaultNanStub = new QString;
    }
}
