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
/// \file optical/LXe/src/LXeEventAction.cc
/// \brief Implementation of the LXeEventAction class
//
//
#include "LXeEventAction.hh"
#include "LXeScintHit.hh"
#include "LXePMTHit.hh"
#include "LXeUserEventInformation.hh"
#include "LXeTrajectory.hh"
#include "LXeRecorderBase.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

#include "fstream"
#include "LXeRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeEventAction::LXeEventAction(LXeRecorderBase* r)
  : fRecorder(r),fSaveThreshold(0),fScintCollID(-1),fPMTCollID(-1),fVerbose(0),
   fPMTThreshold(1),fForcedrawphotons(false),fForcenophotons(false)
{
  fEventMessenger = new LXeEventMessenger(this);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeEventAction::~LXeEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::BeginOfEventAction(const G4Event* anEvent){
 
  //New event, add the user information object
  G4EventManager::
    GetEventManager()->SetUserInformation(new LXeUserEventInformation);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fScintCollID<0)
    fScintCollID=SDman->GetCollectionID("scintCollection");
  if(fPMTCollID<0)
    fPMTCollID=SDman->GetCollectionID("pmtHitCollection");

  if(fRecorder)fRecorder->RecordBeginOfEvent(anEvent);

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::EndOfEventAction(const G4Event* anEvent){

  LXeUserEventInformation* eventInformation
    =(LXeUserEventInformation*)anEvent->GetUserInformation();
 
  G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  int max_nevents=80000;
  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()){
    for (G4int i=0; i<n_trajectories; i++){
      //for (G4int i=0; i<std::min(max_nevents,n_trajectories); i++){ //limit the numer of photons shown
      LXeTrajectory* trj = (LXeTrajectory*)
        ((*(anEvent->GetTrajectoryContainer()))[i]);

      if(trj->GetParticleName()=="opticalphoton"){
        trj->SetForceDrawTrajectory(fForcedrawphotons);
        trj->SetForceNoDrawTrajectory(fForcenophotons);
	//true: draw all traj
        //trj->SetForceDrawTrajectory(true);
        //trj->SetForceNoDrawTrajectory(true);
      }
      trj->DrawTrajectory(50);
    }
  }
 
  LXeScintHitsCollection* scintHC = 0;
  LXePMTHitsCollection* pmtHC = 0;

  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
 
  //Get the hit collections
  if(hitsCE){
    if(fScintCollID>=0)scintHC = (LXeScintHitsCollection*)(hitsCE->GetHC(fScintCollID));
    if(fPMTCollID>=0)pmtHC = (LXePMTHitsCollection*)(hitsCE->GetHC(fPMTCollID));
  }

  if(pmtHC){
    G4int pmts=pmtHC->entries();
    //Gather info from all PMTs
    for(G4int i=0;i<pmts;i++){
      //G4cout << "GetPMTPhysVol = " << (*pmtHC)[i]->GetPMTPhysVol()->GetName() << G4endl;
      if((*pmtHC)[i]->GetPMTPhysVol()->GetName() == "pmt")
	eventInformation->IncHitCount((*pmtHC)[i]->GetPhotonCount());
      if((*pmtHC)[i]->GetPMTPhysVol()->GetName() == "veto_pmt")
	eventInformation->IncVetoHitCount((*pmtHC)[i]->GetPhotonCount());

      if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold){
	if((*pmtHC)[i]->GetPMTPhysVol()->GetName() == "pmt")
	  eventInformation->IncPMTSAboveThreshold();
	if((*pmtHC)[i]->GetPMTPhysVol()->GetName() == "veto_pmt")
	  eventInformation->IncVetoPMTSAboveThreshold();
      }
      else{//wasnt above the threshold, turn it back off
        (*pmtHC)[i]->SetDrawit(false);
      }
    }

    pmtHC->DrawAllHits();
  }
  
  if(eventInformation->GetPMTSAboveThreshold() > 2){
    
    if(fVerbose>0){
      //End of event output. later to be controlled by a verbose level
      G4cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << G4endl << G4endl;

      G4cout << "\tNumber of photons produced by scintillation in this event : "
      	     << eventInformation->GetPhotonCount_Scint() << G4endl;
      // ID Summary
      G4cout << "---------------Inner Detector (ID) Summary" << G4endl;
      G4cout << "\tNumber of photons produced by cerenkov in ID in this event : "
	     << eventInformation->GetPhotonCount_Ceren() << G4endl;
      G4cout << "\tNumber of photons that hit ID PMTs in this event : "
	     << eventInformation->GetHitCount() << G4endl;
      G4cout << "\tNumber of PMTs above threshold("<<fPMTThreshold<<") : "
	     << eventInformation->GetPMTSAboveThreshold() << G4endl;
      G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
	     << eventInformation->GetAbsorptionCount() << G4endl;
      G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
	     << "this event : " << eventInformation->GetBoundaryAbsorptionCount() << G4endl;
      G4cout << "\tUnacounted for photons in this event : "
	     << (eventInformation->GetPhotonCount_Ceren() -
		 eventInformation->GetHitCount() -
		 eventInformation->GetBoundaryAbsorptionCount() -
		 eventInformation->GetAbsorptionCount())
	     << G4endl;

      // Veto Summary
      G4cout << "---------------Veto Summary" << G4endl;
      G4cout << "\tNumber of photons produced by cerenkov in this event : "
	     << eventInformation->GetVetoPhotonCount_Ceren() << G4endl;
      G4cout << "\tNumber of photons that hit veto PMTs in this event : "
	     << eventInformation->GetVetoHitCount() << G4endl;
      G4cout << "\tNumber of veto PMTs above threshold("<<fPMTThreshold<<") : "
	     << eventInformation->GetVetoPMTSAboveThreshold() << G4endl;
      G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
	     << eventInformation->GetVetoAbsorptionCount() << G4endl;
      G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
	     << "this event : " << eventInformation->GetVetoBoundaryAbsorptionCount()
	     << G4endl;
      G4cout << "\tUnacounted for photons in veto in this event : "
	     << (eventInformation->GetVetoPhotonCount_Ceren() -
		 eventInformation->GetVetoHitCount() -
		 eventInformation->GetVetoBoundaryAbsorptionCount() -
		 eventInformation->GetVetoAbsorptionCount())
	     << G4endl << G4endl;
    }
    
  
    //-------------------------------------
    
    LXeTrajectory* trj;
    for(G4int i = 0; i < n_trajectories; i++){
      trj = (LXeTrajectory*) ((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParentID() == 0)
	break;
    }
    
    //G4cout << anEvent->GetEventID() << "  "  << trj->GetParticleName() << "  ";
    //G4cout << trj->GetPointEntries() << "  ";

    double default_value=666.;


    Stat.inX = default_value;
    Stat.inY = default_value;
    Stat.inZ = default_value;

    Stat.outX = default_value;
    Stat.outY = default_value;
    Stat.outZ = default_value;

    Stat.VetoinX = default_value;
    Stat.VetoinY = default_value;
    Stat.VetoinZ = default_value;

    Stat.VetooutX = default_value;
    Stat.VetooutY = default_value;
    Stat.VetooutZ = default_value;            

    G4String VolName;
    G4String preVolName;    
    for(G4int i = 0; i < trj->GetPointEntries(); i++){
      G4VTrajectoryPoint* trjpt = trj->GetPoint(i);
      G4ThreeVector pt = trjpt->GetPosition();    
      G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
      G4VPhysicalVolume* myVolume = theNavigator->LocateGlobalPointAndSetup(pt); 
      VolName = myVolume->GetName();
      //G4cout << VolName << "  ";

      if(i == 0){
	Stat.primaryX = pt.getX()/m;
	Stat.primaryY = pt.getY()/m;
	Stat.primaryZ = pt.getZ()/m;
	//G4cout << pt << G4endl;
      }

      //if(VolName == "veto" && preVolName == "housing"){
      if( hit_enter_veto_from_outside( trj, i ) ){
	Stat.VetoinX = pt.getX()/m;
	Stat.VetoinY = pt.getY()/m;
	Stat.VetoinZ = pt.getZ()/m;
	//G4cout << "Entering veto" << G4endl;
      }
    //      if(VolName == "scintillator" && preVolName == "idin"){
      if( hit_enter_ID( trj, i ) ){
	Stat.inX = pt.getX()/m;
	Stat.inY = pt.getY()/m;
	Stat.inZ = pt.getZ()/m;
	//G4cout << "Entering ID" << G4endl;
      }
  //if(VolName == "veto" && preVolName == "idout"){
      if( hit_leaves_ID( trj, i ) ){
	Stat.outX = pt.getX()/m;
	Stat.outY = pt.getY()/m;
	Stat.outZ = pt.getZ()/m;
	//G4cout << "Exiting ID" << G4endl;
      }
//      if(VolName == "housing" && preVolName == "veto"){
      if( hit_leaves_through_veto( trj, i ) ){
	Stat.VetooutX = pt.getX()/m;
	Stat.VetooutY = pt.getY()/m;
	Stat.VetooutZ = pt.getZ()/m;
	//G4cout << "Exiting veto" << G4endl;
      }

      preVolName = VolName;
    }
    
    Stat.primaryPDG = trj->GetPDGEncoding();
    Stat.PX = trj->GetInitialMomentum().getX();
    Stat.PY = trj->GetInitialMomentum().getY();
    Stat.PZ = trj->GetInitialMomentum().getZ();
    //G4cout << trj->GetInitialMomentum() << G4endl;
    
    //-------------------------------------
    

    
    //-------------
  
    Stat.PhotonCount_Scint = eventInformation->GetPhotonCount_Scint();

    Stat.PhotonCount_Ceren           = eventInformation->GetPhotonCount_Ceren();
    Stat.HitCount                    = eventInformation->GetHitCount();
    Stat.PMTsAboveThreshold          = eventInformation->GetPMTSAboveThreshold();
    Stat.AbsorptionCount             = eventInformation->GetAbsorptionCount();
    Stat.BoundaryAbsorptionCount     = eventInformation->GetBoundaryAbsorptionCount();
    Stat.Unacounted                  = (eventInformation->GetPhotonCount_Ceren() -
					eventInformation->GetAbsorptionCount() -
					eventInformation->GetHitCount() -
					eventInformation->GetBoundaryAbsorptionCount());
    
    Stat.VetoPhotonCount_Ceren       = eventInformation->GetVetoPhotonCount_Ceren();
    Stat.VetoHitCount                = eventInformation->GetVetoHitCount();
    Stat.VetoPMTsAboveThreshold      = eventInformation->GetVetoPMTSAboveThreshold();
    Stat.VetoAbsorptionCount         = eventInformation->GetVetoAbsorptionCount();
    Stat.VetoBoundaryAbsorptionCount = eventInformation->GetVetoBoundaryAbsorptionCount();
    Stat.VetoUnacounted              = (eventInformation->GetVetoPhotonCount_Ceren() -
					eventInformation->GetVetoAbsorptionCount() -
					eventInformation->GetVetoHitCount() -
					eventInformation->GetVetoBoundaryAbsorptionCount());
    
    LXeRunAction* runact = (LXeRunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
    runact->UpdateStatistics(Stat);

    //-------------
    
  }

  //If we have set the flag to save 'special' events, save here
  if(fSaveThreshold&&eventInformation->GetPhotonCount() <= fSaveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();
  
  if(fRecorder)fRecorder->RecordEndOfEvent(anEvent);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
void LXeEventAction::SetSaveThreshold(G4int save){
/*Sets the save threshold for the random number seed. If the number of photons
generated in an event is lower than this, then save the seed for this event
in a file called run###evt###.rndm
*/
  fSaveThreshold=save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}

G4String LXeEventAction::volume_of_point(LXeTrajectory* trj, G4int i){

  G4String VolName;

  if( i < 0 ) return VolName;

  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  G4VTrajectoryPoint* trjpt = trj->GetPoint(i);
  G4ThreeVector pt = trjpt->GetPosition();    
  G4VPhysicalVolume* myVolume = theNavigator->LocateGlobalPointAndSetup(pt); 
  VolName = myVolume->GetName();
    
  return VolName;

}


bool LXeEventAction::hit_enter_veto_from_outside(LXeTrajectory* trj, G4int i){

  if( i <= 0 ) return false;

  G4String preVolName = volume_of_point(trj, i-1);
  G4String VolName = volume_of_point(trj, i);
    
  if(preVolName == "housing" && VolName == "veto")
    return true;
    
  return false;

}



bool LXeEventAction::hit_leaves_through_veto(LXeTrajectory* trj, G4int i){

  if( i <= 0 ) return false;

  G4String preVolName = volume_of_point(trj, i-1);
  G4String VolName = volume_of_point(trj, i);
    
  if(preVolName == "veto" && VolName == "housing")
    return true;
    
  return false;

}


bool LXeEventAction::hit_enter_ID(LXeTrajectory* trj, G4int i){

  if( i <= 0 ) return false;

  G4String preVolName = volume_of_point(trj, i-1);
  G4String VolName = volume_of_point(trj, i);
    
  if(preVolName == "idin" && VolName == "scintillator")
    return true;
    
  return false;

}


bool LXeEventAction::hit_leaves_ID(LXeTrajectory* trj, G4int i){

  if( i <= 0 ) return false;

  G4String preVolName = volume_of_point(trj, i-1);
  G4String VolName = volume_of_point(trj, i);
    
  if(preVolName == "idout" && VolName == "veto")
    return true;
    
  return false;

}





