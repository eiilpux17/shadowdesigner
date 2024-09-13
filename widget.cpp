#include "widget.h"
#include "ui_widget.h"

#include <QColorDialog>
#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->fillColorEdit->setStyleSheet(" ");
    ui->areaWithBox->setValue(300);
    ui->areaHeightBox->setValue(200);
    ui->fillColorEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-fA-F0-9]{0,8}"), ui->fillColorEdit));
    ui->fillColorEdit->setText("36C7A7");


    ui->shadowRadiusBox->setValue(20);
    ui->shadowColorEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-fA-F0-9]{0,8}"), ui->shadowColorEdit));
    ui->shadowColorEdit->setText("000000");
    ui->backgroundEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-fA-F0-9]{0,8}"), ui->shadowColorEdit));
    ui->backgroundEdit->setText("ffffff");

    connect(ui->areaWithBox, SIGNAL(valueChanged(int)), this, SLOT(updateShadow()));
    connect(ui->areaHeightBox, SIGNAL(valueChanged(int)), this, SLOT(updateShadow()));
    connect(ui->fillColorEdit, SIGNAL(textChanged(QString)), this, SLOT(updateShadow()));
    connect(ui->shadowRadiusBox, SIGNAL(valueChanged(int)), this, SLOT(updateShadow()));
    connect(ui->shadowColorEdit, SIGNAL(textChanged(QString)), this, SLOT(updateShadow()));

    connect(ui->pickFillColorButton, SIGNAL(clicked(bool)), this, SLOT(pickFillColor()));
    connect(ui->pickShadowColorButton, SIGNAL(clicked(bool)), this, SLOT(pickShadowColor()));
    connect(ui->pickBackgroundButton, SIGNAL(clicked(bool)), this, SLOT(pickBackground()));

    connect(ui->showGridBox, &QCheckBox::stateChanged, [this](){
        ui->shadowVisualization->setShowGrid(ui->showGridBox->isChecked());
    });

    connect(ui->insideShadowBox, &QCheckBox::stateChanged, [this](){
        ui->shadowVisualization->setInsiderShadow(ui->insideShadowBox->isChecked());
    });

    QTimer::singleShot(0, this, SLOT(updateShadow()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateShadow()
{
    int w = ui->areaWithBox->value();
    int h = ui->areaHeightBox->value();
    ui->shadowVisualization->setTestSize(QSize(w, h));

    QColor fillColor("#" + ui->fillColorEdit->text());
    ui->shadowVisualization->setFillColor(fillColor);

    ui->shadowVisualization->setShadowRadius(ui->shadowRadiusBox->value());

    QColor shadow("#" + ui->shadowColorEdit->text());
    ui->shadowVisualization->setShadowColor(shadow);

    QColor background("#" + ui->backgroundEdit->text());
    ui->shadowVisualization->setBackground(background);
}

void Widget::pickFillColor()
{
    QColor color("#" + ui->fillColorEdit->text());
    if(!color.isValid()){
        color = Qt::white;
    }
    QColor picked = pickColor(color);
    if(picked.isValid()){
        ui->fillColorEdit->blockSignals(true);
        ui->fillColorEdit->setText(picked.name().mid(1));
        ui->fillColorEdit->blockSignals(false);
        updateShadow();
    }
}

void Widget::pickShadowColor()
{
    QColor color("#" + ui->shadowColorEdit->text());
    if(!color.isValid()){
        color = Qt::black;
    }
    QColor picked = pickColor(color);
    if(picked.isValid()){
        ui->shadowColorEdit->blockSignals(true);
        ui->shadowColorEdit->setText(picked.name().mid(1));
        ui->shadowColorEdit->blockSignals(false);
        updateShadow();
    }
}

void Widget::pickBackground()
{
    QColor color("#" + ui->backgroundEdit->text());
    if(!color.isValid()){
        color = Qt::black;
    }
    QColor picked = pickColor(color);
    if(picked.isValid()){
        ui->backgroundEdit->blockSignals(true);
        ui->backgroundEdit->setText(picked.name().mid(1));
        ui->backgroundEdit->blockSignals(false);
        updateShadow();
    }
}

QColor Widget::pickColor(const QColor &init)
{
    QColorDialog dialog(init, this);
    dialog.adjustSize();
    if(dialog.exec() == QDialog::Accepted)
    {
        return dialog.selectedColor();
    }
    return QColor();
}

