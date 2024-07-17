/// \file SPECT/src/ActionInitialization.cc
/// \brief Implementation of the SPECT::ActionInitialization class

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

namespace SPECT
{


void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* generator = new PrimaryGeneratorAction();
  SetUserAction(generator);

  RunAction *runAction = new RunAction();
  SetUserAction(runAction);

}


}
