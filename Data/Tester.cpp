//
//

#include "Tester.h"
#include "math.h"

namespace rf {
Tester::Tester() {}

Tester::Tester(Knn const & model, vector<vector<float>> const & test_set, int nb_classes) {
    knn_=model;
    test_set_=test_set;
    this->nb_classes=nb_classes;
    for(int i=0;i<nb_classes+1;i++){
        vector<int> *v= new vector<int>();
        for(int j=0; j<nb_classes+1;j++){
            v->push_back(0);
        }
        matrice_confusion_.push_back(*v);
    }
}

Tester::Tester(KmeansEM const & model, vector<vector<float>> const
& test_set,int nb_classes){
    kmeansEM_=model;
    test_set_=test_set;
    this->nb_classes=nb_classes;
}

Tester::Tester(Kmeans const & model, vector<vector<float>> const & test_set,int nb_classes){
    kmeans_=model;
    test_set_=test_set;
    this->nb_classes=nb_classes;
}


    void Tester::test_KmeansEM() {
        float precision=0;
        float rappel=0;
        float pres;
        float rec;
        float i_sum;
        float i_sum2;
        for(int i=0;i<test_set_.size();i++){
            pres=0;
            rec=0;
            i_sum=0;
            i_sum2=0;
            for(int j=0; j<test_set_.size();j++){
                if(i!=j){
                    int A=0;
                    int C=0;
                    if(kmeansEM_.same_cluster(test_set_[i],test_set_[j])){
                        A=1;
                        pres++;
                    }
                    if(same_class(test_set_[i],test_set_[j])){
                        C=1;
                        rec++;
                    }
                    int B=0;
                    if((A==1 and same_class(test_set_[i],test_set_[j])) or
                       (A==0 and !same_class(test_set_[i],test_set_[j]))){
                        B=1;
                    }
                    int D=0;
                    if((C==1 and A==1) or (C==0 and A==0)){
                        D=1;
                    }
                    i_sum+=A*B;
                    i_sum2+=C*D;
                }
            }
            if(pres!=0){
                precision+=i_sum/pres;
                rappel+=i_sum2/rec;
            }
        }
        precision=precision/test_set_.size();
        rappel=rappel/test_set_.size();
        precision_=precision;
        rappel_=rappel;
        fscore_=2*(precision_*rappel_)/(precision_+rappel_);
        cout << "test KmeanEM_____________________________" << endl;
        cout << "precision :" << precision << endl;
        cout << "rappel :" << rappel << endl;
        cout << "fscore :" << fscore_ << endl;

    }


// test de Kmeans en utilisant la fonction B-cubed
void Tester::test_Kmeans() {
    float precision=0;float rappel=0;float pres;float rec;float i_sum;float i_sum2;
    for(int i=0;i<test_set_.size();i++){
         pres=0;rec=0;i_sum=0;i_sum2=0;
        for(int j=0; j<test_set_.size();j++){
            if(i!=j){
                int A=0;int C=0;
                if(kmeans_.same_cluster(test_set_[i],test_set_[j])){A=1;pres++;}
                if(same_class(test_set_[i],test_set_[j])){C=1;rec++;}
                int B=0;
                if((A==1 and same_class(test_set_[i],test_set_[j])) or
                (A==0 and !same_class(test_set_[i],test_set_[j]))){B=1;}
                int D=0;
                if((C==1 and A==1) or (C==0 and A==0)){D=1;}
                i_sum+=A*B;i_sum2+=C*D;}}
        if(pres!=0){
            precision+=i_sum/pres;rappel+=i_sum2/rec;}}
    precision=precision/test_set_.size();
    rappel=rappel/test_set_.size();
    precision_=precision;
    rappel_=rappel;
    fscore_=2*(precision_*rappel_)/(precision_+rappel_);
    cout << "test Kmean_____________________________" << endl;
    cout << "precision :" << precision << endl;
    cout << "rappel :" << rappel << endl;
    cout << "fscore :" << fscore_ << endl;

}


// remplissage des bords a droite et en bas de la droite de confusion
void Tester::calcul_matrix() {
    int i;
    for(int ligne=0;ligne<nb_classes;ligne++){
        i=0;
        for(int cologne=0;cologne<nb_classes;cologne++){
            i=i+matrice_confusion_[ligne][cologne];
        }
        matrice_confusion_[ligne][nb_classes]=i;
    }
   for(int col=0;col<nb_classes;col++){
       i=0;
        for(int lig=0; lig<nb_classes;lig++){
            i=i+matrice_confusion_[lig][col];
        }
        matrice_confusion_[nb_classes][col]=i;
    }
}

//affichage de la matrice de confusion
void Tester::print_matrix() {
    for(int i=0;i<matrice_confusion_.size();i++){
        cout << "[";
        for (int j = 0; j < matrice_confusion_[i].size(); ++j) {
            cout << " " << matrice_confusion_[i][j] << ",";
        }
        cout << "]" << endl;
    }
}


// test de KKnn sur l'ensemble de test en remplissant la matrice de confusion et en calculant a partir la précision
// le rappel et le fscore
void Tester::test_Knn() {
    cout << "test Knn_____________________________" << endl;
    for(vector<float> i: test_set_){
        int classe = i[i.size()-1];
        i.pop_back();
        int prediction = knn_.predict(i);
        matrice_confusion_[prediction][classe]+=1;
        i.push_back(classe);
    }
    calcul_matrix();
    print_matrix();
    float pre=0;
    float rap=0;
        for(int i=0;i<nb_classes;i++){
            pre= pre + (float)matrice_confusion_[i][i]/matrice_confusion_[nb_classes][i];
            if(matrice_confusion_[i][nb_classes]!=0){rap=rap +
            (float)matrice_confusion_[i][i]/matrice_confusion_[i][nb_classes];}
        }
    rap=rap/nb_classes;
    pre=pre/nb_classes;
    precision_=pre;
    rappel_=rap;
    fscore_=2*(precision_*rappel_)/(precision_+rappel_);
    cout << "precision :" << precision_ << endl;
    cout << "rappel :" << rappel_ << endl;
    cout << "fscore :" << fscore_ << endl;
}

// vérifie si deux éléments sont de même classe
bool Tester::same_class(vector<float> oi, vector<float> oj) {
    return oi[oi.size()-1]==oj[oj.size()-1];
}

    //getters
    float Tester::getPrecision() const {
        return precision_;
    }
    void Tester::setPrecision(float precision) {
        precision_ = precision;
    }
    float Tester::getRappel() const {
        return rappel_;
    }
    void Tester::setRappel(float rappel) {
        rappel_ = rappel;
    }
    float Tester::getFscore() const {
        return fscore_;
    }
    void Tester::setFscore(float fscore) {
        fscore_ = fscore;
    }

}