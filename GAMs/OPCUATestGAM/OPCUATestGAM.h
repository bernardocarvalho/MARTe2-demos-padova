/**
 * @file OPCUATestGAM.h
 * @brief Header file for class OPCUATestGAM
 * @date 30 Jan 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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

 * @details This header file contains the declaration of the class OPCUATestGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DOCS_USER_SOURCE__STATIC_EXAMPLES_CORE_OPCUATESTGAM_H_
#define DOCS_USER_SOURCE__STATIC_EXAMPLES_CORE_OPCUATESTGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class OPCUATestGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

OPCUATestGAM    ();

    virtual ~OPCUATestGAM();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool Setup();

    virtual bool Execute();

    void SetOutput();

private:

    float64 * firstInputSignal;
    float64 * firstOutputSignal;
    float64 * time;
    uint32 check;
    float64 counter;
    uint64 statCpuCount;
    uint64 firstCpuCount;
    uint64 lastCpuCount;
    uint32 mode;
    TypeDescriptor signalType;
    uint32 numberOfDimensions;
    uint32 numberOfElements;
    uint32 numCycles;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DOCS_USER_SOURCE__STATIC_EXAMPLES_CORE_OPCUATESTGAM_H_ */

