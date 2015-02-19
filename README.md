numeral-qt
==========

Class NumeralFormat designed for formatting numbers. The simplest example:
```c++
using namespace NumeralQt;	// The library's namespace

// Format number 12345.67 with format string "0,0.0". 
// Format string "0,0.0" means "use thousands separator, one digit after dot)":
QString st = NumeralFormat::format(12345.67, "0,0.0"); // st == "12,345.7"
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

The library was inspired by http://numeraljs.com/, but has a slightly different syntax. 

You can define numeral format in one string named "format string". For instance, you would like to format number without thousands separator ("0") and with two digits after dot (".00"), you should use format string "0.00":
```c++
// Format number 12345.678 with format string "0.00"
QString st = NumeralFormat::format(12345.678, "0.00"); // st == "12345.68"
```

Here are more advanced examples. Format number with thousands separator ("0,0") and with two digits after dot (".00"):
```c++
QString st = NumeralFormat::format(12345.678, "0,0.00") // st == "12,345.68"
```
Format number with thousands separator ("0,0") and with necessary digits after dot in an amount from 2 to 4 (".00**"):
```c++
QString st = NumeralFormat::format(12345,       "0,0.00**") // st == "12,345.00"
```
```c++
QString st = NumeralFormat::format(12345.678,   "0,0.00**") // st == "12,345.678"
```
```c++
QString st = NumeralFormat::format(12345.65438, "0,0.00**") // st == "12,345.6544"
```
Format number with sign ("+"), without thousands separator ("0") and no digits after dot:
```c++
QString st = NumeralFormat::format(12345.678, "+0") // st == "+12346"
```
```c++
QString st = NumeralFormat::format(-12345, "+0")    // st == "-12345"
```
Format number as percent ("%") with two digits after dot, with sign ("+"), without thousands separator ("0"):
```c++
QString st = NumeralFormat::format(0.1234, "+0.00%")		// st == "+12.34%"
```
```c++
QString st = NumeralFormat::format(-0.12345, "+0.00%")		// st == "-12.35%"
```

## Storing numeral format
You can use an instance of NumeralFormat class for storing numeral format.
```c++
// You can use default constructor, or copy of NumeralFormat, or from QString
NumeralFormat n("+0.00%");
// Set precision range (the count of digits after dot) between 3 and 4
n.setPrecisionRange(3, 4);

QString st = n.toString(0.1234); // st == "+12.340%"
```

## Working with locale
You can use appropriate locale for formatting. Because of you can't change QLocale parameters, there is NumeralLocale class - a composition of QLocale and changeable group separator. 
```c++
NumeralLocale nl(QLocale::C, " ");  // " " is group separator
QString st = 
	NumeralFormat::format(1234.5678, "0,0.*", "NaN", nl); // st == "1 234.6"
```

Or you can define default numeral locale once, and don't care about specifying it later:
```c++
NumeralFormat::setDefaultNumeralLocale(NumeralLocale(QLocale::C, " "));

// Later
QString st = NumeralFormat::format(1234.5678);  // st == "1 234.5678"
```

## Working with NaN
You can format NaN as you want:
```c++
NumeralFormat::format(qQNaN(), "0.0", "-") // "-" (as we defined in third parameter)
```

Or you can define default NaN stub once, and don't care about specifying it later:
```c++
NumeralFormat::setDefaultNanStub("nOT a nUMBER");

// Later
QString st = NumeralFormat::format(qQNaN()); // st == "nOT a nUMBER"
```