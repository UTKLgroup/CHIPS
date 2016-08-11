//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file optical/LXe/src/LXeMainVolume.cc
/// \brief Implementation of the LXeMainVolume class
//
//
#include "LXeMainVolume.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "LXePMTSD.hh"
#include "LXeScintSD.hh"
#include "G4SystemOfUnits.hh"

#include "iomanip"

LXeScintSD* LXeMainVolume::fScint_SD=NULL;
LXePMTSD* LXeMainVolume::fPmt_SD=NULL;

G4LogicalVolume* LXeMainVolume::fHousing_log = NULL;
//G4LogicalVolume* LXeMainVolume::fScint_log   = NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeMainVolume::LXeMainVolume(G4RotationMatrix *pRot,
                             const G4ThreeVector &tlate,
                             G4LogicalVolume *pMotherLogical,
                             G4bool pMany,
                             G4int pCopyNo,
                             LXeDetectorConstruction* c)
  //Pass info to the G4PVPlacement constructor
  :G4PVPlacement(pRot,tlate,
                 //Temp logical volume must be created here
                 new G4LogicalVolume(new G4Box("temp",1,1,1),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "housing",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
  CopyValues();

  if(!fHousing_log || fUpdated){
 
    // fScint_r  radius of the inner detector
    // fScint_h  half of the inner detector height

    G4double idin_r    = fScint_r + fD_mtl / 2.;
    G4double idout_r   = fScint_r + fD_mtl;
    G4double veto_r    = fScint_r + fD_mtl + fVeto_dr;
    G4double housing_r = fScint_r + fD_mtl + fVeto_dr + fD_mtl;

    G4double idin_h    = fScint_h + fD_mtl / 2.;
    G4double idout_h   = fScint_h + fD_mtl;
    G4double veto_h    = fScint_h + fD_mtl + fVeto_dh;
    G4double housing_h = fScint_h + fD_mtl + fVeto_dr + fD_mtl;

    double factor=1.;

     //*************************** housing and scintillator
    //G4Tubs( const G4String& pName, G4double pRMin, G4double pRMax, G4double pDz, G4double pSPhi,  G4double pDPhi );
    fScint_tub   = new G4Tubs("scint_tub",   0.*cm, fScint_r,  fScint_h,  0.*deg, 360.*deg);
    //fIDin_tub    = new G4Tubs("idin_tub",   0.*cm, idin_r,    idin_h,    0.*deg, 360.*deg); // federico
    //fIDin_tub    = new G4Tubs("idout_tub",   0.*cm, idin_r,    idin_h,    0.*deg, 360.*deg);  // junting
    //fIDout_tub   = new G4Tubs("idout_tub",   0.*cm, idout_r,   idout_h,   0.*deg, 360.*deg);
    fVeto_tub    = new G4Tubs("veto_tub",    0.*cm, factor*veto_r,    factor*veto_h,    0.*deg, 360.*deg);
    //fHousing_tub = new G4Tubs("housing_tub", 0.*cm, housing_r, housing_h, 0.*deg, 360.*deg);
    fHousing_tub = new G4Tubs("housing_tub", 0.*cm, factor*housing_r, factor*housing_h, 0.*deg, 360.*deg);

    fScint_log   = new G4LogicalVolume(fScint_tub,   G4Material::GetMaterial("Water"), "scint_log", 0, 0, 0);
    //fIDout_log   = new G4LogicalVolume(fIDout_tub,   G4Material::GetMaterial("Al"), "idout_log",    0, 0, 0);
    //fIDin_log    = new G4LogicalVolume(fIDin_tub,    G4Material::GetMaterial("Al"), "idin_log",     0, 0, 0);
    fVeto_log    = new G4LogicalVolume(fVeto_tub,    G4Material::GetMaterial("Water"), "veto_log",  0, 0, 0);
    fHousing_log = new G4LogicalVolume(fHousing_tub, G4Material::GetMaterial("Water"),  "housing_log", 0, 0, 0);

    new G4PVPlacement(0, G4ThreeVector(), fScint_log, "scintillator", fVeto_log,    false, 0);
    //new G4PVPlacement(0, G4ThreeVector(), fScint_log, "scintillator", fIDin_log,    false, 0);
    //new G4PVPlacement(0, G4ThreeVector(), fIDin_log,  "idin",         fIDout_log,   false, 0);
    //new G4PVPlacement(0, G4ThreeVector(), fIDout_log, "idout",        fVeto_log,    false, 0);
    new G4PVPlacement(0, G4ThreeVector(), fVeto_log,  "veto",         fHousing_log, false, 0);

    /*
    //surface muon rate studies
    fHousing_tub = new G4Tubs("housing_tub", 10.*m, 10.001*m, 10.*m, 0.*deg, 90.*deg);
    fHousing_tub = new G4Tubs("housing_tub", 0.*m, 10.*m, 0.0005*m, 0.*deg, 360.*deg);
    fHousing_log = new G4LogicalVolume(fHousing_tub, G4Material::GetMaterial("Al"),  "housing_log", 0, 0, 0); 
    */

    //*************** Miscellaneous sphere to demonstrate skin surfaces
    fSphere = new G4Sphere("sphere",0.*mm,2.*cm,0.*deg,360.*deg,0.*deg,
                          360.*deg);
    fSphere_log = new G4LogicalVolume(fSphere,G4Material::GetMaterial("Al"),
                                     "sphere_log");
    if(fSphereOn)
      new G4PVPlacement(0,G4ThreeVector(5.*cm,5.*cm,5.*cm),
                                      fSphere_log,"sphere",fScint_log,false,0);


    //****************** Build PMTs
    G4double innerRadius_pmt = 0.*cm;
    G4double height_pmt = fD_mtl;
    G4double startAngle_pmt = 0.*deg;
    G4double spanningAngle_pmt = 360.*deg;
 
    fPmt = new G4Tubs("pmt_tube",innerRadius_pmt,fOuterRadius_pmt,
                     height_pmt,startAngle_pmt,spanningAngle_pmt);

    //the "photocathode" is a metal slab at the back of the glass that
    //is only a very rough approximation of the real thing since it only
    //absorbs or detects the photons based on the efficiency set below
    fPhotocath = new G4Tubs("photocath_tube",innerRadius_pmt,fOuterRadius_pmt,
                           height_pmt/4.,startAngle_pmt,spanningAngle_pmt);

    //since the PMT is now placed in the water instead of the housing
    //we want to prevent the hits from the back by adding a shield "fPmtbase"
    //setting the height as "height_pmt*0.5" will cause an error from "/geometry/test/cylinder_test true"

    fPmtbase = new G4Tubs("pmtbase_tube",innerRadius_pmt,fOuterRadius_pmt, height_pmt*0.49999,startAngle_pmt,spanningAngle_pmt);

    fPmt_log = new G4LogicalVolume(fPmt,G4Material::GetMaterial("Glass"),
                                  "pmt_log");

    fPhotocath_log = new G4LogicalVolume(fPhotocath,
                                        G4Material::GetMaterial("Al"),
                                        "photocath_log");

    fPmtbase_log = new G4LogicalVolume(fPmtbase,
                                        G4Material::GetMaterial("Al"),
                                        "pmtbase_log");

    new G4PVPlacement(0,G4ThreeVector(0,0,height_pmt/4.),
                                       fPhotocath_log,"photocath",
                                       fPmt_log,false,0);

    new G4PVPlacement(0,G4ThreeVector(0,0,-height_pmt/2.),
                                       fPmtbase_log,"pmtbase",
                                       fPmt_log,false,0);

    //***********Arrange pmts around the outside of housing**********
    //---pmt sensitive detector
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    if(!fPmt_SD){
      fPmt_SD = new LXePMTSD("/LXeDet/pmtSD");
      SDman->AddNewDetector(fPmt_SD);
      //Created here so it exists as pmts are being placed
    }

    fNz = (G4int)((fScint_h * 2. - fOuterRadius_pmt * 2.) / fpmtdz) + 1;
    fNr = (G4int)((2. * pi * fScint_r) / fpmtdr + 0.5); // junting
    //fNr = (G4int)((2. * pi * fScint_r) / fpmtdr ); // federico

    fNz_veto = (G4int)((fScint_h * 2. - fOuterRadius_pmt * 2.) / fpmtdz_veto) + 1;
    fNr_veto = (G4int)((2. * pi * fScint_r) / fpmtdr_veto + 0.5);

    fNtop_veto = NumofPMTonDisk(fScint_r, fOuterRadius_pmt, fpmtdz_veto);
    fNtop = NumofPMTonDisk(fScint_r, fOuterRadius_pmt, fpmtdz);

    //print out settings
    fConstructor->PrintSettings();
    G4cout << std::setw(20) << "Number of PMTs"  << std::setw(20) << " "        <<std::setw(60)  << "Comments"<< G4endl;
    G4cout << std::setw(20) << "       fNr = "   << std::setw(20) << fNr        << std::setw(60) << "Number of inner detector PMTs per horizontal circle"<< G4endl;
    G4cout << std::setw(20) << "       fNz = "   << std::setw(20) << fNz        << std::setw(60) << "Number of inner detector PMT circles"<< G4endl;
    G4cout << std::setw(20) << "       Nside = "   << std::setw(20) << fNz*fNr        << std::setw(60) << "Number of inner detector PMTs on the side"<< G4endl;
    G4cout << std::setw(20) << "     fNtop = "   << std::setw(20) << fNtop      << std::setw(60) << "Number of top PMTs"<< G4endl;
    G4cout << std::setw(20) << "  fNr_veto = "   << std::setw(20) << fNr_veto   << std::setw(60) << "Number of veto PMTs per horizontal circle"<< G4endl;
    G4cout << std::setw(20) << "  fNz_veto = "   << std::setw(20) << fNz_veto   << std::setw(60) << "Number of veto PMT circles"<< G4endl;
    G4cout << std::setw(20) << "fNtop_veto = "   << std::setw(20) << fNtop_veto << std::setw(60) << "Number of top veto PMTs"<< G4endl;

    G4int n_ID_pmts=fNz * fNr + fNtop * 2;
    G4int n_veto_pmts_side=fNz_veto * fNr_veto ;
    G4int n_veto_pmts=n_veto_pmts_side + fNtop_veto;

    G4cout << G4endl;
    G4cout << std::setw(20) << "Total Number of Detector PMT = " << std::setw(20) << n_ID_pmts << G4endl;
    G4cout << std::setw(20) << "    Total Number of Veto PMT = " << std::setw(20) << n_veto_pmts << G4endl;

    G4double coverage=n_ID_pmts*pow(fOuterRadius_pmt,2)/(2.*pow(fScint_r,2) + 2.*fScint_r*fScint_h);

    G4cout << std::setw(20) << "    Photocathode coverage = " << std::setw(20) << coverage << G4endl;

    double Abase=pi*pow(fScint_r,2);
    double OutsideArea = 2.*Abase + 2.*fScint_h*2.*pi*fScint_r;
    double Volume = Abase*2.*fScint_h;
    double waterdensity=1.e3;  // kg / m^3
    double mass = Volume/m/m/m*waterdensity;
    G4cout << std::setw(20) << " base area "  << std::setw(20) << Abase/m/m  <<std::setw(60)  << " m^2 "<< G4endl;
    G4cout << std::setw(20) << " outside area "  << std::setw(20) << OutsideArea/m/m  <<std::setw(60)  << " m^2 "<< G4endl;
    G4cout << std::setw(20) << " volume "  << std::setw(20) << Volume/m/m/m  <<std::setw(60)  << " m^3 "<< G4endl;
    G4cout << std::setw(20) << " mass "  << std::setw(20) << mass  <<std::setw(60)  << " kg "<< G4endl;

    G4cout << G4endl;

    G4int number_of_pmts=0;


    if( fPutMirror ){
      std::clog << " put mirror ************************************************************************ " << std::endl;

      std::clog << " X " << fMirrorX0 << " " << fMirrorY0 << " " << fMirrorZ0 << " L " << fMirrorLA << " " << fMirrorLB << " " << fMirrorLC << " a " << fMirrorAngle << std::endl;
      G4RotationMatrix* rm;
      rm = new G4RotationMatrix();
      rm->rotateY(fMirrorAngle*deg);
      fMirror_box = new G4Box("mirror",fMirrorLA, fMirrorLB, fMirrorLC);
      fMirror_log = new G4LogicalVolume(fMirror_box,G4Material::GetMaterial("Al"),"mirror_log");
      new G4PVPlacement(rm,G4ThreeVector(fMirrorX0, fMirrorY0, fMirrorZ0), fMirror_log,"the_mirror", fVeto_log,false,0);

      const G4int num = 2;
      G4double ephoton[num] = { 2.034*eV, 4.136*eV };
      G4double MirrorReflectivity[num] = {0.9,0.9};
      G4double MirrorEfficiency[num] = {0.0, 0.0};
      G4MaterialPropertiesTable* MirrorPT = new G4MaterialPropertiesTable();
      MirrorPT->AddProperty("REFLECTIVITY", ephoton, MirrorReflectivity, num);
      MirrorPT->AddProperty("EFFICIENCY", ephoton, MirrorEfficiency, num);
      G4OpticalSurface* MirrorSurface = new G4OpticalSurface("MirrorSurface",unified,polished,dielectric_metal);
      MirrorSurface->SetMaterialPropertiesTable(MirrorPT);
      new G4LogicalSkinSurface("mirror_surf",   fMirror_log,   MirrorSurface);

      std::clog << " put mirror ************************************************************************ " << std::endl;
    }

    if( fPutMirror2 ){
      std::clog << " put mirror2 ************************************************************************ " << std::endl;

      std::clog << " X " << fMirror2X0 << " " << fMirror2Y0 << " " << fMirror2Z0 << " L " << fMirror2LA << " " << fMirror2LB << " " << fMirror2LC << " a " << fMirror2Angle << std::endl;
      G4RotationMatrix* rm;
      rm = new G4RotationMatrix();
      rm->rotateY(fMirror2Angle*deg);
      fMirror2_box = new G4Box("mirror2",fMirror2LA, fMirror2LB, fMirror2LC);
      fMirror2_log = new G4LogicalVolume(fMirror2_box,G4Material::GetMaterial("Al"),"mirror2_log");
      new G4PVPlacement(rm,G4ThreeVector(fMirror2X0, fMirror2Y0, fMirror2Z0), fMirror2_log,"the_mirror2", fVeto_log,false,0);

      const G4int num = 2;
      G4double ephoton[num] = { 2.034*eV, 4.136*eV };
      G4double Mirror2Reflectivity[num] = {0.9,0.9};
      G4double Mirror2Efficiency[num] = {0.0, 0.0};
      G4MaterialPropertiesTable* Mirror2PT = new G4MaterialPropertiesTable();
      Mirror2PT->AddProperty("REFLECTIVITY", ephoton, Mirror2Reflectivity, num);
      Mirror2PT->AddProperty("EFFICIENCY", ephoton, Mirror2Efficiency, num);
      G4OpticalSurface* Mirror2Surface = new G4OpticalSurface("Mirror2Surface",unified,polished,dielectric_metal);
      Mirror2Surface->SetMaterialPropertiesTable(Mirror2PT);
      new G4LogicalSkinSurface("mirror2_surf",   fMirror2_log,   Mirror2Surface);

      std::clog << " put mirror2 ************************************************************************ " << std::endl;
    }

    if( ! fAdHocPMTs ){
      if( fPMTsOnTop )
	number_of_pmts += fNtop_veto;
      if( fPMTsOnBottom )
	number_of_pmts += fNtop_veto;
      if( fPMTsOnPositiveX )
	number_of_pmts += n_veto_pmts_side/2;
      if( fPMTsOnNegativeX )
	number_of_pmts += n_veto_pmts_side/2;
    }else{
      number_of_pmts = 5;
    }
    //fPmt_SD->InitPMTs(n_ID_pmts + n_veto_pmts); //let pmtSD know # of pmts
    //fPmt_SD->InitPMTs(n_ID_pmts); //let pmtSD know # of pmts
    //fPmt_SD->InitPMTs(n_veto_pmts); //let pmtSD know # of pmts
    fPmt_SD->InitPMTs(number_of_pmts); //let pmtSD know # of pmts

    //-------
    G4int k=0;
    bool up;
    if( ! fAdHocPMTs ){
      //----------place veto pmts
      if( fPMTsOnPositiveX || fPMTsOnNegativeX ){
	//side
	PlaceVetoPMTs(fPmt_log,
		      fNr_veto, fpmtdz_veto,
		      //fScint_r, fScint_h, 
		      fScint_r, fScint_h - fD_mtl,
		      fD_mtl, 
		      fVeto_dr, fVeto_dh,
		      fOuterRadius_pmt,
		      k, fPmt_SD);
      }
      
      if( fPMTsOnTop ){
	//top
	PlacePMTonDisk(fPmt_log, fVeto_log,
		       //		   fScint_r, (fScint_h + fD_mtl + fD_mtl),
		       fScint_r, fScint_h,
		       fOuterRadius_pmt, fpmtdz_veto,
		       fTopPMTsRotationAngleY,
		       k, fPmt_SD,
		       "pmt");
      }
      
      if( fPMTsOnBottom ){
	PlacePMTonDisk(fPmt_log, fVeto_log,
		       //		   fScint_r, (fScint_h + fD_mtl + fD_mtl),
		       fScint_r, -fScint_h,
		       fOuterRadius_pmt, fpmtdz_veto,
		       fBottomPMTsRotationAngleY, 
		       k, fPmt_SD,
		       "pmt");
      }
    }else{
      PlaceAdHocPMT(fAdHocPMT1x, fAdHocPMT1y, fAdHocPMT1z, fAdHocPMT1angleX, fAdHocPMT1angleY, fPmt_log, fVeto_log, k, fPmt_SD,"pmt");
      PlaceAdHocPMT(fAdHocPMT2x, fAdHocPMT2y, fAdHocPMT2z, fAdHocPMT2angleX, fAdHocPMT2angleY, fPmt_log, fVeto_log, k, fPmt_SD,"pmt");
      PlaceAdHocPMT(fAdHocPMT3x, fAdHocPMT3y, fAdHocPMT3z, fAdHocPMT3angleX, fAdHocPMT3angleY, fPmt_log, fVeto_log, k, fPmt_SD,"pmt");
      PlaceAdHocPMT(fAdHocPMT4x, fAdHocPMT4y, fAdHocPMT4z, fAdHocPMT4angleX, fAdHocPMT4angleY, fPmt_log, fVeto_log, k, fPmt_SD,"pmt");
      PlaceAdHocPMT(fAdHocPMT5x, fAdHocPMT5y, fAdHocPMT5z, fAdHocPMT5angleX, fAdHocPMT5angleY, fPmt_log, fVeto_log, k, fPmt_SD,"pmt");
    }
    
    G4cout << "Total Number of Veto PMT = " << k << G4endl;

    G4cout << "Total Number PMT = " << k << G4endl;

    //**********Setup Sensitive Detectors***************
    if(!fScint_SD){//determine if it has already been created
      fScint_SD = new LXeScintSD("/LXeDet/scintSD");
      SDman->AddNewDetector(fScint_SD);
    }
    //fVeto_log->SetSensitiveDetector(fScint_SD);
    //fScint_log->SetSensitiveDetector(fScint_SD);
    //fHousing_log->SetSensitiveDetector(fScint_SD);
 
    //sensitive detector is not actually on the photocathode.
    //processHits gets done manually by the stepping action.
    //It is used to detect when photons hit and get absorbed&detected at the
    //boundary to the photocathode (which doesnt get done by attaching it to a
    //logical volume.
    //It does however need to be attached to something or else it doesnt get
    //reset at the begining of events
    fPhotocath_log->SetSensitiveDetector(fPmt_SD);

    VisAttributes();
    SurfaceProperties();
  }

  SetLogicalVolume(fHousing_log);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeMainVolume::CopyValues(){
  fUpdated=fConstructor->GetUpdated();

  fScint_x=fConstructor->GetScintX();
  fScint_y=fConstructor->GetScintY();
  fScint_z=fConstructor->GetScintZ();

  fNx=fConstructor->GetNX();
  fNy=fConstructor->GetNY();
  fNz=fConstructor->GetNZ();
  fNr=fConstructor->GetNR();
  fNr_veto=fConstructor->GetNR_Veto();
  fSphereOn=fConstructor->GetSphereOn();

  fDepth=fConstructor->GetDepth();
  fScint_r=fConstructor->GetScintR();
  fScint_h=fConstructor->GetScintH();
  fVeto_dr=fConstructor->GetVetoDR();
  fVeto_dh=fConstructor->GetVetoDH();
  fD_mtl=fConstructor->GetHousingThickness();
  fScintRefl=fConstructor->GetScintHousingReflectivity();
  fPMTBaseReflectivity=fConstructor->GetPMTBaseReflectivity();
  fSidePMTsRotationAngleX=fConstructor->GetSidePMTsRotationAngleX();
  fSidePMTsRotationAngleY=fConstructor->GetSidePMTsRotationAngleY();
  fSidePMTsAimedAtAxis=fConstructor->GetSidePMTsAimedAtAxis();
  fTopPMTsRotationAngleY=fConstructor->GetTopPMTsRotationAngleY();
  fBottomPMTsRotationAngleY=fConstructor->GetBottomPMTsRotationAngleY();
  fPMTsOnTop=fConstructor->GetPMTsOnTop();
  fPMTsOnBottom=fConstructor->GetPMTsOnBottom();
  fPMTsOnPositiveX=fConstructor->GetPMTsOnPositiveX();
  fPMTsOnNegativeX=fConstructor->GetPMTsOnNegativeX();
  fPutMirror=fConstructor->GetPutMirror();
  fMirrorX0=fConstructor->GetMirrorX0();
  fMirrorY0=fConstructor->GetMirrorY0();
  fMirrorZ0=fConstructor->GetMirrorZ0();
  fMirrorLA=fConstructor->GetMirrorLA();
  fMirrorLB=fConstructor->GetMirrorLB();
  fMirrorLC=fConstructor->GetMirrorLC();
  fMirrorAngle=fConstructor->GetMirrorAngle();
  fPutMirror2=fConstructor->GetPutMirror2();
  fMirror2X0=fConstructor->GetMirror2X0();
  fMirror2Y0=fConstructor->GetMirror2Y0();
  fMirror2Z0=fConstructor->GetMirror2Z0();
  fMirror2LA=fConstructor->GetMirror2LA();
  fMirror2LB=fConstructor->GetMirror2LB();
  fMirror2LC=fConstructor->GetMirror2LC();
  fMirror2Angle=fConstructor->GetMirror2Angle();
  fAdHocPMTs=fConstructor->GetAdHocPMTs();
  fAdHocPMT1x=fConstructor->GetAdHocPMT1x();
  fAdHocPMT1y=fConstructor->GetAdHocPMT1y();
  fAdHocPMT1z=fConstructor->GetAdHocPMT1z();
  fAdHocPMT1angleX=fConstructor->GetAdHocPMT1angleX();
  fAdHocPMT1angleY=fConstructor->GetAdHocPMT1angleY();

  fAdHocPMT2x=fConstructor->GetAdHocPMT2x();
  fAdHocPMT2y=fConstructor->GetAdHocPMT2y();
  fAdHocPMT2z=fConstructor->GetAdHocPMT2z();
  fAdHocPMT2angleX=fConstructor->GetAdHocPMT2angleX();
  fAdHocPMT2angleY=fConstructor->GetAdHocPMT2angleY();

  fAdHocPMT3x=fConstructor->GetAdHocPMT3x();
  fAdHocPMT3y=fConstructor->GetAdHocPMT3y();
  fAdHocPMT3z=fConstructor->GetAdHocPMT3z();
  fAdHocPMT3angleX=fConstructor->GetAdHocPMT3angleX();
  fAdHocPMT3angleY=fConstructor->GetAdHocPMT3angleY();

  fAdHocPMT4x=fConstructor->GetAdHocPMT4x();
  fAdHocPMT4y=fConstructor->GetAdHocPMT4y();
  fAdHocPMT4z=fConstructor->GetAdHocPMT4z();
  fAdHocPMT4angleX=fConstructor->GetAdHocPMT4angleX();
  fAdHocPMT4angleY=fConstructor->GetAdHocPMT4angleY();

  fAdHocPMT5x=fConstructor->GetAdHocPMT5x();
  fAdHocPMT5y=fConstructor->GetAdHocPMT5y();
  fAdHocPMT5z=fConstructor->GetAdHocPMT5z();
  fAdHocPMT5angleX=fConstructor->GetAdHocPMT5angleX();
  fAdHocPMT5angleY=fConstructor->GetAdHocPMT5angleY();

  fVetoRefl=fConstructor->GetVetoHousingReflectivity();
  fpmtQE=fConstructor->GetpmtQE();

  fOuterRadius_pmt=fConstructor->GetPMTRadius();
  fpmtdr = fConstructor-> GetPmtDR();
  fpmtdz = fConstructor-> GetPmtDZ();
  fpmtdr_veto = fConstructor->GetPmtDR_Veto();
  fpmtdz_veto = fConstructor->GetPmtDZ_Veto();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int LXeMainVolume::NumofPMTonDisk(G4double r, G4double pmtr, G4double d){
  /*Calculate the number of PMTs on a disk
    r = disk radius
    pmtr = PMT radius
    d = PMT spacing
   */

  ////////////////////////////////////////////////////////////////
  /////                        O
  /////                     O  O  O
  /////                  O  O  O  O  O
  /////               O  O  O  O  O  O  O
  /////            O  O  O  O  O  O  O  O  O
  /////               O  O  O  O  O  O  O
  /////                  O  O  O  O  O
  /////                     O  O  O
  /////                        O
  ////////////////////////////////////////////////////////////////


  G4double x = 0;
  G4double dx = d;
  // n of centers along the central vertical line
  // = 1 + 2 (number of centers on each side)
  G4int ncenter = 1 + 2 * (G4int)((r - pmtr) / dx);
  G4int n_centers = 0;
  while(x < (r - pmtr)){
    // x = value of one vertical line on the right
    // y max = max possible y of a center on this line
    G4double ymax = sqrt(pow(r - pmtr, 2) - pow(x, 2));

    // number of centers along this vertical line
    // = 1 + 2 (number of centers on each side)
    n_centers += 1 + 2 * (G4int)(ymax / dx);
    x += dx;
  }
  // - subtract ncenter to have only values on the right
  // - multiply by 2 to have values on right and left
  // - add back ncenter (values on central vertical line)
  n_centers = (n_centers - ncenter) * 2 + ncenter;
  return n_centers;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeMainVolume::PlacePMTonDisk(G4LogicalVolume* pmt_log, G4LogicalVolume* mothervol_log,
				   G4double r, G4double z,
				   G4double pmtr,G4double d,
				   G4double angleY,
				   //				   G4bool up,
				   G4int &k, LXePMTSD* sd,
				   G4String phyvolname){
  /*
    This is to place the pmts on a disk
    pmt_log = logical volume for pmts to be placed
    mothervol_log = mother volume where the pmts are to be placed
    r = detector radius
    z = pmt z position
    pmtr = pmt radius
    d = pmt spacing
    angleY of PMT rotation
    k = pmt copy number to start with
    sd = sensitive detector for pmts
  */

  G4cout << " placing pmts on disk: detector radius " << r << " pmt z position " << z << " pmt radius " << pmtr << " pmt spacing " << d << " angleY " << angleY << G4endl;

  G4double dx = d;
  G4double dy = d;

  bool print = false;

  G4RotationMatrix* rm;
  rm = new G4RotationMatrix();
  rm->rotateY(angleY*deg);


  if( print )
    G4cout << " ... set pmt " << k << " at pos: (0, 0, " << z << ")" << G4endl;
  new G4PVPlacement(rm, G4ThreeVector(0, 0, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,0,0,z); k++;
  while(dy < (r - pmtr)){
    if( print )
      G4cout << " ... set pmt " << k << " at pos: (0, " << dy << ", " << z << ")" << G4endl;
    new G4PVPlacement(rm, G4ThreeVector(0,  dy, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,0,dy,z); k++;
    if( print )
      G4cout << " ... set pmt " << k << " at pos: (0, " << -dy << ", " << z << ")" << G4endl;
    new G4PVPlacement(rm, G4ThreeVector(0, -dy, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,0,-dy,z); k++;
    dy += d;
  }

  while(dx < (r-pmtr)){
    if( print )
      G4cout << " ... set pmt " << k << " at pos: (" << dx << ", 0, " << z << ")" << G4endl;
    new G4PVPlacement(rm, G4ThreeVector( dx,  0, z), pmt_log, phyvolname, mothervol_log , false, k);  sd->SetPMTPos(k,dx,0,z); k++;
    if( print )
      G4cout << " ... set pmt " << k << " at pos: (" << -dx << ", 0, " << z << ")" << G4endl;
    new G4PVPlacement(rm, G4ThreeVector(-dx, 0, z),  pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,-dx,0,z); k++;
    G4double ymax = sqrt(pow(r - pmtr, 2) - pow(dx, 2));
    dy = d;
    while(dy < ymax){
      if( print )
	G4cout << " ... set pmt " << k << " at pos: (" << dx << ", " << dy << ", " << z << ")" << G4endl;
      new G4PVPlacement(rm, G4ThreeVector( dx,  dy, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,dx,dy,z); k++;
      if( print )
	G4cout << " ... set pmt " << k << " at pos: (" << dx << ", " << -dy << ", " << z << ")" << G4endl;
      new G4PVPlacement(rm, G4ThreeVector( dx, -dy, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,dx,-dy,z); k++;
      if( print )
	G4cout << " ... set pmt " << k << " at pos: (" << -dx << ", " << dy << ", " << z << ")" << G4endl;
      new G4PVPlacement(rm, G4ThreeVector(-dx,  dy, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,-dx,dy,z); k++;
      if( print )
	G4cout << " ... set pmt " << k << " at pos: (" << -dx << ", " << -dy << ", " << z << ")" << G4endl;
      new G4PVPlacement(rm, G4ThreeVector(-dx, -dy, z), pmt_log, phyvolname, mothervol_log , false, k); sd->SetPMTPos(k,-dx,-dy,z); k++;
      dy += d;
    }
    dx += d;
  }

}



void LXeMainVolume::PlaceAdHocPMT(G4double x, G4double y, G4double z, G4double angleX, G4double angleY, 
				  G4LogicalVolume* pmt_log, G4LogicalVolume* mothervol_log,
				  G4int &k, LXePMTSD* sd, G4String phyvolname){

  G4cout << " placing pmts: x " << x << " y " << y << " z " << z << " angleX " << angleX << " angleY " << angleY << G4endl;

  G4RotationMatrix* rm;
  rm = new G4RotationMatrix();
  rm->rotateX(angleX*deg);
  rm->rotateY(angleY*deg);

  new G4PVPlacement(rm, G4ThreeVector(x, y, z), pmt_log, phyvolname, mothervol_log , false, k); 
  sd->SetPMTPos(k,x,y,z); 
  k++;
  return;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeMainVolume::PlaceVetoPMTs(G4LogicalVolume* pmt_log,
				  G4int nr, G4double dz,
				  G4double r, G4double halfh, 
				  G4double mtl, 
				  G4double veto_dr, G4double veto_dh,
				  G4double pmtr,
				  G4int &k,      LXePMTSD* sd){
  
/*PlaceVetoPMTs : a different way to parameterize placement that does not depend on
  calculating the position from the copy number

  pmt_log = logical volume for pmts to be placed
  nr = number of PMTs on a cirlce
  dz = row spacing along z
  r = detector radius
  halfh = half of the detector height (fScint_z)
  mtl = wall thickness
  veto_dr = vertical veto thickness
  veto_dh = top veto thickness
  pmtr = pmt radius
  k = copy number to start with
  sd = sensitive detector for pmts
*/

  G4double x, y, z;

  //pmts on the side
  G4double dtheta = 360. / nr * deg;  
  G4double zmin = -halfh + pmtr;
  G4double zmax =  halfh - pmtr;
  z = zmin;
  r += 5.*cm + mtl;

  bool print = false;

  G4cout << " vetos on the side: placing " << nr << " veto pmts/circle;  row spacing along z " << dz << " detector radius (after correction) " << r << " half of the detector height " << halfh << " wall thickness " << mtl << " vertical veto thickness " << veto_dr << " top veto thickness " << veto_dh << " pmt radius " << pmtr << " dtheta " << dtheta << " zmin " << zmin << " zmax " << zmax << G4endl;


  while(z < zmax){
    for(G4int i = 0; i < nr; i++){
      G4double theta = i * dtheta;
      x = cos(theta) * r;

      if( (fPMTsOnPositiveX && x > 0. ) ||
	  (fPMTsOnNegativeX && x < 0. ) ){

	y = sin(theta) * r;    
	G4RotationMatrix* rm = new G4RotationMatrix();
	// initially PMTs are looking up
	if( fSidePMTsAimedAtAxis ){ // PMTs look horyzontally at axis
	  rm->rotateX(90*deg);
	  rm->rotateY(theta + 90*deg);
	}
	rm->rotateX(fSidePMTsRotationAngleX*deg);
	rm->rotateY(fSidePMTsRotationAngleY*deg);
	
	new G4PVPlacement(rm, G4ThreeVector(x, y, z), pmt_log, "pmt", fVeto_log , false, k);
	sd->SetPMTPos(k,x,y,z);
	if( print )
	  G4cout << " ... set pmt " << k << " at theta " << theta << " pos: (" << x << ", " << y << " , " << z << ")" << G4endl;
	k++;                      
      }
    }
    z += dz;
  }
  
  G4cout << " finally " << k << " veto pmts have been placed on the side " << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeMainVolume::PlacePMTs(G4LogicalVolume* pmt_log,
                              G4RotationMatrix *rot,
                              G4double &a, G4double &b, G4double da,
                              G4double db, G4double amin,
                              G4double bmin, G4int na, G4int nb,
                              G4double &x, G4double &y, G4double &z,
                              G4int &k,LXePMTSD* sd){
/*PlacePMTs : a different way to parameterize placement that does not depend on
  calculating the position from the copy number

  pmt_log = logical volume for pmts to be placed
  rot = rotation matrix to apply
  a,b = coordinates to vary(ie. if varying in the xy plane then pass x,y)
  da,db = value to increment a,b by
  amin,bmin = start values for a,b
  na,nb = number of repitions in a and b
  x,y,z = just pass x,y, and z by reference (the same ones passed for a,b)
  k = copy number to start with
  sd = sensitive detector for pmts
*/
  a=amin;
  for(G4int j=1;j<=na;j++){
    a+=da;
    b=bmin;
    for(G4int i=1;i<=nb;i++){
      b+=db;
      new G4PVPlacement(rot,G4ThreeVector(x,y,z),pmt_log,"pmt",
                        fHousing_log,false,k);
      sd->SetPMTPos(k,x,y,z);
      k++;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeMainVolume::VisAttributes(){
  G4VisAttributes* housing_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  fHousing_log->SetVisAttributes(housing_va);

  //G4VisAttributes* photocath_va = new G4VisAttributes(G4Colour(1.,0.,0.));
  //fPhotocath_log->SetVisAttributes(photocath_va);

  G4VisAttributes* sphere_va = new G4VisAttributes();
  sphere_va->SetForceSolid(true);
  fSphere_log->SetVisAttributes(sphere_va);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeMainVolume::SurfaceProperties(){
  const G4int num = 2;
  G4double ephoton[num] = { 2.034*eV, 4.136*eV };

  //**Scintillator housing properties
  G4double scintReflectivity[num] = {fScintRefl, fScintRefl};
  G4double scintEfficiency[num] = {0.0, 0.0};
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty("REFLECTIVITY", ephoton, scintReflectivity, num);
  scintHsngPT->AddProperty("EFFICIENCY", ephoton, scintEfficiency, num);
  G4OpticalSurface* OpScintHousingSurface =
    new G4OpticalSurface("scintHousingSurface",unified,polished,dielectric_metal);
  OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);

  //**Veto housing properties
  G4double vetoReflectivity[num] = {fVetoRefl, fVetoRefl};
  G4double vetoEfficiency[num] = {0.0, 0.0};
  G4MaterialPropertiesTable* vetoHsngPT = new G4MaterialPropertiesTable();
  vetoHsngPT->AddProperty("REFLECTIVITY", ephoton, vetoReflectivity, num);
  vetoHsngPT->AddProperty("EFFICIENCY", ephoton, vetoEfficiency, num);
  /*
  G4OpticalSurface* OpVetoHousingSurface = new G4OpticalSurface("vetoHousingSurface", unified, polished, dielectric_metal);
  OpVetoHousingSurface->SetMaterialPropertiesTable(vetoHsngPT);
  */
  /*
  G4OpticalSurface* OpVetoHousingSurface = new G4OpticalSurface("vetoHousingSurface");
  OpVetoHousingSurface->SetType(dielectric_LUT); 
  OpVetoHousingSurface->SetModel(LUT);
  OpVetoHousingSurface->SetFinish(groundtyvekair); 
  OpVetoHousingSurface->DumpInfo();
  G4cout << G4endl;
  */
  //G4double SpecularLobe[num]    = {0.85, 0.85};
  G4double SpecularLobe[num]    = {fHousingLobeReflectivity, fHousingLobeReflectivity};
  G4double SpecularSpike[num]   = {0., 0.};
  G4double Backscatter[num]     = {0., 0.};
  G4double RefractiveIndex[num] = {1., 1.};
  G4OpticalSurface* OpVetoHousingSurface = new G4OpticalSurface("vetoHousingSurface");
  OpVetoHousingSurface->SetType(dielectric_dielectric);
  OpVetoHousingSurface->SetFinish(groundbackpainted);
  OpVetoHousingSurface->SetModel(unified);
  OpVetoHousingSurface->SetSigmaAlpha(0.06);
  vetoHsngPT->AddProperty("SPECULARLOBECONSTANT",  ephoton, SpecularLobe,    num);
  vetoHsngPT->AddProperty("SPECULARSPIKECONSTANT", ephoton, SpecularSpike,   num);
  vetoHsngPT->AddProperty("BACKSCATTERCONSTANT",   ephoton, Backscatter,     num);
  vetoHsngPT->AddProperty("RINDEX",                ephoton, RefractiveIndex, num);
  OpVetoHousingSurface->SetMaterialPropertiesTable(vetoHsngPT);

  //**PMT base properties
  //G4double pmtbaseReflectivity[num] = {1., 1.};
  G4double pmtbaseReflectivity[num] = {fPMTBaseReflectivity, fPMTBaseReflectivity};
  G4double pmtbaseEfficiency[num] = {0., 0.};
  G4MaterialPropertiesTable* pmtbasePT = new G4MaterialPropertiesTable();
  pmtbasePT->AddProperty("REFLECTIVITY", ephoton, pmtbaseReflectivity, num);
  pmtbasePT->AddProperty("EFFICIENCY", ephoton, pmtbaseEfficiency, num);
  G4OpticalSurface* OpPmtbaseSurface =
    new G4OpticalSurface("PmtbaseSurface",unified,polished,dielectric_metal);
  OpPmtbaseSurface->SetMaterialPropertiesTable(pmtbasePT);
 
  //**Sphere surface properties
  G4double sphereReflectivity[num] = {1.0, 1.0};
  //G4double sphereReflectivity[num] = {0.0, 0.0};
  G4double sphereEfficiency[num] = {0.0, 0.0};
  G4MaterialPropertiesTable* spherePT = new G4MaterialPropertiesTable();
  spherePT->AddProperty("REFLECTIVITY", ephoton, sphereReflectivity, num);
  spherePT->AddProperty("EFFICIENCY", ephoton, sphereEfficiency, num);
  G4OpticalSurface* OpSphereSurface =
    new G4OpticalSurface("SphereSurface",unified,polished,dielectric_metal);
  OpSphereSurface->SetMaterialPropertiesTable(spherePT);
 
  //**Photocathode surface properties
  G4double photocath_EFF[num]={fpmtQE,fpmtQE}; //Enables 'detection' of photons
  G4double photocath_ReR[num]={1.92,1.92};
  G4double photocath_ImR[num]={1.69,1.69};
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,
                         dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

  //**Create logical skin surfaces
  //new G4LogicalSkinSurface("idin_surf",      fIDin_log,      OpScintHousingSurface);
  //new G4LogicalSkinSurface("idout_surf",     fIDout_log,     OpVetoHousingSurface);
  //new G4LogicalSkinSurface("scintillator_surf",   fScint_log,   OpVetoHousingSurface);
  //new G4LogicalSkinSurface("veto_surf",   fVeto_log,   OpVetoHousingSurface);
  new G4LogicalSkinSurface("housing_surf",   fHousing_log,   OpVetoHousingSurface);

  new G4LogicalSkinSurface("photocath_surf", fPhotocath_log, photocath_opsurf);
  new G4LogicalSkinSurface("pmtbase_surf",   fPmtbase_log,   OpPmtbaseSurface);
  new G4LogicalSkinSurface("sphere_surface", fSphere_log,    OpSphereSurface);

}
