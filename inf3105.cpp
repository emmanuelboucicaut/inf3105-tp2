
#include "DocumentXML.h"
#include "Histoire.h"
#include "arbreavl.h"

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
    ArbreAVL<string> *tabArbre = new ArbreAVL<string>[histoires->size()];

    for( Histoire * histoire : * histoires ) {
        //cout << ( * histoire ).titre() << endl << endl;
        tabArbre[nbHistoires] = ArbreAVL<string>();
        for(iter = histoire->begin(); iter != histoire->end(); ++iter){
            string temp = *iter;
            temp[0] = tolower(temp[0]);
            tabArbre[nbHistoires].inserer(temp);
        }
        ++nbHistoires;
    }

    for(int i = 0; i < nbHistoires; ++i){
        ArbreAVL<string> temp = tabArbre[i];
        cout << *temp.fin() << endl;
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
