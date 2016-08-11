#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int which_pmt(double x0, double y0, double z0, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5]);
int n_of_pmts(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);
bool is_early(double t, double limit);
bool is_cosmic(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);
bool is_beam(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);
bool early_5(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);
bool late_1234(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);
bool there_is_5(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);
bool there_is_late(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold);


void plot_coordinates(){

  std::string base="/scratch/01230/fnova/output/";
std::string direc="2014_06_30_cosmics__CHIPS__refl_10__at_42__depth_60__2_mirrors";
 
  //base="/scratch/01230/fnova/programs/";
  //direc="CHIPS";
 

  TFile input_file((base+direc+"/"+direc+"_data.root").c_str(),"READ");

  double t0;

  TFile output_file((base+direc+"/coordinates.root").c_str(),"RECREATE");

  TTree *output_tree = new TTree("analysis","analysis");

  std::vector<float> x0s, y0s, z0s;
  std::vector<float> times, energies, lengths, deltatimes;

  output_tree->Branch("times", &times);
  output_tree->Branch("deltatimes", &deltatimes);
  output_tree->Branch("energies", &energies);
  output_tree->Branch("lengths", &lengths);
  output_tree->Branch("x0s", &x0s);
  output_tree->Branch("y0s", &y0s);
  output_tree->Branch("z0s", &z0s);


  std::clog << " doing file " << direc << std::endl;
  bool first = true;
  int modulo = 10000;
  int counter = 0;

  TTree *input_tree = (TTree*)input_file.Get("RunTree");
  double time, energy, length, x0, y0, z0, deltatime;
  int event_number;
  int event_number_old = 0;
  TBranch * b_event_number;
  input_tree->SetBranchAddress("event_number", &event_number, &b_event_number);
  TBranch * b_time;
  input_tree->SetBranchAddress("time", &time, &b_time);
  TBranch * b_energy;
  input_tree->SetBranchAddress("energy", &energy, &b_energy);
  TBranch * b_length;
  input_tree->SetBranchAddress("length", &length, &b_length);
  TBranch * b_x0;
  input_tree->SetBranchAddress("x0", &x0, &b_x0);
  TBranch * b_y0;
  input_tree->SetBranchAddress("y0", &y0, &b_y0);
  TBranch * b_z0;
  input_tree->SetBranchAddress("z0", &z0, &b_z0);

  int nbins_x = 29;
  int nbins_y = 29;
  int nbins_z = 30;
  int nbins_phi  = 92;
  int nbins_time = 100;
  double phi, radius;
  double z_limit=2.2;
  double r_limit=2.;
  double tmin=0.;
  double tmax=100.;
  double labelsize=0.045;

  //                top1  top2  side1  side2  bot
  double xmax[5] = {-1.8, -1.8, -2.2, -2.2, 2.5};
  double xmin[5] = {-2.0, -2.0, -2.4, -2.4, 2.0};
  double ymax[5] = {-0.2,  0.22, -0.2, 0.22, 0.01};
  double ymin[5] = {-0.22, 0.2,  -0.22, 0.2, -0.01};
  double zmax[5] = {2.5,   2.5,  2.1,  2.1, -2.1};
  double zmin[5] = {2.3,   2.3,  1.9,  1.9, -2.5};

  double t_early = 20.;
  double threshold = 1.;


  TH1F h_x0("h_x0","x0;x0 (m);number of events",nbins_x,1,-1);
  h_x0.GetXaxis()->SetLabelSize(labelsize);
  h_x0.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_y0("h_y0","y0;y0 (m);number of events",nbins_y,1,-1);
  h_y0.GetXaxis()->SetLabelSize(labelsize);
  h_y0.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_z0("h_z0","z0;z0 (m);number of events",nbins_z,1,-1);
  h_z0.GetXaxis()->SetLabelSize(labelsize);
  h_z0.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_y0_vs_x0("h_y0_vs_x0","y0_vs_x0;x0 (m);y0 (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1));
  h_y0_vs_x0.GetXaxis()->SetLabelSize(labelsize);
  h_y0_vs_x0.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_y0_vs_x0_top("h_y0_vs_x0_top","y0_vs_x0_top;x0 (m);y0 (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1));
  h_y0_vs_x0_top.GetXaxis()->SetLabelSize(labelsize);
  h_y0_vs_x0_top.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_y0_vs_x0_bottom("h_y0_vs_x0_bottom","y0_vs_x0_bottom;x0 (m);y0 (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1));
  h_y0_vs_x0_bottom.GetXaxis()->SetLabelSize(labelsize);
  h_y0_vs_x0_bottom.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_vs_y0("h_z0_vs_y0","z0_vs_y0;y0 (m);z0 (m)",nbins_y,-(r_limit+0.1),(r_limit+0.1),nbins_z,-(r_limit+0.1),(r_limit+0.1));
  h_z0_vs_y0.GetXaxis()->SetLabelSize(labelsize);
  h_z0_vs_y0.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_vs_x0("h_z0_vs_x0","z0_vs_x0;x0 (m);z0 (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_z,-(r_limit+0.1),(r_limit+0.1));
  h_z0_vs_x0.GetXaxis()->SetLabelSize(labelsize);
  h_z0_vs_x0.GetYaxis()->SetLabelSize(labelsize);
  //  TH3F h_z0_vs_y0_vs_x0("h_z0_vs_y0_vs_x0","z0_vs_y0_vs_x0;x0 (m); y0 (m);z0 (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1),nbins_z,-(r_limit+0.1),(r_limit+0.1));
  TH2F h_z0_vs_phi("h_z0_vs_phi","z0_vs_phi;phi (degrees);z0 (m)",nbins_phi,-290.,110.,nbins_z,-(z_limit+0.4),(z_limit+0.4));
  h_z0_vs_phi.GetXaxis()->SetLabelSize(labelsize);
  h_z0_vs_phi.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_vs_phi_upstream("h_z0_vs_phi_upstream","z0_vs_phi_upstream;phi (degrees);z0 (m)",nbins_phi,-290.,110.,nbins_z,-(z_limit+0.4),(z_limit+0.4));
  h_z0_vs_phi_upstream.GetXaxis()->SetLabelSize(labelsize);
  h_z0_vs_phi_upstream.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_vs_phi_downstream("h_z0_vs_phi_downstream","z0_vs_phi_downstream;phi (degrees);z0 (m)",nbins_phi,-290.,110.,nbins_z,-(z_limit+0.4),(z_limit+0.4));
  h_z0_vs_phi_downstream.GetXaxis()->SetLabelSize(labelsize);
  h_z0_vs_phi_downstream.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_time("h_time","time; time(ns);number of events",nbins_time,tmin,tmax);
  h_time.GetXaxis()->SetLabelSize(labelsize);
  h_time.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_deltatime("h_deltatime","deltatime; deltatime(ns);number of events",nbins_time,tmin,tmax);
  h_deltatime.GetXaxis()->SetLabelSize(labelsize);
  h_deltatime.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_which_pmt("h_which_pmt", "which_pmt;which_pmt;number of events",6,0,6);
  h_which_pmt.GetXaxis()->SetLabelSize(labelsize);
  h_which_pmt.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_n_of_pmts("h_n_of_pmts", "n_of_pmts;n_of_pmts;number of events",6,0,6);
  h_n_of_pmts.GetXaxis()->SetLabelSize(labelsize);
  h_n_of_pmts.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_is_early("h_is_early", "is_early;is_early;number of events",2,0,2);
  h_is_early.GetXaxis()->SetLabelSize(labelsize);
  h_is_early.GetYaxis()->SetLabelSize(labelsize);

  TH1F h_is_early_5("h_is_early_5", "is_early_5;is_early_5;number of events",2,0,2);
  h_is_early_5.GetXaxis()->SetLabelSize(labelsize);
  h_is_early_5.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_is_late_1234("h_is_late_1234", "is_late_1234;is_late_1234;number of events",2,0,2);
  h_is_late_1234.GetXaxis()->SetLabelSize(labelsize);
  h_is_late_1234.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_there_is_5("h_there_is_5", "there_is_5;there_is_5;number of events",2,0,2);
  h_there_is_5.GetXaxis()->SetLabelSize(labelsize);
  h_there_is_5.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_there_is_late("h_there_is_late", "there_is_late;there_is_late;number of events",2,0,2);
  h_there_is_late.GetXaxis()->SetLabelSize(labelsize);
  h_there_is_late.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_is_cosmic("h_is_cosmic", "is_cosmic;is_cosmic;number of events",2,0,2);
  h_is_cosmic.GetXaxis()->SetLabelSize(labelsize);
  h_is_cosmic.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_is_beam("h_is_beam", "is_beam;is_beam;number of events",2,0,2);
  h_is_beam.GetXaxis()->SetLabelSize(labelsize);
  h_is_beam.GetYaxis()->SetLabelSize(labelsize);


  Long64_t tentry;
  std::clog << " reading " <<  input_tree->GetEntries() << " entries " << std::endl;
  for (Int_t ientry = 0; ientry < input_tree->GetEntries(); ientry++) {

    if( ientry%modulo == 0 ) std::clog << " reading entry " << ientry << std::endl;

    // load branches values
    tentry = input_tree->LoadTree(ientry);
    b_event_number->GetEntry(tentry);
    b_time->GetEntry(tentry);
    b_energy->GetEntry(tentry);
    b_length->GetEntry(tentry);
    b_x0->GetEntry(tentry);
    b_y0->GetEntry(tentry);
    b_z0->GetEntry(tentry);

    if( first ){
      event_number_old = event_number;
      first = false;
    }

    if( event_number != event_number_old ){
      t0 = *std::min_element(times.begin(), times.end());
      for(size_t it=0; it<times.size(); ++it){
	deltatimes.push_back(times[it] - t0);
      }


      output_tree->Fill();


      for(size_t i=0; i<x0s.size(); i++){
	x0 = x0s.at(i);
	y0 = y0s.at(i);
	z0 = z0s.at(i);
	time = times.at(i);
	deltatime = deltatimes.at(i);
	
	h_x0.Fill(x0);
	h_y0.Fill(y0);
	h_z0.Fill(z0);
	h_y0_vs_x0.Fill(x0, y0);
	h_z0_vs_y0.Fill(y0, z0);
	h_z0_vs_x0.Fill(x0, z0);
	h_time.Fill(time);
	h_deltatime.Fill(deltatime);
	//h_z0_vs_y0_vs_x0.Fill(x0, y0, z0);
	radius=sqrt(pow(x0,2) + pow(y0,2));
	phi = atan2(y0, x0)*180./acos(-1.);
	if( phi > 90. ) phi -= 360.;
	
	if( radius > r_limit ){  // side
	  h_z0_vs_phi.Fill(phi, z0);
	  if( phi > - 90. )
	    h_z0_vs_phi_upstream.Fill(phi, z0);
	  else
	    h_z0_vs_phi_downstream.Fill(phi, z0);
	}else{ // top and bottom
	  if( z0 > z_limit ){
	    h_y0_vs_x0_top.Fill(x0, y0);
	  }else{
	    h_y0_vs_x0_bottom.Fill(x0, y0);
	  }
	}

	int _which_pmt =  which_pmt(x0, y0, z0, xmin, xmax, ymin, ymax, zmin, zmax);
	h_which_pmt.Fill(_which_pmt);
	h_is_early.Fill(is_early(deltatime, t_early));
      }

      h_n_of_pmts.Fill(n_of_pmts(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));

      h_is_early_5.Fill(early_5(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));
      h_is_late_1234.Fill(late_1234(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));
      h_there_is_5.Fill(there_is_5(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));
      h_there_is_late.Fill(there_is_late(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));
      h_is_cosmic.Fill(is_cosmic(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));
      h_is_beam.Fill(is_beam(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, t_early, threshold));

      times.clear();
      energies.clear();
      lengths.clear();
      x0s.clear();
      y0s.clear();
      z0s.clear();
      event_number_old = event_number;
    }

    if( x0 != 0. || y0 != 0. || z0 != 0. && which_pmt(x0, y0, z0, xmin, xmax, ymin, ymax, zmin, zmax) != 0 ){
      times.push_back(time);
      energies.push_back(energy);
      lengths.push_back(length);
      x0s.push_back(x0);
      y0s.push_back(y0);
      z0s.push_back(z0);
    }
 
  }

  input_file.Close();


  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TCanvas c("c","c",600,600);
  double BottomMargin = .17;   // fraction of pad height
  double LeftMargin = .1;
  double RightMargin = .18;
  c.SetBottomMargin(BottomMargin);
  c.SetLeftMargin(LeftMargin);
  c.SetRightMargin(RightMargin);

  string figname="figure/"+direc+"_"+h_y0_vs_x0.GetTitle()+".png";
  h_y0_vs_x0.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_z0_vs_phi.GetTitle()+".png";
  h_z0_vs_phi.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_z0_vs_phi_upstream.GetTitle()+".png";
  h_z0_vs_phi_upstream.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_z0_vs_phi_downstream.GetTitle()+".png";
  h_z0_vs_phi_downstream.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_y0_vs_x0_top.GetTitle()+".png";
  h_y0_vs_x0_top.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_y0_vs_x0_bottom.GetTitle()+".png";
  h_y0_vs_x0_bottom.Draw("colz");
  c.Print(figname.c_str());

  string figname="figure/"+direc+"_"+h_time.GetTitle()+".png";
  h_time.Draw();
  c.Print(figname.c_str());


  TCanvas c_display("c_display","c_display",600,600);
  c_display.SetBottomMargin(BottomMargin);
  c_display.SetLeftMargin(LeftMargin);
  c_display.SetRightMargin(RightMargin);

  TPad * topPad;
  TPad * sidePad;
  TPad * bottomPad;

  double zmax_top = h_y0_vs_x0_top.GetMaximum();
  double zmax_side = h_z0_vs_phi.GetMaximum();
  double zmax_bottom = h_y0_vs_x0_bottom.GetMaximum();
  double the_max = std::max(zmax_top, zmax_side);
  the_max = std::max(the_max, zmax_bottom);
  h_y0_vs_x0_top.GetZaxis()->SetRangeUser(0, the_max);
  h_z0_vs_phi.GetZaxis()->SetRangeUser(0, the_max);
  h_y0_vs_x0_bottom.GetZaxis()->SetRangeUser(0, the_max);
  double x_min=0.33;
  double x_max=0.67;
  double X_Min=0.005;
  double X_Max=0.995;
  double ymaxtop=0.995;
  double ymintop=0.66;
  double ymaxside=0.66;
  double yminside=0.33;
  double ymaxbottom=0.33;
  double yminbottom=0.005;
  c_display.Divide(1,3);
  topPad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_1");
  topPad->SetPad(x_min, ymintop, x_max, ymaxtop);
  sidePad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_2");
  sidePad->SetPad(X_Min, yminside, X_Max, ymaxside);
  bottomPad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_3");
  bottomPad->SetPad(x_min, yminbottom, x_max, ymaxbottom);

  topPad->cd();
  h_y0_vs_x0_top.Draw("col");
  sidePad->cd();
  h_z0_vs_phi.Draw("colz");
  bottomPad->cd();
  h_y0_vs_x0_bottom.Draw("col");
  TPaletteAxis *palette = (TPaletteAxis*)h_z0_vs_phi.GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.95);
  palette->SetY1NDC(0.2);
  palette->SetY2NDC(0.8);
  c_display.Modified();
  c_display.Update();

  figname="figure/"+direc+"_display.eps";
  c_display.Print(figname.c_str());
  figname="figure/"+direc+"_display.png";
  c_display.Print(figname.c_str());

  std::clog << " selected as beam: " << h_is_beam.GetBinContent(2)/h_is_beam.GetEntries() << std::endl;
  std::clog << " selected as cosmic: " << h_is_cosmic.GetBinContent(2)/h_is_cosmic.GetEntries() << std::endl;

  output_file.Write();


  exit(0);


}

int which_pmt(double x0, double y0, double z0, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5]){

  for(size_t i=0; i<5; i++)
    if( x0 > xmin[i] && x0 < xmax[i] &&
	y0 > ymin[i] && y0 < ymax[i] &&
	z0 > zmin[i] && z0 < zmax[i] )
      return i+1;

  return 0;

}


int n_of_pmts(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){

  int active[5];
  for(size_t i=0; i<5; i++)
    active[i] = 0;

  for(size_t i=0; i<x0s.size(); i++){
    int wp = which_pmt(x0s[i], y0s[i], z0s[i], xmin, xmax, ymin, ymax, zmin, zmax ) ;
    if( wp > 0 && wp < 6 )
      active[wp]++;
  }

  int count = 0;
  for(size_t i=0; i<5; i++)
    if( active[i] > threshold )
      count ++;

  return count;

}

bool is_early(double t, double limit){
  
  return (t < limit );

}

bool early_5(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){

  double count = 0.;

  for(size_t i=0; i<x0s.size(); i++){
    int wp = which_pmt(x0s[i], y0s[i], z0s[i], xmin, xmax, ymin, ymax, zmin, zmax ) ;
    if( wp == 5 ) // veto pmt is active
      if( is_early( deltatimes[i], limit) ){ // veto pmt is early
	count ++;
      }

    if( count > threshold ) return true;
  }

  return false;
}

bool late_1234(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){

  double count = 0.;

  for(size_t i=0; i<x0s.size(); i++){
    int wp = which_pmt(x0s[i], y0s[i], z0s[i], xmin, xmax, ymin, ymax, zmin, zmax ) ;
    if( wp == 1 || wp == 2 || wp == 3 || wp == 4 ) // beam pmt is active
      if( !is_early( deltatimes[i], limit) ){ // beam pmt is late
	count++;
      }

    if( count > threshold ) return true;
  }

  return false;
}


bool is_cosmic(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){

  if( !early_5(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, limit, threshold) ) return false;

  return late_1234(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, limit, threshold);

}



bool there_is_5(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){

  double count = 0.;

  for(size_t i=0; i<x0s.size(); i++){
    int wp = which_pmt(x0s[i], y0s[i], z0s[i], xmin, xmax, ymin, ymax, zmin, zmax ) ;
    if( wp == 5 ) // veto pmt is active
      count ++;
  }

  return( count > threshold );

}


bool there_is_late(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){
  double count = 0.;

  for(size_t i=0; i<x0s.size(); i++){
    if( !is_early( deltatimes[i], limit) ){ // pmt is late
      count ++;
    }
  }

  return (count > threshold);
}


bool is_beam(std::vector<float> x0s, std::vector<float> y0s, std::vector<float> z0s, std::vector<float> deltatimes, double xmin[5], double xmax[5], double ymin[5], double ymax[5], double zmin[5], double zmax[5], double limit, double threshold){

  if( there_is_5(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, limit, threshold ) ) return false;
  if( there_is_late(x0s, y0s, z0s, deltatimes, xmin, xmax, ymin, ymax, zmin, zmax, limit, threshold ) ) return false;

  return true;
}

