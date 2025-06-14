#include "save.h"
#include <fstream>
#include <iostream>
#include <istream>

template<typename Enum>
void saveEnum(std::ostream& os, Enum e) {
    int val = static_cast<int>(e);
    os.write(reinterpret_cast<const char*>(&val), sizeof(val));
}
template<typename Enum>
void loadEnum(std::istream& is, Enum& e) {
    int val;
    is.read(reinterpret_cast<char*>(&val), sizeof(val));
    e = static_cast<Enum>(val);
}

void save(std::ostream& os, const Position& pos) {
    int q = pos.getQ();
    int r = pos.getR();
    int s = pos.getS();
    os.write(reinterpret_cast<const char*>(&q), sizeof(q));
    os.write(reinterpret_cast<const char*>(&r), sizeof(r));
    os.write(reinterpret_cast<const char*>(&s), sizeof(s));
}

void load(std::istream& is, Position& pos) {
    int q, r, s;
    is.read(reinterpret_cast<char*>(&q), sizeof(q));
    is.read(reinterpret_cast<char*>(&r), sizeof(r));
    is.read(reinterpret_cast<char*>(&s), sizeof(s));
    pos.setQ(q);
    pos.setR(r);
    pos.setS(s);
}


void save(std::ostream& os, const JetonFaune& jf) {
    saveEnum(os, jf.getType());
}

void load(std::istream& is, JetonFaune& jf) {
    Faune t;
    loadEnum(is, t);
    jf.setType(t);
}

void save(std::ostream& os, const Tuile& t) {
    // Habitats
    for (const auto& h : t.getHabitats()) saveEnum(os, h);

    // Faunes
    const auto& faunes = t.getFaunes();
    size_t fauneCount = faunes.size();
    os.write(reinterpret_cast<const char*>(&fauneCount), sizeof(fauneCount));
    for (const auto& f : faunes) saveEnum(os, f);

    // donneJetonNature
    bool donne = t.getDonneJetonNature();
    os.write(reinterpret_cast<const char*>(&donne), sizeof(donne));

    // optional<Faune>
    bool hasFaunePlace = t.JetonFaunePresent();
    os.write(reinterpret_cast<const char*>(&hasFaunePlace), sizeof(hasFaunePlace));
    if (hasFaunePlace) {
        Faune f = t.getFaunePlace();
        saveEnum(os, f);
    }

    // unique_ptr<Position>
    bool hasPosition = t.positionDefinie();
    os.write(reinterpret_cast<const char*>(&hasPosition), sizeof(hasPosition));
    if (hasPosition) save(os,t.getPosition());

    // placementConfirme
    bool conf = t.getPlacementConfirme();
    os.write(reinterpret_cast<const char*>(&conf), sizeof(conf));
}

void load(std::istream& is, Tuile& t) {
    // Habitats
    for (size_t i = 0; i < 6; ++i) {
        Habitat h;
        loadEnum(is, h);
        t.setHabitat(i, h);
    }

    // Faunes
    size_t fauneCount;
    is.read(reinterpret_cast<char*>(&fauneCount), sizeof(fauneCount));
    std::vector<Faune> faunes(fauneCount);
    for (size_t i = 0; i < fauneCount; ++i) {
        loadEnum(is, faunes[i]);
    }
    t.setFaunes(faunes);

    // donneJetonNature
    bool donne;
    is.read(reinterpret_cast<char*>(&donne), sizeof(donne));
    t.setDonneJetonNature(donne);

    // optional<Faune>
    bool hasFaunePlace;
    is.read(reinterpret_cast<char*>(&hasFaunePlace), sizeof(hasFaunePlace));
    if (hasFaunePlace) {
        Faune f;
        loadEnum(is, f);
        t.setFaunePlace(f);
    }
    else {
        t.setFaunePlace(std::nullopt);
    }

    // unique_ptr<Position>
    bool hasPosition;
    is.read(reinterpret_cast<char*>(&hasPosition), sizeof(hasPosition));
    if (hasPosition) {
        auto pos = std::make_unique<Position>();
        load(is, *pos);
        t.setPosition(*pos);
    }
    else {
        t.setPositionNull();
    }

    // placementConfirme
    bool conf;
    is.read(reinterpret_cast<char*>(&conf), sizeof(conf));
    if (conf) t.confirmerPlacement();
}

void save(std::ostream& os, const Pioche& p) {
    for (const auto& pair : p.getPiocheVisible()) {
        save(os, pair.first);
        save(os, pair.second);
    }
    for (const auto& vis : p.getVisibilite()) {
        os.write(reinterpret_cast<const char*>(&vis.first), sizeof(vis.first));
        os.write(reinterpret_cast<const char*>(&vis.second), sizeof(vis.second));
    }
}
void load(std::istream& is, Pioche& p) {
    auto& piocheVis = p.getPiocheVisibleForWrite();
    for (auto& pair : piocheVis) {
        load(is, pair.first);
        load(is, pair.second);
    }
    auto& vis = p.getVisibiliteForWrite();
    for (auto& v : vis) {
        is.read(reinterpret_cast<char*>(&v.first), sizeof(v.first));
        is.read(reinterpret_cast<char*>(&v.second), sizeof(v.second));
    }
}

void save(std::ostream& os, const EnvJoueur& e) {
    // Pseudo
    size_t len = e.getPseudo().size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(e.getPseudo().data(), len);

    // unordered_map<Position, Tuile>
    const auto& tuiles = e.getTuiles();
    size_t tsize = tuiles.size();
    os.write(reinterpret_cast<const char*>(&tsize), sizeof(tsize));
    for (const auto& [pos, tuile] : tuiles) {
        save(os, pos);
        save(os, tuile);
    }

    // unordered_map<Faune, unordered_set<Position>>
    const auto& mapJetons = e.getMapPositionsJetons();
    size_t fsize = mapJetons.size();
    os.write(reinterpret_cast<const char*>(&fsize), sizeof(fsize));
    for (const auto& [faune, setPos] : mapJetons) {
        saveEnum(os, faune);
        size_t spsize = setPos.size();
        os.write(reinterpret_cast<const char*>(&spsize), sizeof(spsize));
        for (const auto& pos : setPos) save(os, pos);
    }

    // nbJetonNature
    unsigned int nbJetonNature = e.getNbJetonsNature();
    os.write(reinterpret_cast<const char*>(&nbJetonNature), sizeof(nbJetonNature));

    // optional<Position>
    bool hasDernierePosition = e.getDernierePosition().has_value();
    os.write(reinterpret_cast<const char*>(&hasDernierePosition), sizeof(hasDernierePosition));
    if (hasDernierePosition) save(os, e.getDernierePosition().value());

    // optional<Tuile>
    bool hasDerniereTuile = e.getDerniereTuile().has_value();
    os.write(reinterpret_cast<const char*>(&hasDerniereTuile), sizeof(hasDerniereTuile));
    if (hasDerniereTuile) save(os, e.getDerniereTuile().value());

    // placementEnAttente
    bool pe = e.getPlacementEnAttente();
    os.write(reinterpret_cast<const char*>(&pe), sizeof(pe));
}

void load(std::istream& is, EnvJoueur& e) {
    // Pseudo
    size_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    std::string pseudo(len, '\0');
    is.read(&pseudo[0], len);
    e.setPseudo(pseudo);

    // unordered_map<Position, Tuile>
    size_t tsize;
    is.read(reinterpret_cast<char*>(&tsize), sizeof(tsize));
    std::unordered_map<Position, Tuile> tuiles;
    for (size_t i = 0; i < tsize; ++i) {
        Position pos;
        Tuile tuile;
        load(is, pos);
        load(is, tuile);
        tuiles[pos] = tuile;
    }
    e.setTuiles(tuiles);

    // unordered_map<Faune, unordered_set<Position>>
    size_t fsize;
    is.read(reinterpret_cast<char*>(&fsize), sizeof(fsize));
    std::unordered_map<Faune, std::unordered_set<Position>> mapJetons;
    for (size_t i = 0; i < fsize; ++i) {
        Faune faune;
        loadEnum(is, faune);
        size_t spsize;
        is.read(reinterpret_cast<char*>(&spsize), sizeof(spsize));
        std::unordered_set<Position> setPos;
        for (size_t j = 0; j < spsize; ++j) {
            Position pos;
            load(is, pos);
            setPos.insert(pos);
        }
        mapJetons[faune] = setPos;
    }
    e.setMapPositionsJetons(mapJetons);

    // nbJetonNature
    unsigned int nbJetonNature;
    is.read(reinterpret_cast<char*>(&nbJetonNature), sizeof(nbJetonNature));
    e.setNbJetonNature(nbJetonNature);

    // optional<Position>
    bool hasDernierePosition;
    is.read(reinterpret_cast<char*>(&hasDernierePosition), sizeof(hasDernierePosition));
    if (hasDernierePosition) {
        Position pos;
        load(is, pos);
        e.setDernierePosition(pos);
    }
    else {
        e.setDernierePosition(std::nullopt);
    }

    // optional<Tuile>
    bool hasDerniereTuile;
    is.read(reinterpret_cast<char*>(&hasDerniereTuile), sizeof(hasDerniereTuile));
    if (hasDerniereTuile) {
        Tuile t;
        load(is, t);
        e.setDerniereTuile(t);
    }
    else {
        e.setDerniereTuile(std::nullopt);
    }

    // placementEnAttente
    bool pe;
    is.read(reinterpret_cast<char*>(&pe), sizeof(pe));
    e.setPlacementEnAttente(pe);
}


void save(std::ostream& os, const Partie& partie) {
    unsigned int nbJoueurs = partie.getNbJoueurs();
    os.write(reinterpret_cast<const char*>(&nbJoueurs), sizeof(nbJoueurs));

    unsigned int compteurTour = partie.getCompteurTour();
    os.write(reinterpret_cast<const char*>(&compteurTour), sizeof(compteurTour));

    const auto& joueurs = partie.getJoueurs();
    size_t nbJoueursVec = joueurs.size();
    os.write(reinterpret_cast<const char*>(&nbJoueursVec), sizeof(nbJoueursVec));
    for (const auto& joueur : joueurs) save(os, joueur);

    unsigned int joueurCourant = partie.getIndexJoueurCourant();
    os.write(reinterpret_cast<const char*>(&joueurCourant), sizeof(joueurCourant));

    bool pause = partie.getPause();
    os.write(reinterpret_cast<const char*>(&pause), sizeof(pause));

    // unique_ptr<Pioche>
    auto hasPioche = partie.getPioche();
    os.write(reinterpret_cast<const char*>(&hasPioche), sizeof(hasPioche));
    //if (hasPioche != nullptr) 
    save(os, hasPioche);

    saveEnum(os, partie.getMarquage());
    saveEnum(os, partie.getVariante());
}

void load(std::istream& is, Partie& partie) {
    unsigned int nbJoueurs;
    is.read(reinterpret_cast<char*>(&nbJoueurs), sizeof(nbJoueurs));
    // Si tu as un setter pour nbJoueurs, utilise-le

    unsigned int compteurTour;
    is.read(reinterpret_cast<char*>(&compteurTour), sizeof(compteurTour));
    // Si tu as un setter pour compteurTour, utilise-le

    size_t nbJoueursVec;
    is.read(reinterpret_cast<char*>(&nbJoueursVec), sizeof(nbJoueursVec));
    std::vector<EnvJoueur> joueurs(nbJoueursVec);
    for (auto& joueur : joueurs) load(is, joueur);
    partie.setJoueurs(joueurs);

    unsigned int joueurCourant;
    is.read(reinterpret_cast<char*>(&joueurCourant), sizeof(joueurCourant));
    partie.setJoueurCourant(joueurCourant);

    bool pause;
    is.read(reinterpret_cast<char*>(&pause), sizeof(pause));
    partie.setPause(pause);

    // unique_ptr<Pioche>
    bool hasPioche;
    is.read(reinterpret_cast<char*>(&hasPioche), sizeof(hasPioche));
    if (hasPioche) {
        auto pioche = std::make_unique<Pioche>();
        load(is, *pioche);
        partie.setPioche(std::move(pioche));
    }
    else {
        partie.setPioche(nullptr);
    }

    Marquage marquage;
    loadEnum(is, marquage);
    partie.setMarquage(marquage);

    Variante variante;
    loadEnum(is, variante);
    partie.setVariante(variante);
}
