/**
 * @file ADCSimulator.cpp
 * @brief Source file for class ADCSimulator
 * @date 19/09/2020
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
 * the class ADCSimulator (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ADCSimulator.h"
#include "MemoryMapSynchronisedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * The number of signals (2 time signals + 4 ADCs).
 */
const uint32 ADC_SIMULATOR_N_ADCs = 4u; 
const uint32 ADC_SIMULATOR_N_SIGNALS = 2 + ADC_SIMULATOR_N_ADCs;
const float64 ADC_SIMULATOR_PI = 3.14159265359;

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
ADCSimulator::ADCSimulator() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    lastTimeTicks = 0u;
    sleepTimeTicks = 0u;
    timerPeriodUsecTime = 0u;
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    sleepNature = Busy;
    sleepPercentage = 0u;
    adcPeriod = 0.;
    uint32 k;
    for (k=0u; k<ADC_SIMULATOR_N_ADCs; k++) {
        adcValues[k] = NULL_PTR(int32 *);
    }

    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

/*lint -e{1551} the destructor must guarantee that the Timer SingleThreadService is stopped.*/
ADCSimulator::~ADCSimulator() {
    if (!synchSem.Post()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not post EventSem.");
    }
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    uint32 k;
    for (k=0u; k<ADC_SIMULATOR_N_ADCs; k++) {
        if (adcValues[k] != NULL_PTR(int32 *)) {
            delete [] adcValues[k];
        }
    }
}

bool ADCSimulator::AllocateMemory() {
    return true;
}

bool ADCSimulator::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    StreamString sleepNatureStr;
    if (!data.Read("SleepNature", sleepNatureStr)) {
        REPORT_ERROR(ErrorManagement::Information, "SleepNature was not set. Using Default.");
        sleepNatureStr = "Default";
    }
    if (sleepNatureStr == "Default") {
        sleepNature = Default;
    }
    else if (sleepNatureStr == "Busy") {
        sleepNature = Busy;
        if (!data.Read("SleepPercentage", sleepPercentage)) {
            sleepPercentage = 0u;
            REPORT_ERROR(ErrorManagement::Information, "SleepPercentage was not set. Using Default %d.", sleepPercentage);
        }
        if (sleepPercentage > 100u) {
            sleepPercentage = 100u;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported SleepNature.");
        ok = false;
    }

    AnyType arrayDescription = data.GetType("SignalsFrequencies");
    uint32 numberOfElements = 0u;
    if (ok) {
        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
    }
    if (ok) {
        numberOfElements = arrayDescription.GetNumberOfElements(0u);
        ok = (numberOfElements == 4u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly 4 elements shall be defined in the array SignalsFrequencies");
        }
    }
    if (ok) {
        Vector<float32> readVector(&signalsFrequencies[0u], numberOfElements);
        ok = data.Read("SignalsFrequencies", readVector);
    }

    arrayDescription = data.GetType("SignalsGains");
    numberOfElements = 0u;
    if (ok) {
        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
    }
    if (ok) {
        numberOfElements = arrayDescription.GetNumberOfElements(0u);
        ok = (numberOfElements == 4u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly 4 elements shall be defined in the array SignalsGains");
        }
    }
    if (ok) {
        Vector<float32> readVector(&signalsGains[0u], numberOfElements);
        ok = data.Read("SignalsGains", readVector);
    }

    adcFrequency = 2e6;
    if (!data.Read("ADCFrequency", adcFrequency)) {
        REPORT_ERROR(ErrorManagement::Warning, "ADCFrequency not specified. Using default: %d", adcFrequency);
    }
    adcPeriod = 1./adcFrequency;

    if (ok) {
        uint32 cpuMaskIn;
        if (!data.Read("CPUMask", cpuMaskIn)) {
            cpuMaskIn = 0xFFu;
            REPORT_ERROR(ErrorManagement::Warning, "CPUMask not specified using: %d", cpuMaskIn);
        }
        cpuMask = cpuMaskIn;
    }
    if (ok) {
        if (!data.Read("StackSize", stackSize)) {
            stackSize = THREADS_DEFAULT_STACKSIZE;
            REPORT_ERROR(ErrorManagement::Warning, "StackSize not specified using: %d", stackSize);
        }
    }
    if (ok) {
        ok = (stackSize > 0u);

        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u");
        }
    }
    if (ok) {
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(stackSize);
    }
    return ok;
}

bool ADCSimulator::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    //The DataSource shall have 6 signals. The first two are uint32 (counter and time) and other 4 represent an ADC value as int32
    if (ok) {
        ok = (GetNumberOfSignals() == ADC_SIMULATOR_N_SIGNALS);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Exactly %d signals shall be configured", ADC_SIMULATOR_N_SIGNALS);
    }
    uint32 i;
    for (i=0; (i<ADC_SIMULATOR_N_SIGNALS) && (ok); i++) {
        ok = (GetSignalType(i).numberOfBits == 32u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The signal in position %d shall have 32 bits and %d were specified", i, uint16(GetSignalType(i).numberOfBits));
        }
    }
    for (i=0; (i<2) && (ok); i++) {
        ok = (GetSignalType(i).type == UnsignedInteger);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The signal %d shall be of type UnsignedInteger", i);
        }
    }
    for (i=2; (i<6) && (ok); i++) {
        ok = (GetSignalType(i).type == SignedInteger);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The signal %d shall be of type SignedInteger", i);
        }
    }
    uint32 nOfFunctions = GetNumberOfFunctions();
    float32 cycleFrequency = -1.0F;
    bool frequencyFound = false;
    uint32 functionIdx;

    //How many GAMs (functions) are interacting with this DataSource?
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);
        uint32 i;
        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            if (!frequencyFound) {
                ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, cycleFrequency);
                //Found a GAM that wants to synchronise on this DataSourceI. We need to have one and exactly one GAM asking to synchronise in this DataSource.
                frequencyFound = (cycleFrequency > 0.F);
            }
            bool isCounter = false;
            bool isTime = false;
            uint32 signalIdx = 0u;
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(InputSignals, functionIdx, i, nSamples);

            //Is the counter or the time signal?
            StreamString signalAlias;
            if (ok) {
                ok = GetFunctionSignalAlias(InputSignals, functionIdx, i, signalAlias);
            }
            if (ok) {
                ok = GetSignalIndex(signalIdx, signalAlias.Buffer());
            }
            if (ok) {
                isCounter = (signalIdx == 0u);
                isTime = (signalIdx == 1u);
                if (isCounter) {
                    if (nSamples > 1u) {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (counter) shall have one and only one sample");
                    }
                }
                else if (isTime) {
                    if (nSamples > 1u) {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "The second signal (time) shall have one and only one sample");
                    }
                }
                else {
                    //How many samples to read for each cycle?
                    if (adcSamplesPerCycle == 0u) {
                        adcSamplesPerCycle = nSamples;
                    }
                    else {
                        if (adcSamplesPerCycle != nSamples) {
                            ok = false;
                            REPORT_ERROR(ErrorManagement::ParametersError, "All the ADC signals shall have the same number of samples");
                        }
                    }

                }
            }
        }
    }
    if (ok) {
        ok = frequencyFound;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No frequency > 0 was set (i.e. no signal synchronises on this ADCSimulator).");
        }
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "The timer will be set using a cycle frequency of %f Hz", cycleFrequency);

        float64 periodUsec = (1e6 / cycleFrequency);
        timerPeriodUsecTime = static_cast<uint32>(periodUsec);
        float64 sleepTimeT = (static_cast<float64>(HighResolutionTimer::Frequency()) / cycleFrequency);
        sleepTimeTicks = static_cast<uint64>(sleepTimeT);
    }
    if (ok) {
        float32 totalNumberOfSamplesPerSecond = (static_cast<float32>(adcSamplesPerCycle) * cycleFrequency);
        ok = (adcFrequency == static_cast<uint32>(totalNumberOfSamplesPerSecond));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "The adcSamplesPerCycle * cycleFrequency (%u) shall be equal to the ADCs acquisition frequency (%u)",
                totalNumberOfSamplesPerSecond, adcFrequency);
        }
    }
    if (ok) {
        uint32 k;
        for (k=0u; k<ADC_SIMULATOR_N_ADCs; k++) {
            adcValues[k] = new int32[adcSamplesPerCycle];
        }
    }

    return ok;
}

uint32 ADCSimulator::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool ADCSimulator::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = true;
    if (signalIdx == 0u) {
        signalAddress = &counterAndTimer[0];
    }
    else if (signalIdx == 1u) {
        signalAddress = &counterAndTimer[1];
    }
    else if (signalIdx < ADC_SIMULATOR_N_SIGNALS) {
        signalAddress = adcValues[signalIdx - 2u];
    }
    else {
        ok = false;
    }
    return ok;
}

const char8* ADCSimulator::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        float32 frequency = 0.F;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }

        if (frequency > 0.F) {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool ADCSimulator::Synchronise() {
    ErrorManagement::ErrorType err;
    err = synchSem.ResetWait(TTInfiniteWait);
    return err.ErrorsCleared();
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the counter and the timer are always reset irrespectively of the states being changed.*/
bool ADCSimulator::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    bool ok = true;
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        ok = executor.Start();
    }
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the method sleeps for the given period irrespectively of the input info.*/
ErrorManagement::ErrorType ADCSimulator::Execute(ExecutionInfo& info) {
    if (lastTimeTicks == 0u) {
        lastTimeTicks = HighResolutionTimer::Counter();
    }

    uint64 startTicks = HighResolutionTimer::Counter();
    //If we lose cycle, rephase to a multiple of the period.
    uint32 nCycles = 0u;
    while (lastTimeTicks < startTicks) {
        lastTimeTicks += sleepTimeTicks;
        nCycles++;
    }
    lastTimeTicks -= sleepTimeTicks;

    //Sleep until the next period. Cannot be < 0 due to while(lastTimeTicks < startTicks) above
    uint64 sleepTicksCorrection = (startTicks - lastTimeTicks);
    uint64 deltaTicks = sleepTimeTicks - sleepTicksCorrection;

    if (sleepNature == Busy) {
        if (sleepPercentage == 0u) {
            while ((HighResolutionTimer::Counter() - startTicks) < deltaTicks) {
            }
        }
        else {
            float32 totalSleepTime = static_cast<float32>(static_cast<float64>(deltaTicks) * HighResolutionTimer::Period());
            uint32 busyPercentage = (100u - sleepPercentage);
            float32 busyTime = totalSleepTime * (static_cast<float32>(busyPercentage) / 100.F);
            Sleep::SemiBusy(totalSleepTime, busyTime);
        }
    }
    else {
        float32 sleepTime = static_cast<float32>(static_cast<float64>(deltaTicks) * HighResolutionTimer::Period());
        Sleep::NoMore(sleepTime);
    }
    lastTimeTicks = HighResolutionTimer::Counter();

    ErrorManagement::ErrorType err = synchSem.Post();
    counterAndTimer[0] += nCycles;
    counterAndTimer[1] = counterAndTimer[0] * timerPeriodUsecTime;
    uint32 k;
    uint32 s;
    float64 t = counterAndTimer[1];
    t /= 1e6;
    for (s=0u; s<adcSamplesPerCycle; s++) {
        for (k=0u; k<ADC_SIMULATOR_N_ADCs; k++) {
            float32 value = signalsGains[k] * sin(2 * ADC_SIMULATOR_PI * signalsFrequencies[k] * t);
            adcValues[k][s] = static_cast<uint32>(value);
#if 0
            //if (k == 0){
                //if (s == (adcSamplesPerCycle - 1)){
                    REPORT_ERROR(ErrorManagement::FatalError, "adcValues[%d][%d] = %d (%f), signalsGains[%d] = %f signalsFrequencies[%d] = %f t=%f", k, s, adcValues[k][s], value, k, signalsGains[k], k, signalsFrequencies[k], t);
                //}
            //}
#endif
        }
        t += adcPeriod;
    }
    return err;
}

const ProcessorType& ADCSimulator::GetCPUMask() const {
    return cpuMask;
}

uint32 ADCSimulator::GetStackSize() const {
    return stackSize;
}

uint32 ADCSimulator::GetSleepPercentage() const {
    return sleepPercentage;
}

CLASS_REGISTER(ADCSimulator, "1.0")

}

