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
/// \file optical/LXe/include/LXeEventAction.hh
/// \brief Definition of the LXeEventAction class
//

#ifndef LXeEventAction_h
#define LXeEventAction_h 1

#include "LXeEventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "LXeTrajectory.hh"

class G4Event;
class LXeRecorderBase;

struct EvtStat {
  G4int PhotonCount_Scint;

  G4int PhotonCount_Ceren;
  G4int HitCount;
  G4int PMTsAboveThreshold;
  G4int AbsorptionCount;
  G4int BoundaryAbsorptionCount;
  G4int Unacounted;

  G4int VetoPhotonCount_Ceren;
  G4int VetoHitCount;
  G4int VetoPMTsAboveThreshold;
  G4int VetoAbsorptionCount;
  G4int VetoBoundaryAbsorptionCount;
  G4int VetoUnacounted;

  G4int primaryPDG;

  G4double inX;
  G4double inY;
  G4double inZ;

  G4double outX;
  G4double outY;
  G4double outZ;

  G4double VetoinX;
  G4double VetoinY;
  G4double VetoinZ;

  G4double VetooutX;
  G4double VetooutY;
  G4double VetooutZ;

  G4double PX;
  G4double PY;
  G4double PZ;

  G4double primaryX;
  G4double primaryY;
  G4double primaryZ;
};  


class LXeEventAction : public G4UserEventAction
{
  public:

    LXeEventAction(LXeRecorderBase*);
    virtual ~LXeEventAction();

  public:

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetSaveThreshold(G4int );

    void SetEventVerbose(G4int v){fVerbose=v;}

    void SetPMTThreshold(G4int t){fPMTThreshold=t;}

    void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
    void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}

    G4String volume_of_point(LXeTrajectory* trj, G4int i);
    bool hit_enter_veto_from_outside(LXeTrajectory* trj, G4int i);
    bool hit_leaves_through_veto(LXeTrajectory* trj, G4int i);
    bool hit_enter_ID(LXeTrajectory* trj, G4int i);
    bool hit_leaves_ID(LXeTrajectory* trj, G4int i);

  private:

    LXeRecorderBase* fRecorder;
    LXeEventMessenger* fEventMessenger;
    G4int              fSaveThreshold;
    G4int              fScintCollID;
    G4int              fPMTCollID;
    G4int              fVerbose;
    G4int              fPMTThreshold;

    EvtStat Stat; 

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;

};

#endif
