#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QString>

class StyleHelper
{
public:
    static QString getMenuStyleSheet();

    static QString getFirstPageStyle();
    static QString getSecondPageStyle();

    static QString getFirstPageButtonStyle();
    static QString getSecondPageButtonStyle();

    static QString getLeftSlideStyle();
    static QString getRightSlideStyle();

    static QString getFieldStyle();
    static QString getEmptyFieldStyle();
    static QString getFillFieldStyle();
};

#endif
