/**
 * @file FilterDownsamplingGAM.cpp
 * @brief Source file for class FilterDownsamplingGAM
 * @date 01/03/2017
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

 * @details This source file contains the definition of all the methods for
 * the class FilterDownsamplingGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"

#include "FilterDownsamplingGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static const MARTe::float64 PI = 3.14159265359;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FilterDownsamplingGAM::FilterDownsamplingGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    numberOfSignals = 0u;

    input = NULL_PTR(int16 **);
    output = NULL_PTR(float64 **);
    gain = NULL_PTR(float64 *);
    accumulator = NULL_PTR(int64 *);

    numberOfSignals = 0u;
    numberOfElements = 0u;
    alpha20Bit = 0;
    oneMinusAlpha40Bit = 0;
}

FilterDownsamplingGAM::~FilterDownsamplingGAM() {
    using namespace MARTe;
    if (input != NULL_PTR(int16 **)) {
        delete input;
    }
    if (output != NULL_PTR(float64 **)) {
        delete output;
    }
    if (accumulator != NULL_PTR(int64 *)) {
        delete accumulator;
    }
    if (gain != NULL_PTR(float64 *)) {
        delete gain;
    }

}

bool FilterDownsamplingGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == GetNumberOfOutputSignals());
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != GetNumberOfOutputSignals()");
    }
    if (ok) {
        numberOfSignals = GetNumberOfInputSignals();
    }
    if (ok) {
        uint32 i;
        for (i = 0u; (i < numberOfSignals) && (ok); i++) {
            ok = (GetSignalType(InputSignals, 0u) == SignedInteger16Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals) != SignedInteger16Bit");
            }
        }
    }
    if (ok) {
        uint32 i;
        for (i = 0u; (i < numberOfSignals) && (ok); i++) {
            ok = (GetSignalType(OutputSignals, 0u) == Float64Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals) != Float64Bit");
            }
        }
    }
    if (ok) {
        uint32 i;
        uint32 readElements = 0u;
        for (i = 0u; (i < numberOfSignals) && (ok); i++) {
            ok = (GetSignalNumberOfSamples(InputSignals, i, readElements));
            if (ok) {
                if (readElements == 1u) {
                    ok = (GetSignalNumberOfElements(InputSignals, i, readElements));
                }
            }
            if (ok) {
                if (numberOfElements == 0u) {
                    numberOfElements = readElements;
                }
                else {
                    ok = (numberOfElements == readElements);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "The number of elements/samples must be consistent across all the input signals");
                    }
                }
            }
        }
    }
    if (ok) {
        uint32 i;
        uint32 readElements = 0u;
        for (i = 0u; (i < numberOfSignals) && (ok); i++) {
            ok = (GetSignalNumberOfSamples(OutputSignals, i, readElements));
            if (ok) {
                ok = (readElements == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of output samples must be 1");
                }
            }
            if (ok) {
                ok = (GetSignalNumberOfElements(OutputSignals, i, readElements));
                if (ok) {
                    ok = (readElements == 1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of output elements must be 1");
                }

            }
        }
    }
    uint32 s;
    if (ok) {
        accumulator = new int64[numberOfSignals];
        input = new int16*[numberOfSignals];
        output = new float64*[numberOfSignals];
        gain = new float64[numberOfSignals];
        for (s = 0u; (s < numberOfSignals) && (ok); s++) {
            input[s] = static_cast<int16 *>(GetInputSignalMemory(s));
            output[s] = static_cast<float64 *>(GetOutputSignalMemory(s));
            accumulator[s] = 0;
        }
    }
    if (ok) {
        ok = cdb.MoveRelative("OutputSignals");
    }

    for (s = 0u; (s < numberOfSignals) && (ok); s++) {
        ok = cdb.MoveToChild(s);
        if (ok) {
            ok = cdb.Read("Gain", gain[s]);
        }
        if (ok) {
            gain[s] /= (1LLU << 20);
            ok = cdb.MoveToAncestor(1u);
        }
    }

    return ok;
}

bool FilterDownsamplingGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        cdb = dynamic_cast<ConfigurationDatabase &>(data);
    }
    uint32 samplingFrequency;
    if (ok) {
        ok = data.Read("SamplingFrequency", samplingFrequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SamplingFrequency must be specified");
        }
    }
    if (ok) {
        ok = (samplingFrequency > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SamplingFrequency must be > 0");
        }
    }
    uint32 cutOffFrequency;
    if (ok) {
        ok = data.Read("CutOffFrequency", cutOffFrequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "CutOffFrequency must be specified");
        }
    }
    if (ok) {
        ok = (cutOffFrequency > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "CutOffFrequency must be > 0");
        }
    }
    float64 alphaFloat = 0.F;
    if (ok) {
        float64 numerator = -2 * PI * cutOffFrequency;
        alphaFloat = exp(numerator / float64(samplingFrequency));
        REPORT_ERROR(ErrorManagement::Information, "Computed alpha (in floating point) = %f", alphaFloat);
    }
    if (ok) {
        alpha20Bit = static_cast<uint64>(alphaFloat * (1LLU << 20u)); //Calibrate with 2**20 bit resolution
        oneMinusAlpha40Bit = ((1LLU << 40u) - (alpha20Bit * (1LLU << 20u))); //The (1-alpha) is performed at 40 bit resolution, thus (2**40 - alphaFloat * 2**40)
        REPORT_ERROR(ErrorManagement::Information, "Computed alpha20Bit = %u and oneMinusAlpha40Bit %u", alpha20Bit, oneMinusAlpha40Bit);
    }

    return ok;
}

bool FilterDownsamplingGAM::Execute() {
    using namespace MARTe;
    uint32 s;
    uint32 i;
    for (s = 0u; s < numberOfSignals; s++) {
        for (i = 0u; i < numberOfElements; i++) {
            accumulator[s] += (accumulator[s] * alpha20Bit); //Add the previous output
            int64 input64Bit = static_cast<int64>(input[s][i]);
            accumulator[s] += (input64Bit * oneMinusAlpha40Bit); //Add the input scaled to (1 - alpha). Fake 40 bits allow the division to have sufficient resolution later
            accumulator[s] >>= 20u; //Shift back the 20 bits that were used to calibrate the float alpha into an int alpha
        }
        *(output[s]) = static_cast<float64>(accumulator[s] * gain[s]);
    }
    return true;
}

CLASS_REGISTER(FilterDownsamplingGAM, "1.0")

