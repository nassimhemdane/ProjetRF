//
//

#include "Kmeans.h"
#include <limits>

namespace rf{
    Kmeans::Kmeans() {}
    Kmeans::Kmeans(const vector<vector<float>> &instances, int k) {
        k_=k;
        instances_.operator=(instances);
        instance_size_=instances[0].size()-1;
        partitions_.clear();
    }



    // fonction parcourant les clusters et calculant la moyenne de chacun d'entre eux
    void Kmeans::calculate_means() {
        means_.clear();
        for(int i=0;i<k_;i++){
            vector<float> *mean = new vector<float>();
            for(int m=0;m<instance_size_;m++){
                mean->push_back(0);
            }
            for(int j=0;j<partitions_[i].size();j++){
                for(int m=0;m<instance_size_;m++){
                    (*mean)[m]+=partitions_[i][j][m];
                }
            }
            for(int m=0;m<instance_size_;m++){
                (*mean)[m]=(*mean)[m]/partitions_[i].size();
            }
            means_.push_back(*mean);
        }
    }
    //distance l1
    float Kmeans::l1(vector<float> const &target, vector<float> const & object){
        float value = 0;
        for(int i=0; i<object.size()-1; i++){
            value+=abs(target[i]-object[i]);
        }
        return value;
    }

    //fonction calculant la distance d'une instance avec chaque centre de cluster pour choisir le cluster auquel elle
    // appartient
    int Kmeans::select_cluster(vector<float> const & instance){
        pair<int,float> cluster= pair<int,float>(0,INT16_MAX);
        for(int i =0; i<k_;i++){
            float distance=l2(means_[i],instance);
            if(cluster.second>distance){
                cluster.second=distance;
                cluster.first=i;
            }
        }
        return cluster.first;
    }

    //fonction recalculant les moyennes et réaffiliant chaque instance à son estimé meilleur cluster
    void Kmeans::iterate() {
        calculate_means();
        partitions_.clear();
        for(int i=0; i<k_; i++){
            vector<vector<float>> *instance = new vector<vector<float>>();
            partitions_.push_back(*instance);
        }
          for(int j=0; j<instances_.size();j++){
            int p=select_cluster(instances_[j]);
            partitions_[p].push_back(instances_[j]);
        }

    }

    // calcul de la variance...
    void Kmeans::calculate_variance() {
        long m=0;
        for(int i=0;i<k_;i++){
            for(vector<float> j:partitions_[i]){
                m+=l1(means_[i],j);
            }
        }
        variance_=m;
    }

    // selection de k centroids et affiliations des instances à leurs clusters
    void Kmeans::init(){
        means_.clear();
        for(int i =0; i<k_;i++){
            int p=rand() % instances_.size();
            means_.push_back(instances_[p]);
        }
        partitions_.clear();
        for(int i=0; i<k_; i++){
            vector<vector<float>> *instance = new vector<vector<float>>();
            partitions_.push_back(*instance);
        }
        for(int j=0; j<instances_.size();j++){
            int p=select_cluster(instances_[j]);
            partitions_[p].push_back(instances_[j]);
        }
    }

    // setter
    void Kmeans::set_iterations(int iter){
        iterations_=iter;
    }

    // calcul de la fonction de fitness et iteration de l'algorithme kmeans
    void Kmeans::episode() {
        float var=INT16_MAX;
        init();
        calculate_variance();
        while (var!=variance_){
            if(variance_<var){
                var=variance_;
            }
            iterate();
            calculate_variance();
            if(variance_>var){
                var=variance_;
            }
        }
        //print_partitions();

    }

    //execution de nb_iterations episodes et execution de arrange pour restructurer les clusters
    void Kmeans::fit(){
        vector<vector<vector<float>>> partitions_buff=partitions_;
        vector<vector<float>> means_buff=means_;
        float vari=numeric_limits<float>::max();
        for(int i=0; i<iterations_; i++){
            variance_=numeric_limits<float>::max();
            episode();
            if(variance_<vari){
              partitions_buff=partitions_;
                means_buff=means_;
                vari=variance_;
            }
        }
        cout << "_________________________________"<< endl;
        partitions_=partitions_buff;
        means_=means_buff;
        calculate_variance();
        print_partitions();
        arrange();

    }
    //affichages des partitions...
    void Kmeans::print_partitions() {
        int j=0;
        cout << "{ " <<  endl;
        for(vector<vector<float>> i: partitions_){
            j++;
            cout << i.size() << endl;
            for(vector<float> l:i){
                //cout << "[ ";
                for(float f:l){
                  //  cout << f << ", ";
                }
                //cout << " ]," << endl ;
            }
        }
        cout << "}" << endl;
    }

    // distance l2...
    float Kmeans::l2(vector<float> const & target, vector<float> const & object){
        float value = 0;
        for(int i=0; i<object.size()-1; i++){
            value+=pow(target[i]-object[i],2);
        }
        return sqrt(value);
    }

    // utilisation de arranged_partitions pour vérifier si les deux elements appartiennent au même cluster
    bool Kmeans::same_cluster(vector<float> oi, vector<float> oj) {
        int i=-1;
        int j=-1;

        for(pair<int,vector<float>> p:arranged_partitions_){
            if(p.second==oi){
                i=p.first;
            }
            if(p.second==oj){
                j=p.first;
            }
        }
       // cout << "i , j : " << i << j << endl;
        return i==j;
    }

    //transformation des partitions en un ensemble de couples (numero_cluster,instance)
    void Kmeans::arrange(){
        int i=0;
        for(vector<vector<float>> p : partitions_){
            for(vector<float> v:p){
                pair<int,vector<float>> *c = new pair<int,vector<float>>(i,v);
                arranged_partitions_.push_back(*c);

            }
            i++;
        }
    }



}