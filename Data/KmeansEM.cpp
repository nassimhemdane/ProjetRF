//
// Modele Estimation maximisation donnant de mauvais résultats et non inclus dans le rapport
//

#include "KmeansEM.h"

namespace rf{
    KmeansEM::KmeansEM() {}
    KmeansEM::KmeansEM(const vector<vector<float>> &instances, int k) {
        k_=k;
        instances_.operator=(instances);
        instance_size_=instances[0].size()-1;
        partitions_.clear();
    }

    void KmeansEM::compute_probabilities(pair<vector<float>,vector<vector<float>>> & element){

        for(int i=0;i<k_;i++){
            vector<float> *prob_distance = new vector<float>();
            for(int m=0;m<instance_size_;m++){
                float gaussien=
                        (1/(sqrt(variances_[i][m])*sqrt(2*3.14)))*exp(-pow((element.first[m]-means_[i][m]),2)/(2*variances_[i][m]));
              // cout << "Gaussien :" << gaussien << " variance : " << variances_[i][m] << endl;
                prob_distance->push_back(gaussien);
            }
            element.second.push_back(*prob_distance);
        }

    }

    // fonction parcourant les clusters et calculant la moyenne de chacun d'entre eux
    void KmeansEM::calculate_means() {
        means_.clear();
        variances_.clear();
        for(int l=0;l<k_;l++){
            vector<float> *mean = new vector<float>();
            vector<float> *vari = new vector<float>();
            for(int m=0;m<instance_size_;m++){
                mean->push_back(0);
                vari->push_back(0);
            }
            for(int i=0;i<k_;i++) {
                for (int j = 0; j < partitions_[i].size(); j++) {
                    for (int m = 0; m < instance_size_; m++) {
                        float sum = 0;
                        for (int ii = 0; ii < k_; ii++) {
                            sum += partitions_[i][j].second[ii][m];
                        }
                        (*mean)[m] += partitions_[i][j].first[m] * partitions_[i][j].second[l][m] / sum;
                    }
                }
                for (int m = 0; m < instance_size_; m++) {
                    (*mean)[m] = (*mean)[m] / partitions_[i].size();
                    cout << "mean[" << m << "] :" << (*mean)[m] << " partition[i].size:" << partitions_[i].size() << endl;
                }
            }
            for (int j = 0; j < partitions_[l].size(); j++) {
                for(int m=0;m<instance_size_;m++){
                    (*vari)[m]=pow((partitions_[l][j].first[m]-(*mean)[m]),2);
                }
            }
            for (int m = 0; m < instance_size_; m++) {
                (*vari)[m] = (*vari)[m] / partitions_[l].size();
                (*vari)[m]=1;
            }
            if(partitions_[l].size()!=0){
                means_.push_back(*mean);
            }else{
                int p=rand() % instances_.size();
                means_.push_back(instances_[p]);
            }

            variances_.push_back(*vari);
        }
    }
    //distance l1
    float KmeansEM::l1(vector<float> const &target, vector<float> const & object){
        float value = 0;
        for(int i=0; i<object.size()-1; i++){
            value+=abs(target[i]-object[i]);
        }
        return value;
    }

    //fonction calculant la distance d'une instance avec chaque centre de cluster pour choisir le cluster auquel elle
    // appartient
    int KmeansEM::select_cluster(vector<float> const & instance){
        pair<int,float> cluster= pair<int,float>(0,INT16_MAX);
        for(int i =0; i<k_;i++){
            for(float gf:means_[i]){
            //    cout << "gf[" << i << "]: " << gf << endl;
            }
            for(float inst:instance){
              //  cout << "inst:" << inst << endl;
            }
            float distance=l1(means_[i],instance);
            cout << "distance :" << distance << endl;
            if(cluster.second>distance){
                cout << "hehe boi :" << i << endl;
                cluster.second=distance;
                cluster.first=i;
            }
        }
        return cluster.first;
    }


    // selection de k centroids et affiliations des instances à leurs clusters
    void KmeansEM::init(){
        means_.clear();
        for(int i =0; i<k_;i++){
            int p=rand() % instances_.size();
            means_.push_back(instances_[p]);
            vector<float> *zeros=new vector<float>();
            for(int mp=0;mp<instance_size_;mp++){
                zeros->push_back(1);
            }
            variances_.push_back(*zeros);
        }
        partitions_.clear();
        for(int i=0; i<k_; i++){
            vector<pair<vector<float>,vector<vector<float>>>> *instance =
                    new vector<pair<vector<float>,vector<vector<float>>>>();
            partitions_.push_back(*instance);
        }
        for(int j=0; j<instances_.size();j++){
            int p=select_cluster(instances_[j]);
            vector<vector<float>> *vect = new vector<vector<float>>();
            pair<vector<float>,vector<vector<float>>> *element =
                    new pair<vector<float>,vector<vector<float>>>(pair<vector<float>,vector<vector<float>>>(instances_[j],*vect));
            compute_probabilities(*element);
            partitions_[p].push_back(*element);
        }
    }


    //fonction recalculant les moyennes et réaffiliant chaque instance à son estimé meilleur cluster
    void KmeansEM::iterate() {
        calculate_means();
        partitions_.clear();
        for(int i=0; i<k_; i++){
            vector<pair<vector<float>,vector<vector<float>>>> *instance =
                    new vector<pair<vector<float>,vector<vector<float>>>>();
            partitions_.push_back(*instance);
        }
        for(int j=0; j<instances_.size();j++){
            int p=select_cluster(instances_[j]);
            cout << "P :" << p << endl;
            vector<vector<float>> *vect = new vector<vector<float>>();
            pair<vector<float>,vector<vector<float>>> *element =
                    new pair<vector<float>,vector<vector<float>>>(pair<vector<float>,vector<vector<float>>>(instances_[j],*vect));
            compute_probabilities(*element);
            partitions_[p].push_back(*element);
        }

    }

    // calcul de la variance...
    void KmeansEM::calculate_variance() {
        long m=0;
        for(int i=0;i<k_;i++){
            for(pair<vector<float>,vector<vector<float>>> j:partitions_[i]){
                m+=l1(means_[i],j.first);
            }
        }
        variance_=m;
    }



    // setter
    void KmeansEM::set_iterations(int iter){
        iterations_=iter;
    }

    // calcul de la fonction de fitness et iteration de l'algorithme kmeans
    void KmeansEM::episode() {
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
    void KmeansEM::fit(){
        vector<vector<pair<vector<float>,vector<vector<float>>>>> partitions_buff=partitions_;
        vector<vector<float>> means_buff=means_;
        float vari=INT16_MAX;
        for(int i=0; i<iterations_; i++){
            variance_=INT16_MAX;
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
       print_partitions();
        calculate_variance();
        arrange();

    }
    //affichages des partitions...
    void KmeansEM::print_partitions() {
        int j=0;
        cout << "{ " << endl;
        for(vector<pair<vector<float>,vector<vector<float>>>> i: partitions_){
            j++;
            cout << i.size() << endl;
            for(pair<vector<float>,vector<vector<float>>> l:i){
                //cout << "[ ";
                for(float f:l.first){
                    //cout << f << ", ";
                }
                //cout << " ]," << endl ;
            }
        }
        cout << "}" << endl;
    }

    // distance l2...
    float KmeansEM::l2(vector<float> const & target, vector<float> const & object){
        float value = 0;
        for(int i=0; i<object.size()-1; i++){
            value+=pow(target[i]-object[i],2);
        }
        return sqrt(value);
    }

    // utilisation de arranged_partitions pour vérifier si les deux elements appartiennent au même cluster
    bool KmeansEM::same_cluster(vector<float> oi, vector<float> oj) {
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
    void KmeansEM::arrange(){
        int i=0;
        for(vector<pair<vector<float>,vector<vector<float>>>> p : partitions_){
            for(pair<vector<float>,vector<vector<float>>> v:p){
                pair<int,vector<float>> *c = new pair<int,vector<float>>(i,v.first);
                arranged_partitions_.push_back(*c);

            }
            i++;
        }
    }



}