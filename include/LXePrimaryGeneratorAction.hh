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
/// \file optical/LXe/include/LXePrimaryGeneratorAction.hh
/// \brief Definition of the LXePrimaryGeneratorAction class
//
//
#ifndef LXePrimaryGeneratorAction_h
#define LXePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

//from CRY
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "Randomize.hh"
//#include "/scratch/01230/fnova/programs/CRY/cry_v1.7/src/CRYSetup.h"
#include <CRYSetup.h>
#include <CRYGenerator.h>
#include <CRYParticle.h>
#include <CRYUtils.h>
#include "vector"
#include "RNGWrapper.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleTable.hh"

class G4ParticleGun;
class G4Event;
class G4GeneralParticleSource;

class LXePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    LXePrimaryGeneratorAction(const char *inputfile);
    virtual ~LXePrimaryGeneratorAction();
 
  public:

    virtual void GeneratePrimaries(G4Event* anEvent);

  //from CRY
  public:

  void InputCRY();
  void UpdateCRY(std::string* MessInput);
  void CRYFromFile(G4String newValue);
  void useCRY(G4bool usecry);
  void use_particle_gun(G4bool use);
  std::string GetEnv( const std::string & var );

  private:

  std::vector<CRYParticle*> *vect; // vector of generated particles
  G4ParticleTable* particleTable;
  G4ParticleGun* particleGun;
  G4GeneralParticleSource* GPS;
  CRYGenerator* gen;
  G4int InputState;
  PrimaryGeneratorMessenger* gunMessenger;
  G4bool fuseCRY;
  G4bool f_use_particle_gun;
  // Added by PS  
  G4float elapsedTime;
    
};

#endif
