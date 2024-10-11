#include "high_precision.h"

HighPrecision::HighPrecision() {}

HighPrecision::~HighPrecision() {}

QString HighPrecision::add(QString a, QString b) {
    int     len_a = a.length();
    int     len_b = b.length();
    int     len   = len_a > len_b ? len_a : len_b;
    QString result;
    int     carry = 0;
    for (int i = 0; i < len; i++) {
        int temp = 0;
        if (i < len_a) {
            temp += a[len_a - i - 1].toLatin1() - '0';
        }
        if (i < len_b) {
            temp += b[len_b - i - 1].toLatin1() - '0';
        }
        temp += carry;
        carry  = temp / 10;
        temp   = temp % 10;
        result = QString::number(temp) + result;
    }
    if (carry != 0) {
        result = QString::number(carry) + result;
    }
    return result;
}

QString HighPrecision::add(QString a, int b) { return add(a, QString::number(b)); }

QString HighPrecision::sub(QString a, QString b) {
    int     len_a = a.length();
    int     len_b = b.length();
    int     len   = len_a > len_b ? len_a : len_b;
    QString result;
    int     carry = 0;
    for (int i = 0; i < len; i++) {
        int temp = 0;
        if (i < len_a) {
            temp += a[len_a - i - 1].toLatin1() - '0';
        }
        if (i < len_b) {
            temp -= b[len_b - i - 1].toLatin1() - '0';
        }
        temp -= carry;
        if (temp < 0) {
            temp += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result = QString::number(temp) + result;
    }
    return result;
}

QString HighPrecision::mul(QString a, QString b) {
    int     len_a = a.length();
    int     len_b = b.length();
    int     len   = len_a + len_b;
    QString result(len, '0');
    for (int i = 0; i < len_a; i++) {
        int carry = 0;
        for (int j = 0; j < len_b; j++) {
            int temp = (a[len_a - i - 1].toLatin1() - '0') * (b[len_b - j - 1].toLatin1() - '0') + carry +
                (result[len - i - j - 1].toLatin1() - '0');
            carry                   = temp / 10;
            temp                    = temp % 10;
            result[len - i - j - 1] = temp + '0';
        }
        result[len - i - len_b - 1] = carry + '0';
    }
    int i = 0;
    while (result[i] == '0') {
        i++;
    }
    if (i == len) {
        return "0";
    }
    return result.mid(i);
}

QString HighPrecision::div(QString a, QString b) {
    int     len_a = a.length();
    int     len_b = b.length();
    int     len   = len_a - len_b;
    QString result(len + 1, '0');
    QString temp;
    for (int i = 0; i < len_b; i++) {
        temp += a[i];
    }
    for (int i = 0; i <= len; i++) {
        int j = 0;
        while (temp >= b) {
            temp = sub(temp, b);
            j++;
        }
        result[i] = j + '0';
        if (i < len) {
            temp += a[i + len_b];
        }
    }
    int i = 0;
    while (result[i] == '0') {
        i++;
    }
    if (i == len + 1) {
        return "0";
    }
    return result.mid(i);
}

bool HighPrecision::compare(QString a, QString b) {
    int len_a = a.length();
    int len_b = b.length();
    if (len_a > len_b) {
        return true;
    } else if (len_a < len_b) {
        return false;
    } else {
        for (int i = 0; i < len_a; i++) {
            if (a[i] > b[i]) {
                return true;
            } else if (a[i] < b[i]) {
                return false;
            }
        }
    }
    return true;
}