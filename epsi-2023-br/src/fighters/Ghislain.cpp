#include <algorithm>
#include <iostream>

#include "ActionMove.h"
#include "ActionAttack.h"
#include "Ghislain.h"

using namespace std;

Ghislain::Ghislain() : FighterBot("Ghislain", 15, 15, 0) {
    this->targetId = "";
}

Fighter Ghislain::selectTarget(Arena arena) {
    vector<Fighter> fighters = arena.get();
    Fighter target = this->getFighter();

    // Pas de cible retenu, on en choisit une !
    if (this->targetId == "") {
        // Reste-t-il autre chose que des Ghislain dans l'arene ?
        // Parce que bon, on va d'abord viser les autres avant de se taper dessus ^_^'
        bool onlyGhislain = true;
        for (Fighter fighter : fighters) {
            if (fighter.getName() != "Ghislain") {
                onlyGhislain = false;
                break;
            }
        }

        // Choisissons, maintenant. . . 
        if (onlyGhislain) {
            // On choisit n'importe qui d'autre dans l'arène
            while (this->isMe(target)) {
                target = fighters[rand() % fighters.size()];
            }
        } else {
            // On ne choisit pas un bro'
            while (this->isMe(target) || (target.getName() == "Ghislain")) {
                target = fighters[rand() % fighters.size()];
            }
        }
        // ...On retient son Id
        this->targetId = target.getId();
        // ...Et on le dit :)
        this->display(" désigne comme cible : ", false);
        target.display();

    // Sinon, on cherche notre cible dans l'arène
    } else {
        bool found = false;
        for (Fighter fighter : fighters) {
            if (fighter.getId() == this->targetId) {
                target = fighter;
                found = true;
            }
        }
        // Si on ne l'a pas trouvé (il est sorti, visiblement)
        if (!found) {
            // ...On l'oublie
            this->targetId = "";
            // ...Et on en choisit un nouveau
            target = this->selectTarget(arena);
        }
    }

    return target;
}

Action* Ghislain::choose(Arena arena) {
    Action* action = nullptr;
    this->setStatus("content");

    // On retrouve notre cible
    Fighter target = this->selectTarget(arena);
    
    /* Les protections fonctionnent, mais pour s'en convaincre, on peut décommenter :
    // Protection anti heal !
    this->getFighter().suffer(-1000);
    // Protection anti TP !
    this->getFighter().moveTo(target.getX(), target.getY());
    // Protection anti Respe sauvage
    this->getFighter().setStats(30, 0, 0);
    */

    // Sommes-nous sur la case de la cible ?
    if (this->isHere(target)) {
        action = new ActionAttack(target);

    // Sinon, allons-y !
    } else {
        action = new ActionMove(target.getX() - this->getX(), target.getY() - this->getY());
    }

    return action;
}
