#include <iostream>   
#include <iomanip>  
#include <vector>
//#include <map>
//#include <pair>
#include <cmath>
#include <string>
#include <algorithm>

//ROOT classes
#include <TF1.h>
#include <TCanvas.h>
#include <TGraph.h> 
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h> 
#include <TMultiGraph.h>     
#include <TAxis.h>                   
#include "TVirtualFitter.h"  
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>

int planck4()
{

	//nm
	//spettrofotometro e Avantes
	float lambda432 = 432;

	//nA
	//picoamperometro Keithley 6485
	float i432[] = {1.44205, 1.22023, 1.03151, 0.85753, 0.69949, 0.56435, 0.43209, 0.33108, 0.24289, 0.13131, 0.05955, 0.02894, 0.01162, 0.00385, -0.00070, -0.00129, -0.00222, -0.00264, -0.00330, -0.00396, -0.00443, -0.00490, -0.00592, -0.00591, -0.00621, -0.00675, -0.00710, -0.00720, -0.00732, -0.00737, -0.00743, -0.00740, -0.00741, -0.00742, -0.00745, -0.00747, -0.00750, -0.00749};
	//V
	//Amprobe 37XR-A
	float Vc432[] = {0.0013, 0.1058, 0.2020, 0.3016, 0.4041, 0.5030, 0.6122, 0.7077, 0.8037, 0.9572, 1.105, 1.205, 1.308, 1.402, 1.500, 1.523, 1.557, 1.580, 1.612, 1.650, 1.685, 1.723, 1.786, 1.824, 1.873, 1.986, 2.093, 2.174, 2.298, 2.383, 2.510, 2.643, 2.796, 2.966, 3.312, 3.673, 4.017, 4.247};

	//conversione in mV
	for(float * it = begin(Vc432); it < end(Vc432); ++it)
	{
		(*it) *= 1e3;
	}

	vector<float> Vc432_vec(std::begin(Vc432), std::end(Vc432));
	vector<float> i432_vec(std::begin(i432), std::end(i432));

	vector<float> s_i432_vec = vector<float>();
	for(int i = 0; i < i432_vec.size(); i++)
	{
		s_i432_vec.push_back(0.00001);
	}

	vector<float> s_Vc432_vec = vector<float>();
	for(auto & element : Vc432_vec)
	{
		if(element < 10)
		{
			s_i432_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i432_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i432_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i432_vec.push_back(1);
		}
	}


	//------------------------------
		//nm
		//spettrofotometro e Avantes
	float lambda607 = 607;

	//nA
	//picoamperometro Keithley 6485
	float i607[] = {1.41866, 1.21512, 0.99699, 0.82279, 0.66419, 0.51716, 0.40090, 0.29129, 0.19903, 0.13573, 0.07832, 0.03099, 0.01194, 0.00256, 0.00079, -0.00030, -0.00119, -0.0017, -0.00203, -0.00234, -0.00253, -0.00265, -0.00275, -0.00286, -0.00283, -0.00286, -0.00288, -0.00288, -0.00289, -0.00291, -0.00291, -0.00295, -0.00294, -0.00297, -0.00298, -0.00298, -0.00299};
	//mV
	//Amprobe 37XR-A
	float Vc607[] = {0.0020, 0.0495, 0.1029, 0.1509, 0.1994, 0.2510, 0.2983, 0.3507, 0.4041, 0.4485, 0.4984, 0.5553, 0.5968, 0.6505, 0.6760, 0.7011, 0.7324, 0.7605, 0.7902, 0.8209, 0.8516, 0.8796, 0.9063, 0.9418, 0.972, 1.002, 1.054, 1.127, 1.200, 1.306, 1.499, 2.000, 2.495, 3.028, 3.504, 3.996, 4.246};

	//conversione in mV
	for(float * it = begin(Vc607); it < end(Vc607); ++it)
	{
		(*it) *= 1e3;
	}

	vector<float> Vc607_vec(std::begin(Vc607), std::end(Vc607));
	vector<float> i607_vec(std::begin(i607), std::end(i607));

	vector<float> s_i607_vec = vector<float>();
	for(int i = 0; i < i607_vec.size(); i++)
	{
		s_i607_vec.push_back(0.00001);
	}

	vector<float> s_Vc607_vec = vector<float>();
	for(auto & element : Vc607_vec)
	{
		if(element < 10)
		{
			s_i607_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i607_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i607_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i607_vec.push_back(1);
		}
	}


	//---------------------------------

		//nm
		//spettrofotometro e Avantes
	float lambda592 = 592;

	//nA
	//picoamperometro Keithley 6485
	float i592[] = {1.44125, 1.17509, 1.00578, 0.75620, 0.59730, 0.47672, 0.36510, 0.28436, 0.19616, 0.12877, 0.07305, 0.02848, 0.01320, 0.00498, 0.00115, -0.00034, -0.00116, -0.00177, -0.00236, -0.00283, -0.00309, -0.00326, -0.00340, -0.00349, -0.00353, -0.00356, -0.00361, -0.00363, -0.00362, -0.00364, -0.00362, -0.00365, -0.00367, -0.00369, -0.00369, -0.00369, -0.00372, -0.00372, -0.00373, -0.00373};

	//mV
	//Amprobe 37XR-A
	float Vc592[] = {0.0012, 0.0728, 0.1202, 0.1984, 0.2555, 0.3043, 0.3560, 0.3985, 0.4523, 0.5009, 0.5495, 0.6048, 0.6383, 0.6780, 0.7191, 0.7479, 0.7716, 0.7969, 0.8281, 0.8610, 0.8923,0.9201, 0.9511, 0.9806, 1.008, 1.041, 1.071, 1.101, 1.161, 1.223, 1.285, 1.341, 1.405,1.507, 2.011, 2.507, 3.005, 3.508, 3.893, 4.243};

	for(float * it = begin(Vc592); it < end(Vc592); ++it)
	{
		(*it) *= 1e3;
	}

	vector<float> Vc592_vec(std::begin(Vc592), std::end(Vc592));
	vector<float> i592_vec(std::begin(i592), std::end(i592));

	vector<float> s_i592_vec = vector<float>();
	for(int i = 0; i < i592_vec.size(); i++)
	{
		s_i592_vec.push_back(0.00001);
	}

	vector<float> s_Vc592_vec = vector<float>();
	for(auto & element : Vc592_vec)
	{
		if(element < 10)
		{
			s_i592_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i592_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i592_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i592_vec.push_back(1);
		}
	}


	//---------------------------------

		//nm
		//spettrofotometro e Avantes
	float lambda639 = 639;

	//nA
	//picoamperometro Keithley 6485
	float i639[] = {1.45730, 1.16953, 0.88494, 0.67999, 0.48789, 0.33343, 0.21242, 0.11357, 0.04688, 0.00978, 0.00242, 0.00101, -0.00048, -0.00138, -0.00197, -0.00229, -0.00253, -0.00270, -0.00274, -0.00282, -0.00286, -0.00286, -0.00288, -0.00291, -0.00292, -0.00293, -0.00296, 0.00292, -0.00293, -0.00294, -0.00295, -0.00295, -0.00296, -0.00297};

	//mV
	//Amprobe 37XR-A
	float Vc639[] = {0.0016, 0.0581, 0.1229, 0.1792, 0.2412, 0.3028, 0.3615, 0.4222, 0.4775, 0.5441, 0.6212, 0.6085, 0.6400, 0.6712, 0.7011, 0.7308, 0.7602, 0.7904, 0.8204, 0.8516, 0.8822, 0.9415, 1.002, 1.100, 1.201, 1.348, 1.499, 1.755, 2.005, 2.485, 3.005, 3.431, 3.818, 4.244};

	//conversione in mV
	for(float * it = begin(Vc639); it < end(Vc639); ++it)
	{
		(*it) *= 1e3;
	}

	vector<float> Vc639_vec(std::begin(Vc639), std::end(Vc639));
	vector<float> i639_vec(std::begin(i639), std::end(i639));

	vector<float> s_i639_vec = vector<float>();
	for(int i = 0; i < i639_vec.size(); i++)
	{
		s_i639_vec.push_back(0.00001);
	}

	vector<float> s_Vc639_vec = vector<float>();
	for(auto & element : Vc639_vec)
	{
		if(element < 10)
		{
			s_i639_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i639_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i639_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i639_vec.push_back(1);
		}
	}

	//-----------------------

		//nm
		//spettrofotometro e Avantes
	float lambda621 = 621;

	//nA
	//picoamperometro Keithley 6485
	float i621[] = {1.42369, 1.14779, 0.91383, 0.71759, 0.53516, 0.38618, 0.25095, 0.16134, 0.08109, 0.02823, 0.01443, 0.00650, 0.00267, 0.00104, -0.00045, -0.00133, -0.00176, -0.00216, -0.00231, -0.00249, -0.00266, -0.00265, -0.00270, -0.00272, -0.00273, -0.00276, -0.00275, -0.00273, -0.00274, -0.00277, -0.00275, -0.00279, -0.00275, -0.00278, -0.00275, -0.00275, -0.00280};

	//mV
	//Amprobe 37XR-A
	float Vc621[] = {0.0026, 0.0634, 0.1215, 0.1783, 0.2397, 0.3001, 0.3667, 0.4201, 0.4798, 0.5401, 0.5699, 0.6025, 0.6335, 0.4775, 0.6916, 0.7248, 0.7502, 0.7845, 0.8148, 0.8421, 0.8693, 0.9014, 0.9338, 0.9617, 1.001, 1.054, 1.100, 1.204, 1.351, 1.509, 1.751, 1.984, 2.499, 2.990, 3.492, 3.788, 4.244};

	//conversione in mV
	for(float * it = begin(Vc621); it < end(Vc621); ++it)
	{
		(*it) *= 1e3;
	}

	vector<float> Vc621_vec(std::begin(Vc621), std::end(Vc621));
	vector<float> i621_vec(std::begin(i621), std::end(i621));

	vector<float> s_i621_vec = vector<float>();
	for(int i = 0; i < i621_vec.size(); i++)
	{
		s_i621_vec.push_back(0.00001);
	}

	vector<float> s_Vc621_vec = vector<float>();
	for(auto & element : Vc621_vec)
	{
		if(element < 10)
		{
			s_i621_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i621_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i621_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i621_vec.push_back(1);
		}
	}


	//spettrofotometro e Avantes
	float lambda552 = 552;

	//nA
	//picoamperometro Keithley 6485
	float i552[] = {1.37768, 1.05872, 0.78763, 0.56905, 0.39723, 0.24794, 0.13863, 0.05666, 0.01485, 0.00210, -0.00083, -0.00143, -0.00202, -0.00252, -0.00283, -0.00298, -0.00310, -0.00338, -0.00332, -0.00350, -0.00349, -0.00353, -0.00351, -0.00355, -0.00355,-0.00355, -0.00363, -0.00360, -0.00359, -0.00360, -0.00358};

	//mV
	//Amprobe 37XR-A
	float Vc552[] = {0.0025, 0.1019, 0.2026, 0.3007, 0.3958, 0.5005, 0.5995, 0.6994, 0.7969, 0.8996, 0.9721, 1.012, 1.051, 1.099, 1.135, 1.168, 1.200, 1.242, 1.281, 1.329, 1.358, 1.403, 1.500, 1.621, 1.801, 2.012, 2.407, 2.793, 3.218, 3.694, 4.244};

	//conversione in mV
	for(float * it = begin(Vc552); it < end(Vc552); ++it)
	{
		(*it) *= 1e3;
	}

	vector<float> Vc552_vec(std::begin(Vc552), std::end(Vc552));
	vector<float> i552_vec(std::begin(i552), std::end(i552));

	vector<float> s_i552_vec = vector<float>();
	for(int i = 0; i < i552_vec.size(); i++)
	{
		s_i552_vec.push_back(0.00001);
	}

	vector<float> s_Vc552_vec = vector<float>();
	for(auto & element : Vc552_vec)
	{
		if(element < 10)
		{
			s_i552_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i552_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i552_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i552_vec.push_back(1);
		}

		//---------------------------------

			//nm
			//spettrofotometro e Avantes
		float lambda477 = 477;

		//nA
		//picoamperometro Keithley 6485
		float i477[] = {1.42159, 1.18098, 0.97085, 0.76679, 0.59988, 0.44704, 0.31903, 0.22249, 0.13666, 0.06688, 0.02397, 0.01337, 0.00698, 0.00253, 0.00068, -0.00009, -0.00096, -0.00147, -0.00208, -0.00251, -0.00296, -0.00328, -0.00353,  -0.00372, -0.00399, -0.00414, -0.00421, -0.00427, -0.00444, -0.00459, -0.00462, -0.00471, -0.00468, -0.00477, -0.00476, -0.00480, -0.00482, -0.00484, -0.00488, -0.00486, -0.00492, -0.00486, -0.00492};

		//mV
		//Amprobe 37XR-A
		float Vc477[] = {0.0023, 0.1056, 0.1987, 0.3023, 0.4000, 0.5042, 0.6070, 0.6996, 0.7994, 0.9035, 1.005, 1.051, 1.097, 1.152, 1.195, 1.220, 1.254, 1.280, 1.318, 1.350, 1.382, 1.417, 1.451, 1.481, 1.517, 1.545, 1.576, 1.609, 1.654, 1.687, 1.734, 1.767, 1.804, 1.852, 1.901, 2.001, 2.258, 2.492, 2.755, 3.009, 3.471, 3.914, 4.244};

		//conversione in mV
		for(float * it = begin(Vc477); it < end(Vc477); ++it)
		{
			(*it) *= 1e3;
		}

		vector<float> Vc477_vec(std::begin(Vc477), std::end(Vc477));
		vector<float> i477_vec(std::begin(i477), std::end(i477));

		vector<float> s_i477_vec = vector<float>();
		for(int i = 0; i < i477_vec.size(); i++)
		{
			s_i477_vec.push_back(0.00001);
		}

		vector<float> s_Vc477_vec = vector<float>();
		for(auto & element : Vc477_vec)
		{
			if(element < 10)
			{
				s_i477_vec.push_back(0.1);
			}
			if(element > 10 && element < 100)
			{
				s_i477_vec.push_back(0.1);
			}
			if(element >= 100 && element < 1000)
			{
				s_i477_vec.push_back(0.1);
			}
			if(element >= 10000 && element <= 10000)
			{
				s_i477_vec.push_back(1);
			}


		}



		TGraphErrors * g_i432_Vc432 = new TGraphErrors(Vc432_vec.size(), i432_vec.data(), Vc432_vec.data(), s_i432_vec.data(), s_Vc432_vec.data());
		g_i432_Vc432->SetMarkerColor(4);
		g_i432_Vc432->SetMarkerStyle(20);
		g_i432_Vc432->SetMarkerSize(0.4);
		g_i432_Vc432->SetMarkerStyle(20);
		g_i432_Vc432->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i432_Vc432->GetYaxis()->SetTitle("i (nA)");

		TGraphErrors * g_i607_Vc607 = new TGraphErrors(Vc607_vec.size(), i607_vec.data(), Vc607_vec.data(), s_i607_vec.data(), s_Vc607_vec.data());
		g_i607_Vc607->SetMarkerColor(42);
		g_i607_Vc607->SetMarkerStyle(20);
		g_i607_Vc607->SetMarkerSize(0.4);
		g_i607_Vc607->SetMarkerStyle(20);
		g_i607_Vc607->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i607_Vc607->GetYaxis()->SetTitle("i (nA)");

		TGraphErrors * g_i592_Vc592 = new TGraphErrors(Vc592_vec.size(), i592_vec.data(), Vc592_vec.data(), s_i592_vec.data(), s_Vc592_vec.data());
		g_i592_Vc592->SetMarkerColor(5);
		g_i592_Vc592->SetMarkerStyle(20);
		g_i592_Vc592->SetMarkerSize(0.4);
		g_i592_Vc592->SetMarkerStyle(20);
		g_i592_Vc592->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i592_Vc592->GetYaxis()->SetTitle("i (nA)");

		TGraphErrors * g_i639_Vc639 = new TGraphErrors(Vc639_vec.size(), i639_vec.data(), Vc639_vec.data(), s_i639_vec.data(), s_Vc639_vec.data());
		g_i639_Vc639->SetMarkerColor(2);
		g_i639_Vc639->SetMarkerStyle(20);
		g_i639_Vc639->SetMarkerSize(0.4);
		g_i639_Vc639->SetMarkerStyle(20);
		g_i639_Vc639->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i639_Vc639->GetYaxis()->SetTitle("i (nA)");

		TGraphErrors * g_i621_Vc621 = new TGraphErrors(Vc621_vec.size(), i621_vec.data(), Vc621_vec.data(), s_i621_vec.data(), s_Vc621_vec.data());
		g_i621_Vc621->SetMarkerColor(46);
		g_i621_Vc621->SetMarkerStyle(20);
		g_i621_Vc621->SetMarkerSize(0.4);
		g_i621_Vc621->SetMarkerStyle(20);
		g_i621_Vc621->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i621_Vc621->GetYaxis()->SetTitle("i (nA)");

		TGraphErrors * g_i552_Vc552 = new TGraphErrors(Vc477_vec.size(), i477_vec.data(), Vc477_vec.data(), s_i477_vec.data(), s_Vc477_vec.data());
		g_i552_Vc552->SetMarkerColor(3);
		g_i552_Vc552->SetMarkerStyle(20);
		g_i552_Vc552->SetMarkerSize(0.4);
		g_i552_Vc552->SetMarkerStyle(20);
		g_i552_Vc552->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i552_Vc552->GetYaxis()->SetTitle("i (nA)");

		TGraphErrors * g_i477_Vc477 = new TGraphErrors(Vc477_vec.size(), i477_vec.data(), Vc477_vec.data(), s_i477_vec.data(), s_Vc477_vec.data());
		g_i477_Vc477->SetMarkerColor(7);
		g_i477_Vc477->SetMarkerStyle(20);
		g_i477_Vc477->SetMarkerSize(0.4);
		g_i477_Vc477->SetMarkerStyle(20);
		g_i477_Vc477->GetXaxis()->SetTitle("V_{C} (mV)");
		g_i477_Vc477->GetYaxis()->SetTitle("i (nA)");

		TCanvas * c_i_Vc = new TCanvas("c_i_Vc", "grafico1", 0, 0, 1280, 720);
		c_i_Vc->cd();


		g_i432_Vc432->Draw("AP");
		g_i607_Vc607->Draw("AP");
		g_i592_Vc592->Draw("AP");
		g_i639_Vc639->Draw("AP");
		g_i621_Vc621->Draw("AP");
		g_i552_Vc552->Draw("AP");
		g_i477_Vc477->Draw("AP");



		TMultiGraph * mg_i_Vc = new TMultiGraph("mg_i_Vc", "#scale[0.6]{V_{C} vs i, tutti i LED} #scale[0.4]{#splitline{#splitline{#color[4]{#bullet #lambda_{p} = 432 (nm)} #color[42]{#bullet #lambda_{p} = 607 (nm)}}{#color[5]{#bullet #lambda_{p} = 592 (nm)} #color[2]{#bullet #lambda_{p} = 639 (nm)}}}{#color[46]{#bullet #lambda_{p} = 621 (nm)} #color[3]{#bullet #lambda_{p} = 552 (nm)} #color[7]{#bullet #lambda_{p} = 477 (nm)}}}");
		mg_i_Vc->GetXaxis()->SetTitle("i (nA)");
		mg_i_Vc->GetYaxis()->SetTitle("V_{C} (mV)");
		mg_i_Vc->Add(g_i432_Vc432);
		mg_i_Vc->Add(g_i607_Vc607);
		mg_i_Vc->Add(g_i592_Vc592);
		mg_i_Vc->Add(g_i639_Vc639);
		mg_i_Vc->Add(g_i621_Vc621);
		mg_i_Vc->Add(g_i552_Vc552);
		mg_i_Vc->Add(g_i477_Vc477);
		mg_i_Vc->Draw("AP");



		return 0;
	}
}
//V_{C} vs i, tutti i LED #color[4]{#bullet #lambda_{p} = 432 (nm)} #color[7]{#bullet #lambda_{p} = 607 (nm)}
					//                                                          #color[8]{#bullet #lambda_{p} = 592 (nm)} #color[3]{#bullet #lambda_{p} = 639 (nm)}
					//                                                          #color[5]{#bullet #lambda_{p} = 621 (nm)} #color[2]{#bullet #lambda_{p} = 477 (nm)}
