#include <iostream>
#include <vector>
#include <TH1F.h>
#include <string>
#include <cmath>

using namespace std;



vector<float> recursiveFitHistogramWithGaussian1(TCanvas * CANVAS, vector<float> DATA, string TITLE, string X_AXIS_LABEL, float BIN_WIDTH, int N_OF_SIGMAS, float FROM, float TO){

	cout << endl << "C A L L E D" << endl << endl;

	float DATA_min = *min_element(begin(DATA), end(DATA));
	float DATA_max = *max_element(begin(DATA), end(DATA));
	
	cout << "DATA_min: " << DATA_min << endl;
	cout << "DATA_max: " << DATA_max << endl;
	
	vector<float> bins_low_edges_DATA;

	for(float i = DATA_min - 3 * BIN_WIDTH / 2; i < DATA_max + BIN_WIDTH / 2; i += BIN_WIDTH){
		cout << i << endl;
		bins_low_edges_DATA.push_back(i);
	}

	cout << "bins_low_edges_DATA.size(): " << bins_low_edges_DATA.size() << endl;

    string title_canvas_str = TITLE;
    char title_canvas_char[title_canvas_str.size() + 1];
    std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
    title_canvas_char[title_canvas_str.size()] = '\0';


	float n_of_bins = bins_low_edges_DATA.size() - 1;

	TH1F * h_DATA  = new TH1F("Stats", title_canvas_char, n_of_bins, bins_low_edges_DATA.data());
	for(auto & datum: DATA){h_DATA->Fill(datum);}
	h_DATA->Fit("gaus");



    string x_axis_label_str = X_AXIS_LABEL;
    char x_axis_label_char[x_axis_label_str.size() + 1];
    std::copy(x_axis_label_str.begin(), x_axis_label_str.end(), x_axis_label_char);
    x_axis_label_char[x_axis_label_str.size()] = '\0';

    h_DATA->GetXaxis()->SetTitle(x_axis_label_char);
    h_DATA->GetYaxis()->SetTitle("frequenze (adim)");

	TF1 * fz_gauss_DATA = h_DATA->GetFunction("gaus");
	float chi2_fz_gaus_DATA = fz_gauss_DATA->GetChisquare();
	fz_gauss_DATA->SetRange(FROM, TO);

	cout << "chi2: " << chi2_fz_gaus_DATA << endl;

	int exit_or_no_exit = 0;

	float constant_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(0);
	float mean_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(1);
	float sigma_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(2);

	for(vector<float>::const_iterator it = DATA.begin(); it != DATA.end(); it++){
		if(abs(*it - mean_of_fz_gauss_DATA) > N_OF_SIGMAS * sigma_of_fz_gauss_DATA){
			DATA.erase(it);
			++exit_or_no_exit;
		}
	}

	if(exit_or_no_exit == 0){};
	if(exit_or_no_exit != 0){recursiveFitHistogramWithGaussian1(CANVAS, DATA, TITLE, X_AXIS_LABEL, BIN_WIDTH, N_OF_SIGMAS, FROM, TO);}

	float scarto_medio_assoluto = 0;
	for(auto & datum: DATA){scarto_medio_assoluto += abs(datum - mean_of_fz_gauss_DATA);}
	scarto_medio_assoluto = scarto_medio_assoluto / DATA.size();


	vector<float> return_vec {constant_of_fz_gauss_DATA, mean_of_fz_gauss_DATA, sigma_of_fz_gauss_DATA, scarto_medio_assoluto};
	
	cout << "constant_of_fz_gauss_DATA: " << constant_of_fz_gauss_DATA << endl << "mean_of_fz_gauss_DATA: " << mean_of_fz_gauss_DATA << endl << "sigma_of_fz_gauss_DATA: " 
		 << sigma_of_fz_gauss_DATA <<  endl << "scarto_medio_assoluto: " << scarto_medio_assoluto << endl;
	return return_vec;
}



vector<float> recursiveFitHistogramWithGaussian2(TCanvas * CANVAS, vector<float> DATA, string TITLE, string X_AXIS_LABEL, int N_OF_SIGMAS){

	CANVAS->cd();
	cout << endl << "C A L L E D" << endl << endl;

	float DATA_min = *min_element(begin(DATA), end(DATA));
	float DATA_max = *max_element(begin(DATA), end(DATA));
	
	cout << "DATA_min: " << DATA_min << endl;
	cout << "DATA_max: " << DATA_max << endl;
	
	string title_canvas_str = TITLE;
    char title_canvas_char[title_canvas_str.size() + 1];
    std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
    title_canvas_char[title_canvas_str.size()] = '\0';


	int n_of_bins = round(sqrt(DATA.size()));
	cout << "n_of:bins: " << n_of_bins << endl;

	TH1F * h_DATA  = new TH1F("Stats", title_canvas_char, n_of_bins, DATA_min, DATA_max);

	for(auto & datum: DATA){h_DATA->Fill(datum);}
	h_DATA->Fit("gaus");


    string x_axis_label_str = X_AXIS_LABEL;
    char x_axis_label_char[x_axis_label_str.size() + 1];
    std::copy(x_axis_label_str.begin(), x_axis_label_str.end(), x_axis_label_char);
    x_axis_label_char[x_axis_label_str.size()] = '\0';

    h_DATA->GetXaxis()->SetTitle(x_axis_label_char);
    h_DATA->GetYaxis()->SetTitle("frequenze (adim)");

	TF1 * fz_gauss_DATA = h_DATA->GetFunction("gaus");
	float chi2_fz_gaus_DATA = fz_gauss_DATA->GetChisquare();
	//fz_gauss_DATA->SetRange(FROM, TO);

	cout << "chi2: " << chi2_fz_gaus_DATA << endl;

	int exit_or_no_exit = 0;

	float constant_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(0);
	float mean_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(1);
	float sigma_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(2);

	for(vector<float>::const_iterator it = DATA.begin(); it != DATA.end(); it++){
		if(abs(*it - mean_of_fz_gauss_DATA) > N_OF_SIGMAS * sigma_of_fz_gauss_DATA){
			DATA.erase(it);
			++exit_or_no_exit;
		}
	}

	if(exit_or_no_exit == 0){};
	if(exit_or_no_exit != 0){recursiveFitHistogramWithGaussian2(CANVAS, DATA, TITLE, X_AXIS_LABEL, N_OF_SIGMAS);}


	float scarto_medio_assoluto = 0;
	for(auto & datum: DATA){scarto_medio_assoluto += abs(datum - mean_of_fz_gauss_DATA);}
	scarto_medio_assoluto = scarto_medio_assoluto / DATA.size();


	vector<float> return_vec {constant_of_fz_gauss_DATA, mean_of_fz_gauss_DATA, sigma_of_fz_gauss_DATA, scarto_medio_assoluto};
	
	cout << "constant_of_fz_gauss_DATA: " << constant_of_fz_gauss_DATA << endl << "mean_of_fz_gauss_DATA: " << mean_of_fz_gauss_DATA << endl << "sigma_of_fz_gauss_DATA: " 
		 << sigma_of_fz_gauss_DATA <<  endl << "scarto_medio_assoluto: " << scarto_medio_assoluto << endl;
	return return_vec;
}	


int lenti(){

	gStyle->SetOptFit();

	vector<TCanvas *> c_tcanvas_ptr_vec;

    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 1, DRITTA, AVVICINO ALLA LENTE POI ALLONTANO_______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente1_dritta_avvicinamento = new TCanvas("c_convergente1_dritta_avvicinamento", "c_convergente1_dritta_avvicinamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente1_dritta_avvicinamento);

	float q_convergente1_dritta_avvicinamento [] = {1520, 1522, 1522, 1526, 1511, 1517, 1516, 1513, 1515, 1517, 1514, 1518, 1516, 1514, 1515, 1512, 1517, 1518, 1517, 1519, 1518, 1516, 1522, 1520, 1512, 
													1514, 1511, 1521, 1519, 1518, 1513, 1513, 1515, 1515, 1517, 1512, 1515, 1514, 1529, 1515, 1512, 1517, 1515, 1530, 1512, 1514, 1515, 1522, 1516, 1512, 1514, 1512, 1515, 1515, 
													1512, 1512, 1519, 1512, 1512, 1516};

	vector<float> q_convergente1_dritta_avvicinamento_vec = vector<float>(begin(q_convergente1_dritta_avvicinamento), end(q_convergente1_dritta_avvicinamento));


	vector<float> q_convergente1_dritta_avvicinamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente1_dritta_avvicinamento, q_convergente1_dritta_avvicinamento_vec, "Prima lente convergente, dritta, avvicinamento", "q' (cm)", 3);




    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 1, DRITTA, ALLONTANO ALLA LENTE POI AVVICINO_______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente1_dritta_allontanamento = new TCanvas("c_convergente1_dritta_allontanamento", "c_convergente1_dritta_allontanamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente1_dritta_allontanamento);

	float q_convergente1_dritta_allontanamento [] = {1515, 1526, 1525, 1522, 1523, 1525, 1526, 1522, 1523, 1525, 1524, 1519, 1514, 1516, 1524, 1519, 1520, 1519, 1537, 1523, 1518, 1523, 1521, 1522, 1519, 
													 1522, 1515, 1521, 1521, 1523, 1521, 1525, 1523, 1523, 1524, 1529, 1524, 1523, 1527, 1519, 1522, 1521, 1522, 1519, 1521, 1523, 1520, 1526, 1526, 1523, 
													 1519, 1520, 1521, 1521, 1523, 1518, 1519, 1523, 1517};

	vector<float> q_convergente1_dritta_allontanamento_vec = vector<float>(begin(q_convergente1_dritta_allontanamento), end(q_convergente1_dritta_allontanamento));


	vector<float> q_convergente1_dritta_allontanamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente1_dritta_allontanamento, q_convergente1_dritta_allontanamento_vec, "Prima lente convergente, dritta, allontanamento", "q' (cm)", 3);




    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 1, RUOTATA, AVVICINO ALLA LENTE POI ALLONTANO______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente1_ruotata_avvicinamento = new TCanvas("c_convergente1_ruotata_avvicinamento", "c_convergente1_ruotata_avvicinamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente1_ruotata_avvicinamento);

	float q_convergente1_ruotata_avvicinamento [] = {1525, 1527, 1525, 1523, 1514, 1525, 1522, 1515, 1514, 1523, 1515, 1516, 1515, 1515, 1521, 1514, 1524, 1527, 1514, 1515, 1518, 1520, 1518, 1522, 1524, 
													 1522, 1517, 1513, 1524, 1518, 1513, 1517, 1517, 1513, 1515, 1513, 1509, 1515, 1512, 1523};

	vector<float> q_convergente1_ruotata_avvicinamento_vec = vector<float>(begin(q_convergente1_ruotata_avvicinamento), end(q_convergente1_ruotata_avvicinamento));


	vector<float> q_convergente1_ruotata_avvicinamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente1_ruotata_avvicinamento, q_convergente1_ruotata_avvicinamento_vec, "Prima lente convergente, ruotata, avvicinamento", "q' (cm)", 3);




	cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 1, RUOTATA, ALLONTANO ALLA LENTE POI AVVICINO______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente1_ruotata_allontanamento = new TCanvas("c_convergente1_ruotata_allontanamento", "c_convergente1_ruotata_allontanamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente1_ruotata_allontanamento);

	float q_convergente1_ruotata_allontanamento [] = {1511, 1515, 1511, 1517, 1516, 1514, 1519, 1522, 1527, 1519, 1524, 1520, 1514, 1523, 1524, 1528, 1515, 1513, 1518, 1516, 1523, 1516,  1522, 1517, 
													  1522, 1521, 1520, 1522, 1519, 1520, 1515, 1520, 1515, 1520, 1516, 1514, 1526, 1523, 1518, 1521};

	vector<float> q_convergente1_ruotata_allontanamento_vec = vector<float>(begin(q_convergente1_ruotata_allontanamento), end(q_convergente1_ruotata_allontanamento));


	vector<float> q_convergente1_ruotata_allontanamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente1_ruotata_allontanamento, q_convergente1_ruotata_allontanamento_vec, "Prima lente convergente, ruotata, allontanamento", "q' (cm)", 3);




	cout << "__________________________________________________________________________________SECONDA LENTE___________________________________________________________________________________" << endl;


 	cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 2, DRITTA, AVVICINO ALLA LENTE POI ALLONTANO_______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente2_dritta_avvicinamento = new TCanvas("c_convergente2_dritta_avvicinamento", "c_convergente2_dritta_avvicinamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente2_dritta_avvicinamento);

	float q_convergente2_dritta_avvicinamento [] = {786, 796, 812, 803, 790, 800, 797, 789, 797, 779, 797, 796, 789, 799, 787, 802, 794, 808, 778, 784, 795, 807, 800, 782, 783, 783, 796, 800, 798, 802, 
													789, 791, 783, 800, 786, 797, 793, 774, 803, 779};

	vector<float> q_convergente2_dritta_avvicinamento_vec = vector<float>(begin(q_convergente2_dritta_avvicinamento), end(q_convergente2_dritta_avvicinamento));


	vector<float> q_convergente2_dritta_avvicinamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente2_dritta_avvicinamento, q_convergente2_dritta_avvicinamento_vec, "Seconda lente convergente, dritta, avvicinamento", "q' (cm)", 3);




    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 2, DRITTA, ALLONTANO ALLA LENTE POI AVVICINO_______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente2_dritta_allontanamento = new TCanvas("c_convergente2_dritta_allontanamento", "c_convergente2_dritta_allontanamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente2_dritta_allontanamento);

	float q_convergente2_dritta_allontanamento [] = {829, 816, 833, 838, 831, 837, 830, 828, 838, 829, 843, 840, 843, 842, 838, 835, 827, 837, 836, 831, 826, 831, 836, 828, 830, 833, 831, 836, 821, 835, 
													 833, 841, 932, 831, 829, 835, 822, 834, 828, 839};

	vector<float> q_convergente2_dritta_allontanamento_vec = vector<float>(begin(q_convergente2_dritta_allontanamento), end(q_convergente2_dritta_allontanamento));


	vector<float> q_convergente2_dritta_allontanamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente2_dritta_allontanamento, q_convergente2_dritta_allontanamento_vec, "Seconda lente convergente, dritta, allontanamento", "q' (cm)", 3);




    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 2 RUOTATA, AVVICINO ALLA LENTE POI ALLONTANO_______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente2_ruotata_avvicinamento = new TCanvas("c_convergente2_ruotata_avvicinamento", "c_convergente2_ruotata_avvicinamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente2_ruotata_avvicinamento);

	float q_convergente2_ruotata_avvicinamento [] = {804, 795, 778, 794, 780, 782, 773, 793, 795, 786, 794, 803, 787, 796, 789, 797, 789, 785, 800, 798, 799, 786, 795, 798, 796, 794, 803, 795, 792, 790, 
													 791, 790, 792, 794, 790, 780, 791, 802, 775, 790};

	vector<float> q_convergente2_ruotata_avvicinamento_vec = vector<float>(begin(q_convergente2_ruotata_avvicinamento), end(q_convergente2_ruotata_avvicinamento));


	vector<float> q_convergente2_ruotata_avvicinamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente2_ruotata_avvicinamento, q_convergente2_ruotata_avvicinamento_vec, "Seconda lente convergente, ruotata, avvicinamento", "q' (cm)", 3);




	cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________LENTE 2, RUOTATA, ALLONTANO ALLA LENTE POI AVVICINO______________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TCanvas * c_convergente2_ruotata_allontanamento = new TCanvas("c_convergente2_ruotata_allontanamento", "c_convergente2_ruotata_allontanamento", 0, 0, 1280, 720);
	c_tcanvas_ptr_vec.push_back(c_convergente2_ruotata_allontanamento);

	float q_convergente2_ruotata_allontanamento [] = {1,2,3,4,5,6,7,8,9,10,11,12,13,12,11,10,9.8,7,6,5,4,3,2,1};

	vector<float> q_convergente2_ruotata_allontanamento_vec = vector<float>(begin(q_convergente2_ruotata_allontanamento), end(q_convergente2_ruotata_allontanamento));


	vector<float> q_convergente2_ruotata_allontanamento_returned_vec = recursiveFitHistogramWithGaussian2(c_convergente2_ruotata_allontanamento, q_convergente2_ruotata_allontanamento_vec, "Seconda lente convergente, ruotata, allontanamento AAAAAAAAAAAAAAAAAAAAAAAAAA", "q' (cm)", 3);



















/*
	for(vector<TCanvas *>::const_iterator it = c_ptr_vec.begin(); it != c_ptr_vec.end(); it++){
		(*it)->cd();
		(*it)->Draw();
	}
	*/


return 0;
}