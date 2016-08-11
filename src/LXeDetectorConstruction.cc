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
/// \file optical/LXe/src/LXeDetectorConstruction.cc
/// \brief Implementation of the LXeDetectorConstruction class
//
//
#include "LXeDetectorConstruction.hh"
#include "LXePMTSD.hh"
#include "LXeScintSD.hh"
#include "LXeDetectorMessenger.hh"
#include "LXeMainVolume.hh"
#include "LXeWLSSlab.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4NistManager.hh"

#include "iomanip"

G4bool LXeDetectorConstruction::fSphereOn = false;
G4bool LXeDetectorConstruction::fSaveHitOn = false;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorConstruction::LXeDetectorConstruction()
  : fLXe_mt(NULL), fMPTPStyrene(NULL)
{
  fExperimentalHall_box = NULL;
  fExperimentalHall_log = NULL;
  fExperimentalHall_phys = NULL;

  fLXe = fAl = fAir = fVacuum = fGlass = NULL;
  fPstyrene = fPMMA = fPethylene1 = fPethylene2 = NULL;

  fN = fO = fC = fH = NULL;

  fUpdated = false;

  SetDefaults();

  fDetectorMessenger = new LXeDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorConstruction::~LXeDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::DefineMaterials(){
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;

  G4int polyPMMA = 1;
  G4int nC_PMMA = 3+2*polyPMMA;
  G4int nH_PMMA = 6+2*polyPMMA;

  G4int polyeth = 1;
  G4int nC_eth = 2*polyeth;
  G4int nH_eth = 4*polyeth;

  //***Elements
  fH = new G4Element("H", "H", z=1., a=1.01*g/mole);
  fC = new G4Element("C", "C", z=6., a=12.01*g/mole);
  fN = new G4Element("N", "N", z=7., a= 14.01*g/mole);
  fO = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);

  //***Materials
  //Liquid Xenon
  fLXe = new G4Material("LXe",z=54.,a=131.29*g/mole,density=3.020*g/cm3);
  //Aluminum
  fAl = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);
  //Vacuum
  fVacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
                          density=universe_mean_density,kStateGas,0.1*kelvin,
                          1.e-19*pascal);
  //Air
  fAir = new G4Material("Air", density= 1.29*mg/cm3, 2);
  fAir->AddElement(fN, 70*perCent);
  fAir->AddElement(fO, 30*perCent);
  //Glass
  fGlass = new G4Material("Glass", density=1.032*g/cm3,2);
  fGlass->AddElement(fC,91.533*perCent);
  fGlass->AddElement(fH,8.467*perCent);
  //Polystyrene
  fPstyrene = new G4Material("Polystyrene", density= 1.03*g/cm3, 2);
  fPstyrene->AddElement(fC, 8);
  fPstyrene->AddElement(fH, 8);
  //Fiber(PMMA)
  fPMMA = new G4Material("PMMA", density=1190*kg/m3,3);
  fPMMA->AddElement(fH,nH_PMMA);
  fPMMA->AddElement(fC,nC_PMMA);
  fPMMA->AddElement(fO,2);
  //Cladding(polyethylene)
  fPethylene1 = new G4Material("Pethylene1", density=1200*kg/m3,2);
  fPethylene1->AddElement(fH,nH_eth);
  fPethylene1->AddElement(fC,nC_eth);
  //Double cladding(flourinated polyethylene)
  fPethylene2 = new G4Material("Pethylene2", density=1400*kg/m3,2);
  fPethylene2->AddElement(fH,nH_eth);
  fPethylene2->AddElement(fC,nC_eth);
  //Water
  G4NistManager* NISTman = G4NistManager::Instance();
  fWater  = NISTman->FindOrBuildMaterial("G4_WATER");
  fWater->SetName("Water");
  //G4cout << *(G4Material::GetMaterialTable());

  //***Material properties tables

  const G4int lxenum = 3;
  G4double lxe_Energy[lxenum]    = { 7.0*eV , 7.07*eV, 7.14*eV };
  G4double lxe_SCINT[lxenum] = { 0.1, 1.0, 0.1 };
  G4double lxe_RIND[lxenum]  = { 1.59 , 1.57, 1.54 };
  G4double lxe_ABSL[lxenum]  = { 35.*cm, 35.*cm, 35.*cm};
  fLXe_mt = new G4MaterialPropertiesTable();
  fLXe_mt->AddProperty("FASTCOMPONENT", lxe_Energy, lxe_SCINT, lxenum);
  fLXe_mt->AddProperty("SLOWCOMPONENT", lxe_Energy, lxe_SCINT, lxenum);
  fLXe_mt->AddProperty("RINDEX",        lxe_Energy, lxe_RIND,  lxenum);
  fLXe_mt->AddProperty("ABSLENGTH",     lxe_Energy, lxe_ABSL,  lxenum);
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD",12000./MeV);
  fLXe_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  fLXe_mt->AddConstProperty("FASTTIMECONSTANT",20.*ns);
  fLXe_mt->AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  fLXe_mt->AddConstProperty("YIELDRATIO",1.0);
  fLXe->SetMaterialPropertiesTable(fLXe_mt);
  fLXe->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  G4cout << "LXe Material Properties Table" << G4endl;
  fLXe_mt->DumpTable();
  G4cout << G4endl;
 
  fWater_mt = new G4MaterialPropertiesTable();
  const G4int lwaternum = 32;
  G4double lwater_Energy[lwaternum] =
    { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
      2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
      2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
      2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
      2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
      3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
      3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
      3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
  
  G4double lwater_RIND[lwaternum] =
    { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
      1.346,  1.3465, 1.347,  1.3475, 1.348,
      1.3485, 1.3492, 1.35,   1.3505, 1.351,
      1.3518, 1.3522, 1.3530, 1.3535, 1.354,
      1.3545, 1.355,  1.3555, 1.356,  1.3568,
      1.3572, 1.358,  1.3585, 1.359,  1.3595,
      1.36,   1.3608};
  
  G4double lwater_ABSL[lwaternum] =
    {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
     15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
     45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
     52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
     30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
     17.500*m, 14.500*m };

  fWater_mt->AddProperty("ABSLENGTH",     lwater_Energy, lwater_ABSL, lwaternum);
  fWater_mt->AddProperty("RINDEX",        lwater_Energy, lwater_RIND, lwaternum);
  fWater->SetMaterialPropertiesTable(fWater_mt);
  G4cout << "Water Material Properties Table" << G4endl;
  fWater_mt->DumpTable();
  G4cout << G4endl;

  G4double glass_RIND[lxenum]={1.49,1.49,1.49};
  G4double glass_AbsLength[lxenum]={420.*cm,420.*cm,420.*cm};
  G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH",lxe_Energy,glass_AbsLength,lxenum);
  glass_mt->AddProperty("RINDEX",lxe_Energy,glass_RIND,lxenum);
  fGlass->SetMaterialPropertiesTable(glass_mt);

  G4double vacuum_Energy[lxenum]={2.0*eV,7.0*eV,7.14*eV};
  G4double vacuum_RIND[lxenum]={1.,1.,1.};
  G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,lxenum);
  fVacuum->SetMaterialPropertiesTable(vacuum_mt);
  fAir->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex

  const G4int wlsnum = 4;
  G4double wls_Energy[] = {2.00*eV,2.87*eV,2.90*eV,3.47*eV};
 
  G4double rIndexPstyrene[wlsnum]={ 1.5, 1.5, 1.5, 1.5};
  G4double absorption1[wlsnum]={2.*cm, 2.*cm, 2.*cm, 2.*cm};
  G4double scintilFast[wlsnum]={0.00, 0.00, 1.00, 1.00};
  fMPTPStyrene = new G4MaterialPropertiesTable();
  fMPTPStyrene->AddProperty("RINDEX",wls_Energy,rIndexPstyrene,wlsnum);
  fMPTPStyrene->AddProperty("ABSLENGTH",wls_Energy,absorption1,wlsnum);
  fMPTPStyrene->AddProperty("FASTCOMPONENT",wls_Energy, scintilFast,wlsnum);
  fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);
  fMPTPStyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
  fMPTPStyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
  fPstyrene->SetMaterialPropertiesTable(fMPTPStyrene);

  // Set the Birks Constant for the Polystyrene scintillator

  fPstyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  G4double RefractiveIndexFiber[wlsnum]={ 1.60, 1.60, 1.60, 1.60};
  G4double AbsFiber[wlsnum]={9.00*m,9.00*m,0.1*mm,0.1*mm};
  G4double EmissionFib[wlsnum]={1.0, 1.0, 0.0, 0.0};
  G4MaterialPropertiesTable* fiberProperty = new G4MaterialPropertiesTable();
  fiberProperty->AddProperty("RINDEX",wls_Energy,RefractiveIndexFiber,wlsnum);
  fiberProperty->AddProperty("WLSABSLENGTH",wls_Energy,AbsFiber,wlsnum);
  fiberProperty->AddProperty("WLSCOMPONENT",wls_Energy,EmissionFib,wlsnum);
  fiberProperty->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);
  fPMMA->SetMaterialPropertiesTable(fiberProperty);

  G4double RefractiveIndexClad1[wlsnum]={ 1.49, 1.49, 1.49, 1.49};
  G4MaterialPropertiesTable* clad1Property = new G4MaterialPropertiesTable();
  clad1Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad1,wlsnum);
  clad1Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
  fPethylene1->SetMaterialPropertiesTable(clad1Property);

  G4double RefractiveIndexClad2[wlsnum]={ 1.42, 1.42, 1.42, 1.42};
  G4MaterialPropertiesTable* clad2Property = new G4MaterialPropertiesTable();
  clad2Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad2,wlsnum);
  clad2Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
  fPethylene2->SetMaterialPropertiesTable(clad2Property);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LXeDetectorConstruction::Construct(){
  DefineMaterials();
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LXeDetectorConstruction::ConstructDetector()
{
  //The experimental hall walls are all 1m away from housing walls
  G4double expHall_x = fScint_r + fD_mtl + fVeto_dr + fD_mtl + 150.*m;
  G4double expHall_y = fScint_r + fD_mtl + fVeto_dr + fD_mtl + 150.*m;
  G4double expHall_z = fDepth + 2. * (fScint_h + fD_mtl + fVeto_dh + fD_mtl) + 1.*m;
  //expHall_z = full detector height + distance between detector top and water surface + 1 m extra 

  //Create experimental hall
  fExperimentalHall_box  = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  fExperimentalHall_log  = new G4LogicalVolume(fExperimentalHall_box, fVacuum, "expHall_log", 0, 0, 0);
  fExperimentalHall_phys = new G4PVPlacement(0, G4ThreeVector(), fExperimentalHall_log, "expHall", 0, false, 0);

  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);

  G4double main_volume_z = -(fScint_h + fD_mtl + fVeto_dh + fD_mtl + fDepth);

  G4cout << " experimental hall: (" << expHall_x << ", " << expHall_y << ", " << expHall_z << ") main volume z: " << main_volume_z << G4endl;

  //Place the main volume
  if(fMainVolume){
    new LXeMainVolume(0,G4ThreeVector(0, 0, main_volume_z),fExperimentalHall_log,false,0,this);
  }

  //Place the WLS slab
  if(fWLSslab){
    G4double wls_slab_z = -fScint_z/2.-fSlab_z-1.*cm;
    
    G4cout << " wls_slab_z " << wls_slab_z << G4endl;

    G4VPhysicalVolume* slab = new LXeWLSSlab(0,G4ThreeVector(0.,0.,wls_slab_z),
                                             fExperimentalHall_log,false,0,
                                             this);

    //Surface properties for the WLS slab
    G4OpticalSurface* scintWrap = new G4OpticalSurface("ScintWrap");
 
    new G4LogicalBorderSurface("ScintWrap", slab,
                               fExperimentalHall_phys,
                               scintWrap);
 
    scintWrap->SetType(dielectric_metal);
    scintWrap->SetFinish(polished);
    scintWrap->SetModel(glisur);

    const G4int num = 2;

    G4double pp[num] = {2.0*eV, 3.5*eV};
    G4double reflectivity[num] = {1., 1.};
    G4double efficiency[num] = {0.0, 0.0};
    
    G4MaterialPropertiesTable* scintWrapProperty 
      = new G4MaterialPropertiesTable();

    scintWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
    scintWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
    scintWrap->SetMaterialPropertiesTable(scintWrapProperty);
  }

  return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetDimensions(G4ThreeVector dims){
  this->fScint_x=dims[0];
  this->fScint_y=dims[1];
  this->fScint_z=dims[2];
  fUpdated=true;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetHousingThickness(G4double d_mtl){
  this->fD_mtl=d_mtl;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNX(G4int nx){
  this->fNx=nx;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNY(G4int ny){
  this->fNy=ny;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNZ(G4int nz){
  this->fNz=nz;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetPMTRadius(G4double outerRadius_pmt){
  this->fOuterRadius_pmt=outerRadius_pmt;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetPmtDR(G4double pmtdr){
  this->fpmtdr = pmtdr;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetPmtDZ(G4double pmtdz){
  this->fpmtdz = pmtdz;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetPmtDR_Veto(G4double pmtdr_veto){
  this->fpmtdr_veto = pmtdr_veto;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetPmtDZ_Veto(G4double pmtdz_veto){
  this->fpmtdz_veto = pmtdz_veto;
  fUpdated=true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetScintR(G4double scintr){
  this->fScint_r = scintr;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetDepth(G4double scintr){
  this->fDepth = scintr;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetScintH(G4double scinth){
  this->fScint_h = scinth;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetVetoDR(G4double vetodr){
  this->fVeto_dr = vetodr;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetVetoDH(G4double vetodh){
  this->fVeto_dh = vetodh;
  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::PrintSettings(){
  G4cout << G4endl;
  G4cout << std::setw(20) << "Detector Settings " << std::setw(20) << "m" << std::setw(60) << "comments"<< G4endl;
  G4cout << std::setw(20) << "fDepth = " << std::setw(20) << fDepth/m << std::setw(60) << "depth the detector"<< G4endl;
  G4cout << std::setw(20) << "fScint_r = " << std::setw(20) << fScint_r/m << std::setw(60) << "radius of the inner detector"<< G4endl;
  G4cout << std::setw(20) << "fScint_h = " << std::setw(20) << fScint_h/m << std::setw(60) << "half of the inner detector height"<< G4endl;
  G4cout << std::setw(20) << "  fD_mtl = " << std::setw(20) << fD_mtl/m   << std::setw(60) << "thickness of detector housing"<< G4endl;
  G4cout << std::setw(20) << "fVeto_dr = " << std::setw(20) << fVeto_dr/m << std::setw(60) << "thickness of the vertical veto wall"<< G4endl;
  G4cout << std::setw(20) << "fVeto_dh = " << std::setw(20) << fVeto_dh/m << std::setw(60) << "thickness of the top veto"<< G4endl;
  G4cout << std::setw(20) << "   fScintRefl = " << std::setw(20) << fScintRefl    << std::setw(60) << "reflectivity of the inner detector"<< G4endl;
  G4cout << std::setw(20) << "   fHousingLobeReflectivity = " << std::setw(20) << fHousingLobeReflectivity    << std::setw(60) << " housing lobe reflectivity "<< G4endl;
  G4cout << std::setw(20) << "   fPMTBaseReflectivity = " << std::setw(20) << fPMTBaseReflectivity    << std::setw(60) << " pmts base reflectivity "<< G4endl;
  G4cout << std::setw(20) << "   fSidePMTsRotationAngleX = " << std::setw(20) << fSidePMTsRotationAngleX          << std::setw(60) << " side PMTs angle of rotation (X)"<< G4endl;
  G4cout << std::setw(20) << "   fSidePMTsRotationAngleY = " << std::setw(20) << fSidePMTsRotationAngleY          << std::setw(60) << " side PMTs angle of rotation (Y)"<< G4endl;
  G4cout << std::setw(20) << "   fSidePMTsAimedAtAxis = " << std::setw(20) << fSidePMTsAimedAtAxis          << std::setw(60) << " side PMTs aimed at axis"<< G4endl;
  G4cout << std::setw(20) << "   fTopPMTsRotationAngleY = " << std::setw(20) << fTopPMTsRotationAngleY          << std::setw(60) << " top PMTs angle of rotation (Y)"<< G4endl;
  G4cout << std::setw(20) << "   fBottomPMTsRotationAngleY = " << std::setw(20) << fBottomPMTsRotationAngleY          << std::setw(60) << " top PMTs angle of rotation (Y)"<< G4endl;
  G4cout << std::setw(20) << "   fPMTsOnTop = " << std::setw(20) << fPMTsOnTop    << std::setw(60) << " put pmts on top "<< G4endl;
  G4cout << std::setw(20) << "   fPMTsOnBottom = " << std::setw(20) << fPMTsOnBottom    << std::setw(60) << " put pmts on bottom "<< G4endl;
  G4cout << std::setw(20) << "   fPMTsOnPositiveX = " << std::setw(20) << fPMTsOnPositiveX    << std::setw(60) << " put pmts on positive x "<< G4endl;
  G4cout << std::setw(20) << "   fPMTsOnNegativeX = " << std::setw(20) << fPMTsOnNegativeX    << std::setw(60) << " put pmts on negative x "<< G4endl;
  G4cout << std::setw(20) << "   fPutMirror = " << std::setw(20) << fPutMirror    << std::setw(60) << " put mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorX0 = " << std::setw(20) << fMirrorX0    << std::setw(60) << " X0 of mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorY0 = " << std::setw(20) << fMirrorY0    << std::setw(60) << " Y0 of mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorZ0 = " << std::setw(20) << fMirrorZ0    << std::setw(60) << " Z0 of mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorLA = " << std::setw(20) << fMirrorLA    << std::setw(60) << " LA of mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorLB = " << std::setw(20) << fMirrorLB    << std::setw(60) << " LB of mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorLC = " << std::setw(20) << fMirrorLC    << std::setw(60) << " LC of mirror "<< G4endl;
  G4cout << std::setw(20) << "   fMirrorAngle = " << std::setw(20) << fMirrorAngle    << std::setw(60) << " angle of mirror "<< G4endl;

  G4cout << std::setw(20) << "   fPutMirror2 = " << std::setw(20) << fPutMirror2    << std::setw(60) << " put mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2X0 = " << std::setw(20) << fMirror2X0    << std::setw(60) << " X0 of mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2Y0 = " << std::setw(20) << fMirror2Y0    << std::setw(60) << " Y0 of mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2Z0 = " << std::setw(20) << fMirror2Z0    << std::setw(60) << " Z0 of mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2LA = " << std::setw(20) << fMirror2LA    << std::setw(60) << " LA of mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2LB = " << std::setw(20) << fMirror2LB    << std::setw(60) << " LB of mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2LC = " << std::setw(20) << fMirror2LC    << std::setw(60) << " LC of mirror2 "<< G4endl;
  G4cout << std::setw(20) << "   fMirror2Angle = " << std::setw(20) << fMirror2Angle    << std::setw(60) << " angle of mirror2 "<< G4endl;

  G4cout << std::setw(20) << "   fAdHocPMTs = " << std::setw(20) << fAdHocPMTs    << std::setw(60) << " ad hoc pmts "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT1x = " << std::setw(20) << fAdHocPMT1x          << std::setw(60) << " x of adhoc PMT 1 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT1y = " << std::setw(20) << fAdHocPMT1y          << std::setw(60) << " y of adhoc PMT 1 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT1z = " << std::setw(20) << fAdHocPMT1z          << std::setw(60) << " z of adhoc PMT 1 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT1angleX = " << std::setw(20) << fAdHocPMT1angleX          << std::setw(60) << " angleX of adhoc PMT 1 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT1angleY = " << std::setw(20) << fAdHocPMT1angleY          << std::setw(60) << " angleY of adhoc PMT 1 "<< G4endl;

  G4cout << std::setw(20) << "   fAdHocPMT2x = " << std::setw(20) << fAdHocPMT2x          << std::setw(60) << " x of adhoc PMT 2 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT2y = " << std::setw(20) << fAdHocPMT2y          << std::setw(60) << " y of adhoc PMT 2 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT2z = " << std::setw(20) << fAdHocPMT2z          << std::setw(60) << " z of adhoc PMT 2 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT2angleX = " << std::setw(20) << fAdHocPMT2angleX          << std::setw(60) << " angleX of adhoc PMT 2 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT2angleY = " << std::setw(20) << fAdHocPMT2angleY          << std::setw(60) << " angleY of adhoc PMT 2 "<< G4endl;

  G4cout << std::setw(20) << "   fAdHocPMT3x = " << std::setw(20) << fAdHocPMT3x          << std::setw(60) << " x of adhoc PMT 3 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT3y = " << std::setw(20) << fAdHocPMT3y          << std::setw(60) << " y of adhoc PMT 3 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT3z = " << std::setw(20) << fAdHocPMT3z          << std::setw(60) << " z of adhoc PMT 3 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT3angleX = " << std::setw(20) << fAdHocPMT3angleX          << std::setw(60) << " angleX of adhoc PMT 3 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT3angleY = " << std::setw(20) << fAdHocPMT3angleY          << std::setw(60) << " angleY of adhoc PMT 3 "<< G4endl;

  G4cout << std::setw(20) << "   fAdHocPMT4x = " << std::setw(20) << fAdHocPMT4x          << std::setw(60) << " x of adhoc PMT 4 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT4y = " << std::setw(20) << fAdHocPMT4y          << std::setw(60) << " y of adhoc PMT 4 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT4z = " << std::setw(20) << fAdHocPMT4z          << std::setw(60) << " z of adhoc PMT 4 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT4angleX = " << std::setw(20) << fAdHocPMT4angleX          << std::setw(60) << " angleX of adhoc PMT 4 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT4angleY = " << std::setw(20) << fAdHocPMT4angleY          << std::setw(60) << " angleY of adhoc PMT 4 "<< G4endl;

  G4cout << std::setw(20) << "   fAdHocPMT5x = " << std::setw(20) << fAdHocPMT5x          << std::setw(60) << " x of adhoc PMT 5 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT5y = " << std::setw(20) << fAdHocPMT5y          << std::setw(60) << " y of adhoc PMT 5 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT5z = " << std::setw(20) << fAdHocPMT5z          << std::setw(60) << " z of adhoc PMT 5 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT5angleX = " << std::setw(20) << fAdHocPMT5angleX          << std::setw(60) << " angleX of adhoc PMT 5 "<< G4endl;
  G4cout << std::setw(20) << "   fAdHocPMT5angleY = " << std::setw(20) << fAdHocPMT5angleY          << std::setw(60) << " angleY of adhoc PMT 5 "<< G4endl;


  G4cout << std::setw(20) << "   fVetoRefl = "  << std::setw(20) << fVetoRefl     << std::setw(60) << "reflectivity of the veto walls"<< G4endl;

  G4cout << G4endl;
  G4cout << std::setw(20) << "PMT Settings " << std::setw(20) << "m" << std::setw(60) << "comments"<< G4endl; 
  G4cout << std::setw(20) << "fOuterRadius_pmt = " << std::setw(20) << fOuterRadius_pmt/m << std::setw(60) << "PMT radius"<< G4endl;
  G4cout << std::setw(20) << "          fpmtdz = " << std::setw(20) << fpmtdz/m           << std::setw(60) << "vertical spacing of the inner detector PMT"<< G4endl;
  G4cout << std::setw(20) << "          fpmtdr = " << std::setw(20) << fpmtdr/m           << std::setw(60) << "horizontal spacing of the inner detector PMT"<< G4endl;
  G4cout << std::setw(20) << "     fpmtdz_veto = " << std::setw(20) << fpmtdz_veto/m      << std::setw(60) << "vertical spacing of the veto PMT"<< G4endl;
  G4cout << std::setw(20) << "     fpmtdr_veto = " << std::setw(20) << fpmtdr_veto/m      << std::setw(60) << "horizontal spacing of the veto PMT"<< G4endl;
  G4cout << std::setw(20) << "          fpmtQE = " << std::setw(20) << fpmtQE              << std::setw(60) << "pmt QE"<< G4endl;
  G4cout << std::setw(20) << "      fSaveHitOn = " << std::setw(20) << fSaveHitOn          << std::setw(60) << "save/not save hit info"<< G4endl;

  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetDefaults(){
  //Resets to default values
  fDepth = 18.*m;

  fScint_x = 17.8*cm;
  fScint_y = 17.8*cm;
  fScint_z = 22.6*cm;
  fNx = 2;
  fNy = 2;
  fNz = 10;
  fNr = 80;
  fNr_veto = 22;

  fD_mtl=0.0635*cm;
  fScint_r = 1200.*cm;
  fScint_h = 1000.*cm;
  fVeto_dr = 100.*cm;           //vertical veto thickness
  fVeto_dh = 100.*cm;           //top veto thickness

  fOuterRadius_pmt = 12.7*cm;    //10 inch PMTs
  fpmtdr = 86.*cm;
  fpmtdz = 86.*cm;
  fpmtdr_veto = 344.*cm;
  fpmtdz_veto = 344.*cm;

  fScintRefl=0.2;
  fHousingLobeReflectivity=1;
  fPMTBaseReflectivity=1;
  fSidePMTsRotationAngleX=0.;
  fSidePMTsRotationAngleY=0.;
  fSidePMTsAimedAtAxis=false;
  fTopPMTsRotationAngleY=0.;
  fBottomPMTsRotationAngleY=0.;
  fPMTsOnTop=false;
  fPMTsOnBottom=false;
  fPMTsOnPositiveX=false;
  fPMTsOnNegativeX=false;
  fAdHocPMTs=false;
  fPutMirror=false;
  fMirrorX0=0.;
  fMirrorY0=0.;
  fMirrorZ0=0.;
  fMirrorLA=0.;
  fMirrorLB=0.;
  fMirrorLC=0.;
  fMirrorAngle=0.;
  fPutMirror2=false;
  fMirror2X0=0.;
  fMirror2Y0=0.;
  fMirror2Z0=0.;
  fMirror2LA=0.;
  fMirror2LB=0.;
  fMirror2LC=0.;
  fMirror2Angle=0.;

  fAdHocPMT1x=0.;
  fAdHocPMT1y=0.;
  fAdHocPMT1z=0.;
  fAdHocPMT1angleX=0.;
  fAdHocPMT1angleY=0.;

  fAdHocPMT2x=0.;
  fAdHocPMT2y=0.;
  fAdHocPMT2z=0.;
  fAdHocPMT2angleX=0.;
  fAdHocPMT2angleY=0.;

  fAdHocPMT3x=0.;
  fAdHocPMT3y=0.;
  fAdHocPMT3z=0.;
  fAdHocPMT3angleX=0.;
  fAdHocPMT3angleY=0.;

  fAdHocPMT4x=0.;
  fAdHocPMT4y=0.;
  fAdHocPMT4z=0.;
  fAdHocPMT4angleX=0.;
  fAdHocPMT4angleY=0.;

  fAdHocPMT5x=0.;
  fAdHocPMT5y=0.;
  fAdHocPMT5z=0.;
  fAdHocPMT5angleX=0.;
  fAdHocPMT5angleY=0.;
  fVetoRefl=0.8;

  fpmtQE = 0.15;

  fSphereOn = false;
  fSaveHitOn = false;

  fNfibers=15;
  fWLSslab=false;
  fMainVolume=true;
  fSlab_z=2.5*mm;

  G4UImanager::GetUIpointer()
    ->ApplyCommand("/LXe/detector/scintYieldFactor 1.");

  if(fLXe_mt)fLXe_mt->AddConstProperty("SCINTILLATIONYIELD",12000./MeV);
  if(fMPTPStyrene)fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);

  fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::UpdateGeometry(){

  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();
  G4SurfaceProperty::CleanSurfacePropertyTable();

  //define new one
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  fUpdated=false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetMainScintYield(G4double y){
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD",y/MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void LXeDetectorConstruction::SetWLSScintYield(G4double y){
  fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",y/MeV);
}
