#include "jetonfauneitem.h"

JetonFauneItem::JetonFauneItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_isVisible(true)
{
}

void JetonFauneItem::setJetonFaune(const JetonFaune& jeton)
{
    m_jeton = jeton;
    loadJetonImage();
}

void JetonFauneItem::setVisible(bool visible)
{
    m_isVisible = visible;
    QGraphicsPixmapItem::setVisible(visible);//to make it invisible if needed
}

void JetonFauneItem::loadJetonImage()
{
    QString imagePath = getImagePath();
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            pixmap = pixmap.scaled(SIZE, SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            setPixmap(pixmap);
        }
    }
}

QString JetonFauneItem::getImagePath() const
{
    switch (m_jeton.getType()) {
    case Faune::ours:
        return ":/tokens/bear.png";
    case Faune::wapiti:
        return ":/tokens/elk.png";
    case Faune::saumon:
        return ":/tokens/salmon.png";
    case Faune::buse:
        return ":/tokens/hawk.png";
    case Faune::renard:
        return ":/tokens/fox.png";
    default:
        return QString();
    }
}
