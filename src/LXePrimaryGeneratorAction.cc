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
/// \file optical/LXe/src/LXePrimaryGeneratorAction.cc
/// \brief Implementation of the LXePrimaryGeneratorAction class
//
//
#include "LXePrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include <iostream>
//#include "iomanip"

#include "fstream"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePrimaryGeneratorAction::LXePrimaryGeneratorAction(const char *inputfile)
  :fuseCRY(true),f_use_particle_gun(false)
{

  // define a particle gun
  particleGun = new G4ParticleGun();

  // define a general particle source
  GPS = new G4GeneralParticleSource();

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    if( *inputfile !=0)  //....only complain if a filename was given
      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    std::string cry_directory=GetEnv("CRY_DIR");
    std::string cry_data=cry_directory+"/data";
    CRYSetup *setup=new CRYSetup(setupString,cry_data.c_str());

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }

  // create a vector to store the CRY particle properties
  vect=new std::vector<CRYParticle*>;

  // Create the messenger file
  gunMessenger = new PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePrimaryGeneratorAction::~LXePrimaryGeneratorAction(){
  delete particleGun;
  delete GPS;
}

std::string LXePrimaryGeneratorAction::GetEnv( const std::string & var ) {
  const char * val = ::getenv( var.c_str() );
  if ( val == 0 ) {
    return "";
  }
  else {
    return val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePrimaryGeneratorAction::useCRY(G4bool usecry){
  fuseCRY = usecry;
}

void LXePrimaryGeneratorAction::use_particle_gun(G4bool use){
  f_use_particle_gun = use;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePrimaryGeneratorAction::InputCRY()
{
  InputState=1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
  std::string cry_directory=GetEnv("CRY_DIR");
  std::string cry_data=cry_directory+"/data";
  CRYSetup *setup=new CRYSetup(*MessInput,cry_data.c_str());

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState=0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePrimaryGeneratorAction::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    std::string cry_directory=GetEnv("CRY_DIR");
    std::string cry_data=cry_directory+"/data";
    CRYSetup *setup=new CRYSetup(setupString,cry_data.c_str());

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

  std::cout << "fuseCRY = " << fuseCRY << std::endl;
  std::cout << "InputState = " << InputState << std::endl;  

  //if using CRY
  if(fuseCRY){
    if (InputState != 0) {
      G4String* str = new G4String("CRY library was not successfully initialized");
      //G4Exception(*str);
      G4Exception("PrimaryGeneratorAction", "1",
		  RunMustBeAborted, *str);
      exit(1);
    }
    G4String particleName;
    vect->clear();
    gen->genEvent(vect);
    // added to get normalisation by PS
    //elapsedTime = gen->timeSimulated();  
    //cout << "#############################################################" << end;  
    //cout << "#############################################################" << end;
    //cout << "Elapsed time for particle cosmic ray event generation : " << << endl;  
    //cout << "#############################################################" << end;
    //cout << "#############################################################" << end;
    //....debug output
    /*
    G4cout << "\nEvent=" << anEvent->GetEventID() << " "
	   << "CRY generated nparticles=" << vect->size()
	   << G4endl;
    */
        
    for ( unsigned j=0; j<vect->size(); j++) {
      particleName=CRYUtils::partName((*vect)[j]->id());
      
      //....debug output
      /*
      G4cout << "  "          << particleName << " "
	     << "charge="      << (*vect)[j]->charge() << " "
	     << std::setprecision(4)
	     << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
	     << "pos (m)"
	     << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
	     << " " << "direction cosines "
	     << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
	     << " " << G4endl;
      */
      
      /*
      std::ofstream fcosmic;
      fcosmic.open("fcosmic.txt", std::ios::app);
      fcosmic << particleName << " "
	      <<  (*vect)[j]->charge() << " "
	      << std::setprecision(4)
	      << (*vect)[j]->ke()*MeV << " "
	      << (*vect)[j]->x() << " "
	      << (*vect)[j]->y() << " "
	      << (*vect)[j]->z() << " "
	      << (*vect)[j]->u() << " "
	      << (*vect)[j]->v() << " "
	      << (*vect)[j]->w() << " "
	      << " " << G4endl;
      fcosmic.close();
      */
      
      particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
      particleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
      particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*m, (*vect)[j]->y()*m, (*vect)[j]->z()*m));
      particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
      particleGun->SetParticleTime((*vect)[j]->t());
      particleGun->GeneratePrimaryVertex(anEvent);
      delete (*vect)[j];
    }
  }
  // if not using CRY
  else{
    if( f_use_particle_gun )
      particleGun->GeneratePrimaryVertex(anEvent);
    else
      GPS->GeneratePrimaryVertex(anEvent);
  }
  
}

