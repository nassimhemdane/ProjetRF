//
//

#include "Data.h"

namespace rf{
    Data::Data() {}
    Data::Data(string rep, string type) {
        rep_=rep;
        data_ = charger(rep,type);
        type_=type;
    }
    Data::Data(string type) {
        type_=type;
        data_ = charger(type);
    }

    // charge à nouveau les données
    void Data::reload() {
        data_=charger(rep_,type_);
    }
    void Data::reload(string type, string rep) {
        data_=charger(type,rep);
        type_=type;
        rep_=rep;
    }

    //génère à chaque itérations (9*11) fois une chaine de caractère suivant le format indiqué dans l'énoncé et ouvre
    // le fichier correspondant créant un vecteur et y insérant les lignes du fichiers. On inserera ces vecteurs dans
    // une matrice 9*11 en fonction de leur classe et de leur identifiant
    vector<vector<vector<float>>> Data::charger(string rep, string type) {
        vector<vector<vector<float>>> matrix;
        ifstream monFlux;
        stringstream nom_fichier;
        for(int i=1;i<10;i++){
            vector<vector<float>> classe;
            for(int j=1; j<12; j++){
                vector<float> element;
                if(j<10){
                    nom_fichier << rep <<"/s0" << i << "n00" << j << "." << type;
                    monFlux.open(nom_fichier.str());
                    string valeur;

                    while(getline(monFlux, valeur))
                    {
                        element.push_back(stof(valeur));
                    }
                    nom_fichier.str("");
                    monFlux.close();
                }
                else{
                    nom_fichier << rep <<"/s0" << i << "n0" << j << "." << type;
                    monFlux.open(nom_fichier.str());

                    string valeur;
                    while(getline(monFlux, valeur) )
                    {
                        element.push_back(stof(valeur));
                    }
                    nom_fichier.str("");
                    monFlux.close();
                }
                classe.push_back(element);
            }
            matrix.push_back(classe);
        }
        return matrix;
    }

    // fonction identique à la précédente sauf qu'elle prend un répertoire par défaut
    vector<vector<vector<float>>> Data::charger(string type) {
        vector<vector<vector<float>>> matrix;
        ifstream monFlux;
        stringstream nom_fichier;
        for(int i=1;i<10;i++){
            vector<vector<float>> classe;
            for(int j=1; j<12; j++){
                vector<float> element;
                if(j<10){
                    nom_fichier << "Files/s0" << i << "n00" << j << "." << type;
                    monFlux.open(nom_fichier.str());
                    string valeur;
                    while(getline(monFlux, valeur))
                    {
                        element.push_back(stof(valeur));
                    }
                    nom_fichier.str("");
                    monFlux.close();
                }
                else{
                    nom_fichier << "Files/s0" << i << "n0" << j << "." << type;
                    monFlux.open(nom_fichier.str());

                    string valeur;
                    while(getline(monFlux, valeur))
                    {
                        element.push_back(stof(valeur));
                    }
                    nom_fichier.str("");
                    monFlux.close();
                }
                classe.push_back(element);
            }
            matrix.push_back(classe);
        }
        return matrix;
    }


    //getters
    vector<vector<vector<float>>> Data::get_data() {
        return data_;
    }
    string Data::get_type() {
        return type_;
    }


}