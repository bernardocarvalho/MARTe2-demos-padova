/**
 * @file AtcaIopDAC.h
 * @brief Header file for class AtcaIopDAC
 * @date 19/01/2024
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

 * @details This header file contains the declaration of the class AtcaIopDAC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATCA_IOP_DAC_H
#define ATCA_IOP_DAC_H

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
     * The number of signals 
     */

    const uint32 ATCA_IOP_N_DACs = 2u;
    const uint32 ATCA_IOP_MAX_DAC_CHANNELS = 16u;

/**
 * @brief A DataSource which provides an analogue output interface to the ATCA IOP  boards.
 * @details The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +AtcaIop_0_DAC = {
 *     Class = AtcaIop::AtcaIopDAC
 *     DeviceName = "/dev/atca_v6_dac_2" //Mandatory
 *     Signals = {
 *         DAC0_0 = {
 *             Type = float32 //Mandatory. Only type that is supported.
 *             OutputRange = 10.0 //Mandatory. The channel Module Output Range in volt.
 *             //OutputPolarity = Bipolar //Optional. Possible values: Bipolar, Unipolar. Default value Unipolar.
 *         }
 *     }
 * }
 * </pre>
 * Note that at least one of the GAMs writing to this DataSource must have set one of the signals with Trigger=1 (which forces the writing of all the signals to the DAC).
 */
    class AtcaIopDAC: public DataSourceI, public MessageI {
        public:
            CLASS_REGISTER_DECLARATION()
                /**
                 * @brief Default constructor
                 * @post
                 *   Counter = 0
                 *   Time = 0
                 */
                AtcaIopDAC    ();

            /**
             * @brief Destructor. Stops the EmbeddedThread.
             */
            virtual ~AtcaIopDAC();

            /**
             * @brief See DataSourceI::AllocateMemory.
             *  * @return true.
             */
            virtual bool AllocateMemory();

            /**
              gg* @brief See DataSourceI::GetNumberOfMemoryBuffers.
             * @return 1.
             */
            virtual uint32 GetNumberOfMemoryBuffers();

            /**
             * @brief See DataSourceI::GetSignalMemoryBuffer.
             */
            virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                    const uint32 bufferIdx,
                    void *&signalAddress);


            /**
             * @brief See DataSourceI::GetNumberOfMemoryBuffers.
             * @details Only OutputSignals are supported.
             * @return MemoryMapSynchronisedOutputBroker if Trigger == 1 for any of the signals, MemoryMapOutputBroker otherwise.
             */
            virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

            /**
             * @brief See DataSourceI::GetInputBrokers.
             * @return false.
             */
            virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                    const char8* const functionName,
                    void * const gamMemPtr);

            /**
             * @brief See DataSourceI::GetOutputBrokers.
             * @details If the functionName is one of the functions which requested a Trigger,
             * it adds a MemoryMapSynchronisedOutputBroker instance to the outputBrokers,
             * otherwise it adds a MemoryMapOutputBroker instance to the outputBrokers.
             * @param[out] outputBrokers where the BrokerI instances have to be added to.
             * @param[in] functionName name of the function being queried.
             * @param[in] gamMemPtr the GAM memory where the signals will be read from.
             * @return true if the outputBrokers can be successfully configured.
             */
            virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                    const char8* const functionName,
                    void * const gamMemPtr);


            /**
             * @brief See StatefulI::PrepareNextState.
             * @details NOOP.
             * @return true.
             */
            virtual bool PrepareNextState(const char8 * const currentStateName,
                    const char8 * const nextStateName);


            /**
             * @brief Loads and verifies the configuration parameters detailed in the class description.
             * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
             */

            virtual bool Initialise(StructuredDataI & data);


            /**
             * @brief Final verification of all the parameters and setup of the board configuration.
             * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
             *  are valid and consistent with the board parameters set during the initialisation phase.
             * In particular the following conditions shall be met:
             * - At least one triggering signal was requested by a GAM (with the property Trigger = 1)
             * - All the DAC channels have type float32.
             * - The number of samples of all the DAC channels is exactly one.
             * @return true if all the parameters are valid and consistent with the board parameters and if the board can be successfully configured with
             *  these parameters.
             */
            virtual bool SetConfiguredDatabase(StructuredDataI & data);


            /**
             * @details Writes the value of all the DAC channels to the board.
             * @return true if the writing of all the channels is successful.
             */
            virtual bool Synchronise();


        private:
            /**
             * The board device name
             */
            StreamString deviceName;
            /**
             * The board file descriptor
             */
            int32 boardFileDescriptor;

            /**
             * DAC values
             */
            int32 dacValues[ATCA_IOP_N_DACs];

            /**
             * The signal memory
             */
            float32 *channelsMemory;

            /**
             * The DACs that are enabled
             */
            // bool dacEnabled[ATCA_IOP_MAX_DAC_CHANNELS];

            /**
            * The board individual channel output ranges
            */
            float32 outputRange[ATCA_IOP_MAX_DAC_CHANNELS];

            /**
             * The number of enabled DACs
             */
            uint32 numberOfDACsEnabled;

            /**
             * Filter to receive the RPC which allows to change the...
             */
            ReferenceT<RegisteredMethodsMessageFilter> filter;

            /**
             * True if at least one trigger was set.
             */
            bool triggerSet;
            
            int32 SetDacReg(uint32 channel, float32 val) const;

    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATCA_IOP_DAC_H */

//  vim: syntax=cpp ts=4 sw=4 sts=4 sr et
