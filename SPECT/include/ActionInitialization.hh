/// \file SPECT/include/ActionInitialization.hh
/// \brief Definition of the SPECT::ActionInitialization class

#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH 1

#include "G4VUserActionInitialization.hh"
#include "RunAction.hh"


namespace SPECT
{
    // Action initialization class

    class ActionInitialization : public G4VUserActionInitialization
    {
        public:
            ActionInitialization() = default;
            ~ActionInitialization() override = default;

            void Build() const override;
    };

}

#endif