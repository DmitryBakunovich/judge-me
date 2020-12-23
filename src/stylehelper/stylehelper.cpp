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

QString StyleHelper::getFirstPageStyle() {
    return "#mainWidget {"
           "image: url(:/courtcasewindow/images/courtcasewindow/first-window.png);"
           "}";
}

QString StyleHelper::getSecondPageStyle() {
    return "#mainWidget {"
           "image: url(:/courtcasewindow/images/courtcasewindow/second-window.png);"
           "}";
}

QString StyleHelper::getFirstPageButtonStyle() {
    return "#firstPageButton {"
           "image: url(:/courtcasewindow/images/courtcasewindow/first-page-button-switch.png);"
           "border: none;"
           "}";
}

QString StyleHelper::getSecondPageButtonStyle() {
    return "#secondPageButton {"
           "image: url(:/courtcasewindow/images/courtcasewindow/second-page-button-switch.png);"
           "border: none;"
           "}";
}

QString StyleHelper::getEmptyFieldStyle() {
    return "background-color: #afc0da;"
           "border: 2px solid #a24b51;"
           "border-radius: 20px;"
           "font-size: 14px;"
           "padding-left: 10px;"
           "color: #566d8e";
}

QString StyleHelper::getFillFieldStyle() {
    return "background-color: #afc0da;"
           "border: 2px solid #eff1f6;"
           "border-radius: 20px;"
           "font-size: 14px;"
           "padding-left: 10px;"
           "color: #566d8e";
}

QString StyleHelper::getFieldStyle() {
    return "background-color: #afc0da;"
           "border: none;"
           "border-radius: 20px;"
           "font-size: 14px;"
           "padding-left: 10px;"
           "color: #566d8e";
}

QString StyleHelper::getLeftSlideStyle() {
    return "QPushButton {"
           "image: url(:/courtcasewindow/images/courtcasewindow/left-slide.png);"
           "border: none;"
           "}"
           "QPushButton:hover {"
           "image: url(:/courtcasewindow/images/courtcasewindow/left-slide-hover.png);"
           "border: none;"
           "}";
}

QString StyleHelper::getRightSlideStyle() {
    return "QPushButton {"
           "image: url(:/courtcasewindow/images/courtcasewindow/right-slide.png);"
           "border: none;"
           "}"
           "QPushButton:hover {"
           "image: url(:/courtcasewindow/images/courtcasewindow/right-slide-hover.png);"
           "border: none;"
           "}";
}
