#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>

using namespace std; 


void diodosilicio(){

    cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££"<< endl;

	cout << fixed;

    gStyle->SetOptFit(1111);
    gStyle->SetOptTitle(1); 
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); 
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.481); 
    gStyle->SetStatY(0.880);


    //tutte le misure
    const int nmisure = 30;

    //numero di misure non a corrente nulla
    const int nmisure1 = 27;

    //le grandezze con ...1 sono quelle con i punti sperimentali in meno.
    
    //V
    //Amprobe 37XR-A
    //#punti = 30
    float Vgen[] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.2,1.4,1.6,1.8,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0};
    
    //V
    //Amprobe 37XR-A
    //#punti = 27
    float Vgen1[] = {0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.2,1.4,1.6,1.8,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0};
  
    //V
    //Amprobe 37XR-A
    //#punti = 30
    float Vd[] = {0,0.1028,0.1874,0.2946,0.3712,0.4128,0.4456,0.4571,0.4587,0.4701,0.4780,0.4918,0.4978,0.5086,0.5154,0.5209,0.5432,0.5580,0.5701,0.5787,0.5866,0.5931,0.5989,0.6040,0.6085,0.6121,0.6159,0.6194,0.6227,0.6259};
    
    //V
    //Amprobe 37XR-A
    //#punti = 27
    float Vd1[]  = {0.2946,0.3712,0.4128,0.4456,0.4571,0.4587,0.4701,0.4780,0.4918,0.4978,0.5086,0.5154,0.5209,0.5432,0.5580,0.5701,0.5787,0.5866,0.5931,0.5989,0.6040,0.6085,0.6121,0.6159,0.6194,0.6227,0.6259};

    //mA
    //Amprobe 37XR-A
    //#punti = 30
    float Id[] = {0.000,0.000,0.000,0.001,0.014,0.045,0.111,0.1490,0.155,0.207,0.257,0.355,0.407,0.523,0.611,0.699,1.145,1.576,2.047,2.470,2.936,3.372,3.827,4.285,4.743,5.179,5.630,6.081,6.548,6.986};

    //mA
    //Amprobe 37XR-A
    //#punti = 27
    float Id1[] = {0.001,0.014,0.045,0.111,0.1490,0.155,0.207,0.257,0.355,0.407,0.523,0.611,0.699,1.145,1.576,2.047,2.470,2.936,3.372,3.827,4.285,4.743,5.179,5.630,6.081,6.548,6.986};

    //multimetro utilizzato Amprobe 37XR-A 
    //DC Voltage Ranges 1000 mV, 10 V, 100 V, 1000 V ± (0.1 % rdg + 5 dgt)
    //DC Current 100 μA range: ± (0.5 % rdg + 10 dgt)
    //           1000 μA to 400 mA ranges: ± (0.5 % rdg + 5 dgts)

    //V
    //errore di sensibilità dell'Amprobe 37XR-A
    float sVgen[nmisure];
    for(int i = 0; i < nmisure; i++){sVgen[i] = 0.1;};
    float sVgen1[nmisure1];
    for(int i = 0; i < nmisure1; i++){sVgen1[i] = 0.1;};

    //V
    //errore di sensibilità dell'Amprobe 37XR-A
    float sVd[nmisure];
    for(int i = 0; i < nmisure; i++){sVd[i] = 0.001 * Vd[i] + 5 * 0.0001;};
    float sVd1[nmisure1];
    for(int i = 0; i < nmisure1; i++){sVd1[i] = 0.001 * Vd1[i] + 5 * 0.0001;};

    //mA
    //errore di sensibilità dell'Amprobe 37XR-A
    float sId[nmisure];
    for(int i = 0; i < nmisure; i++){
        if(Id[i] < 1) {sId[i] = 0.005 * Id[i] + 10 * 0.001;};  
        if(Id[i] >= 1) {sId[i] = 0.005 * Id[i] + 5 * 0.001;}; 
    };
    float sId1[nmisure1];
    for(int i = 0; i < nmisure1; i++){
        if(Id1[i] < 1) {sId1[i] = 0.005 * Id1[i] + 10 * 0.001;};  
        if(Id1[i] >= 1) {sId1[i] = 0.005 * Id1[i] + 5 * 0.001;}; 
    };


    for(int j = 0 ; j < nmisure ; j++){
        cout << "Measurement number " << j << ":" 
        << " \t Vgen = (" << setprecision(5) << Vgen[j] << " +- " << setprecision(6) << sVgen[j]  << ") V," 
        << " \t Vd = (" << setprecision(5) << Vd[j] << " +- " << setprecision(6) << sVd[j] << ") V," 
        << " \t Id = (" << setprecision(5) << Id[j] << " +- " << setprecision(6) << sId[j] << ") mA," 
        << " \t sVgen/Vgen " << setprecision(4) << sVgen[j] / Vgen[j]
        << " \t sVd/Vd " << setprecision(4) << sVd[j] / Vd[j]
        << " \t sId/Id " << setprecision(4) << sId[j] / Id[j]
        << endl;   
    }

    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________________________________________________________FIT CON N = 30____________________________________________________________________________________________________________" << endl;
    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    TCanvas * cIdVd1 = new TCanvas("cIdVd1","grafico1",0,0,1280,720);
    cIdVd1->SetFillColor(0);
    cIdVd1->cd();
    gStyle->SetOptFit(1111);

    TGraphErrors *gIdVd1 = new TGraphErrors(nmisure,Vd,Id,sVd,sId);
    gIdVd1->SetMarkerSize(0.4);
    gIdVd1->SetMarkerStyle(20);
    gIdVd1->SetTitle("Funzione di fit:    I_{d} = I_{s} #left[exp#left(V_{d} / (0.026 #upoint #eta)#right) - 1#right]");
    gIdVd1->GetXaxis()->SetTitle("V_{d} [V]");
    gIdVd1->GetYaxis()->SetTitle("I_{d} [mA]");
    gIdVd1->Draw("AP");

    cout << "\n\n --- Ipotesi  Id = [0] * (pow(e,x / ([1] * 0.026)) - 1)--- \n" <<endl;
    TF1 *funz1 = new TF1("funz1","[0] * (pow(e,x / ([1] * 0.026)) - 1)",0,0.7);
    funz1->SetParNames("I_{s} (mA)", "#eta (adim)");
    funz1->SetParameter(0,3.31e-6);
    funz1->SetParameter(1,1.639);
    //come ho stimato i parametri iniziali: https://www.wolframalpha.com/input/?i=two+equaitons+system&assumption=%7B%22F%22,+%22SolveSystemOf2EquationsCalculator%22,+%22equation1%22%7D+-%3E%224.28%3Dx(e%5E(0.60%2F(y*0.026))-1)%22&assumption=%7B%22F%22,+%22SolveSystemOf2EquationsCalculator%22,+%22equation2%22%7D+-%3E%220.41%3Dx(e%5E(0.5%2F(y*0.026))-1)%22

    funz1->SetParLimits(0,1e-12,1e-5);
    funz1->SetParLimits(1,0,3);
  
    funz1->SetLineColor(8);
    gIdVd1->Fit(funz1,"ERM+");

    cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cIdVd1->SaveAs("DIODOSILICIO 1.png");


    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________________________________________________________FIT CON N = 27____________________________________________________________________________________________________________" << endl;
    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    TCanvas * cIdVd2 = new TCanvas("cIdVd2","grafico2",0,0,1280,720);
    cIdVd2->SetFillColor(0);
    cIdVd2->cd();
    gStyle->SetOptFit(1111);

    TGraphErrors *gIdVd2 = new TGraphErrors(nmisure1,Vd1,Id1,sVd1,sId1);
    gIdVd2->SetMarkerSize(0.4);
    gIdVd2->SetMarkerStyle(20);
    gIdVd2->SetTitle("Funzione di fit:    I_{d} = I_{s} #left[exp#left(V_{d} / (0.026 #upoint #eta)#right) - 1#right]");
    gIdVd2->GetXaxis()->SetTitle("V_{d} [V]");
    gIdVd2->GetYaxis()->SetTitle("I_{d} [mA]");
    gIdVd2->Draw("AP");

    cout << "\n\n --- Ipotesi  Id = [0] * (pow(e,x / ([1] * 0.026)) - 1)--- \n" << endl;
    TF1 * funz2 = new TF1("funz1","[0] * (pow(e,x / ([1] * 0.026)) - 1)",0,0.7);
    funz2->SetParNames("I_{s} (mA)", "#eta (adim)");
    funz2->SetParameter(0,3.31e-6);
    funz2->SetParameter(1,1.639);
    //come ho stimato i parametri iniziali: https://www.wolframalpha.com/input/?i=two+equaitons+system&assumption=%7B%22F%22,+%22SolveSystemOf2EquationsCalculator%22,+%22equation1%22%7D+-%3E%224.28%3Dx(e%5E(0.60%2F(y*0.026))-1)%22&assumption=%7B%22F%22,+%22SolveSystemOf2EquationsCalculator%22,+%22equation2%22%7D+-%3E%220.41%3Dx(e%5E(0.5%2F(y*0.026))-1)%22

    funz2->SetParLimits(0,1e-12,1e-5);
    funz2->SetParLimits(1,0,3);
  
    funz2->SetLineColor(7);
    gIdVd2->Fit(funz2,"ERM+");

    cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cIdVd2->SaveAs("DIOSOSILICO 2.png");


    //__________________________________________________________________________________________________________________________________________________________________________________________________________________________
    std::ofstream out("DIODOSILICIO_CSV.csv");
      
    out << "Vgen (V)" << endl;
    for (auto & value : Vgen){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sVgen (V)" << endl;
    for (auto & value : sVgen){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Vd (V)" << endl;
    for (auto & value : Vd){out << fixed << showpoint << setprecision(7) << value  << endl;}
    out << endl;

    out << "sVd (V)" << endl;
    for (auto & value : sVd){out << fixed << showpoint << setprecision(7) << value  << endl;}
    out << endl;

    out << "Id (mA)" << endl;
    for (auto & value : Id){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sId (mA)" << endl;
    for (auto & value : sId){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

};
