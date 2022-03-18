#include "shadowfactory.h"
#include <QPainter>
#include <QtMath>

QT_BEGIN_NAMESPACE
extern void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

QImage ShadowFactory::createShadow(const QPainterPath &path, qreal radius, const QColor &shadowColor, const QColor &fillColor, qreal devicePixelRatio)
{
    // 参考rect，支持任意路径
    return QImage();
}

QImage ShadowFactory::createShadow(const QRect &rect, qreal radius, const QColor &shadowColor, const QColor &fillColor, qreal devicePixelRatio)
{
    const int _radius = qCeil(radius);
    const qreal ratio = devicePixelRatio;
    QRect targetRect = rect.adjusted(0, 0, _radius, _radius);
    QSize imageSize = QSize(targetRect.right() + 1, targetRect.bottom() + 1) * ratio; //始终假设左侧有足够的空间，以右侧为准

    QImage source(imageSize, QImage::Format_ARGB32_Premultiplied);
    source.setDevicePixelRatio(ratio);
    source.fill(0);
    QPainter painter(&source);
    painter.setPen(Qt::NoPen);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(shadowColor);
    painter.drawRect(rect);
    painter.end();

    source.setDevicePixelRatio(1.0);

    //模糊图像
    QImage target(imageSize, QImage::Format_ARGB32_Premultiplied);
    target.fill(0);
    if(radius > 0)
    {
        QPainter blurPainter(&target);
        blurPainter.setRenderHint(QPainter::Antialiasing);
        qt_blurImage(&blurPainter, source, radius * ratio, false, true);
        blurPainter.end();
    }
    target.setDevicePixelRatio(ratio);

    //上一步操作会使中心区域模糊，重新填充
    painter.begin(&target);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(target.rect(), shadowColor);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(fillColor.isValid() ? fillColor : shadowColor);
    painter.drawRect(rect);
    painter.end();
    return target;
}

QImage ShadowFactory::createShadow(const QSize &baseSize, qreal radius, const QColor &shadowColor, const QColor &fillColor, qreal devicePixelRatio)
{
    if(baseSize.isValid())
    {
        QRect rect(QPoint(0, 0), baseSize);
        const int _radius = qCeil(radius);
        rect.moveTopLeft(QPoint(_radius, _radius));
        return createShadow(rect, radius, shadowColor, fillColor, devicePixelRatio);
    }
    return QImage();
}
