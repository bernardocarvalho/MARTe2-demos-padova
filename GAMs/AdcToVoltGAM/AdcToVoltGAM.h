/**
 * @file AdcToVoltGAM.h
 * @brief Header file for class AdcToVoltGAM
 * @date 06 mag 2019
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class AdcToVoltGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMS_ADCTOVOLTGAM_ADCTOVOLTGAM_H_
#define 		GAMS_ADCTOVOLTGAM_ADCTOVOLTGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*------------------------------------------------32---------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


class AdcToVoltGAM : public GAM {
public:
    CLASS_REGISTER_DECLARATION()
    AdcToVoltGAM();

    virtual ~AdcToVoltGAM();

    virtual bool Initialise(StructuredDataI & data);


    virtual bool Setup();


    virtual bool Execute();

private:

    uint16 maxIn;
    uint16 minIn;
    float64 maxOut;
    float64 minOut;
    uint16 *input;
    int16 *voltage;
    uint32 *numberOfElements;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMS_ADCTOVOLTGAM_ADCTOVOLTGAM_H_ */

