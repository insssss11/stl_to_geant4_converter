// written by Hyunmin Yang, HANUL, KOREA university
// The detector construction, primary gen., action init are based on
// persistency/gdml/G01/load_gdml.cc
// Main program for demonstration of directly converting STL file to a G4TessellatedSolid instance.

#include <vector>

#include "G01PrimaryGeneratorAction.hh"
#include "G01DetectorConstruction.hh"
#include "G01ActionInitialization.hh"

#include "G4Types.hh"
#include "G4String.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "FTFP_BERT.hh"
// --------------------------------------------------------------

void PrintUsage()
{
  G4cout << G4endl;
  G4cout << "Usage: load_stl <input_stl_file:mandatory> <unit: [m][cm][mm]> <binary or ascii>" << G4endl;
  G4cout << G4endl;  
}

int main(int argc, char **argv)
{
  G4String format;
  G4String file;
  G4String unit;
  if (argc != 4)
  {
    PrintUsage();
    return -1;
  }
  file = argv[1];unit = argv[2];format = argv[3];
  if(!((unit == "m" || unit == "cm" || unit == "mm")))
  {
    PrintUsage();
    return -1;
  }
  else if(!((format == "binary" || format == "ascii")))
#ifdef G4MULTITHREADED
  printf("Multi Thread Mode(%1d threads)\n", 4);
  G4MTRunManager *runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(4);
#else
  printf("Multi Thread Mode\n");
  G4RunManager *runManager = new G4RunManager;
#endif
  runManager->SetUserInitialization(new G01DetectorConstruction(file, format, unit));
  runManager->SetUserInitialization(new FTFP_BERT);
  runManager->SetUserInitialization(new G01ActionInitialization());

  runManager->Initialize();

  // Initialize visualization
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  G4UIExecutive *ui = new G4UIExecutive(argc, argv);
  UImanager->ApplyCommand("/control/execute vis.mac");
  ui->SessionStart();
  delete ui;

  delete visManager;
  delete runManager;

  return 0;
}
