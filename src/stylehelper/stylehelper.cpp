#include "stylehelper.h"

QString StyleHelper::getMenuStyleSheet() {
    return "QMenu {"
           "border: none"
           "}"
           "QMenu::item {"
           "padding: 2px 5px 2px 2px;"
           "}"
           "QMenu::item:selected {"
           "background-color: rgb(0, 85, 127);"
           "color: rgb(255, 255, 255);"
           "}"
           "QMenu::item::disabled {"
           "background-color:black;"
           "color: white;"
           "}";
}
