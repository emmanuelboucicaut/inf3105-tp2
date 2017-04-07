
#include "DocumentXML.h"
#include "Histoire.h"
#include "arbreavl.h"
#include "arbremap.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;
///salut mon Étienne.
void calculRequete(ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres, Pile<string> *mots, int nbHistoires);
void calculerIDF(int nbHistoires, ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret);
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> * foret, ArbreMap<int, string> *titres);
int getOccurences(string mot, int index, ArbreMap<string, int> * foret);
void prompt();
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

    //cout << "Occurences TEST :" << getOccurences("brain", 1, foret) << endl;
    calculerIDF(nbHistoires, arbreIDF, foret);

    Pile<string> test;
    test.empiler("space");           ////j'ai créé une pile parce j'aime bin ça les piles...Structure pour stocker mes string de test pour les calculs...
                                        ///je propose qu'on mette les string séparées de l'utilisateur dans une Pile également...Si t'es d'accord
    calculRequete(arbreIDF, foret, titres, &test, nbHistoires);

    prompt();
    delete [] foret;
    delete arbreIDF;

	return 0;
}

/************************************************
 *                    FONCTIONS                 *
 ***********************************************/

//Elle est fonctionnelle et semble bien fonctionner...sauf que certains résultats sont différents de ceux du prof...je dois continuer de vérifier pourquoi.
//affiche pas encore les 5 résultats...j'affiche juste les mises a jours de somme. Which is alright for debugging soo leave it that way plzzzzz.
//permet juste de calculer uen requete avec UN SEUL MOT.
void calculRequete(ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres, Pile<string> *mots, int nbHistoires){
    string mot;
    string titre;
    while(!mots->vide()){
        double somme = 0;
        int tf = 0;
        double idf;
        mot = mots->depiler();
        idf = arbreIDF->operator[](mot);
        for(int index = 0; index < nbHistoires; ++index){
            tf = getOccurences(mot, index, foret);
            if( (tf * idf) >= somme){
                //titre = foret[index].titre()
                somme = tf * idf;
                cout << somme << " : " << titres->operator[](index) << endl;
            }
        }
    }
}

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
     if (sousArbre->contient(mot)){
       ArbreMap<string, int>::Iterateur it = sousArbre->rechercher(mot);
       nbOccurences = it.valeur();
     }
     return nbOccurences;
}

void prompt(){
    string input;
    cout << "Recherche : ";
    while(getline(cin, input) && input != "exit"){
        cout << "Recherche : ";
    }
    cout << endl << "Fin du programme." << endl;
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
