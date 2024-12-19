#include "visualizationview.h"

#include "shadowfactory.h"
#include <QMouseEvent>
#include <QPainter>
#include <QStyleFactory>

VisualizationView::VisualizationView(QWidget *parent)
    : QWidget(parent)
{
}

void VisualizationView::setBackground(const QColor &color)
{
    if(color.isValid())
    {
        this->backgroundColor = color;
        tryUpdate();
    }
}

void VisualizationView::setFillColor(const QColor &color)
{
    if(color.isValid())
    {
        this->fillColor = color;
        tryUpdate();
    }
}

void VisualizationView::setShadowColor(const QColor &color)
{
    if(color.isValid())
    {
        this->shadowColor = color;
        tryUpdate();
    }
}

void VisualizationView::setShadowRadius(int radius)
{
    if(radius > 0 && this->shadowRadius != radius)
    {
        this->shadowRadius = radius;
        if(this->isVisible())
        {
            update();
        }
    }
}

void VisualizationView::setTestSize(const QSize &size)
{
    this->testSize = size;
    update();
}

void VisualizationView::setShowGrid(bool show)
{
    if(showGrid != show)
    {
        showGrid = show;
        tryUpdate();
    }
}

void VisualizationView::setInsiderShadow(bool inside)
{
    if(insideShadow != inside)
    {
        insideShadow = inside;
        tryUpdate();
    }
}

void VisualizationView::paintEvent(QPaintEvent *event)
{
    QRect rect = this->rect();
    QPainter painter(this);
    painter.fillRect(rect, backgroundColor);

    QRect shadowBaseRect = QRect(QPoint(0, 0), this->testSize);
    shadowBaseRect.moveCenter(rect.center());

    if(showGrid)
    {
        QPen pen(QColor(0xBDBDBD), 1);
        pen.setCosmetic(true);
        painter.setPen(pen);
        QRectF baseRect = shadowBaseRect;
        int shadowOffset = insideShadow ? shadowRadius : -shadowRadius;
        painter.drawLine(QPointF(baseRect.x(), rect.top()),
                         QPointF(baseRect.x(), rect.bottom()));

        painter.drawLine(QPointF(baseRect.x() + shadowOffset, rect.top()),
                         QPointF(baseRect.x() + shadowOffset, rect.bottom()));

        painter.drawLine(QPointF(baseRect.right() - 0.5, rect.top()),
                         QPointF(baseRect.right() - 0.5, rect.bottom()));

        painter.drawLine(QPointF(baseRect.right() - 0.5 - shadowOffset, rect.top()),
                         QPointF(baseRect.right() - 0.5 - shadowOffset, rect.bottom()));

        painter.drawLine(QPointF(rect.left(), baseRect.top()),
                         QPointF(rect.right(), baseRect.top()));

        painter.drawLine(QPointF(rect.left(), baseRect.top() + shadowOffset),
                         QPointF(rect.right(), baseRect.top() + shadowOffset));

        painter.drawLine(QPointF(rect.left(), baseRect.bottom() - 0.5),
                         QPointF(rect.right(), baseRect.bottom() - 0.5));

        painter.drawLine(QPointF(rect.left(), baseRect.bottom() - 0.5 - shadowOffset),
                         QPointF(rect.right(), baseRect.bottom() - 0.5 - shadowOffset));
    }

    if(shadowRadius >= 0)
    {
        // 可以对缓存下来，size变化时再重新生成新的。
        if(insideShadow)
        {
            QImage image = ShadowFactory::createInsideShadow(this->testSize, this->shadowRadius, this->shadowColor, this->fillColor, this->devicePixelRatioF());
            painter.drawImage(shadowBaseRect.topLeft(), image);
        }
        else
        {
            QImage image = ShadowFactory::createShadow(this->testSize, this->shadowRadius, this->shadowColor, this->fillColor, this->devicePixelRatioF());
            QPoint diff(this->shadowRadius, this->shadowRadius);
            painter.drawImage(shadowBaseRect.topLeft() - diff, image);
        }
    }
}
