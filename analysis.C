
struct EvtStat {
  int PhotonCount_Scint;

  int PhotonCount_Ceren;
  int HitCount;
  int PMTsAboveThreshold;
  int AbsorptionCount;
  int BoundaryAbsorptionCount;
  int Unacounted;

  int VetoPhotonCount_Ceren;
  int VetoHitCount;
  int VetoPMTsAboveThreshold;
  int VetoAbsorptionCount;
  int VetoBoundaryAbsorptionCount;
  int VetoUnacounted;

  int primaryPDG;

  double inX;
  double inY;
  double inZ;

  double outX;
  double outY;
  double outZ;

  double VetoinX;
  double VetoinY;
  double VetoinZ;

  double VetooutX;
  double VetooutY;
  double VetooutZ;

  double PX;
  double PY;
  double PZ;

  double primaryX;
  double primaryY;
  double primaryZ;
};  


void analysis(){

  TString base="/scratch/01230/fnova/output/2014_05_03_gps__CHIPS__refl_0__no_looking_down__yes__bottom__depth_60/2014_05_03_gps__CHIPS__refl_0__no_looking_down__yes__bottom__depth_60";
  //TString base="/scratch/01230/fnova/output/2014_04_10c__CHIPS__cosmics__module_chips_detector/2014_04_10c__CHIPS__cosmics__module_chips_detector";
  //TString base="/scratch/01230/fnova/output/2014_04_10c__CHIPS__gun__full_chips_detector/2014_04_10c__CHIPS__gun__full_chips_detector";
  //TString base="/scratch/01230/fnova/output/2014_04_10c__CHIPS__gun__module_chips_detector/2014_04_10c__CHIPS__gun__module_chips_detector";

  TFile input_file(base+".root","OPEN");
  TFile output_file(base+"_analysis.root","RECREATE");
  TTree *output_tree = new TTree("analysis","analysis");

  int nbins = 100;
  double ncerenmin=0.;
  double ncerenmax=2.5e6;
  TH1F h_PhotonCount_Scint("h_PhotonCount_Scint","h_PhotonCount_Scint",nbins,1,-1);
  TH1F h_PhotonCount_Ceren("h_PhotonCount_Ceren","number of Cerenkov #gamma's produced;number of Cerenkov #gamma's produced;number of events",nbins,ncerenmin, ncerenmax);
  TH1F h_PhotonCount_Ceren_top_to_bottom("h_PhotonCount_Ceren_top_to_bottom","top to bottom;Cerenkov #gamma's (top to bottom);number of events",nbins,ncerenmin, ncerenmax);
  TH1F h_PhotonCount_Ceren_top_to_side("h_PhotonCount_Ceren_top_to_side","top to side;Cerenkov #gamma's (top to side);number of events",nbins,ncerenmin, ncerenmax);
  TH1F h_PhotonCount_Ceren_side_to_bottom("h_PhotonCount_Ceren_side_to_bottom","side to bottom;Cerenkov #gamma's (side to bottom);number of events",nbins,ncerenmin, ncerenmax);
  TH1F h_PhotonCount_Ceren_side_to_side("h_PhotonCount_Ceren_side_to_side","side to side;Cerenkov #gamma's (side to side);number of events",nbins,ncerenmin, ncerenmax);
  TH1F h_HitCount("h_HitCount","number of #gamma's hitting a PMT;number of #gamma's hitting a PMT;number of events",nbins,1,-1);
  TH1F h_PMTsAboveThreshold("h_PMTsAboveThreshold","number of PMT's above threshold;number of PMT's above threshold;number of events",nbins,1,-1);
  TH1F h_AbsorptionCount("h_AbsorptionCount","number of #gamma's absorbed in water;number of #gamma's absorbed in water;number of events",nbins,1,-1);
  TH1F h_BoundaryAbsorptionCount("h_BoundaryAbsorptionCount","number of #gamma's absorbed in boundaries;number of #gamma's absorbed in boundaries;number of events",nbins,1,-1);
  TH1F h_primaryPDG("h_primaryPDG","primary id;primary id;number of events",nbins,1,-1);
  TH1F h_inX("h_inX","inX;inX (m);number of events",nbins,1,-1);
  TH1F h_inY("h_inY","inY;inY (m);number of events",nbins,1,-1);
  TH1F h_inZ("h_inZ","inZ;inZ (m);number of events",nbins,1,-1);
  TH2F h_inY_vs_inX("h_inY_vs_inX","inY_vs_inX;inX (m);inY (m)",nbins,1,-1,nbins,1,-1);
  TH1F h_outX("h_outX","outX;outX (m);number of events",nbins,1,-1);
  TH1F h_outY("h_outY","outY;outY (m);number of events",nbins,1,-1);
  TH1F h_outZ("h_outZ","outZ;outZ (m);number of events",nbins,1,-1);
  TH2F h_outY_vs_outX("h_outY_vs_outX","outY_vs_outX;outX (m);outY (m)",nbins,1,-1,nbins,1,-1);
  TH1F h_PX("h_PX","PX;PX (MeV);number of events",nbins,1,-1);
  TH1F h_PY("h_PY","PY;PY (MeV);number of events",nbins,1,-1);
  TH1F h_PZ("h_PZ","PZ;PZ (MeV);number of events",nbins,1,-1);
  TH2F h_PY_vs_PX("h_PY_vs_PX","PY_vs_PX;PX (MeV);PY (MeV)",nbins,1,-1,nbins,1,-1);
  TH1F h_primaryX("h_primaryX","primaryX;primaryX (m);number of events",nbins,1,-1);
  TH1F h_primaryY("h_primaryY","primaryY;primaryY (m);number of events",nbins,1,-1);
  TH1F h_primaryZ("h_primaryZ","primaryZ;primaryZ (m);number of events",nbins,1,-1);
  TH2F h_primaryY_vs_primaryX("h_primaryY_vs_primaryX","primaryY_vs_primaryX;primary X (m);primary Y (m)",nbins,1,-1,nbins,1,-1);
  TH1F h_length_top_to_bottom("h_length_top_to_bottom","length_top_to_bottom;length (top to bottom) (m);number of events",nbins,1,-1);
  TH1F h_length_side_to_bottom("h_length_side_to_bottom","length_side_to_bottom;length (side to bottom) (m);number of events",nbins,1,-1);
  TH1F h_length_top_to_side("h_length_top_to_side","length_top_to_side;length (top to side) (m);number of events",nbins,1,-1);
  TH1F h_length_side_to_side("h_length_side_to_side","length_side_to_side;length (side to side) (m);number of events",nbins,1,-1);
  TH1F h_P("h_P","P;P (MeV);number of events",nbins,1,-1);
  TH1F h_E("h_E","E;E (MeV);number of events",nbins,1,-1);
  TH1F h_cos2theta("h_cos2theta","cos2theta;cos^{2}#theta;number of events",nbins,1,-1);
  TH1F h_cos3theta("h_cos3theta","cos3theta;cos^{3}#theta;number of events",nbins,1,-1);
  TH1F h_phi("h_phi","phi;#phi;number of events",nbins,1,-1);
  THStack hs("hs","number of Cerenkov #gamma's produced");
  TH3F h_veto_in("h_veto_in", "h_veto_in; x (m); y (m); z (m)", 30, -2.,4.,30,-2.,2.,30,-64.,-59.);
  TH3F h_veto_out("h_veto_out", "h_veto_out; x (m); y (m); z (m)", 30, -2.,4.,30,-2.,2.,30,-64.,-59.);
  TH3F h_primary("h_primary", "h_primary; x (m); y (m); z (m)", 30, -2.,4.,30,-2.,2.,30,-64.,-59.);

  string tree_name = "RunTree";
  string branch_name = "Stat";
  /// open sequentially all input trees
  TIter nextkey(input_file.GetListOfKeys()); 
  TKey *key; 
  EvtStat stat;
  double dx, dy, dz, length;
  double default_value=666.;
  bool enter_from_top = false;
  bool exit_from_bottom = false;

  //double min_z_to_enter_from_top=-22.;  // full chips
  //double max_z_to_exit_from_bottom=-38.; // full chips
  double min_z_to_enter_from_top=-68.5;  // module chips
  double max_z_to_exit_from_bottom=-64.;  // module chips

  double logic_max_z = 0.;
  double momentum, energy, costheta, phi;
  double muon_mass=105.66; // MeV
  while (key = (TKey*)nextkey()) {
    string input_tree_name=key->GetName();
    string keyclass=key->GetClassName();
    //std::clog << " i " << input_tree_name << " k " << keyclass << std::endl;

    if( !strcmp(input_tree_name.c_str(),tree_name.c_str()) ){
      TTree * t = (TTree*)input_file.Get(input_tree_name.c_str());
      TBranch * b = (TBranch*)t->GetBranch(branch_name.c_str());
      t->SetBranchAddress(b->GetName(), &stat);

      /// loop on tree entries and fill histograms
      Long64_t tentry;
      for (Int_t ientry = 0; ientry < t->GetEntries(); ientry++) {
	// load branches values
	tentry = t->LoadTree(ientry);
	b->GetEntry(tentry);

	//if( stat.inZ > logic_max_z ) continue;
	//if( stat.outZ > logic_max_z ) continue;

	
	(stat.inZ > min_z_to_enter_from_top) ? enter_from_top = true : enter_from_top = false; 
	(stat.outZ < max_z_to_exit_from_bottom) ? exit_from_bottom = true : exit_from_bottom = false;


	h_veto_in.Fill(stat.VetoinX, stat.VetoinY, stat.VetoinZ);
	h_veto_out.Fill(stat.VetooutX, stat.VetooutY, stat.VetooutZ);
	h_primary.Fill(stat.primaryX, stat.primaryY, stat.primaryZ);

	h_PhotonCount_Scint.Fill(stat.PhotonCount_Scint);
	h_PhotonCount_Ceren.Fill(stat.PhotonCount_Ceren);
	if( enter_from_top && exit_from_bottom )
	  h_PhotonCount_Ceren_top_to_bottom.Fill(stat.PhotonCount_Ceren);
	else if( enter_from_top && !exit_from_bottom ) 
	  h_PhotonCount_Ceren_top_to_side.Fill(stat.PhotonCount_Ceren);
	else if( !enter_from_top && exit_from_bottom ) 
	  h_PhotonCount_Ceren_side_to_bottom.Fill(stat.PhotonCount_Ceren);
	else 
	  h_PhotonCount_Ceren_side_to_side.Fill(stat.PhotonCount_Ceren);
	h_HitCount.Fill(stat.HitCount);
	h_PMTsAboveThreshold.Fill(stat.PMTsAboveThreshold);
	h_AbsorptionCount.Fill(stat.AbsorptionCount);
	h_BoundaryAbsorptionCount.Fill(stat.BoundaryAbsorptionCount);
	h_primaryPDG.Fill(stat.primaryPDG);
	h_inX.Fill(stat.inX);
	h_inY.Fill(stat.inY);
	h_inZ.Fill(stat.inZ);
	h_inY_vs_inX.Fill(stat.inX, stat.inY);
	h_outX.Fill(stat.outX);
	h_outY.Fill(stat.outY);
	h_outZ.Fill(stat.outZ);
	h_outY_vs_outX.Fill(stat.outX, stat.outY);
	h_PX.Fill(stat.PX);
	h_PY.Fill(stat.PY);
	h_PZ.Fill(stat.PZ);
	h_PY_vs_PX.Fill(stat.PX, stat.PY);
	momentum=sqrt(
		      pow(stat.PX,2)+
		      pow(stat.PY,2)+
		      pow(stat.PZ,2));
	h_P.Fill(momentum);
	energy = sqrt(pow(momentum,2) + pow(muon_mass,2));
	h_E.Fill(energy);
	costheta = stat.PZ/momentum;
	phi = atan2(stat.PY, stat.PX);
	h_cos2theta.Fill(pow(costheta,2));
	h_cos3theta.Fill(pow(costheta,3));
	h_phi.Fill(phi);
  

	h_primaryX.Fill(stat.primaryX);
	h_primaryY.Fill(stat.primaryY);
	h_primaryZ.Fill(stat.primaryZ);
	h_primaryY_vs_primaryX.Fill(stat.primaryX, stat.primaryY);
	dx = stat.outX - stat.inX;
	dy = stat.outY - stat.inY;
	dz = stat.outZ - stat.inZ;
	length = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));

	if( enter_from_top && exit_from_bottom )
	  h_length_top_to_bottom.Fill(length);
	if( !enter_from_top && exit_from_bottom )
	  h_length_side_to_bottom.Fill(length);
	if( enter_from_top && !exit_from_bottom )
	  h_length_top_to_side.Fill(length);
	if( !enter_from_top && !exit_from_bottom )
	  h_length_side_to_side.Fill(length);


      }
    }

  }

  h_PhotonCount_Ceren_top_to_bottom.SetFillColor(kRed);
  h_PhotonCount_Ceren_top_to_side.SetFillColor(kBlue);
  h_PhotonCount_Ceren_side_to_bottom.SetFillColor(kGreen);
  h_PhotonCount_Ceren_side_to_side.SetFillColor(kBlack);

  hs.Add(&h_PhotonCount_Ceren_side_to_side);
  hs.Add(&h_PhotonCount_Ceren_side_to_bottom);
  hs.Add(&h_PhotonCount_Ceren_top_to_side);
  hs.Add(&h_PhotonCount_Ceren_top_to_bottom);
  hs.Write();

  output_tree->Fill();
  output_file.Write();

  exit();

}
