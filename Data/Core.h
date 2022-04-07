//
//Classe résponsable de la gestion des ensembles de données, elle gère toutes les éventuelles opérations
// sur l'ensemble de donnés comme par exemple la division en ensemble de test et d'entrainement
//

#ifndef PROJETRF_CORE_H
#define PROJETRF_CORE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Data.h"

namespace rf {
    class Core{
    public:
        // Constructeur avec un objet de type Data comme paramètre pour construire un tableau de donnés étiquetées
        // à partir de la matrice des donnés se trouvant dans l'objet Data.
        Core(Data & data);
        Core();
        Core(vector<vector<vector<float>>> & matrix,string type);

        //permet de charger l'ensemble de donnés à partir d'une matrice de données
        void charge(vector<vector<vector<float>>> const &matrix);

        // permet d'ajouter une classe à un vecteur de caractéristiques
        void add(vector<float> const &instance, int classe);

        //affiche les instances
        void print_instances();
        void print_instances(vector<vector<float>> const & ensemble);

        //mélange et divise l'ensemble de données de manière brute selon un ratio (seed)
        void split(float seed);

        //retire une donnée de l'ensemble de données
        void split_one();

        //divise l'ensemble de données équitablement entre les classes.
        void split_equal(float seed);

        void pretraitement_F0();

        //getters
        vector<vector<float>> get_instances();
        string get_type();
        vector<vector<float>> get_test_set();
        vector<vector<float>> get_train_set();
    private:
        //matrices des données
        vector<vector<vector<float>>> matrix_;

        // type de features
        string type_;

        //toutes les instances étiquetées
        vector<vector<float>> instances_;

        // ensemble de test
        vector<vector<float>> test_set_;

        // ensemble de données
        vector<vector<float>> train_set_;
    };

}


#endif //PROJETRF_CORE_H
