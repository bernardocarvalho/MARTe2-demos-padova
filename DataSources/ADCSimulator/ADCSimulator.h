/**
 * @file ADCSimulator.h
 * @brief Header file for class ADCSimulator
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

 * @details This header file contains the declaration of the class ADCSimulator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ADC_SIMULATOR_H 
#define ADC_SIMULATOR_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A DataSource that simulates an ADC board
 * TODO
 * <pre>
 * +ADCSim = {
 *     Class = ADCSimulator
 *     SignalsFrequencies = {1, 10, 20, 30}
 *     SignalsGains = {32768, 16384, 8192, 4096}
 *     ADCFrequency = 2000000
 *     Signals = {
 *         Counter = {
 *             Type = uint32 //int32 also supported
 *         }
 *         Time = {
 *             Type = uint32 //int32 also supported
 *             Frequency = 1000
 *         }
 *         ADC0 = {
 *             Type = uint32
 *         }
 *         ADC1 = {
 *             Type = uint32
 *         }
 *         ADC2 = {
 *             Type = uint32
 *         }
 *         ADC3 = {
 *             Type = uint32
 *         }
 *     }
 * }
 * </pre>
 */
class ADCSimulator: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor
     * @post
     *   Counter = 0
     *   Time = 0
     */
ADCSimulator    ();

    /**
     * @brief Destructor. Stops the EmbeddedThread.
     */
    virtual ~ADCSimulator();

    /**
     * @brief See DataSourceI::AllocateMemory.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details Only InputSignals are supported.
     * @return MemoryMapSynchronisedInputBroker if frequency > 0, MemoryMapInputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief Waits on an EventSem for the period given by 1/Frequency to elapse on Execute.
     * @return true if the semaphore is successfully posted.
     */
    virtual bool Synchronise();

    /**
     * @brief Callback function for an EmbeddedThread.
     * @details Sleeps (Busy or Default) for the period given by 1/Frequency and post an EventSem which is waiting on
     *  the Synchronise method.
     * @param[in] info not used.
     * @return NoError if the EventSem can be successfully posted.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Resets the counter and the timer to zero and starts the EmbeddedThread.
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) and loops
     * on the ExecuteMethod.
     * @return true if the EmbeddedThread can be successfully started.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Initialises the ADCSimulator
     * @param[in] data configuration in the form:
     * +ADCSim = {
     *     Class = ADCSimulator
     *     Signals = {
     *         Counter = {
     *             Type = uint32 //int32 also supported
     *         }
     *         Time = {
     *             Type = uint32 //int32 also supported
     *             Frequency = 1000
     *         }
     *         ADC0 = {
     *             Type = uint32
     *         }
     *         ADC1 = {
     *             Type = uint32
     *         }
     *         ADC2 = {
     *             Type = uint32
     *         }
     *         ADC3 = {
     *             Type = uint32
     *         }
     *     }
     * }
     * @return TODO
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Verifies that two, and only two, signal are set with the correct type.
     * @details Verifies that two, and only two, signal are set; that the signals are
     * 32 bits in size with a SignedInteger or UnsignedInteger type and that a Frequency > 0 was set in one of the two signals.
     * @param[in] data see DataSourceI::SetConfiguredDatabase
     * @return true if the rules above are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously wait for the time to elapse.
     * @return the affinity of the thread which is going to be used to asynchronously wait for the time to elapse.
     */
    const ProcessorType& GetCPUMask() const;

    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously wait for the time to elapse.
     * @return the stack size of the thread which is going to be used to asynchronously wait for the time to elapse.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Gets the percentage of the time to sleep using the OS sleep (i.e. the non-busy Sleep).
     * @return the percentage of the time to sleep using the OS sleep (i.e. the non-busy Sleep).
     */
    uint32 GetSleepPercentage() const;

private:
    /**
     * The two supported sleep natures.
     */
    enum ADCSimulatorSleepNature {
        Default = 0,
        Busy = 1
    };

    /**
     * The non-busy sleep percentage. Valid if
     * ADCSimulatorSleepNature == Busy
     */
    uint32 sleepPercentage;

    /**
     * The selected sleep nature.
     */
    ADCSimulatorSleepNature sleepNature;

    /**
     * Current counter and timer
     */
    uint32 counterAndTimer[2];

    /**
     * ADC values
     */
    int32 *adcValues[4];

    /**
     * Number of samples to read on each cycle
     */
    uint32 adcSamplesPerCycle;

    /**
     * The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;

    /**
     * The EmbeddedThread where the Execute method waits for the period to elapse.
     */
    SingleThreadService executor;

    /**
     * HighResolutionTimer::Counter() value after the last Sleep.
     */
    uint64 lastTimeTicks;

    /**
     * Sleeping period in units of ticks.
     */
    uint64 sleepTimeTicks;

    /**
     * Sleeping period.
     */
    uint32 timerPeriodUsecTime;

    /**
     * Index of the function which has the signal that synchronises on this DataSourceI.
     */
    uint32 synchronisingFunctionIdx;

    /**
     * The affinity of the thread that asynchronously generates the time.
     */
    ProcessorType cpuMask;

    /**
     * The size of the stack of the thread that asynchronously generates the time.
     */
    uint32 stackSize;

    /**
     * The simulated signals frequencies.
     */
    float32 signalsFrequencies[4];

    /**
     * The simulated signals gains.
     */
    float32 signalsGains[4];

    /**
     * The simulated ADC frequency
     */
    uint32 adcFrequency;

    /**
     * The simulated ADC period
     */
    float64 adcPeriod;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ADC_SIMULATOR_H */

