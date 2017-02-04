#include "PropertyPane.h"
#include <QLineEdit>
#include <QCheckBox>
#include "sdf/IncludedElement.h"

using namespace std;

PropertyPane::PropertyPane(QWidget *parent) : QWidget(parent) {
    layout = new QFormLayout;

    setLayout(layout);

    QWidget *blankWidget = nullptr;
    layout->addRow("Properties", blankWidget);

}

void PropertyPane::showProperties(SDFElement *element) {
    clearChildren();
    QWidget *blankWidget = nullptr;
    layout->addRow("Properties", blankWidget);

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
}

void PropertyPane::clearChildren() {
    for(QWidget *child : findChildren<QWidget*>()) {
        delete child;
    }
}
