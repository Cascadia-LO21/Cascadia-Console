#ifndef PIOCHEITEM_H
#define PIOCHEITEM_H

#include <QGraphicsItemGroup>
#include <array>
#include "pioche.h"
#include "jetonfauneitem.h"

class PiocheItem : public QGraphicsItemGroup
{
public:
    explicit PiocheItem(QGraphicsItem *parent = nullptr);

    void setPioche(const Pioche& pioche);

private:
    void setupLayout();
    void updateDisplay();

    Pioche m_pioche;
    std::array<JetonFauneItem*, 4> m_jetonItems;//4 taille officielle par défaut de la pioche

    static constexpr int SPACING = 10;
    static constexpr int JETON_SIZE = 60;
};

#endif // PIOCHEITEM_H
