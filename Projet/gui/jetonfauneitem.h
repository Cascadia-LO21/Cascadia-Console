#ifndef JETONFAUNEITEM_H
#define JETONFAUNEITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "jeton_faune.h"

class JetonFauneItem : public QGraphicsPixmapItem
{
public:
    explicit JetonFauneItem(QGraphicsItem *parent = nullptr);

    void setJetonFaune(const JetonFaune& jeton);
    void setVisible(bool visible);

private:
    void loadJetonImage();
    QString getImagePath() const;

    JetonFaune m_jeton;
    bool m_isVisible;

    static constexpr int SIZE = 60;
};

#endif // JETONFAUNEITEM_H
