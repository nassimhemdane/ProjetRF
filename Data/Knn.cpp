//
//

#include "Knn.h"
#include "math.h"
#include <algorithm>
namespace rf{
    Knn::Knn() {}
    Knn::Knn(const vector<vector<float>> &instances) {
        fit(instances);
    }

    vector<vector<float>> Knn::get_data() {
        return data_;
    }

    int Knn::get_k() {
        return k_;
    }

    // fonction utilisé pour trier (fait office de comparateur)
    bool myfunction (pair<int,float> i,pair<int,float> j) { return (i.second<j.second); }

    // fonction utile lors de la selection de classe
    int is_element(vector<pair<int,int>> const & matchs, int valeur){
        for(int i=0;i<matchs.size();i++){
            if(matchs[i].first==valeur){
                return i;
            }
        }
        return -1;
    }
    //selection d'une classe selon les voisins en les comptant et en trouvant la classe majoritaire
    int select_class(vector<pair<int,float>>  const & voisins){
        vector<pair<int,int>> *matchs = new vector<pair<int,int>>();
        int j;
        for(int i=0;i<voisins.size();i++){
            j=is_element(*matchs,voisins[i].first);
            if(j!=-1){
                (*matchs)[j]=pair<int,int>((*matchs)[j].first,(*matchs)[j].second+1);
            }else{
                matchs->push_back(pair<int,int>(voisins[i].first,1));
            }
        }
        pair<int,int> max=pair<int,int>(0,0);
        for(int j=0; j<matchs->size();j++){
            //cout << (*matchs)[j].first << " " << (*matchs)[j].second << endl;
            if((*matchs)[j].second>max.second){
                max.first=(*matchs)[j].first;
                max.second=(*matchs)[j].second;
            }
        }
        return max.first;

    }

    //copie des instances dans la table de Knn
    void Knn::fit(vector<vector<float>> const &instances) {
        data_.operator=(instances);
    }

    //prediction en retrouvant les k plus proches voisons puis en determinant la classe grace aux voisins
    int Knn::predict(vector<float> const &instance) {
        vector<pair<int,float>> *classment=new vector<pair<int,float>>();
        for(vector<float> & inst: data_){
            pair<int,float> valuation;
            valuation.second=l2(inst,instance);
            valuation.first=inst[inst.size()-1];
            classment->push_back(valuation);
        }

        vector<pair<int,float>> *voisins = new vector<pair<int,float>>();
        vector<float> *max_values = new vector<float>();
        sort((*classment).begin(),(*classment).end(),myfunction);
        for(int l=0; l<k_;l++){
            voisins->push_back((*classment)[l]);
        }

        return select_class(*voisins);

    }

    // distance l2
    float Knn::l2(vector<float> const & target, vector<float> const & object){
        float value = 0;
        for(int i=0; i<object.size(); i++){
            value+=pow(target[i]-object[i],2);
        }
        return sqrt(value);
    }

    // distance l1
    float Knn::l1(vector<float> const &target, vector<float> const & object){
        float value = 0;
        for(int i=0; i<object.size(); i++){
            value+=abs(target[i]-object[i]);
        }
        return value;
    }
}
