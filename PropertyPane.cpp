#include "PropertyPane.h"
#include <QLineEdit>
#include <QCheckBox>
#include <sdf/Scene.h>
#include <QPushButton>
#include <QtWidgets/QColorDialog>
#include "sdf/IncludedElement.h"

using namespace std;

PropertyPane::PropertyPane(QWidget *parent) : QWidget(parent) {
    layout = new QFormLayout;

    setLayout(layout);
}

void setButtonBackground(QPushButton *button, QColor color) {
    QString stylesheet = "QPushButton {background-color: rgba(";
    stylesheet += std::to_string(color.red()).c_str();
    stylesheet += ", ";
    stylesheet += std::to_string(color.green()).c_str();
    stylesheet += ", ";
    stylesheet += std::to_string(color.blue()).c_str();
    stylesheet += ", ";
    stylesheet += std::to_string(color.alpha()).c_str();
    stylesheet += ");}";
    button->setStyleSheet(stylesheet);
}

QColor QColorFromSDFColor(const Color &color) {
    return QColor{static_cast<int>(color.r * 255),
                  static_cast<int>(color.g * 255),
                  static_cast<int>(color.b * 255),
                  static_cast<int>(color.a * 255)};
}

Color SDFColorFromQColor(const QColor &color) {
    return Color(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
}

void PropertyPane::showProperties(SDFElement *element) {
    clearChildren();

    IncludedElement *includedElement;
    if( (includedElement = dynamic_cast<IncludedElement*>(element)) != nullptr) {
        QLineEdit *nameLineEdit = new QLineEdit{includedElement->name.value_or(string{""}).c_str()};
        connect(nameLineEdit, &QLineEdit::textChanged, [includedElement](const QString &text){
            includedElement->name = text.toStdString();
        });

        QLineEdit *uriLineEdit = new QLineEdit{includedElement->uri.c_str()};
        connect(uriLineEdit, &QLineEdit::textChanged, [includedElement](const QString &text){
            includedElement->uri = text.toStdString();
        });

        QCheckBox *staticCheckBox = new QCheckBox;
        staticCheckBox->setChecked(includedElement->isStatic.value_or(false));
        connect(staticCheckBox, &QCheckBox::stateChanged, [includedElement,staticCheckBox]() {
            includedElement->isStatic = staticCheckBox->isChecked();
        });

        layout->addRow("Name", nameLineEdit);
        layout->addRow("URI", uriLineEdit);
        layout->addRow("Static", staticCheckBox);
    }

    Scene *scene;
    if( (scene = dynamic_cast<Scene*>(element)) != nullptr) {
        QPushButton *ambientButton = new QPushButton;
        ambientButton->setFlat(true);
        setButtonBackground(ambientButton, QColorFromSDFColor(scene->ambient));
        connect(ambientButton, &QPushButton::clicked, [scene,ambientButton,this](){
            auto chosenColor = QColorDialog::getColor(QColorFromSDFColor(scene->ambient),this,"Select Ambient Color");
            if(chosenColor.isValid()) {
                scene->ambient = SDFColorFromQColor(chosenColor);
                setButtonBackground(ambientButton, chosenColor);
            }
        });

        QPushButton *backgroundButton = new QPushButton;
        backgroundButton->setFlat(true);
        setButtonBackground(backgroundButton, QColorFromSDFColor(scene->background));
        connect(backgroundButton, &QPushButton::clicked, [scene,backgroundButton,this](){
            auto chosenColor = QColorDialog::getColor(QColorFromSDFColor(scene->background),this,"Select Background Color");
            if(chosenColor.isValid()) {
                scene->background = SDFColorFromQColor(chosenColor);
                setButtonBackground(backgroundButton, chosenColor);
            }
        });

        QCheckBox *shadowsCheckBox = new QCheckBox;
        shadowsCheckBox->setChecked(scene->shadows);
        connect(shadowsCheckBox, &QCheckBox::stateChanged, [scene,shadowsCheckBox]() {
            scene->shadows = shadowsCheckBox->isChecked();
        });

        QCheckBox *gridCheckBox = new QCheckBox;
        gridCheckBox->setChecked(scene->show_grid);
        connect(gridCheckBox, &QCheckBox::stateChanged, [scene,gridCheckBox]() {
            scene->show_grid = gridCheckBox->isChecked();
        });

        QCheckBox *originCheckBox = new QCheckBox;
        originCheckBox->setChecked(scene->show_origin);
        connect(originCheckBox, &QCheckBox::stateChanged, [scene,originCheckBox]() {
            scene->show_origin = originCheckBox->isChecked();
        });

        layout->addRow("Ambient", ambientButton);
        layout->addRow("Background", backgroundButton);
        layout->addRow("Shadows", shadowsCheckBox);
        layout->addRow("Show Grid", gridCheckBox);
        layout->addRow("Show Origin", originCheckBox);
    }
}

void PropertyPane::clearChildren() {
    for(QWidget *child : findChildren<QWidget*>()) {
        delete child;
    }
}
