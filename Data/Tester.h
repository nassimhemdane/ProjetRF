//
//Classe responsable de l'ensemble des tests faits sur les modèles.
//
#ifndef PROJETRF_TESTER_H
#define PROJETRF_TESTER_H
#include "Knn.h"
#include "Kmeans.h"
#include "KmeansEM.h"

namespace rf{

class Tester {
public:
    // constructeur prennant en paramètre un modèle (dans notre cas kmeans ou knn) et un ensemble de test pour faire
    //les tests
    Tester();
    Tester(Knn const & model, vector<vector<float>> const & test_set,int nb_classes);
    Tester(Kmeans const & model, vector<vector<float>> const & test_set,int nb_classes);
    Tester(KmeansEM const & model, vector<vector<float>> const & test_set,int nb_classes);

    // fonction responsable de tester le modèle Kmeans
    void test_Kmeans();

    //fonction responsable de tester le modèle Knn
    void test_Knn();

    void test_KmeansEM();

    //fonction comparant la classe de deux éléments
    bool same_class(vector<float> oi, vector<float> oj);

    //fonction affichant la matrice de confusion
    void print_matrix();

    // getters et setters
    float getPrecision() const;
    void setPrecision(float precision);
    float getRappel() const;
    void setRappel(float rappel);
    float getFscore() const;
    void setFscore(float fscore);

private:
    // fonction calculant la matrice de confusion
    void calcul_matrix();

    vector<vector<float>> test_set_;
    vector<vector<float>> train_set_;
    Knn knn_;
    Kmeans kmeans_;
    KmeansEM kmeansEM_;
    vector<vector<int>> matrice_confusion_;
    float precision_;
    float rappel_;
    float fscore_;
    int nb_classes;
};
}


#endif //PROJETRF_TESTER_H
