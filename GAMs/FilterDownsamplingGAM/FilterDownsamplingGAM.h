/**
 * @file FilterDownsamplingGAM.h
 * @brief Header file for class FilterDownsamplingGAM
 * @date 06/03/2017
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

 * @details This header file contains the declaration of the class FilterDownsamplingGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILTERDOWNSAMPLINGGAM_H_
#define FILTERDOWNSAMPLINGGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which implements a first order IIR filter and decimates the output signal.
 * @details The same filter parameters are applied to all the signals.
 * The input signals shall have type int16 and the output signal shall have type float64.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +GAMFilterDownsampling = {
 *     Class = FilterDownsamplingGAM
 *     SamplingFrequency = 2000000 //Compulsory sampling frequency of the input signals
 *     CutOffFrequency = 200 //Compulsory cut-off frequency of the filter
 *     InputSignals = {
 *         ADC0_0 = {
 *              DataSource = DDB1
 *              Type = int16 //Only type that is supported
 *              NumberOfElements = 2000
 *              NumberOfDimensions = 1
 *         }
 *         ADC1_0 = {
 *              DataSource = DDB1
 *              NumberOfElements = 2000
 *              NumberOfDimensions = 1
 *         }
 *         ...
 *     }
 *     OutputSignals = {
 *         ADC0_0_filtered = {
 *             DataSource = DDB1
 *             Type = float64 //Only type that is supported
 *             NumberOfElements = 1 //Shall be one (decimated output)
 *             NumberOfDimensions = 1
 *             Gain = 1.52587890625e-04 //Gain to apply to the output signal
 *        }
 *        ADC1_0_filtered = {
 *             DataSource = DDB1
 *             Type = float64
 *             NumberOfElements = 1
 *             NumberOfDimensions = 1
 *             Gain = 1.52587890625e-04
 *        }
 *        ...
 *     }
 * }
 */
class FilterDownsamplingGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
FilterDownsamplingGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~FilterDownsamplingGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == GetNumberOfOutputSignals() &&
     *  - GetSignalType(InputSignals, *) == SignedInteger16Bit &&
     *  - GetSignalType(OutputSignals, *) == Float32Bit
     *  @return true if the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Applies the filter to all the input signals.
     * @return true.
     */
    virtual bool Execute();

private:
    /**
     * Filter state.
     */
    MARTe::int64 *accumulator;

    /**
     * Gain to apply to the output signal.
     */
    MARTe::float64 *gain;

    /**
     * Parameter alpha of the filter (alpha = e**(-2 PI f_cut_off/f_sampling), multiplied by 2**20
     */
    MARTe::int64 alpha20Bit;

    /**
     * The value of (1 - alpha) * 2**40
     */
    MARTe::int64 oneMinusAlpha40Bit;

    /**
     * The input signal list.
     */
    MARTe::int16 **input;

    /**
     * The filtered output signal list.
     */
    MARTe::float64 **output;

    /**
     * The number of signals.
     */
    MARTe::uint32 numberOfSignals;

    /**
     * The number of elements in the input signals.
     */
    MARTe::uint32 numberOfElements;

    /**
     * StructuredData received at Initialise.
     */
    MARTe::ConfigurationDatabase cdb;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILTERDOWNSAMPLINGGAM_H_ */

