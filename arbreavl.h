/*
  INF3105 - Structures de données et algorithmes
  Squelette pour Lab6 + TP2
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include "pile.h"

using namespace std;

template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    void inserer(const T&);
    void enlever(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    int  hauteur() const;

    ArbreAVL& operator = (const ArbreAVL&);

    // Puisque les fonctions suivantes retournent des itérateurs, il faut *annoncer* la décalaration classe Iterateur
    // On ne peut pas déclarer Iterateur tout de suite, car Iterateur a besoin de Noeud.
    class Iterateur;

    Iterateur debut() const;
    Iterateur fin() const;

    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

  private:
    class Noeud;
    Noeud* racine;

    bool inserer(const T&, Noeud*&);

    bool enlever(const T& e, Noeud*& n);

    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);

    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;

    const T& max(Noeud*) const;

  public:
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;

        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;

      friend class ArbreAVL;
    };

  private:
    class Noeud{
      public:
        Noeud(const T&);
        Noeud *gauche;
        Noeud *droite;
        int equilibre;
        T contenu;
    };
};

//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c):
gauche(NULL), droite(NULL), equilibre(0), contenu(c)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() : racine(NULL)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre) : racine(NULL)
{
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(element, racine);
}

template <class T>
bool ArbreAVL<T>::inserer(const T& element, Noeud*& noeud)
{

    if(noeud==NULL)
    {
        noeud = new Noeud(element);
        return true;
    }

    if(element < noeud->contenu){
        if(inserer(element, noeud->gauche))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == 1) return true;
            assert(noeud->equilibre == 2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
                rotationGaucheDroite(noeud);
        }

        return false;
    }

    if(element > noeud->contenu){
        if(inserer(element, noeud->droite))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;
            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
                rotationDroiteGauche(noeud);
        }

        return false;
    }

    // element == noeud->contenu
    noeud->contenu = element;  // Mise à jour
    return false;
}

template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    // Vous n'aurez probablement pas besoin de cette fonction.
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->droite;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb =  -(ea < 0 ? ea : 0) + 1 + eb;
    int  nea =  ea + (neb > 0 ? neb : 0) + 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
bool ArbreAVL<T>::vide() const
{

    if(racine == NULL) {
        return true;
    }

    return false;

}

template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{

    if(noeud == NULL) {
        return;
    }

    vider(noeud->gauche);
    vider(noeud->droite);
    delete noeud;
    noeud = NULL;
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    if(source == NULL) {
        return;
    }

    noeud = new Noeud(source->contenu);
    noeud->equilibre = source->equilibre;

    if(source->gauche != NULL) {
        copier(source->gauche, noeud->gauche);
    }

    if(source->droite != NULL) {
        copier(source->droite, noeud->droite);
    }

}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{

    Iterateur iter(*this);
    iter = iter.arbre_associe.rechercher(element);

    if(iter.courant == NULL) {
        return false;
    }

    if(iter.courant->contenu == element) {
        return true;
    }

    return false;

}

template <class T>
int ArbreAVL<T>::hauteur() const{

    int compteur = 0;
    Iterateur iter(*this);
    iter.courant = this->racine;

    while(iter) {
        if(iter.courant->equilibre >= 0) {
            iter.courant = iter.courant->gauche;
            compteur += 1;
            continue;
        }

        if(iter.courant->equilibre < 0) {
            iter.courant = iter.courant->droite;
            compteur += 1;
            continue;
        }
    }

    return compteur;

}

template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{

    if(n == NULL) {
        return n->contenu;
    }

    while(n->droite != NULL) {
            n = n->droite;
    }

    return n->contenu;

}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{

    Iterateur iter(*this);

    while(iter.courant->gauche != NULL) {
        iter.chemin.empiler(iter.courant);
        iter.courant = iter.courant->gauche;
    }

    return iter;

}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{

    Iterateur iter(*this);
    iter.courant = iter.arbre_associe.racine;

    while(iter) {

        if(iter.courant->contenu == e) {
            return iter;
        }

        if(e < iter.courant->contenu) {
            if(iter.courant->gauche != NULL) {
                iter.chemin.empiler(iter.courant);
                iter.courant = iter.courant->gauche;
                continue;
            }

            return iter;
        }

        if(e > iter.courant->contenu) {
            if(iter.courant->droite != NULL) {
                iter.chemin.empiler(iter.courant);
                iter.courant = iter.courant->droite;
                continue;
            }

            return iter;
        }

    }

    return iter;

}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    iter = rechercher(e);

    if(iter.courant == NULL) {
        return iter;
    }

    if(iter.courant->contenu == e) {
        return iter;
    }

    iter++;
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Iterateur iter(*this);
    iter = rechercher(e);

    if(iter.courant == NULL) {
        return iter;
    }

    if(iter.courant->contenu == e) {
        return iter;
    }

    while(iter) {
        iter.courant = iter.chemin.depiler();

        if(iter.courant->contenu < e) {
            return iter;
        }

    }

    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    vider();
    copier(autre.racine, racine);
    return *this;
}

//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    T valeurActuelle;

    valeurActuelle = this->courant->contenu;

    if(this->courant->droite == NULL){

        while(this->courant->contenu <= valeurActuelle) {

            if(this->chemin.vide()) {
                //return NULL;
            } else {
                this->courant = this->chemin.depiler();
            }

        }
        return *this;
    }

    if(this->courant->droite != NULL) {
        this->chemin.empiler(this->courant);
        this->courant = this->courant->droite;

        while(this->courant->gauche != NULL) {
            this->chemin.empiler(this->courant);
            this->courant = this->courant->gauche;
        }

        return *this;
    }

    return *this;

}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif


