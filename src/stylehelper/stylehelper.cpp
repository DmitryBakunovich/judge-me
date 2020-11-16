#include "src/stylehelper/stylehelper.h"

QString StyleHelper::getMaximizeStyleSheet()
{
    return "#maximaz {"
           "image: url(:/titlebar/images/titlebar/restore.png);"
           "background-repeat: no-repeat;"
           "background-color: #554d93;"
           "padding-left: 5px;"
           "padding-right: 5px;"
           "padding-top: 7px;"
           "padding-bottom: 7px;"
           "border: 1px solid #554d93;"
           "}"
           "#maximaz:hover {"
           "image: url(:/titlebar/images/titlebar/restore-hover.png);"
           "background-color: #6d64ae;"
           "border: 1px solid #6d64ae;"
           "};";
}

QString StyleHelper::getRestoreStyleSheet()
{
    return "#maximaz {"
           "image: url(:/titlebar/images/titlebar/maximize.png);"
           "background-repeat: no-repeat;"
           "background-color: #554d93;"
           "padding-left: 5px;"
           "padding-right: 5px;"
           "padding-top: 7px;"
           "padding-bottom: 7px;"
           "border: 1px solid #554d93;"
           "}"
           "#maximaz:hover {"
           "image: url(:/titlebar/images/titlebar/maximize-hover.png);"
           "background-color: #6d64ae;"
           "border: 1px solid #6d64ae;"
           "};";
}

