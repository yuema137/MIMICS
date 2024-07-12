/// \file SPECT/src/ActionInitialization.cc
/// \brief Implementation of the SPECT::ActionInitialization class

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

namespace SPECT
{


void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction());
}


}
