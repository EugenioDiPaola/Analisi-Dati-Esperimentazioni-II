#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>

using namespace std; 


void pannellosolare(){
    //abbiamo usato due multimetri Amprobe 37XR-A per le grandezze Iamp_d, Iamp_f, Vvolt_d, Vvolt_f, Vp_d, Vp_f

    //DC Voltage Ranges 1000 mV, 10 V, 100 V, 1000 V ± (0.1 % rdg + 5 dgt)
    //DC Current 100 μA range: ± (0.5 % rdg + 10 dgt)
    //           1000 μA to 400 mA ranges: ± (0.5 % rdg + 5 dgts)

    const int nmisure = 14;//numero di misure con luce diretta e del fondo


    //grandezze lampadina
    //V
    float Vlamp = 12;
    float sVlamp = 0.1;//o 0.01?
    //A
    //mezza cifra perchè oscillava tra 4.4 e 4.5
    float Ilamp = 4.45;
    float sIlamp = 0.05;
    //W
    float Plamp = Vlamp * Ilamp;
    float sPlamp = sqrt(pow(Ilamp,2) * pow(sVlamp,2) + pow(Vlamp,2) * pow(sIlamp,2));

    //µW
    //converto Plamp e relativo errore in µW
    Plamp = Plamp * 1e6;
    sPlamp = sPlamp * 1e6;

    cout << "Plamp: (" << Plamp << "+-" << sPlamp << endl;

    //raggio del pannello
    //mm
    //misurato con il calibro
    float r = 59.650;
//------------
    float sr = 0.01;//non sono sicuro sia l'errore giusto, metto l'errore di quello che ho a casa
//----------------
    //distanza [filamento della lampadina - pannello]
    //mm
    //misurato con un righello
    float d1 = 99; 
    float sd1 = 1;

    //distanza [0 del carrello - carrello in varie posizioni] == [pannello con il carrello a 0 - pannello in varie posizioni]
    //mm 
    //potrebbe sembrare che la precisione sia sui mm, ma in realtà è che siamo riusciti a ottenere sempre la cifra spaccata al mm, ma la precisione è sul centesimo di mm
    float d2[] = {0.00,50.00,80.03,110.00,140.00,170.00,200.00,230.00,260.00,290.00,320.00,350.00,380.00,410.00};
    //mettiamo come errore quello sull'ultima cifra, un centesimo di mm.
    float sd2[nmisure];
    for(int i = 0; i < nmisure; i++){sd2[i] = 0.05;};

    //distanza totale
    //mm
    //d = d1 + d2 = [filamento della lampadina - pannello con il carrello a 0] + [0 del carrello - pannello in varie posizioni] 
    //              = [filamento della lampadina - pannello con il carrello a 0] + [pannello con il carrello a 0 - pannello in varie posizioni] 
    float d[nmisure];
    for(int i = 0; i < nmisure; i++){d[i] = d1 + d2[i];};
    float sd[nmisure];
    for(int i = 0; i < nmisure; i++){sd[i] = sqrt(pow(sd1,2) + pow(sd2[i],2));}
  
    //grandezze misurate con luce diretta (d = diretta)
    //mA
    float Iamp_d[] = {34.38,28.9,24.93,21.53,18.61,16.03,13.65,11.89,10.38,9.167,8.057,7.152,6.327,5.72};
    //mV
    float Vvolt_d[] = {390.5,327.5,282.1,243.8,210.4,181.3,154.7,134.5,117.5,103.7,91.10,80.80,71.50,64.50};
    //mV
    float Vp_d[] = {438.7,397,368.7,347.8,331.5,317.6,301.5,290,279,269,259,249.5,240,232.5};

    //grandezze derivate, con luce diretta
    //mV / mA = Ohm
    float Ramp_d[nmisure];
    for(int i = 0; i < nmisure; i++){Ramp_d[i] = Vvolt_d[i] / Iamp_d[i];};
    //mV / mA - Ohm = Ohm
    float Rp_d[nmisure];
    for(int i = 0; i < nmisure; i++){Rp_d[i] = Vp_d[i] / Iamp_d[i] - Ramp_d[i];};
    //mV / Ohm = mA
    float Icc_d[nmisure];
    for(int i = 0; i < nmisure; i++){Icc_d[i] = Vp_d[i] / Rp_d[i];};
    //mV * mA = µW
    float Ppannello_d[nmisure];
    for(int i = 0; i < nmisure; i++){Ppannello_d[i] = Vp_d[i] * Icc_d[i];};

    //errori sulle grandezze misurate, con luce diretta
    float sIamp_d[nmisure];
    for(int i = 0;i < nmisure;i++){
        if(Iamp_d[i] < 10.0) sIamp_d[i] = 0.005 * Iamp_d[i] + 5 * 0.001;
        if(Iamp_d[i] >= 10.0) sIamp_d[i] = 0.005 * Iamp_d[i] + 5 * 0.01;
    };

    float sVvolt_d[nmisure];
    for(int i = 0; i < nmisure; i++){
        if(Vvolt_d[i] < 100.0) sVvolt_d[i] = 0.001 * Vvolt_d[i] + 5 * 0.01;
        if(Vvolt_d[i] >= 100.0) sVvolt_d[i] = 0.001 * Vvolt_d[i] + 5 * 0.1;
    };

    float sVp_d[nmisure];
    for(int i = 0; i < nmisure;i++){
        if(Vp_d[i] < 100.0) sVp_d[i] = 0.001 * Vp_d[i] + 5 * 0.01;
        if(Vp_d[i] >= 100.0) sVp_d[i] = 0.001 * Vp_d[i] + 5 * 0.1;
    };

    //errori sulle grandezze derivate, con luce diretta
    float sRamp_d[nmisure];
    for(int i = 0; i < nmisure; i++){sRamp_d[i] = sqrt(pow(Vvolt_d[i],2) / pow(Iamp_d[i],4) * pow(sIamp_d[i],2) + 1 / pow(Iamp_d[i],2) * pow(sVvolt_d[i],2));}

    float sRp_d[nmisure];
    for(int i = 0; i < nmisure; i++){sRp_d[i] = sqrt(1 / pow(Iamp_d[i],2) * pow(sVp_d[i],2) + pow(Vp_d[i],2) / pow(Iamp_d[i],4) * pow(sIamp_d[i],2) + 1 * pow(sRamp_d[i],2));}

    float sIcc_d[nmisure];
    for(int i = 0; i < nmisure; i++){sIcc_d[i] = sqrt(pow(Vp_d[i],2) / pow(Rp_d[i],4) * pow(sRp_d[i],2) + 1 / pow(Rp_d[i],2) * pow(sVp_d[i],2));}

    float sPpannello_d[nmisure];
    for(int i = 0; i < nmisure; i++){sPpannello_d[i] = sqrt(pow(Vp_d[i],2) * pow(sIcc_d[i],2) + pow(Icc_d[i],2) * pow(sVp_d[i],2));}


    //grandezze misurate, con luce oscurata
    //mA
    float Iamp_f[] = {1.67,1.908,2.05,2,2.185,2.205,1.971,1.972,1.598,1.57,1.442,1.307,1.093,1.066};
    //mV
    float Vvolt_f[] = {18,21.5,23.1,24.1,24.6,24.8,22.2,22.2,18,17.6,16.2,14.7,12.3,11.5};
    //mV
    float Vp_f[] = {127,149.5,150.7,155,157.8,160,152,152.8,138.6,137.6,132.5,125.8,114.7,113.3};

    //grandezze derivate, con luce oscurata
    //mV / mA = Ohm
    float Ramp_f[nmisure];
    for(int i = 0; i < nmisure; i++){Ramp_f[i] = Vvolt_f[i] / Iamp_f[i];};
    //mV / mA - Ohm = Ohm
    float Rp_f[nmisure];
    for(int i = 0; i < nmisure; i++){Rp_f[i] = Vp_f[i] / Iamp_f[i] - Ramp_f[i];};
    //mV / Ohm = mA
    float Icc_f[nmisure];
    for(int i = 0; i < nmisure; i++){Icc_f[i] = Vp_f[i] / Rp_f[i];};
    //mV * mA = µW
    float Ppannello_f[nmisure];
    for(int i = 0; i < nmisure; i++){Ppannello_f[i] = Vp_f[i] * Icc_f[i];};

    //errori sulle grandezze misurate, con luce diretta
    float sIamp_f[nmisure];
    for(int i = 0;i < nmisure;i++){
        if(Iamp_f[i] < 10.0) sIamp_f[i] = 0.005 * Iamp_f[i] + 5 * 0.001;
        if(Iamp_f[i] >= 10.0) sIamp_f[i] = 0.005 * Iamp_f[i] + 5 * 0.01;
    };

    float sVvolt_f[nmisure];
    for(int i = 0; i < nmisure; i++){
        if(Vvolt_f[i] < 100.0) sVvolt_f[i] = 0.001 * Vvolt_f[i] + 5 * 0.01;
        if(Vvolt_f[i] >= 100.0) sVvolt_f[i] = 0.001 * Vvolt_f[i] + 5 * 0.1;
    };

    float sVp_f[nmisure];
    for(int i = 0; i < nmisure;i++){
        if(Vp_f[i] < 100.0) sVp_f[i] = 0.001 * Vp_f[i] + 5 * 0.01;
        if(Vp_f[i] >= 100.0) sVp_f[i] = 0.001 * Vp_f[i] + 5 * 0.1;
    };

    //errori sulle grandezze derivate, con luce diretta
    float sRamp_f[nmisure];
    for(int i = 0; i < nmisure; i++){sRamp_f[i] = sqrt(pow(Vvolt_f[i],2) / pow(Iamp_f[i],4) * pow(sIamp_f[i],2) + 1 / pow(Iamp_f[i],2) * pow(sVvolt_f[i],2));}

    float sRp_f[nmisure];
    for(int i = 0; i < nmisure; i++){sRp_f[i] = sqrt(1 / pow(Iamp_f[i],2) * pow(sVp_f[i],2) + pow(Vp_f[i],2) / pow(Iamp_f[i],4) * pow(sIamp_f[i],2) + 1 * pow(sRamp_f[i],2));}

    float sIcc_f[nmisure];
    for(int i = 0; i < nmisure; i++){sIcc_f[i] = sqrt(pow(Vp_f[i],2) / pow(Rp_f[i],4) * pow(sRp_f[i],2) + 1 / pow(Rp_f[i],2) * pow(sVp_f[i],2));}

    float sPpannello_f[nmisure];
    for(int i = 0; i < nmisure; i++){sPpannello_f[i] = sqrt(pow(Vp_f[i],2) * pow(sIcc_f[i],2) + pow(Icc_f[i],2) * pow(sVp_f[i],2));}


  
    //diretta meno il fondo
    float Icc[nmisure];
    for(int i = 0; i < nmisure; i++){Icc[i] = Icc_d[i] - Icc_f[i];};

    float Vp[nmisure];
    for(int i = 0; i < nmisure; i++){Vp[i] = Vp_d[i] - Vp_f[i];};

    float Ppannello[nmisure];
    for(int i = 0; i < nmisure; i++){Ppannello[i] = Ppannello_d[i] - Ppannello_f[i];};


    float sIcc[nmisure];
    for(int i = 0; i < nmisure; i++){sIcc[i] = sqrt(pow(sIcc_d[i],2) + pow(sIcc_f[i],2));};

    float sVp[nmisure];
    for(int i = 0; i < nmisure; i++){sVp[i] = sqrt(pow(sVp_d[i],2) + pow(sVp_f[i],2));};

    float sPpannello[nmisure];
    for(int i = 0; i < nmisure; i++){sPpannello[i] = sqrt(pow(sPpannello_d[i],2) + pow(sPpannello_f[i],2));};

    cout << "Icc_d[0] " << Icc_d[0] << endl;
    cout << "sIcc_d[0] " << sIcc_d[0] << endl;

    cout << "Icc_f[0] " << Icc_f[0] << endl;
    cout << "sIcc_f[0] " << sIcc_f[0] << endl;

    cout << "Vp_d[0] " << Vp_d[0] << endl;
    cout << "sVp_d[0] " << sVp_d[0] << endl;

    cout << "Vp_f[0] " << Vp_f[0] << endl;
    cout << "sVp_f[0] " << sVp_f[0] << endl;

    cout << "sPpannello[0] " << sPpannello[0] << endl;


    cout << "sPpannello_d[0] " << sPpannello_d[0] << endl;
    cout << "sPpannello_f[0] " << sPpannello_f[0] << endl;
    cout << "sPpannello[0] " << sPpannello[0] << endl;

    cout << "\n\n"<< endl;
    cout << "Plamp: " << Plamp << "+-" << sPlamp << endl;
    cout << "r: " << r << "+-" << sr << endl;
    cout << "\n\n"<< endl;

    //stampa le misure e i relativi errori delle grandezze con luce diretta
    /*for(int j = 0 ; j < nmisure ; j++){cout

        << " \t sIcc = (" << setprecision(5) << Icc[j] << " +- " << setprecision(2) << sIcc[j] << ") mA," 
        << " \t Vp = (" << setprecision(5) << Vp[j] << " +- " << setprecision(2) << sVp[j] << ") V," 
        << " \t Ppannello = (" << setprecision(5) << Ppannello[j] << " +- " << setprecision(2) << sPpannello[j] << ") mW." 
        
        << " \t sIcc_d = (" << setprecision(5) << Icc_d[j] << " +- " << setprecision(2) << sIcc_d[j] << ") mA," 
        << " \t Vp_d = (" << setprecision(5) << Vp_d[j] << " +- " << setprecision(2) << sVp_d[j] << ") V," 
        << " \t Ppannello_d = (" << setprecision(5) << Ppannello_d[j] << " +- " << setprecision(2) << sPpannello_d[j] << ") mW." 

        << " \t sIcc_f = (" << setprecision(5) << Icc_f[j] << " +- " << setprecision(2) << sIcc_f[j] << ") mA," 
        << " \t Vp_f = (" << setprecision(5) << Vp_f[j] << " +- " << setprecision(2) << sVp_f[j] << ") V," 
        << " \t Ppannello_f = (" << setprecision(5) << Ppannello_f[j] << " +- " << setprecision(2) << sPpannello_f[j] << ") mW." 

        << " \t sIcc = (" << setprecision(5) << Icc[j] << " +- " << setprecision(2) << sIcc[j] << ") mA," 
        << " \t Vp = (" << setprecision(5) << Vp[j] << " +- " << setprecision(2) << sVp[j] << ") V," 
        << " \t Ppannello = (" << setprecision(5) << Ppannello[j] << " +- " << setprecision(2) << sPpannello[j] << ") mW." 
        << endl;   
    }*/
  
    //impostazioni grafiche
    gStyle->SetOptFit(1111);
    gStyle->SetOptTitle(1); 
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); 
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.9); 
    gStyle->SetStatY(0.880);  

 
    TCanvas * cIccvsd = new TCanvas("#DeltaIcc vs d","grafico1",0,0,1280,720);
    cIccvsd->SetFillColor(0);
    cIccvsd->cd();
    TGraphErrors * gIccvsd = new TGraphErrors(nmisure,d,Icc,sd,sIcc);
    gIccvsd->SetMarkerSize(0.4);
    gIccvsd->SetMarkerStyle(20);
    gIccvsd->SetTitle("#DeltaI_{cc} vs d");
    gIccvsd->GetXaxis()->SetTitle("d (mm)");
    gIccvsd->GetYaxis()->SetTitle("#DeltaI_{cc} (mA)");
    gIccvsd->Draw("ALP");
    cIccvsd->SaveAs("cIccvsd.png");


    TCanvas * cVpvsd = new TCanvas("#DeltaVp vs d","grafico2",0,0,1280,720);
    cVpvsd->SetFillColor(0);
    cVpvsd->cd();
    TGraphErrors * gVpvsd = new TGraphErrors(nmisure,d,Vp,sd,sVp);
    gVpvsd->SetMarkerSize(0.4);
    gVpvsd->SetMarkerStyle(20);
    gVpvsd->SetTitle("#DeltaV_{p} vs d");
    gVpvsd->GetXaxis()->SetTitle("d (mm)");
    gVpvsd->GetYaxis()->SetTitle("#DeltaV_{p} (mV)");
    gVpvsd->Draw("ALP");
    cVpvsd->SaveAs("cVpvsd.png");
    cVpvsd->SaveAs("cVpvsd.png");

    //errori fittizi ma abbastanza grandi da essere visibili sul grafico. d'altra parte l'errore reale su tutta l'esperienza è molto più grande di quello che abbiamo sulle grandezze misurate direttamente e quelle derivate per tutta una serie di ragioni.
    //float sPpannello1[nmisure];
    //for (int i = 0; i < nmisure;i++){sPpannello1[i] = 10000;};

    //float sd1[nmisure];
    //for (int i = 0; i < nmisure;i++){sd1[i] = 7;};


    cout << "\n\n --- Ipotesi  [0] * 1/2 * (1 - sqrt(pow(x,2) / (pow([1],2) + pow(x,2)))) * [2] --- \n" <<endl;

    TCanvas * cPpannellovsd = new TCanvas("Ppannello vs d","grafico3",0,0,1280,720);
    cPpannellovsd->SetFillColor(0);
    cPpannellovsd->cd();
    TGraphErrors * gPpannellovsd = new TGraphErrors(nmisure,d,Ppannello,sd,sPpannello);
    gPpannellovsd->SetMarkerSize(0.4);
    gPpannellovsd->SetMarkerStyle(20);
    gPpannellovsd->SetTitle("#scale[0.7]{Funzione di fit:     #DeltaP_{pannello} = #frac{#eta}{2} #upoint #left[1 - #sqrt{d^{2} / #left(r^{2} + d^{2}#right)}#right] #upoint P_{lamp}}");
    gPpannellovsd->GetXaxis()->SetTitle("d (mm)");
    gPpannellovsd->GetYaxis()->SetTitle("#DeltaP_{pannello} (#muW)");
    gPpannellovsd->Draw("AP");

    //fit con efficienza, raggio del pannello e potenza della lampadina  
    TF1 * funz1 = new TF1("funz1","[0] * 1/2 * (1 - sqrt(pow(x,2) / (pow([1],2) + pow(x,2)))) * [2]",0,600);
    funz1->SetParNames("#eta (adim)","r (mm)","P_{lamp} (#muW)");
    funz1->SetParameter(0,0.03);//stima a sensazione
    funz1->SetParameter(1,r);//
    funz1->SetParameter(2,Plamp);//µW

    //funz1->SetParLimits(0,0,0.08);        
    funz1->SetParLimits(1,r - sr,r + sr);//mm
    funz1->SetParLimits(2,Plamp - sPlamp,Plamp + sPlamp);//µW

    funz1->SetLineColor(2);
    gPpannellovsd->Fit(funz1,"VERM+");

    cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cPpannellovsd->SaveAs("cPpannellovsd.png");


    cout << "\n\n --- Ipotesi  [0] * 1/2 * (1 - sqrt(pow(x,2) / (pow(59.65,2) + pow(x,2)))) * 53.4e+06 --- \n" <<endl;

    TCanvas * cPpannellovsd2 = new TCanvas("Ppannello vs d 2","grafico4",0,0,1280,720);
    cPpannellovsd2->SetFillColor(0);
    cPpannellovsd2->cd();
    TGraphErrors * gPpannellovsd2 = new TGraphErrors(nmisure,d,Ppannello,sd,sPpannello);
    gPpannellovsd2->SetMarkerSize(0.4);
    gPpannellovsd2->SetMarkerStyle(20);
    gPpannellovsd2->SetTitle("#scale[0.7]{Funzione di fit:    #DeltaP_{pannello} = #frac{#eta}{2} #upoint #left[1 - #sqrt{d^{2} / #left(59.65^{2} + d^{2}#right)}#right] #upoint 53.4 #upoint 10^{6}}");
    gPpannellovsd2->GetXaxis()->SetTitle("d (mm)");
    gPpannellovsd2->GetYaxis()->SetTitle("#DeltaP_{pannello} (#muW)");
    gPpannellovsd2->Draw("AP");

    
    TF1 * funz2 = new TF1("funz2","[0] * 1/2 * (1 - sqrt(pow(x,2) / (pow(59.65,2) + pow(x,2)))) * 53.4e+06",0,600);
    funz2->SetParNames("#eta (adim)");
    funz2->SetParameter(0,0.03);
    //funz1->SetParameter(1,60);
    //funz1->SetParameter(2,52.8e+06);//µW

    //funz2->SetParLimits(0,0,0.08);        
    //funz1->SetParLimits(1,55,65);  
    //funz1->SetParLimits(2,50e+06,50e+06);//µW

    funz2->SetLineColor(3);
    gPpannellovsd2->Fit(funz2,"ERM+");

    cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cPpannellovsd2->SaveAs("cPpannellovsd2.png");


    std::ofstream out("ppannellosolare.csv");

    out << "d1 " << d1 << endl ;

    out << "d2" << endl;
    for (auto & value : d2){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sd2" << endl;
    for (auto & value : sd2){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;


    out << "d" << endl;
    for (auto & value : d){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sd" << endl;
    for (auto & value : sd){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;
    out << endl;
    out << endl;
    out << endl;

    out << "----------------------------------------------------------------------------------------------------------------------------------"<< endl;
    out << "Iamp_d" << endl;
    for (auto & value : Iamp_d){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "sIamp_d" << endl;
    for (auto & value : sIamp_d){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "Vvolt_d" << endl;
    for (auto & value : Vvolt_d){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sVvolt_d" << endl;
    for (auto & value : sVvolt_d){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "Ramp_d" << endl;
    for (auto & value : Ramp_d){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "sRamp_d" << endl;
    for (auto & value : sRamp_d){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "Vp_d" << endl;
    for (auto & value : Vp_d){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "sVp_d" << endl;
    for (auto & value : sVp_d){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "Rp_d" << endl;
    for (auto & value : Rp_d){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sRp_d" << endl;
    for (auto & value : sRp_d){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Icc_d" << endl;
    for (auto & value : Icc_d){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sIcc_d" << endl;
    for (auto & value : sIcc_d){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Ppannello_d" << endl;
    for (auto & value : Ppannello_d){out << fixed << showpoint << setprecision(7)  << value / 1e6 << endl;}
    out << endl;

    out << "sPpannello_d" << endl;
    for (auto & value : sPpannello_d){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;}
   

    out << endl;
    out << endl;
    out << endl;
    out << endl;
    out << endl;

    out << "----------------------------------------------------------------------------------------------------------------"<< endl;

    out << "Iamp_f" << endl;
    for (auto & value : Iamp_f){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "sIamp_f" << endl;
    for (auto & value : sIamp_f){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "Vvolt_f" << endl;
    for (auto & value : Vvolt_f){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sVvolt_f" << endl;
    for (auto & value : sVvolt_f){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "Ramp_f" << endl;
    for (auto & value : Ramp_f){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "sRamp_f" << endl;
    for (auto & value : sRamp_f){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "Vp_f" << endl;
    for (auto & value : Vp_f){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "sVp_f" << endl;
    for (auto & value : sVp_f){out << fixed << showpoint << setprecision(7) << value << endl;}

    out << "Rp_f" << endl;
    for (auto & value : Rp_f){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sRp_f" << endl;
    for (auto & value : sRp_f){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Icc_f" << endl;
    for (auto & value : Icc_f){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sIcc_f" << endl;
    for (auto & value : sIcc_f){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Ppannello_f" << endl;
    for (auto & value : Ppannello_f){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;}
    out << endl;

    out << "sPpannello_f" << endl;
    for (auto & value : sPpannello_f){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;}
   

    out << endl;
    out << endl;
    out << endl;
    out << endl;
    out << endl;

    cout << "----------------------------------------------------------------------------------------------------------------"<< endl;



    out << "Icc" << endl;
    for (auto & value : Icc){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sIcc" << endl;
    for (auto & value : sIcc){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "Vp" << endl;
    for (auto & value : Vp){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sVp" << endl;
    for (auto & value : sVp){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Ppannello" << endl;
    for (auto & value : Ppannello){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;} 
    out << endl;

    out << "sPpannello" << endl;
    for (auto & value : sPpannello){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;} 
    out << endl;


  




    //II parte. le misure sono tutte dirette, ometto il pedice _d; uso il perdice_320

    const int nmisure_320 = 33;

    //320 era la 10ma misura prima

    //distanza [0 del carrello - pannello] 
    //mm
    float d_2_320 = d2[10];
    float sd_2_320 = sd2[10];
    float d_320 = d1 + 320;
    float sd_320 = sqrt(pow(sd_2_320,2) + pow(sd1,2));
    cout << d_320 << "+-" << sd_320 << endl;

    //mA
    float Iamp_320[nmisure_320] = {7.847,7.74,7.626,7.518,7.401,7.279,7.155,7.033,6.908,6.775,6.651,6.532,6.412,6.297,6.179,6.065,5.953,5.843,5.733,5.623,5.521,5.421,5.324,5.231,5.14,4.736,4.036,3.506,3.092,2.773,2.504,2.281,2.108};//33

    //mV
    float Vvolt_320[nmisure_320] = {96.9,103.3,109.4,115,120.5,125.9,131,135.7,140.3,145,149,152.7,156.2,159.6,162.8,165.8,168.8,171.5,174.1,176.8,179,181.2,183.3,185.3,187.2,195,207,215.1,220.9,225.1,228.5,231.2,233.3};//33

    //mV
    float Vp_320 = Vp_d[10];
    float sVp_320 =  sVp_d[10];

    //Ohm
    float Rvar_320[nmisure_320] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,30,40,50,60,70,80,90,100};//33
    float sRvar_320[nmisure_320];
    for (int i = 0; i < nmisure_320; ++i){sRvar_320[i] = 0.05 * Rvar_320[i];}

    //Ohm
    float Ramp_320 = Ramp_d[10];
    float sRamp_320 = sRamp_d[10];
    
    //Ohm
    float Rcarico_320[nmisure_320];
    for (int i = 0; i < nmisure_320; ++i){Rcarico_320[i] = Ramp_320 + Rvar_320[i];}
    float sRcarico_320[nmisure_320];
    for (int i = 0; i < nmisure_320; ++i){sRcarico_320[i] = sqrt(pow(sRamp_320,2) + pow(sRvar_320[i],2));};
    
    //mV / mA - Ohm = Ohm
    float Rp_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){Rp_320[i] = Vp_320 / Iamp_320[i] - Rcarico_320[i];}
    //mV / Ohm = mA
    float Icc_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){Icc_320[i] = Vp_320 / Rp_320[i];}
    //mV * mA = µW
    float Ppannello_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){Ppannello_320[i] = Vp_320 * Icc_320[i];}

    float Pcarico_320[nmisure_320];
    for (int i = 0; i < nmisure_320; ++i){Pcarico_320[i] = Iamp_320[i] * Vvolt_320[i];}
   
   


    //errori sulle grandezze misurate, con luce diretta
    float sIamp_320[nmisure_320];
    for(int i = 0;i < nmisure_320;i++){
        if(Iamp_320[i] < 10.0) sIamp_320[i] = (0.5  / 100) * Iamp_320[i] + 5 * 0.001;
        if(Iamp_320[i] >= 10.0) sIamp_320[i] = (0.5 / 100) * Iamp_320[i] + 5 * 0.01;
        cout << sIamp_320[i] << endl;
    };
    cout << endl;

    float sVvolt_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){
        if(Vvolt_320[i] < 100.0) sVvolt_320[i] = (0.1 / 100) * Vvolt_320[i] + 5 * 0.01;
        if(Vvolt_320[i] >= 100.0) sVvolt_320[i] = (0.1 / 100) * Vvolt_320[i] + 5 * 0.1;
        cout << sVvolt_320[i] << endl;
    };

    cout << endl;

    float sRp_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){sRp_320[i] = sqrt(1 / pow(Iamp_320[i],2) * pow(sVp_320,2) + pow(Vp_320,2) / pow(Iamp_320[i],4) * pow(sIamp_320[i],2) + 1 * pow(sRamp_320,2));}

    float sIcc_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){sIcc_320[i] = sqrt(pow(Vp_320,2) / pow(Rp_320[i],4) * pow(sRp_320[i],2) + 1 / pow(Rp_320[i],2) * pow(sVp_320,2));}

    float sPpannello_320[nmisure_320];
    for(int i = 0; i < nmisure_320; i++){sPpannello_320[i] = sqrt(pow(Vp_320,2) * pow(sIcc_320[i],2) + pow(Icc_320[i],2) * pow(sVp_320,2));}

    float sPcarico_320[nmisure_320];
    for (int i = 0; i < nmisure_320; ++i){sPcarico_320[i] = sqrt(pow(Vvolt_320[i],2) * pow(sIamp_320[i],2) + pow(Iamp_320[i],2) * pow(sVvolt_320[i],2));
      cout << sPcarico_320[i] << endl;}


    out << "Iamp_320" << endl;
    for (auto & value : Iamp_320){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "sIamp_320" << endl;
    for (auto & value : sIamp_320){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "Vvolt_320" << endl;
    for (auto & value : Vvolt_320){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sVvolt_320" << endl;
    for (auto & value : sVvolt_320){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "Ramp_320" << endl;
    out << fixed << showpoint << setprecision(7) << Ramp_320 << endl;

    out << "sRamp_320" << endl;
    out << fixed << showpoint << setprecision(7) << sRamp_320 << endl;

    out << "Vp_320" << endl;
    out << fixed << showpoint << setprecision(7) << Vp_320 << endl;

    out << "sVp_320" << endl;
    out << fixed << showpoint << setprecision(7) << sVp_320 << endl;

    out << "Rp_320" << endl;
    for (auto & value : Rp_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sRp_320" << endl;
    for (auto & value : sRp_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sRp_320" << endl;
    for (auto & value : sRp_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Icc_320" << endl;
    for (auto & value : Icc_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sIcc_320" << endl;
    for (auto & value : sIcc_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Ppannello_320" << endl;
    for (auto & value : Ppannello_320){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;}
    out << endl;

    out << "sPpannello_320" << endl;
    for (auto & value : sPpannello_320){out << fixed << showpoint << setprecision(7) << value / 1e6 << endl;}



    out << "Rcarico_320" << endl;
    for (auto & value : Rcarico_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sRcarico_320" << endl;
    for (auto & value : sRcarico_320){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "Pcarico_320" << endl;
    for (auto & value : Pcarico_320){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sPcarico_320" << endl;
    for (auto & value : sPcarico_320){out << fixed << showpoint << setprecision(7) << value << endl;}
   



    /*
    float Ramp_media_d;
    float sRamp_media_f;
    float somma_amp_d = 0;
    for(int i = 0; i < nmisure; i++){somma_amp_d = somma_amp_d + Iamp_d[i];}
    Ramp_media_d = somma_amp_d / nmisure;

    float Ramp_media_f;
    float sRamp_media_f;
    float somma_amp_f = 0;
    for(int i = 0; i < nmisure; i++){somma_amp_f = somma_amp_f + Iamp_f[i];}
    Ramp_media_f = somma_amp_f / nmisure;

    float Ramp_media = (Ramp_media_d + Ramp_media_f) / 2;
    float sRamp_media;

    //Ohm
    float Rcarico [nmisure2];
    float sRCarico[nmisure2];
    for(int i = 0; i < nmisure 2; i++){Rvar[i] + Ramp_media;}
    for(int i = 0; i < nmisure 2; i++){sqrt(pow(sRvar[i],2) + pow(sRamp_media[i],2));}
    */

    TCanvas * cPpannelloRcarico_320 = new TCanvas("cPpannelloRcarico_320","grafico5",0,0,1280,720);
    cPpannelloRcarico_320->SetFillColor(0);
    cPpannelloRcarico_320->cd();
    TGraphErrors * gPpannelloRcarico_320 = new TGraphErrors(nmisure_320,Rcarico_320,Pcarico_320,sRcarico_320,sPcarico_320);
    gPpannelloRcarico_320->SetMarkerSize(0.4);
    gPpannelloRcarico_320->SetMarkerStyle(20);
    gPpannelloRcarico_320->SetTitle("P_{carico} vs R_{carico}, d = (419#pm1) mm");
    gPpannelloRcarico_320->GetXaxis()->SetTitle("R_{carico} (#Omega)");
    gPpannelloRcarico_320->GetYaxis()->SetTitle("P_{carico} (#muW)");
    gPpannelloRcarico_320->Draw("ALP");
    cPpannelloRcarico_320->SaveAs("cPpannelloRcarico_320.png");

    TCanvas * cIccVvolt_320 = new TCanvas("cIccVvolt_320","grafico6",0,0,1280,720);
    cIccVvolt_320->SetFillColor(0);
    cIccVvolt_320->cd();
    TGraphErrors * gIccVvolt_320 = new TGraphErrors(nmisure_320,Vvolt_320,Icc_320,sVvolt_320,sIcc_320);
    gIccVvolt_320->SetMarkerSize(0.4);
    gIccVvolt_320->SetMarkerStyle(20);
    gIccVvolt_320->SetTitle("I_{cc} vs V_{volt}, d = (419#pm1) mm");
    gIccVvolt_320->GetXaxis()->SetTitle("I_{cc} (mA)");
    gIccVvolt_320->GetYaxis()->SetTitle("V_{volt} (mV)");
    gIccVvolt_320->Draw("ALP");
    cIccVvolt_320->SaveAs("cIccVvolt_320.png");


};
