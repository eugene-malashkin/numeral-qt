numeral-qt
==========

Class NumeralFormat designed for formatting numbers. The simplest example:
```c++
using namespace NumeralQt;	// The library's namespace

// Format number 12345.67 with format string "0,0.0" (use thousands separator, one digit after dot):
QString st = NumeralFormat::format(12345.67, "0,0.0"); // "12,345.7"
```

## Usage

Just include "numeral-qt.pri" in your project (some *.pro file created in Qt Creator):
```
# Includes numeral-qt library (change path if needed):
include(../numeral-qt/numeral-qt.pri)
```

And then include numeral.h in your code:
```c++
#include <numeral.h>
```

Or you can just copy two files, numeral.h and .cpp, in appropriate directory and use it without "numeral-qt.pri".

The library uses C++11.

## Format string

This library was inspired by http://numeraljs.com/, but has a slightly different syntax. 

You can define numeral format in one string named "format string". For instance, you would like to format number without thousands separator ("0") and with two digits after dot (".00"):
```c++
QString st = NumeralFormat::format(12345.678, "0.00"); // result "12345.68", format string is "0.00"
```

Here are more advanced examples. Format number with thousands separator ("0,0") and with two digits after dot (".00"):
```c++
NumeralFormat::format(12345.678, "0,0.00"); // "12,345.68"
```
Format number with thousands separator ("0,0") and with necessary digits after dot in an amount from 2 to 4 (.00**):
```c++
NumeralFormat::format(12345,       "0,0.00**"); // "12,345.00"   minimal digits after dot = 2
NumeralFormat::format(12345.678,   "0,0.00**"); // "12,345.678"  digits after dot between 2 and 4
NumeralFormat::format(12345.65438, "0,0.00**"); // "12,345.6544" maximal digits after dot = 4
```
Format number with sign ("+"), without thousands separator ("0") and no digits after dot:
```c++
NumeralFormat::format(12345.678, "+0"); // "+12345"
NumeralFormat::format(-12345, "+0");    // "-12345"
```
Format number as percent ("%") with two digits after dot, with sign ("+"), without thousands separator ("0"):
```c++
NumeralFormat::format(0.1234, "+0.00%"); // "+12.34%"
NumeralFormat::format(-0.12345, "+0.00%"); // "-12.35%"
```

## Storing numeral format
You can use instance of NumeralFormat class for storing numeral format.
```c++
NumeralFormat n("+0.00%");  // you can use default constructor, or copy of NumeralFormat, or from QString
n.setPrecisionRange(3, 4);  // set precision range (the count of digits after dot) between 3 and 4
QString st = n.toString(0.1234); // "+12.340%"
```

## Working with locale
You can use appropriate locale for formatting. Because of you can't change locale parameters, there is NumeralLocale class - a composition of QLocale and changeable group separator. 
```c++
NumeralLocale nl(QLocale::C, " ");  // " " is group separator
NumeralFormat::format(1234.5678, "0,0.*", "NaN", nl); // "1 234.6"
```

Or you can define default numeral locale once, and don't care about specifying it later:
```c++
NumeralFormat::setDefaultNumeralLocale(NumeralLocale(QLocale::C, " "));

// Later
NumeralFormat::format(1234.5678);  //"1 234.5678"
```

## Working with NaN
You can format NaN as you want:
```c++
NumeralFormat::format(qQNaN(), "0.0", "-"); // "-" as we defined in third parameter
```

Or you can define default NaN stub once, and don't care about specifying it later:
```c++
NumeralFormat::setDefaultNanStub("nOT a nUMBER");

// Later
NumeralFormat::format(qQNaN()); // "nOT a nUMBER"
```