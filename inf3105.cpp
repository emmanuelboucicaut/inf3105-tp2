
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


int main() {
    int nbHistoires = 0;
    vector<Histoire *> * histoires = lireDocuments(string("listeDocument.xml"));
    vector<string> mots;
    vector<string>::const_iterator iter;
    ArbreMap<string, int> *tabArbre = new ArbreMap<string, int>[histoires->size()];

    for( Histoire * histoire : * histoires ) {
        tabArbre[nbHistoires] = ArbreMap<string, int>();
        for(iter = histoire->begin(); iter != histoire->end(); ++iter){
            string temp = *iter;
            temp[0] = tolower(temp[0]);
            tabArbre[nbHistoires][temp]++;
        }
        ++nbHistoires;
    }

    for(int i = 0; i < nbHistoires; ++i){
        ArbreMap<string, int> *temp = &tabArbre[i];
        ArbreMap<string, int>::Iterateur it = temp->fin();
        cout << it.cle() << endl;
    }
    delete [] tabArbre;
	return 0;
}

/************************************************
 *                    FONCTIONS                 *
 ***********************************************/

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
