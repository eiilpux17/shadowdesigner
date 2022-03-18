#ifndef VISUALIZATIONVIEW_H
#define VISUALIZATIONVIEW_H

#include <QWidget>

class VisualizationView : public QWidget
{
    Q_OBJECT
public:
    explicit VisualizationView(QWidget *parent = nullptr);

    void setBackground(const QColor & color);
    void setFillColor(const QColor & color);
    void setShadowColor(const QColor & color);
    void setShadowRadius(int radius);
    void setTestSize(const QSize & size);
    void setShowGrid(bool show);

protected:
    void paintEvent(QPaintEvent *event);
private:
    inline void tryUpdate(){
        if(this->isVisible()){
            update();
        }
    }

private:
    QColor backgroundColor = Qt::white;
    QColor fillColor = QColor(0x36C7A7);
    QColor shadowColor = Qt::black;
    int shadowRadius = 20;
    QSize testSize;
    bool showGrid = false;
};

#endif // VISUALIZATIONVIEW_H
