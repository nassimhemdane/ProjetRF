//
// Classe responsable de gérer la lecture des fichiers
// Elle construit une matrice de vecteurs représentant les instances avec comme colones
// les classes et comme lignes les instances
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#ifndef PROJETRF_DATA_H
#define PROJETRF_DATA_H
using namespace std;

namespace rf {
    class Data {
    public:
        //constructeurs auquels on donne le repertoire et le type de fichiers
        Data(string rep, string type);
        Data(string type);
        Data();
        //geters
        vector<vector<vector<float>>> get_data();
        string get_type();
        void reload();
        void reload(string rep, string type);

    private:
        string type_; //type des données
        string rep_; // reprtoire de travail
        vector<vector<vector<float>>> data_; // matrice des données
        vector<vector<vector<float>>> charger(string type); // fonction responsable de charger les donnés
        vector<vector<vector<float>>> charger(string rep, string type); // fonction responsable de charger les donnés
    };
}

#endif //PROJETRF_DATA_H
