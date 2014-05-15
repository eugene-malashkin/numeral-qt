numeral-qt
==========

Class Numeral designed for formatting numbers. The simplest example:
```
QString st = Numeral::format(12345.67); // "12,345.67"
```

## Format string

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

## Storying numeral format
You can use instance of Numeral class for storying numeral format.
```
Numeral n("+0.00%");  // you can use default constructor, copy of Numeral or from QString
n.setPrecision(4);
QString st = n.toString(0.1234); // "+12.3400%"
```

## Working with QLocale
You can use appropriate locale for formatting:
```
QLocale locale(QLocale::C);
locale.
```
