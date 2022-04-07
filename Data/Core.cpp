//
//

#include "Core.h"
#include <random>
#include <algorithm>
namespace rf{
    Core::Core() {}
    Core::Core(Data & data) {
        charge(data.get_data());
        type_=data.get_type();
    }
    Core::Core(vector<vector<vector<float>>> &matrix, string type) {
        charge(matrix);
        type_=type;
    }

    // La fonction parcourt la matrice en ajoutant à chaque itération la ligne de la matrice à l'instance puis
    // l'insère dans la table d'instances du Core
    void Core::charge(vector<vector<vector<float>>> const &matrix) {
        instances_.clear();
        matrix_.clear();
        matrix_=matrix;
        for(int i=0; i<9; i++){
            for(int j=0; j<11; j++){
                add(matrix[i][j],i);
            }
        }
    }

    void Core::pretraitement_F0() {
        if(type_=="F0"){
            for(int i=0; i<instances_.size();i++){
                for(int j=0; j<instances_[i].size()-1;j++){
                    instances_[i][j]/=10000;
                }
            }
        }
    }

    //Créer une copie de l'instance en parmètre et l'ajoute a la table du Core
    void Core::add(vector<float> const &instance, int classe) {
        vector<float> *instanceN= new vector<float>(instance);
        instanceN->push_back(classe);
        instances_.push_back(*instanceN);
    }

    // affichage des instances...
    void Core::print_instances() {
        cout << "{" << endl;
        for(vector<float> v:instances_){
            cout << "[";
            for(float f: v){
                cout << f << ",";
            }
            cout << "]" << endl;
        }
        cout << "}" << endl;
    }

    // affichages des instances en paramètre....
    void Core::print_instances(vector<vector<float>> const & ensemble) {
        cout << "{" << endl;
        for(vector<float> v:ensemble){
            cout << "[";
            for(float f: v){
                cout << f << ",";
            }
            cout << "]" << endl;
        }
        cout << "}" << endl;
    }


    //réordonne aléatoirement la matrice des données (pas les instances) puis pour chaque ligne il prend les
    // (seed) x (nombres d'instance d'une classe) premiers elements de chaque classe pour les insérer dans l'ensemble
    // d'entrainement et laisse le reste pour l'ensemble de test
    void Core::split_equal(float seed) {
        random_device rd;
        int seuil;
        int j;
        test_set_.clear();
        train_set_.clear();
        for(int i=0; i<9; i++){
            shuffle(matrix_[i].begin(),matrix_[i].end(),rd);
            seuil=matrix_[i].size()*seed;
            j=0;
            while (j<seuil){
                vector<float> *instanceN= new vector<float>(matrix_[i][j]);
                instanceN->push_back(i);
                train_set_.push_back(*instanceN);
                j++;
            }
            while (j<matrix_[i].size()){
                vector<float> *instanceN= new vector<float>(matrix_[i][j]);
                instanceN->push_back(i);
                test_set_.push_back(*instanceN);
                j++;
            }
        }
    }

    // Reordone aléatoirement les INSTANCES cette fois-ci et prend les seed*nombre d'instances premiers pour l'ensemble
    // d'entrainement et le reste pour l'ensemble de test
    void Core::split(float seed) {
        test_set_.clear();
        train_set_.clear();
        random_device rd;
        int seuil=get_instances().size()*seed;
        shuffle(instances_.begin(),instances_.end(),rd);
        int i=0;
        while(i<seuil){
            train_set_.push_back(instances_[i]);
            i++;
        }
        while (i<instances_.size()){
            test_set_.push_back(instances_[i]);
            i++;
        }
    }

    //getters
    vector<vector<float>> Core::get_test_set() { return test_set_;}
    vector<vector<float>> Core::get_train_set() { return train_set_;}
    vector<vector<float>> Core::get_instances() {
        return instances_;
    }
    string Core::get_type() {
        return type_;
    }





}