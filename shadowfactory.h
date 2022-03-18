#ifndef SHADOWFACTORY_H
#define SHADOWFACTORY_H

#include <QColor>
#include <QImage>
#include <QRect>

class ShadowFactory
{
  public:
    static QImage createShadow(const QPainterPath &path, qreal radius, const QColor &shadowColor, const QColor &fillColor = QColor(), qreal devicePixelRatio = 1.0);
    static QImage createShadow(const QRect &rect, qreal radius, const QColor &shadowColor, const QColor &fillColor = QColor(), qreal devicePixelRatio = 1.0);
    static QImage createShadow(const QSize &baseSize, qreal radius, const QColor &shadowColor, const QColor &fillColor = QColor(), qreal devicePixelRatio = 1.0);
};

#endif // SHADOWFACTORY_H
