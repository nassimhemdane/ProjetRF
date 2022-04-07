//
//Classe du modèle Knn
//

#ifndef PROJETRF_KNN_H
#define PROJETRF_KNN_H

#include "Core.h"


namespace rf{
class Knn {
public:
    // constructeur avec l'ensemble des instances étiquetées comme paramètre (l'ensemble d'entrainement)
    Knn();
    Knn(vector<vector<float>> const & instances);
    // fonction inserant les données dans la table de Knn
    void fit(vector<vector<float>> const & instances);
    // fonction responsable de la prédiction
    int predict(vector<float> const & instance);
    //distance l2
    float l2(vector<float> const & target, vector<float> const & object);
    // distance l1
    float l1(vector<float> const & target, vector<float> const & object);
    //getters
    vector<vector<float>> get_data();
    int get_k();

private:
    // table de knn
    vector<vector<float>> data_;
    //paramètre k
    int k_=5;
};

}
#endif //PROJETRF_KNN_H
