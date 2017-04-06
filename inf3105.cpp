
#include "DocumentXML.h"
#include "Histoire.h"
#include "arbreavl.h"
#include "arbremap.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

vector< Histoire *> * lireDocuments(string a_nomFichier);
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> * foret);
int getOccurences(string mot, int index, ArbreMap<string, int> * foret);
void prompt();

int main() {
    vector<Histoire *> *histoires = lireDocuments(string("listeDocument.xml"));
    ArbreMap<string, int> *foret = new ArbreMap<string, int>[histoires->size()];
    int nbHistoires;
    nbHistoires = histoiresToArbre(histoires, foret); // Permet de faire le transfert des mots des histoires dans leurs arbre respectif.
                                                      // Renvoie le nombre d'histoire
    cout << "Occurences de The :" << getOccurences("ajajajajaj", 2, foret) << endl;
    delete [] foret;
    prompt();
	  return 0;
}

/************************************************
 *                    FONCTIONS                 *
 ***********************************************/
/**
 * [histoiresToArbre Fonction permettant de faire le transfert d'un vecteur
 * d'histoires a des arbres]
 * @param  histoires [Vecteur d'histoire a transferer]
 * @param  foret     [Arbre dans lequel on y mettra d'autres arbres contenant les mots]
 * @return           [Le nombre d'histoires transferee]
 */
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> * foret){
      vector<string>::const_iterator iter;
      int nbHistoires = 0;
      for( Histoire * histoire : * histoires ) {
        foret[nbHistoires] = ArbreMap<string, int>();
        for(iter = histoire->begin(); iter != histoire->end(); ++iter){
            string temp = *iter;
            temp[0] = tolower(temp[0]);
            foret[nbHistoires][temp]++;
        }
        ++nbHistoires;
    }
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
     int nbOccurences = -1;
     if (sousArbre->contient(mot)){
       ArbreMap<string, int>::Iterateur it = sousArbre->rechercher(mot);
       nbOccurences = it.valeur();
     }
     return nbOccurences;
}

void prompt(){
    string input;
    cout << "Recherche : ";
    getline(cin, input);
    while(input != "exit"){
        cout << "Recherche : ";
        getline(cin, input);
    }
    cout << "Fin du programme." << endl;
}

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
