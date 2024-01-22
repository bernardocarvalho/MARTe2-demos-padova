/**
 * @file AtcaIopADC.h
 * @brief Header file for class AtcaIopADC
 * @date 19/10/2023
 * @author Andre Neto / Bernardo Carvalho
 *
 * Based on Example:
 * https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova/-/tree/master/DataSources/ADCSimulator
 *
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

 * @details This header file contains the declaration of the class AtcaIopADC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATCA_IOP_ADC_H
#define ATCA_IOP_ADC_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

    /**
     * The number of signals (2 time signals + ).
     */
    const uint32 ATCA_IOP_MAX_CHANNELS = 32u;
    
    const uint32 ATCA_IOP_N_TIMCNT = 2u;
    const uint32 ATCA_IOP_N_ADCs = 12u;
    const uint32 ATCA_IOP_N_INTEGRALS = ATCA_IOP_N_ADCs ; //12u;
    //const uint32 ADC_SIMULATOR_N_ADCs = 2u;
    const uint32 ATCA_IOP_N_SIGNALS = (ATCA_IOP_N_TIMCNT + ATCA_IOP_N_ADCs +
            ATCA_IOP_N_INTEGRALS);
    /**
     * The number of buffers to synchronise with the DMA
     */
    const uint32 NUMBER_OF_BUFFERS = 8u;

    /**
     * @brief A DataSource that simulates an ADC board
     * TODO
     * <pre>
     * +ADCSim = {
     *     Class = ADCSimulator
     *     DeviceName = "/dev/atca_v6" //Mandatory
     *     BoardId = 0          //   Mandatory
     *     ChopperPeriod = 2000
     *     ElectricalOffsets = {1, 10, 20, -30, 1, 1, -10, 10}
     *     WiringOffsets = {1, 10, 20, -30, 1, 1, -10, 10}
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
     *         ADC0Decim = {
     *             Type = uint32
     *         }
     *         ADC1Decim = {
     *             Type = uint32
     *         }
     *         ADC2Decim = {
     *             Type = uint32
     *         }
     *         ...
     *         ADC7Decim = {
     *             Type = uint32
     *         }
     *     }
     * }
     * </pre>
     */
    class AtcaIopADC: public DataSourceI, public MessageI, public EmbeddedServiceMethodBinderI {
        public:
            CLASS_REGISTER_DECLARATION()
                /**
                 * @brief Default constructor
                 * @post
                 *   Counter = 0
                 *   Time = 0
                 */
                AtcaIopADC    ();

            /**
             * @brief Destructor. Stops the EmbeddedThread.
             */
            virtual ~AtcaIopADC();

            /**
             * @brief See DataSourceI::AllocateMemory.
             */
            virtual bool AllocateMemory();

            /**
              gg* @brief See DataSourceI::GetNumberOfMemoryBuffers.
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
              if (boardFileDescriptor != -1) {
              close(boardFileDescriptor);
              }
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
             * @brief Initialises the AtcaIopADC
             * @param[in] data configuration in the form:
             * +AtcaIopADC = {
             *     Class = AtcaIopADC
             *     Signals = {
             *         Counter = {
             *             Type = uint32 //int32 also supported
             *         }
             *         Time = {
             *             Type = uint32 //int32 also supported
             *             Frequency = 10000
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

            /**
             * Find the currentDMABufferIndex and synchronize on data arrival
             */
//            int32 CurrentBufferIndex(uint64 waitLimitUs) const;

        private:
            /**
             * The board identifier
             */
            //uint32 boardId;
            /**
             * The numberOfChannels
             */
            uint32 numberOfChannels;
            /**
             * The FPGA ADC Decimation factor from 2MSPS
             */
            uint32 rtDecimation;
            /**
             * The board device name
             */
            StreamString deviceName;
            /**
             * The board device name
             */
            StreamString deviceDmaName;
            /**
             * The board file descriptor
             */
            int32 boardFileDescriptor;

            /**
             * The board Dma descriptor
             */
            int32 boardDmaDescriptor;

            /**
             * The two supported sleep natures.
             */
            enum AtcaIopADCSleepNature {
                Default = 0,
                Busy = 1
            };

            /**
             * The non-busy sleep percentage. Valid if
             * AtcaIopADCSleepNature == Busy
             */
            uint32 sleepPercentage;

            /**
             * The selected sleep nature.
             */
            AtcaIopADCSleepNature sleepNature;

            /**
             * Current counter and timer
             */
            uint32 counterAndTimer[2];

            /**
             * ADC values
             */
            int32 adcValues[ATCA_IOP_MAX_CHANNELS];

            /**
             * ADC Integral values
             */
            int64 adcIntegralValues[ATCA_IOP_MAX_CHANNELS];

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
            //float32 signalsFrequencies[4];

            /**
             * The simulated signals gains.
             */

            /**
             * The Electrical Offset Parameters.
             */
            int32 electricalOffsets[ATCA_IOP_MAX_CHANNELS];

            /**
             * The Wiring Offset Parameters.
             */
            float32 wiringOffsets[ATCA_IOP_MAX_CHANNELS];

            /**
             * The ADC chopping period in samples
             */
            uint16 chopperPeriod;

            /**
             * The simulated ADC frequency
             */
            uint32 adcFrequency;

            /**
             * The simulated ADC period
             */
            float64 adcPeriod;

            /**
             * For the ATCA Master board
             */
            uint32 isMaster;
            /**
             * Pointer to mapped memory
             */
            int32 * mappedDmaBase;
            uint32   mappedDmaSize;
            /**
             * The last written buffer
             */
            uint8 currentBufferIdx;

            /**
             * The oldest written DMA buffer index
             */
            uint8 oldestBufferIdx;
            /**
             * The last read buffer index
             */
            uint8 lastBufferIdx;

            /*
             *  Find the latest completed buffer without synchronization
             */
            int32 GetLatestBufferIndex() const;
            uint32 GetOldestBufferIdx() const;
            int32 PollDma(uint64 waitLimitTicks) const;

            /**
             * filter to receive the RPC which ...
             */
            ReferenceT<RegisteredMethodsMessageFilter> filter;

    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATCA_IOP_ADC_H */

//  vim: syntax=cpp ts=4 sw=4 sts=4 sr et
