#include <vector>
#include "math.h"
#include "Data/Data.h"
#include "Data/Core.h"
#include "Data/Knn.h"
#include "Data/Kmeans.h"
#include "Data/Tester.h"
#include "Data/KmeansEM.h"
#include <iostream>
#include <fstream>
using namespace std;





int main()
{

    rf::Data *donnes = new rf::Data("Files","GFD");
    rf::Core *core = new rf::Core(*donnes);

    //core->pretraitement_F0();
    //core->print_instances();
    ofstream knnfile;
    ofstream kmeansfile;
    knnfile.open ("knnF0.txt");
    kmeansfile.open("kmeansF0.txt");
   float max_Knn=0;
   float max_Kmean=0;
   float var_Knn=0;
   float var_Kmeans=0;
   float Knn_pres=0;
   float Knn_recal=0;
   float Knn_fscore=0;
   float Kmeans_pres=0;
   float Kmeans_recal=0;
   float Kmeans_fscore=0;
   vector<float> *v_fscore_knn = new vector<float>();
   vector<float> *v_fscore_kmeans = new vector<float>();
   for(int i=0;i<30;i++){
       // KNN
       core->split_equal(0.66);
       rf::Knn *model = new rf::Knn(core->get_train_set());

       rf::Tester *tester= new rf:: Tester(*model,core->get_test_set(),9);
       tester->test_Knn();
       v_fscore_knn->push_back(tester->getFscore());
       knnfile << tester->getFscore() << endl;
       Knn_pres+=tester->getPrecision();
       Knn_recal+=tester->getRappel();
       Knn_fscore+=tester->getFscore();
       if(tester->getFscore()>max_Knn){
           max_Knn=tester->getFscore();
       }
       //KMEANS
       rf::Core *core = new rf::Core(*donnes);
       core->split_equal(0);
       rf::Kmeans *model2 = new rf::Kmeans(core->get_test_set(),9);
       model2->fit();

       rf::Tester * tester2 = new rf:: Tester(*model2,core->get_test_set(),9);
       tester2->test_Kmeans();
       kmeansfile << tester2->getFscore() << endl;
       v_fscore_kmeans->push_back(tester2->getFscore());
       Kmeans_fscore+=tester2->getFscore();
       Kmeans_pres+=tester2->getPrecision();
       Kmeans_recal+=tester2->getRappel();
       if(tester2->getFscore()>max_Kmean){
           max_Kmean=tester2->getFscore();
       }
   }
   kmeansfile.close();
   knnfile.close();
   Knn_fscore/=100;
   Knn_recal/=100;
   Knn_pres/=100;
   Kmeans_recal/=100;
   Kmeans_pres/=100;
   Kmeans_fscore/=100;
     for(float fsc:*v_fscore_knn){
        var_Knn+=pow((Knn_fscore-fsc),2);
    }
    for(float fsc:*v_fscore_kmeans){
        var_Kmeans+=pow((Kmeans_fscore-fsc),2);
    }
    var_Knn/=100;
    var_Kmeans/=100;
   cout << "Knn________________" << endl;
   cout << "Recal : " << Knn_recal << endl;
   cout << "Precision : " << Knn_pres << endl;
   cout << "Fscore : " << Knn_fscore << endl;
   cout << "Max Fscore : " << max_Knn << endl;
   cout << "Variance : " << var_Knn << endl;

   cout << "Kmeans_________________" << endl;
   cout << "Recal : " << Kmeans_recal << endl;
   cout << "Precision : " << Kmeans_pres << endl;
   cout << "Fscore : " << Kmeans_fscore << endl;
   cout << "Max Fscore : " << max_Kmean << endl;
   cout << "Variance : " << var_Kmeans << endl;

    /*
    string methode;
    do {
        cout << "*************************************************" << endl;
        cout << "*                                               *" << endl;
        cout << "*  What methode you want to use?                *" << endl;
        cout << "*                                               *" << endl;
        cout << "*  1- KNN                                       *" << endl;
        cout << "*  2- K-means                                   *" << endl;
        cout << "*  3- Both                                      *" << endl;
        cout << "*                                               *" << endl;
        cout << "*  Answer by writing the corresponding number   *" << endl;
        cout << "*                                               *" << endl;
        cout << "*************************************************" << endl;
        cout << endl;
        cout << " Your answer: ";
        getline (std::cin, methode);
        cout << endl;
    }
    while (methode.length() !=2 and methode.at(0) != '1' and methode.at(0) != '2' and methode.at(0) != '3');
    string data;
    do {
        cout << endl << "*************************************************" << endl;
        cout << "*                                               *" << endl;
        cout << "*  Please choose a representaion ?              *" << endl;
        cout << "*                                               *" << endl;
        cout << "*  1- E34                                       *" << endl;
        cout << "*  2- GFD                                       *" << endl;
        cout << "*  3- SA                                        *" << endl;
        cout << "*  4- F0                                        *" << endl;
        cout << "*                                               *" << endl;
        cout << "*  Answer by writing the corresponding number   *" << endl;
        cout << "*                                               *" << endl;
        cout << "*************************************************" << endl;
        cout << endl;
        cout << " Your answer: ";
        getline (std::cin, data);
        cout << endl;
    }
    while (data.length() !=2 and data.at(0) != '1' and data.at(0) != '2' and data.at(0) != '3' and data.at(0) != '4');

    string times_str;
    int times=0;
    bool isnum = true;
    do {
        cout << " How many times : ";
        getline (std::cin, times_str);
        cout << endl;
        for(char& c : times_str) {
            if(c>'9' or c<'0') {
                isnum = false;
                break;
            }
            times = times * 10 + c - '0';
        }
    }
    while (!isnum);
    switch(data.at(0)) {
        case '1': data = "E34";
        case '2': data = "GFD";
        case '3': data = "SA";
        case '4': data = "F0";
    }

    // Rechercher les données
    rf::Data *donnes = new rf::Data("Files", data);

    if(methode.at(0) == '1' or methode.at(0) == '3') {
        rf::Core *core = new rf::Core(*donnes);
        //core->pretraitement_F0();
        //core->print_instances();
        ofstream knnfile;
        knnfile.open ("knn.txt");
        float max_Knn=0;
        float var_Knn=0;
        float Knn_pres=0;
        float Knn_recal=0;
        float Knn_fscore=0;
        vector<float> *v_fscore_knn = new vector<float>();

        for(int i=0;i<times;i++){
            core->split_equal(0.66);
            rf::Knn *model = new rf::Knn(core->get_train_set());

            rf::Tester *tester= new rf:: Tester(*model,core->get_test_set(),9);
            tester->test_Knn();
            v_fscore_knn->push_back(tester->getFscore());
            knnfile << tester->getFscore() << endl;
            Knn_pres+=tester->getPrecision();
            Knn_recal+=tester->getRappel();
            Knn_fscore+=tester->getFscore();
            if(tester->getFscore()>max_Knn){
                max_Knn=tester->getFscore();
            }
        }
        Knn_fscore/=times;
        Knn_recal/=times;
        Knn_pres/=times;
        for(float fsc:*v_fscore_knn){
            var_Knn+=pow((Knn_fscore-fsc),2);
        }
        var_Knn/=times;
        cout << "Knn_final________________" << endl;
        cout << "Recal : " << Knn_recal << endl;
        cout << "Precision : " << Knn_pres << endl;
        cout << "Fscore : " << Knn_fscore << endl;
        cout << "Max Fscore : " << max_Knn << endl;
        cout << "Variance : " << var_Knn << endl;
    }
    if(methode.at(0) == '2' or methode.at(0) == '3') {
        ofstream kmeansfile;
        kmeansfile.open("kmeans"+data+".txt");
        float max_Kmean=0;
        float var_Kmeans=0;
        float Kmeans_pres=0;
        float Kmeans_recal=0;
        float Kmeans_fscore=0;
        vector<float> *v_fscore_kmeans = new vector<float>();
        for(int i=0;i<times;i++){
            rf::Core *core = new rf::Core(*donnes);
            core->split_equal(0);
            rf::Kmeans *model = new rf::Kmeans(core->get_test_set(),9);
            model->fit();
            rf::Tester * tester2 = new rf:: Tester(*model,core->get_test_set(),9);
            tester2->test_Kmeans();
            kmeansfile << tester2->getFscore() << endl;
            v_fscore_kmeans->push_back(tester2->getFscore());
            Kmeans_fscore+=tester2->getFscore();
            Kmeans_pres+=tester2->getPrecision();
            Kmeans_recal+=tester2->getRappel();
            if(tester2->getFscore()>max_Kmean){
                max_Kmean=tester2->getFscore();
            }
            kmeansfile.close();
            Kmeans_recal/=times;
            Kmeans_pres/=times;
            Kmeans_fscore/=times;
            var_Kmeans/=times;
        }
        for(float fsc:*v_fscore_kmeans){
            var_Kmeans+=pow((Kmeans_fscore-fsc),2);
        }
        cout << "Kmeans_final_________________" << endl;
        cout << "Recal : " << Kmeans_recal << endl;
        cout << "Precision : " << Kmeans_pres << endl;
        cout << "Fscore : " << Kmeans_fscore << endl;
        cout << "Max Fscore : " << max_Kmean << endl;
        cout << "Variance : " << var_Kmeans << endl;
    }

    */
    return 0;
}