#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>


void led()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << fixed;

	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.9);
	gStyle->SetStatY(0.4);


	//tutte le misure
	const int nmisure = 34;

	//numero di misure non a corrente nulla
	//V
	const int nmisure1 = 22;

	//le grandezze con ...1 sono quelle con i punti sperimentali in meno.

	//V
	//Amprobe 37XR-A
	//#punti = 34
	float Vgen[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.2,1.4,1.6,1.7,1.8,2,2.2,2.4,2.6,2.8,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

	//V
	//Amprobe 37XR-A
	//#punti = 22
	float Vgen1[] = {1.6,1.7,1.8,2,2.2,2.4,2.6,2.8,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

	//V
	//Amprobe 37XR-A
	//#punti = 34
	float Vd[] = {0.104,0.275,0.298,0.434,0.546,0.639,0.7755,0.854,0.993,1.067,1.237,1.426,1.598,1.625,1.643,1.68,1.693,1.705,1.714,1.724,1.73,1.757,1.777,1.79,1.803,1.813,1.823,1.831,1.839,1.846,1.853,1.859,1.866,1.872};

	//V
	//Amprobe 37XR-A
	//#punti = 22
	float Vd1[] = {1.598,1.625,1.643,1.68,1.693,1.705,1.714,1.724,1.73,1.757,1.777,1.79,1.803,1.813,1.823,1.831,1.839,1.846,1.853,1.859,1.866,1.872};

	//mA
	//Amprobe 37XR-A
	//#punti = 34
	float Id[] = {0,0,0,0,0,0,0,0,0,0,0,0,0.03,0.055,0.082,0.19,0.264,0.343,0.427,0.523,0.603,1.039,1.502,1.918,2.392,2.824,3.296,3.747,4.18,4.628,5.077,5.522,6.001,6.447};

	//mA
	//Amprobe 37XR-A
	//#punti = 22
	float Id1[] = {0.03,0.055,0.082,0.19,0.264,0.343,0.427,0.523,0.603,1.039,1.502,1.918,2.392,2.824,3.296,3.747,4.18,4.628,5.077,5.522,6.001,6.447};



	//Amprobe 37XR-A 
	//DC Voltage Ranges 1000 mV, 10 V, 100 V, 1000 V ± (0.1 % rdg + 5 dgt)
	//DC Current 100 μA range: ± (0.5 % rdg + 10 dgt)
	//           1000 μA to 400 mA ranges: ± (0.5 % rdg + 5 dgts)


	//V
	//errore di sensibilità dell'Amprobe 37XR-A
	float sVgen[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVgen[i] = 0.1;
	};
	float sVgen1[nmisure];
	for(int i = 0; i < nmisure1; i++)
	{
		sVgen1[i] = 0.1;
	};

	//V
	//errore di sensibilità dell'Amprobe 37XR-A
	float sVd[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVd[i] = 0.001;
	}//{sVd[i] = 0.001 * Vd[i] + 5 * 0.001;};
	float sVd1[nmisure];
	for(int i = 0; i < nmisure1; i++)
	{
		sVd1[i] = 0.001;
	}//{sVd1[i] = 0.001 * Vd1[i] + 5 * 0.001;};

//mA
//errore di sensibilità dell'Amprobe 37XR-A
	float sId[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		if(Id[i] < 1)
		{
			sId[i] = 0.001;
		}//{sId[i] = 0.005 * Id[i] + 10 * 0.001;}; //[0-1(mA)]
		if(Id[i] >= 1)
		{
			sId[i] = 0.001;
		} // {sId[i] = 0.005 * Id[i] +  5 * 0.001;}; //[1(mA)-400(mA)]
	};
	float sId1[nmisure];
	for(int i = 0; i < nmisure1; i++)
	{
		if(Id[i] < 1)
		{
			sId[i] = 0.001;
		}//if(Id1[i] < 1){sId1[i] = 0.005 * Id1[i] + 10 * 0.001;}; //[0-1(mA)]
		if(Id[i] >= 1)
		{
			sId[i] = 0.001;
		}//if(Id1[i] >= 1) {sId1[i] = 0.005 * Id1[i] +  5 * 0.001;}; //[1(mA)-400(mA)]
	};

	//errori sull'ultima cifra. ho provato a usare l'errore sull'ultima cifra
	//come errore perchè il chi quadro con gli errori dai datasheets dei multimetri
	//viene troppo piccolo e dunque gli errori potrebbero essere sovrastimati. 
	//tuttavia con l'errore sull'ultima cifra il chi quadro viene troppo grande
	float sVd2[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVd2[i] = 0.005;
	};
	float sId2[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sId2[i] = 0.005;
	};


	//print dei dati
	for(int j = 0; j < nmisure; j++)
	{
		std::cout << "Measurement number " << j << ":"
			<< " \t Vgen = (" << setprecision(5) << Vgen[j] << " +- " << setprecision(4) << sVgen[j] << ") V,"
			<< " \t Vd = (" << setprecision(5) << Vd[j] << " +- " << setprecision(4) << sVd[j] << ") V,"
			<< " \t Id = (" << setprecision(5) << Id[j] << " +- " << setprecision(4) << sId[j] << ") mA,"
			<< " \t sVgen/Vgen " << setprecision(3) << sVgen[j] / Vgen[j]
			<< " \t sVd/Vd " << setprecision(3) << sVd[j] / Vd[j]
			<< " \t sId/Id " << setprecision(3) << sId[j] / Id[j]
			<< endl;
	}

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________________________________________________________________FIT CON N = 34____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * cVdId1 = new TCanvas("cVdId1", "grafico1", 0, 0, 1280, 720);
	cVdId1->SetFillColor(0);
	cVdId1->cd();

	TGraphErrors * gVdId1 = new TGraphErrors(nmisure, Id, Vd, sId, sVd);
	gVdId1->SetMarkerSize(0.4);
	gVdId1->SetMarkerStyle(20);
	gVdId1->SetTitle("#scale[1]{Funzione di fit:    V_{d} = #eta #upoint 0.026(V) #upoint ln#left(I_{d} / I_{s} + 1#right) + R_{d}I_{d}}");
	gVdId1->GetXaxis()->SetTitle("I_{d} (mA)");
	gVdId1->GetYaxis()->SetTitle("V_{d} (V)");
	gVdId1->Draw("AP");

	std::cout << "\n\n --- Ipotesi  Vd = [0] * 0.026 * log1p(x / [1]) + [2] * x --- \n" << endl;
	TF1 * funz1 = new TF1("funz1", "[0] * 0.026 * log1p( x / [1]) + [2] * x", 0, 7);
	funz1->SetParNames("#eta (adim)", "I_{s} (mA)", "R_{d} (k#Omega)");
	funz1->SetParameter(0, 1.5);
	funz1->SetParameter(1, 1e-15); //mA  mettendo lo stesso valore del diodo al silicio il chi quadro esplode
								  //e anche visivamente il fit non è buono, nonostante quelli che sono errori 
								  //piuttosto grandi, dunque ho ampliato il range per Is e l'ho fissato a un valore più o meno in mezzo
	funz1->SetParameter(2, 5e-3);  //kOhm

	funz1->SetParLimits(0, 0, 3);            //adim
	funz1->SetParLimits(1, 1e-20, 1e-11);    //mA //variando l'estremo superiore ottengo comportamenti strani... lo lascio così perch converge e viene errore più piccolo del parametro, però basta variare un po il range e succedono cose strane, anche se il parametro rimane comunque più o meno nello stesso punto all'interno dell'intervallo. secondo me gli errori sono troppo grandi e l'algortimo dunque non funziona bene
	funz1->SetParLimits(2, 1e-4, 50e-3);     //kOhm

	funz1->SetLineColor(4);
	gVdId1->Fit(funz1, "ERM+");


	std::cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cVdId1->SaveAs("LED 1.png");

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________________________________________________________________FIT CON N = 22____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * cVdId2 = new TCanvas("cVdId2", "grafico2", 0, 0, 1280, 720);
	cVdId2->SetFillColor(0);
	cVdId2->cd();
	TGraphErrors * gVdId2 = new TGraphErrors(nmisure1, Id1, Vd1, sId1, sVd1);
	gVdId2->SetMarkerSize(0.4);
	gVdId2->SetMarkerStyle(20);
	gVdId2->SetTitle("#scale[1]{Funzione di fit:    V_{d} = #eta #upoint 0.026(V) #upoint ln#left(I_{d} / I_{s} + 1#right) + R_{d}I_{d}}");
	gVdId2->GetXaxis()->SetTitle("I_{d} (mA)");
	gVdId2->GetYaxis()->SetTitle("V_{d} (V)");
	gVdId2->Draw("AP");

	std::cout << "\n\n --- Ipotesi  Vd = [0] * 0.026 * log1p(x / [1]) + [2] * x --- \n" << endl;
	TF1 * funz2 = new TF1("funz1", "[0] * 0.026 * log1p( x / [1]) + [2] * x", 0, 7);
	funz2->SetParNames("#eta (adim)", "I_{s} (mA)", "R_{d} (k#Omega)");
	funz2->SetParameter(0, 1.5);
	funz2->SetParameter(1, 1e-15); //mA  mettendo lo stesso valore del diodo al silicio il chi quadro esplode
								  //e anche visivamente il fit non è buono, nonostante quelli che sono errori 
								  //piuttosto grandi, dunque ho ampliato il range per Is e l'ho fissato a un valore più o meno in mezzo
	funz2->SetParameter(2, 5e-3);  //kOhm

	funz2->SetParLimits(0, 0, 3);            //adim
	funz2->SetParLimits(1, 1e-20, 1e-11);    //mA //variando l'estremo superiore ottengo comportamenti strani... lo lascio così perch converge e viene errore più piccolo del parametro, però basta variare un po il range e succedono cose strane, anche se il parametro rimane comunque più o meno nello stesso punto all'interno dell'intervallo. secondo me gli errori sono troppo grandi e l'algortimo dunque non funziona bene
	funz2->SetParLimits(2, 1e-4, 50e-3);     //kOhm

	funz2->SetLineColor(4);
	gVdId2->Fit(funz2, "ERM+");


	std::cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cVdId2->SaveAs("LED 2.png");


	//__________________________________________________________________________________________________________________________________________________________________________________________________________________________
	std::ofstream out("DIODOSILICIO_CSV.csv");

	out << "Vgen (V)" << endl;
	for(auto & value : Vgen)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVgen (V)" << endl;
	for(auto & value : sVgen)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "Vd (V)" << endl;
	for(auto & value : Vd)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVd (V)" << endl;
	for(auto & value : sVd)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "Id (mA)" << endl;
	for(auto & value : Id)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sId (mA)" << endl;
	for(auto & value : sId)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;
}
//gli errori su Vd per il diodo LED sono più grandi che per il diodo al silicio 
//perchè il diodo LED raggiunge valori di Vd maggiori, che moltiplicando 
//i fattori dei datasheet dei multimetri diventano più grandi (di circa 7 volte), ma è tutto giusto
