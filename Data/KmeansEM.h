//
// Modele Estimation maximisation donnant de mauvais résultats et non inclus dans le rapport
//

#include "Core.h"
#include "math.h"

#ifndef PROJETRF_KMEANSEM_H
#define PROJETRF_KMEANSEM_H

namespace rf {
    class KmeansEM {
    public:
        //constructeur nécéssitant les instances et le nombre de classes
        KmeansEM();
        KmeansEM(vector<vector<float>> const &instances, int k);

        //Fonction principale responsable du clusturing (voir plus en détail dans le .cpp)
        void fit();

        // fonction reponsable de recalculer les moyennes
        void calculate_means();

        // fonction de fitness (fonction a optimiser pour l'algorithme de culsturing)
        void calculate_variance();

        float variance(vector<vector<vector<float>>> partitions);

        //une itération consiste à sauvegarder l'ancien partionnement et d'en créer un nouveau
        void iterate();

        //initialisation de l'algotithme
        void init();

        // affichage des partitions
        void print_partitions();

        //un épisode consiste à faire une itération puis l'évaluer
        void episode();

        void compute_probabilities(pair<vector<float>,vector<vector<float>>>  & element);

        // fonction qui réarange les partitions sous un autre format d'ensemble de couple (numero_cluster,elements)
        //pratique pour les calculs de performances
        void arrange();

        //fonction qui vérifie si deux éléments font partie du même cluster
        bool same_cluster(vector<float> oi,vector<float> oj);

        //seter pour le nombre d'itérations de kmeans
        void set_iterations(int iter);

        // la distance l2
        float l2(vector<float> const &target, vector<float> const &object);

        // la distance l1
        float l1(vector<float> const &target, vector<float> const &object);

        //getters
        vector<vector<float>> get_partitions();
        int get_k();

    private:
        // fonction responsable d'affilier un cluster à une instance
        int select_cluster(vector<float> const & instance);
        //instances étiquetées
        vector<vector<float>> instances_;
        //partitions étiquetées
        vector<vector<pair<vector<float>,vector<vector<float>>>>> partitions_;
        //partitions sous la forme d'un ensemble de couples (numero_partition,instances)
        vector<pair<int,vector<float>>> arranged_partitions_;
        // vecteur des moyennes
        vector<vector<float>> means_;
        vector<vector<float>> variances_;
        //Le paramètre K de de K means
        int k_ = 11;
        // variance (fonction de fitness)
        float variance_=1;
        // taille des instances
        int instance_size_;
        //nombre d'itérations de Kmeans
        int iterations_=100;
    };
}


#endif //PROJETRF_KMEANSEM_H
