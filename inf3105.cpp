/*
   Travail pratique 2
   COURS : INF3105
   Enseignant : Bruno Malenfant
   Etudiants :
    Etienne Carrier :: CARE28119204
    Jean-Lou Hallé  ::
   Date : 2017-04-13
 */
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

typedef struct Node {
        double valeur;
        string cle;
} Node;
bool commentTrier(Node a, Node b) {
        bool trie;
        trie = a.valeur > b.valeur;
        return trie;
}


void calculRequete(ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres, vector<string> *mots, int nbHistoires, int nombreDeFois);
void calculerIDF(int nbHistoires, ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret);
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> * foret, ArbreMap<int, string> *titres);
int getOccurences(string mot, int index, ArbreMap<string, int> * foret);
int prompt(vector<string> *pile);
void afficherPlusGrand(ArbreMap<string, double> *fin, int nombreDeFois);
vector< Histoire *> * lireDocuments(string a_nomFichier);

int main()  {
        //Vecteur contenant les pointeurs
        vector<Histoire *> *histoires = lireDocuments(string("listeDocument.xml"));
        //Arbre Map contenant les occurences pour chacuns des textes
        ArbreMap<string, int> *foret = new ArbreMap<string, int>[histoires->size()];
        //Arbre map contenant les IDF des mots
        ArbreMap<string, double> *arbreIDF = new ArbreMap<string, double>();
        //Arbre mat contenant les titres
        ArbreMap<int, string> *titres = new ArbreMap<int, string>();
        //Verifier le nombre d'histoire
        int nbHistoires = histoiresToArbre(histoires, foret, titres);
        //Calculer les IDF
        calculerIDF(nbHistoires, arbreIDF, foret);
        //Vecteur qui contient les mots entres par l'utilisatuer
        vector<string> motsUtilisateurs;
        //tant que il y a des mots
        while (prompt(&motsUtilisateurs)) {
                //Repondre par les metriques
                calculRequete(arbreIDF, foret, titres, &motsUtilisateurs, nbHistoires, 5);
        }
        delete [] foret;
        delete arbreIDF;

        return 0;
}


/**
 * [afficherPlusGrand Fonction qui permet d'affichier, en fonction du nombre d'elements demandes,
 * l'IDF les plus grands]
 * @param metrique     [Arbre contenant les metriques de tous les textes]
 * @param nombreDeFois [Nombre de metriques que l'on veut avoir ]
 */
void afficherPlusGrand(ArbreMap<string, double> * metrique, int nombreDeFois){
        vector <Node> resultat;
        string str, maxStr;
        for (ArbreMap<string,double>::Iterateur iter = metrique->debut(); iter; ++iter) {
                Node temp;
                temp.valeur = iter.valeur();
                temp.cle = iter.cle();
                resultat.push_back(temp);
        }
        sort(resultat.begin(), resultat.end(), commentTrier);
        for (int i = 0; i < nombreDeFois; ++i)
                cout << resultat[i].cle << " : " << resultat[i].valeur << endl;
}
/**
 * [calculRequete  Fonction plus ou moins principale du TP. Calcule l'IDF des mots passes en parametre]
 * @param arbreIDF     [Arbre contenant les IDF de tous les mots]
 * @param foret        [Arbre Map contenant les occurences pour chacuns des textes]
 * @param titres       [Arbre contenant les titres des histoires]
 * @param mots         [Vecteur qui contient les mots entres par l'utilisateur]
 * @param nbHistoires  [Le nombre d'histoire total]
 * @param nombreDeFois [Le nombre de metrique que l'on veut afficher]
 */
void calculRequete(ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres, vector <string> *mots, int nbHistoires, int nombreDeFois){
        string mot;
        string titre;
        ArbreMap<string, double> temp;
        double idf = 0, total = 0;
        int tf = 0;
        for (int i = 0; i < nbHistoires; ++i) {
                for (int y = 0; y < mots->size(); ++y) {
                        idf = arbreIDF->operator[]((*mots)[y]);
                        tf = getOccurences((*mots)[y], i, foret);
                        total += idf * tf;
                }
                temp[titres->operator[](i)] = total;
                total = 0;
        }
        if (temp.vide()) exit(0);
        afficherPlusGrand(&temp, nombreDeFois);
        mots->clear();
}
/**
 * [calculerIDF Fonction permettant de calculer l'IDF]
 * @param nbHistoires [Le nombre d'histoire]
 * @param arbreIDF    [Arbre contenant les IDF]
 * @param foret        [Arbre Map contenant les occurences pour chacuns des textes]
 */
void calculerIDF(int nbHistoires, ArbreMap<string, double> *arbreIDF, ArbreMap<string, int> *foret){
        ArbreMap<string, int> temp;
        for(int i = 0; i < nbHistoires; ++i) {
                ArbreMap<string, int> *sousArbre = &foret[i];
                for(ArbreMap<string, int>::Iterateur it = sousArbre->debut(); it; ++it) {
                        temp[it.cle()]++;
                }
        }
        for(ArbreMap<string, int>::Iterateur it = temp.debut(); it; ++it) {
                arbreIDF->operator[](it.cle()) = log2((double)nbHistoires / it.valeur());
        }
}

/**
 * [histoiresToArbre Fonction permettant de faire le transfert d'un vecteur
 * d'histoires a des arbres]
 * @param  histoires [Vecteur d'histoire a transferer]
 * @param  foret        [Arbre Map contenant les occurences pour chacuns des textes]
 * @return           [Le nombre d'histoires transferee]
 */
int histoiresToArbre(vector<Histoire *> *histoires, ArbreMap<string, int> *foret, ArbreMap<int, string> *titres){
        vector<string>::const_iterator iter;
        int nbHistoires = 0;
        int indexe = 0;
        for( Histoire * histoire : *histoires ) {
                titres->operator[](indexe) = histoire->titre();
                ++indexe;
                foret[nbHistoires] = ArbreMap<string, int>();
                for(iter = histoire->begin(); iter != histoire->end(); ++iter) {
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
 * @param  foret [Arbre Map contenant les occurences pour chacuns des textes]
 * @return       [Le nombre d'occurence du mot]
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
        if (getline(cin, temp) && temp.length() > 0) {
                for(int i = 0; i < temp.length(); ++i) {
                        if (isalpha(temp.at(i)) || temp.at(i) == '-') {
                                sousChaine += tolower(temp.at(i));
                        } else if (!sousChaine.empty()) {
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
        while( !( *it )->estElement() ) ++it;
        courrant = ( Element * )( *it );

        for( Contenu * contenu : *courrant ) {
                if( contenu->estElement() ) {
                        Element * element = ( Element * )contenu;
                        DocumentXML * doc = lireFichierXML( element->attribut( string( "fichier" ) ) );
                        vector<Histoire *> * h = extraireHistoires( *doc );
                        histoires->insert( histoires->end(), h->begin(), h->end() );

                }
        }

        return histoires;
}
