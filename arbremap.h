/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Automne 2016 | TP2
    http://ericbeaudry.ca/INF3105/tp2/
*/
#if !defined(__ARBREMAP__H__)
#define __ARBREMAP__H__

#include "arbreavl.h"

template <class K, class V>
class ArbreMap{
    class Entree{
        public:
            Entree(const K& c):cle(c),valeur(){}
            K cle;
            V valeur;
            bool operator< (const Entree& e) const {return cle < e.cle;}
            bool operator> (const Entree& e) const {return cle > e.cle;}
            bool operator== (const Entree& e) const {return cle == e.cle;}
    };
    ArbreAVL<Entree> entrees;
  public:
      class Iterateur{
        public:
          Iterateur(ArbreMap& a) : iter(a.entrees.debut()) {}
          Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iter(i) {}
          operator bool() const {return iter.operator bool();};
          Iterateur& operator++() {++iter; return *this;}
          const K& cle() const {return (*iter).cle;}
          V& valeur() {return (V&) (*iter).valeur;}
        private:
          typename ArbreAVL<Entree>::Iterateur iter;
      };


      Iterateur debut() {return Iterateur(*this);}
      Iterateur fin() {return Iterateur(entrees.fin());}
      Iterateur rechercher(const K& cle) {return Iterateur(entrees.rechercher(cle));}

      bool contient(const K&) const;
      void enlever(const K&);
      void vider();
      bool vide() const;

      const V& operator[] (const K&) const;
      V& operator[] (const K&);
};



template <class K, class V>
void ArbreMap<K,V>::vider(){

     entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{

    return entrees.vide();
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    // À compléter
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    Entree entree(c);
    return entrees.contient(entree);
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(c);
    return entrees[iter].valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c)
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(c);
    if(!iter){
      entrees.inserer(Entree(c));
      iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}


#endif
