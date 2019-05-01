/**
 * @file ControllerEx2.h
 * @brief Header file for class ControllerEx2
 * @date 06/04/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ControllerEx2
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_CONTROLLEREXAMPLE2_H_
#define EXAMPLES_CORE_CONTROLLEREXAMPLE2_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageI.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * Configuration structures
 */
struct Gain {
    MARTe::float32 gain1;
    MARTe::int32 gain2;
    MARTe::float32 gain3[6];
};
struct Gains {
    struct Gain lowGains;
    struct Gain highGains;
};

/**
 * @brief A MARTe::Object class that will read directly read its configuration from a structure.
 */
class ControllerEx2: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    ControllerEx2    ();

    virtual ~ControllerEx2 ();

    /**
     * Read all the properties from the Gains struct (names must match the ones of the struct!
     * Gains1 = {
     *     Low = {
     *         Gain1 = -1.0
     *         Gain2 = -3.0
     *         Gain3 = {-9.0, -8.0, -7.0, -6.0, -5.0, -4.0}
     *     }
     *     High = {
     *         Gain1 = 7.0
     *         Gain2 = 9.0
     *     }
     * }
     * Gains2 = {
     *     Low = {
     *         Gain1 = -1.1
     *         Gain2 = -3.1
     *         Gain3 = {-9.1, -8.1, -7.1, -6.1, -5.1, -4.1}
     *     }
     *     High = {
     *         Gain1 = 7.1
     *         Gain2 = 9.1
     *         Gain3 = {9.1, 8.1, 7.1, 6.1, 5.1, 4.1}"
     *     }
     * }
     */
    virtual bool Initialise(MARTe::StructuredDataI &data);

    /**
     * @brief Example of an RPC.
     */
    MARTe::ErrorManagement::ErrorType ResetGain1();

private:
    bool LoadGains(Gain &gainToLoad, MARTe::StructuredDataI &data);
    void PrintGains(Gain &gainToPrint);
    void PrintGainsInfo();

    /**
     * A list of properties.
     */
    Gains gains1;
    Gains gains2;
    MARTe::float32 tau;
};

}
#endif
