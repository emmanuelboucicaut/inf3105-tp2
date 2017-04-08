
#include "DocumentXML.h"
#include "Histoire.h"
#include "arbreavl.h"
#include "arbremap.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <cmath>
using namespace std;
///salut mon Étienne.
void calculRequete(ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres, vector<string> *mots, int nbHistoires, int nombreDeFois);
void calculerIDF(int nbHistoires, ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret);
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> * foret, ArbreMap<int, string> *titres);
int getOccurences(string mot, int index, ArbreMap<string, int> * foret);
int prompt(vector<string> *pile);
void afficherPlusGrand(ArbreMap<string, double> *fin, int nombreDeFois);
vector< Histoire *> * lireDocuments(string a_nomFichier);

int main() {
    vector<Histoire *> *histoires = lireDocuments(string("listeDocument.xml"));
    ArbreMap<string, int> *foret = new ArbreMap<string, int>[histoires->size()];
    ArbreMap<string, double> *arbreIDF = new ArbreMap<string, double>();
    ArbreMap<int, string> *titres = new ArbreMap<int, string>();     //Il est pertinant d'avoir un Tree Map [indexe:titre] car il faut garder trace des titres pour le output.
                                                                    // je le créer dans 'histoiresToArbre'...juste besoin d'itérer une fois dans histoire yessssssssss.
                                                                    //Good ole' tree map.
                                                                    //Gimme some apples you fuckin tree
                                                                    //its getting late jme trouve drole ak mes comments
    int nbHistoires = histoiresToArbre(histoires, foret, titres);
    calculerIDF(nbHistoires, arbreIDF, foret);
    vector<string> test;
    while (prompt(&test)){
      calculRequete(arbreIDF, foret, titres, &test, nbHistoires, 5);
    }
    delete [] foret;
    delete arbreIDF;

	return 0;
}



void afficherPlusGrand(ArbreMap<string, double> * fin, int nombreDeFois){
     double max = 0, maxAbs = 0;
     string str;
     for (int i = 0 ; i < nombreDeFois; ++i){
         for (ArbreMap<string,double>::Iterateur iter = fin->debut(); iter ; ++iter){
           if (iter.valeur() > max && i == 0){
               max = iter.valeur();
               str = iter.cle();
           } else if (iter.valeur() > max && iter.valeur() < maxAbs){
               max = iter.valeur();
               str = iter.cle();
           }
         }
         if (max != 0) cout << max << " : " << str << endl;
         maxAbs = max;
         max = 0;
    }
}
/************************************************
 *                    FONCTIONS                 *
 ***********************************************/

//Elle est fonctionnelle et semble bien fonctionner...sauf que certains résultats sont différents de ceux du prof...je dois continuer de vérifier pourquoi.
//affiche pas encore les 5 résultats...j'affiche juste les mises a jours de somme. Which is alright for debugging soo leave it that way plzzzzz.
//permet juste de calculer uen requete avec UN SEUL MOT.
void calculRequete(ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres, vector <string> *mots, int nbHistoires, int nombreDeFois){
    string mot;
    string titre;
    ArbreMap<string, double> temp;
    double idf = 0, total = 0;
    int tf = 0;
    for (int i = 0; i < nbHistoires; ++i){
      for (int y = 0; y < mots->size(); ++y){
          idf = arbreIDF->operator[]((*mots)[y]);
          tf = getOccurences((*mots)[y], i, foret);
          total += idf * tf;
      }
     // cout << total << " : " << titres->operator[](i) << endl;
      temp[titres->operator[](i)] = total;
      total = 0;
    }
    afficherPlusGrand(&temp, nombreDeFois);
    mots->clear();
}
/**
 * [calculerIDF description]
 * @param nbHistoires [description]
 * @param arbreIDF    [description]
 * @param foret       [description]
 */
void calculerIDF(int nbHistoires, ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret){
    ArbreMap<string, int> temp;
    for(int i = 0; i < nbHistoires; ++i){
        ArbreMap<string, int> *sousArbre = &foret[i];
        for(ArbreMap<string, int>::Iterateur it = sousArbre->debut(); it; ++it){
            temp[it.cle()]++;
        }
    }
    for(ArbreMap<string, int>::Iterateur it = temp.debut(); it; ++it){
        arbreIDF->operator[](it.cle()) = log2((double)nbHistoires / it.valeur());
    }
}

/**
 * [histoiresToArbre Fonction permettant de faire le transfert d'un vecteur
 * d'histoires a des arbres]
 * @param  histoires [Vecteur d'histoire a transferer]
 * @param  foret     [Arbre dans lequel on y mettra d'autres arbres contenant les mots]
 * @return           [Le nombre d'histoires transferee]
 */
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres){
      vector<string>::const_iterator iter;
      int nbHistoires = 0;
      int indexe = 0;
      for( Histoire * histoire : * histoires ) {
        titres->operator[](indexe) = histoire->titre();
        ++indexe;
        foret[nbHistoires] = ArbreMap<string, int>();
        for(iter = histoire->begin(); iter != histoire->end(); ++iter){
            string temp = *iter;
            temp[0] = tolower(temp[0]);
            foret[nbHistoires][temp]++;
        }
        ++nbHistoires;
    }
    return nbHistoires;
}
/**
 * [getOccurences Fonction retournant le nombre d'occurence d'un mot dans un sous arbre de foret]
 * @param  mot   [Mot a rechercher]
 * @param  index [Sous arbre a chercher]
 * @param  foret [Ensemble d'arbre dans lequel l'on effectue la recherche binaire]
 * @return       [Le nombre d'occurence du mot]
 * ----------------------------------------------TO-DO --------------------------------------------
 * je sais que la fonction cause probleme si l'on met un mot qu'il n'y a pas dans la foret. C'est donc
 * une fonction a retravailler
 */
int getOccurences(string mot, int index, ArbreMap<string, int> * foret){
     ArbreMap<string, int> *sousArbre = &foret[index];
     ArbreMap<string, int>::Iterateur iter = sousArbre->debut();
     int nbOccurences = -1;
     if (sousArbre->contient(mot)) {
       ArbreMap<string, int>::Iterateur it = sousArbre->rechercher(mot);
       nbOccurences = it.valeur();
      }
     return nbOccurences;
}
/**
 * [prompt Prend une pile de string a remplir en parametre]
 * @param  mots [Pile de Strings]
 * @return      [Renvoie 0 si la pile est vide, 1 sinon]
 */
int prompt(vector<string> *mots){
  string temp;
  string sousChaine;
  int fin;
  cout << "requete : ";
  if (getline(cin, temp) && temp.length() > 0){
       for(int i = 0; i < temp.length(); ++i){
         if (isalpha(temp.at(i)) || temp.at(i) == '-'){
           sousChaine += temp.at(i);
         } else if (!sousChaine.empty()){
           mots->push_back(sousChaine);
           sousChaine.clear();
         }
       }
       mots->push_back(sousChaine);

  }
  return (!mots->empty());
}
/**
 * Fonctions fournies pas le professeur
 */
vector< Histoire *> * lireDocuments( string a_nomFichier) {
    vector<Histoire *> * histoires = new vector< Histoire * >();
    DocumentXML * listeFichiers = lireFichierXML( a_nomFichier );
    Element * courrant = listeFichiers->racine();
    vector< Contenu * >::const_iterator it = courrant->begin();
    // trouver <liste>
    while( ! ( * it )->estElement() ) ++ it;
    courrant = ( Element * )( * it );

    for( Contenu * contenu : * courrant ) {
        if( contenu->estElement() ) {
            Element * element = ( Element * )contenu;
            DocumentXML * doc = lireFichierXML( element->attribut( string( "fichier" ) ) );
            vector<Histoire *> * h = extraireHistoires( * doc );
            histoires->insert( histoires->end(), h->begin(), h->end() );

        }
    }

    return histoires;
}
