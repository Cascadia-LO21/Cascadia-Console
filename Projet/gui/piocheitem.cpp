#include "piocheitem.h"

PiocheItem::PiocheItem(QGraphicsItem *parent) : QGraphicsItemGroup(parent)
{
    setupLayout();
}

void PiocheItem::setupLayout()
{
    // Create 4 jeton items for bottom row
    for (int i = 0; i < 4; ++i) {
        m_jetonItems[i] = new JetonFauneItem();

        // Position jetons in bottom row
        int x = i * (JETON_SIZE + SPACING);
        int y = JETON_SIZE + SPACING; // Bottom row
        m_jetonItems[i]->setPos(x, y);

        addToGroup(m_jetonItems[i]);
    }

    // TODO: Add tuile items to top row later
    // Top row would be at y = 0
}

void PiocheItem::setPioche(const Pioche& pioche)
{
    m_pioche = pioche;
    updateDisplay();
}

void PiocheItem::updateDisplay()
{
    // Only update if we have valid pioche data
    try {
        const auto& piocheVisible = m_pioche.getPiocheVisible();
        const auto& visibilite = m_pioche.getVisibilite();

        for (int i = 0; i < 4; ++i) {
            m_jetonItems[i]->setJetonFaune(piocheVisible[i].second);
            m_jetonItems[i]->setVisible(visibilite[i].second);
        }
    } catch (...) {
        // If pioche data is not valid, hide all jetons
        for (int i = 0; i < 4; ++i) {
            m_jetonItems[i]->setVisible(false);
        }
    }
}
