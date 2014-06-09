numeral-qt
==========

Class Numeral designed for formatting numbers. The simplest example:
```
QString st = Numeral::format(12345.67); // "12,345.67"
```

## Format string

This library is infused by http://numeraljs.com/, but has a slightly different syntax. 

You can define numeral format in one string named "format string". For instance, you would like to format number without thousands separator ("0") and with two digits after dot (".00"):
```
QString st = Numeral::format(12345.678, "0.00"); // result "12345.68", format string is "0.00"
```

Here are more advanced examples. Format number with thousands separator ("0,0") and with two digits after dot (".00"):
```
Numeral::format(12345.678, "0,0.00"); // "12,345.68"
```
Format number with thousands separator ("0,0") and with necessary digits after dot, but at least, with two digits (".00*"):
```
Numeral::format(12345.678, "0,0.00*"); // "12,345.678"
```
Format number with sign ("+"), without thousands separator ("0") and with necessary digits after dot (no restrictions):
```
Numeral::format(12345.678, "+0.*"); // "+12345.678"
Numeral::format(-12345, "+0.*");    // "-12345"
```
Format number as percent ("%") with two digits after dot, with sign ("+"), without thousands separator ("0") and with three digits after dot:
```
Numeral::format(0.1234, "+0.00%"); // "+12.340%"
Numeral::format(-0.12345, "+0.00%"); // "-12.345%"
```

## Storing numeral format
You can use instance of Numeral class for storing numeral format.
```
Numeral n("+0.00%");  // you can use default constructor, copy of Numeral or from QString
n.setPrecision(4);
QString st = n.toString(0.1234); // "+12.3400%"
```

## Working with locale
You can use appropriate locale for formatting. Because of you can't change locale parameters, there is NumeralLocale class - a composition of QLocale and changeable group separator. 
```
NumeralLocale nl(QLocale::C, " ");  // " " is group separator
Numeral::format(1234.5678, "0,0.*", "-", nl); // "1 234.5678"
```

Or you can define default numeral locale once, and don't care about specifying it later:
```
Numeral::setDefaultNumeralLocale(NumeralLocale(QLocale::C, " "));

// Later
Numeral::format(1234.5678);  //"1 234.5678"
```

## Working with NaN
You can format NaN as you want:
```
Numeral::format(qQNaN(), "0.0", "-"); // "-"
```

Or you can define default NaN stub once, and don't care about specifying it later:
```
Numeral::setDefaultNanStub("nOT a nUMBER");

// Later
Numeral::format(qQNaN()); // "nOT a nUMBER"
```