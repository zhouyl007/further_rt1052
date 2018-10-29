/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_TSC_H_
#define _FSL_TSC_H_

#include "fsl_common.h"

/*!
 *  @addtogroup tsc
 *  @{
 */

/*******************************************************************************
* Definitions
******************************************************************************/
/*! @brief TSC driver version */
#define FSL_TSC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*!
* @ Controller detection mode.
*/
typedef enum _tsc_detection_mode
{
    kTSC_Detection4WireMode = 0U, /*!< 4-Wire Detection Mode. */
    kTSC_Detection5WireMode = 1U, /*!< 5-Wire Detection Mode. */
} tsc_detection_mode_t;

/*!
* @ Coordinate value mask.
*/
typedef enum _tsc_corrdinate_value_selection
{
    kTSC_XCoordinateValueSelection = 0U, /*!< X coordinate value is selected. */
    kTSC_YCoordinateValueSelection = 1U, /*!< Y coordinate value is selected. */
} tsc_corrdinate_value_selection_t;

/*!
* @ Interrupt signal enable/disable mask.
*/
enum _tsc_interrupt_signal_mask
{
    kTSC_IdleSoftwareSignalEnable = TSC_INT_SIG_EN_IDLE_SW_SIG_EN_MASK, /*!< Enable the interrupt signal when the
                                                                           controller has return to idle status.
                                                                             The signal is only valid after using
                                                                           TSC_ReturnToIdleStatus API. */
    kTSC_ValidSignalEnable =
        TSC_INT_SIG_EN_VALID_SIG_EN_MASK, /*!< Enable the interrupt signal when controller receives a detect signal
                                             after measurement. */
    kTSC_DetectSignalEnable =
        TSC_INT_SIG_EN_DETECT_SIG_EN_MASK, /*!< Enable the interrupt signal when controller receives a detect signal. */
    kTSC_MeasureSignalEnable = TSC_INT_SIG_EN_MEASURE_SIG_EN_MASK, /*!< Enable the interrupt signal after the touch
                                                                      detection which follows measurement. */
};

/*!
* @ Interrupt enable/disable mask.
*/
enum _tsc_interrupt_mask
{
    kTSC_IdleSoftwareInterruptEnable =
        TSC_INT_EN_IDLE_SW_INT_EN_MASK, /*!< Enable the interrupt when the controller has return to idle status.
                                             The interrupt is only valid after using TSC_ReturnToIdleStatus API. */
    kTSC_DetectInterruptEnable =
        TSC_INT_EN_DETECT_INT_EN_MASK, /*!< Enable the interrupt when controller receive a detect signal. */
    kTSC_MeasureInterruptEnable = TSC_INT_EN_MEASURE_INT_EN_MASK, /*!< Enable the interrupt after the touch detection
                                                                     which follows measurement. */
};

/*!
* @ Interrupt Status flag mask.
*/
enum _tsc_interrupt_status_flag_mask
{
    kTSC_IdleSoftwareFlag =
        TSC_INT_STATUS_IDLE_SW_MASK, /*!< This flag is set if the controller has return to idle status.
                                        The flag is only valid after using TSC_ReturnToIdleStatus API. */
    kTSC_ValidSignalFlag =
        TSC_INT_STATUS_VALID_MASK, /*!< This flag is set if controller receives a detect signal after measurement. */
    kTSC_DetectSignalFlag = TSC_INT_STATUS_DETECT_MASK, /*!< This flag is set if controller receives a detect signal. */
    kTSC_MeasureSignalFlag =
        TSC_INT_STATUS_MEASURE_MASK, /*!< This flag is set after the touch detection which follows measurement.
                                          Note: Valid signal falg will be cleared along with measure signal flag. */
};

/*!
* @ ADC status flag mask.
*/
enum _tsc_adc_status_flag_mask
{
    kTSC_ADCCOCOSignalFlag =
        TSC_DEBUG_MODE_ADC_COCO_MASK, /*!< This signal is generated by ADC when a conversion is completed. */
    kTSC_ADCConversionValueFlag =
        TSC_DEBUG_MODE_ADC_CONV_VALUE_MASK, /*!< This signal is generated by ADC and indicates the result of an ADC
                                               conversion. */
};

/*!
* @ TSC status flag mask.
*/
enum _tsc_status_flag_mask
{
    kTSC_IntermediateStateFlag = TSC_DEBUG_MODE2_INTERMEDIATE_MASK,  /*!< This flag is set if TSC is in intermediate
                                                                        state, between two state machine states. */
    kTSC_DetectFiveWireFlag = TSC_DEBUG_MODE2_DETECT_FIVE_WIRE_MASK, /*!< This flag is set if TSC receives a 5-wire
                                                                        detect signal. It is only valid when the TSC in
                                                                        detect state and DETECT_ENABLE_FIVE_WIRE bit is
                                                                        set. */
    kTSC_DetectFourWireFlag = TSC_DEBUG_MODE2_DETECT_FOUR_WIRE_MASK, /*!< This flag is set if TSC receives a 4-wire
                                                                        detect signal. It is only valid when the TSC in
                                                                        detect state and DETECT_ENABLE_FOUR_WIRE bit is
                                                                        set. */
    kTSC_GlitchThresholdFlag = TSC_DEBUG_MODE2_DE_GLITCH_MASK, /*!< This field indicates glitch threshold.The threshold
                                                                  is defined by number of clock cycles. See
                                                                  "tsc_glitch_threshold_t".
                                                                    If value = 00, Normal function: 0x1fff ipg clock
                                                                  cycles, Low power mode: 0x9 low power clock cycles.
                                                                    If value = 01, Normal function: 0xfff ipg clock
                                                                  cycles, Low power mode: :0x7 low power clock cycles.
                                                                    If value = 10, Normal function: 0x7ff ipg clock
                                                                  cycles, Low power mode:0x5 low power clock cycles.
                                                                    If value = 11, Normal function: 0x3 ipg clock
                                                                  cycles, Low power mode:0x3 low power clock cycles. */
    kTSC_StateMachineFlag =
        TSC_DEBUG_MODE2_STATE_MACHINE_MASK, /*!< This field indicates the state of TSC. See "tsc_state_machine_t";
                                                 if value = 000, Controller is in idle state.
                                                 if value = 001, Controller is in 1st-Pre-charge state.
                                                 if value = 010, Controller is in 1st-detect state.
                                                 if value = 011, Controller is in x-measure state.
                                                 if value = 100, Controller is in y-measure state.
                                                 if value = 101, Controller is in 2nd-Pre-charge state.
                                                 if value = 110, Controller is in 2nd-detect state. */
};

/*!
* @brief TSC state machine. These seven states are TSC complete workflow.
*/
typedef enum _tsc_state_machine
{
    kTSC_IdleState = 0U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT,         /*!< Controller is in idle state. */
    kTSC_1stPreChargeState = 1U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT, /*!< Controller is in 1st-Pre-charge state. */
    kTSC_1stDetectState = 2U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT,    /*!< Controller is in 1st-detect state. */
    kTSC_XMeasureState = 3U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT,     /*!< Controller is in x-measure state. */
    kTSC_YMeasureState = 4U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT,     /*!< Controller is in y-measure state. */
    kTSC_2ndPreChargeState = 5U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT, /*!< Controller is in 2nd-Pre-charge state. */
    kTSC_2ndDetectState = 6U << TSC_DEBUG_MODE2_STATE_MACHINE_SHIFT,    /*!< Controller is in 2nd-detect state. */
} tsc_state_machine_t;

/*!
* @brief TSC glitch threshold.
*/
typedef enum _tsc_glitch_threshold
{
    kTSC_glitchThresholdALT0 =
        0U << TSC_DEBUG_MODE2_DE_GLITCH_SHIFT, /*!< Normal function: 0x1fff ipg clock cycles, Low power mode: 0x9 low
                                                          power clock cycles. */
    kTSC_glitchThresholdALT1 =
        1U << TSC_DEBUG_MODE2_DE_GLITCH_SHIFT, /*!< Normal function: 0xfff ipg clock cycles, Low power mode: :0x7 low
                                                          power clock cycles. */
    kTSC_glitchThresholdALT2 =
        2U << TSC_DEBUG_MODE2_DE_GLITCH_SHIFT, /*!< Normal function: 0x7ff ipg clock cycles, Low power mode: :0x5 low
                                                          power clock cycles. */
    kTSC_glitchThresholdALT3 =
        3U << TSC_DEBUG_MODE2_DE_GLITCH_SHIFT, /*!< Normal function: 0x3 ipg clock cycles, Low
                                                          power mode: :0x3 low power clock cycles. */
} tsc_glitch_threshold_t;

/*!
* @ Hardware trigger select signal, select which ADC channel to start conversion.
*/
typedef enum _tsc_trigger_signal
{
    kTSC_TriggerToChannel0 = 1U << 0U, /*!< Trigger to ADC channel0. ADC_HC0 register will be used to conversion. */
    kTSC_TriggerToChannel1 = 1U << 1U, /*!< Trigger to ADC channel1. ADC_HC1 register will be used to conversion. */
    kTSC_TriggerToChannel2 = 1U << 2U, /*!< Trigger to ADC channel2. ADC_HC2 register will be used to conversion. */
    kTSC_TriggerToChannel3 = 1U << 3U, /*!< Trigger to ADC channel3. ADC_HC3 register will be used to conversion. */
    kTSC_TriggerToChannel4 = 1U << 4U, /*!< Trigger to ADC channel4. ADC_HC4 register will be used to conversion. */
} tsc_trigger_signal_t;

/*!
* @ TSC controller ports.
*/
typedef enum _tsc_port_source
{
    kTSC_WiperPortSource = 0U, /*!< TSC controller wiper port. */
    kTSC_YnlrPortSource = 1U,  /*!< TSC controller ynlr port. */
    kTSC_YpllPortSource = 2U,  /*!< TSC controller ypll port. */
    kTSC_XnurPortSource = 3U,  /*!< TSC controller xnur port. */
    kTSC_XpulPortSource = 4U,  /*!< TSC controller xpul port. */
} tsc_port_source_t;

/*!
* @ TSC port mode.
*/
typedef enum _tsc_port_mode
{
    kTSC_PortOffMode = 0U,               /*!< Disable pull up/down mode. */
    kTSC_Port200k_PullUpMode = 1U << 2U, /*!< 200k-pull up mode. */
    kTSC_PortPullUpMode = 1U << 1U,      /*!< Pull up mode. */
    kTSC_PortPullDownMode = 1U << 0U,    /*!< Pull down mode. */
} tsc_port_mode_t;

/*!
* @ Controller configuration.
*/
typedef struct _tsc_config
{
    bool enableAutoMeasure;    /*!< Enable the auto-measure. It indicates after detect touch, whether automatic start
                                  measurement */
    uint32_t measureDelayTime; /*!< Set delay time(0U~0xFFFFFFU) to even potential distribution ready.It is a
                                    preparation for measure stage. If measure dalay time is too short, maybe it would
                                    have an undesired effect on measure value. */
    uint32_t prechargeTime;    /*!< Set pre-charge time(1U~0xFFFFFFFFU) to make the upper layer of
                                     screen to charge to positive high. It is a preparation for detection stage.
                                     Pre-charge time must is greater than 0U, otherwise TSC could not work normally.
                                     If pre-charge dalay time is too short, maybe it would have an undesired effect on
                                     generation of valid signal(kTSC_ValidSignalFlag).*/
    tsc_detection_mode_t detectionMode; /*!< Select the detection mode. See "tsc_detection_mode_t". */
} tsc_config_t;

/*******************************************************************************
* API
******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
* @brief Initialize the TSC module.
*
* @param base TSC peripheral base address.
* @param config Pointer to "tsc_config_t" structure.
*/
void TSC_Init(TSC_Type *base, const tsc_config_t *config);

/*!
* @brief De-initializes the TSC module.
*
* @param base TSC peripheral base address.
*/
void TSC_Deinit(TSC_Type *base);

/*!
* @brief Gets an available pre-defined settings for the controller's configuration.
*
* This function initializes the converter configuration structure with available settings.
* The default values of measureDelayTime and prechargeTime is tested on LCD8000-43T screen and work normally.
* The default values are:
* @code
*    config->enableAutoMeausre = false;
*    config->measureDelayTime = 0xFFFFU;
*    config->prechargeTime = 0xFFFFU;
*    config->detectionMode = kTSC_4WireDetectionMode;
* @endCode
* @param config Pointer to "tsc_config_t" structure.
*/
void TSC_GetDefaultConfig(tsc_config_t *config);

/*!
* @brief Make the TSC module return to idle status after finish the current state operation.
*        Application could check TSC status to confirm that the controller has return to idle status.
*
* @param base TSC peripheral base address.
*/
static inline void TSC_ReturnToIdleStatus(TSC_Type *base)
{
    /* TSC_FLOW_CONTROL_DISABLE_MASK is a HW self-clean bit. */
    base->FLOW_CONTROL |= TSC_FLOW_CONTROL_DISABLE_MASK;
}

/*!
* @brief Start sense detection and (if work in auto-measure mode) measure after detect a touch.
*
* @param base TSC peripheral base address.
*/
static inline void TSC_StartSenseDetection(TSC_Type *base)
{
    /* TSC_FLOW_CONTROL_START_SENSE_MASK is a HW self-clean bit. */
    base->FLOW_CONTROL |= TSC_FLOW_CONTROL_START_SENSE_MASK;
}

/*!
* @brief start measure X/Y coordinate value after detect a touch.
*
* @param base TSC peripheral base address.
*/
static inline void TSC_StartMeasure(TSC_Type *base)
{
    /* TSC_FLOW_CONTROL_START_MEASURE_MASK is a HW self-clean bit. */
    base->FLOW_CONTROL |= TSC_FLOW_CONTROL_START_MEASURE_MASK;
}

/*!
* @brief Drop measure X/Y coordinate value after detect a touch and controller return to idle status.
*
* @param base TSC peripheral base address.
*/
static inline void TSC_DropMeasure(TSC_Type *base)
{
    /* TSC_FLOW_CONTROL_DROP_MEASURE_MASK is a HW self-clean bit. */
    base->FLOW_CONTROL |= TSC_FLOW_CONTROL_DROP_MEASURE_MASK;
}

/*!
* @brief This is a synchronization reset, which resets every register except IPS directly access ones.
*
* @param base TSC peripheral base address.
*/
static inline void TSC_SoftwareReset(TSC_Type *base)
{
    /* TSC_FLOW_CONTROL_SW_RST_MASK is a HW self-clean bit. */
    base->FLOW_CONTROL |= TSC_FLOW_CONTROL_SW_RST_MASK;
}

/*!
* @brief Get Y coordinate value or X coordinate value. The value is an ADC conversion value.
*
* @param base TSC peripheral base address.
* @param selection Select alternative measure value which is Y coordinate value or X coordinate value.
*        See "tsc_corrdinate_value_selection_t".
* @return If selection is "kTSC_XCoordinateValueSelection", the API returns x-coordinate vlaue.
*         If selection is "kTSC_YCoordinateValueSelection", the API returns y-coordinate vlaue.
*/
uint32_t TSC_GetMeasureValue(TSC_Type *base, tsc_corrdinate_value_selection_t selection);

/*!
* @brief Enable the interrupt signals. Interrupt signal will be set when corresponding event happens.
*        Specific events point to "_tsc_interrupt_signal_mask" .
*        Specific interrupt signal point to "_tsc_interrupt_status_flag_mask";
*
* @param base TSC peripheral base address.
* @param mask Interrupt signals mask. See "_tsc_interrupt_signal_mask".
*/
static inline void TSC_EnableInterruptSignals(TSC_Type *base, uint32_t mask)
{
    base->INT_SIG_EN |= mask;
}

/*!
* @brief Disable the interrupt signals. Interrupt signal will be set when corresponding event happens.
*        Specific events point to "_tsc_interrupt_signal_mask".
*        Specific interrupt signal point to "_tsc_interrupt_status_flag_mask";
*
* @param base TSC peripheral base address.
* @param mask Interrupt signals mask. See "_tsc_interrupt_signal_mask".
*/
static inline void TSC_DisableInterruptSignals(TSC_Type *base, uint32_t mask)
{
    base->INT_SIG_EN &= ~mask;
}

/*!
* @brief Enable the interrupts. Notice: Only interrupts and signals are all enabled, interrupts
*        could work normally.
*
* @param base TSC peripheral base address.
* @param mask Interrupts mask. See "_tsc_interrupt_mask".
*/
static inline void TSC_EnableInterrupts(TSC_Type *base, uint32_t mask)
{
    base->INT_EN |= mask;
}

/*!
* @brief Disable the interrupts.
*
* @param base TSC peripheral base address.
* @param mask Interrupts mask. See "_tsc_interrupt_mask".
*/
static inline void TSC_DisableInterrupts(TSC_Type *base, uint32_t mask)
{
    base->INT_EN &= ~mask;
}

/*!
* @brief Get interrupt status flags. Interrupt status falgs are valid when corresponding
*        interrupt signals are enabled.
*
* @param base TSC peripheral base address.
* @return Status flags asserted mask. See "_tsc_interrupt_status_flag_mask".
*/
static inline uint32_t TSC_GetInterruptStatusFlags(TSC_Type *base)
{
    return base->INT_STATUS;
}

/*!
* @brief Clear interrupt status flags. Interrupt status falgs are valid when corresponding
*        interrupt signals are enabled.
*
* @param base TSC peripheral base address.
* @param mask Status flags mask. See "_tsc_interrupt_status_flag_mask".
*/
static inline void TSC_ClearInterruptStatusFlags(TSC_Type *base, uint32_t mask)
{
    base->INT_STATUS = mask;
}

/*!
* @brief Get the status flags of ADC working with TSC.
*
* @param base TSC peripheral base address.
* @return Status flags asserted mask. See "_tsc_adc_status_flag_mask".
*/
static inline uint32_t TSC_GetADCStatusFlags(TSC_Type *base)
{
    return base->DEBUG_MODE;
}

/*!
* @brief Get the status flags of TSC.
*
* @param base TSC peripheral base address.
* @return Status flags asserted mask. See "_tsc_status_flag_mask".
*/
static inline uint32_t TSC_GetStatusFlags(TSC_Type *base)
{
    return base->DEBUG_MODE2;
}

/*!
 *@}
 */

/*******************************************************************************
 * Debug API
 ******************************************************************************/

/*!
* @brief Enable/Disable debug mode. Once work in debug mode, then all
* TSC outputs will be controlled by software. Software can also observe all TSC inputs
* through debug interface. Furthermore, the debug registers also provides current state
* machine states. Software can always check the current hardware state.
*
* @param base TSC peripheral base address.
* @param enable Switcher of the debug mode. "true" means debug mode,"false" means non-debug mode.
*/
static inline void TSC_EnableDebugMode(TSC_Type *base, bool enable)
{
    if (enable)
    {
        base->DEBUG_MODE |= TSC_DEBUG_MODE_DEBUG_EN_MASK;
    }
    else
    {
        base->DEBUG_MODE &= ~TSC_DEBUG_MODE_DEBUG_EN_MASK;
    }
}

/*!
* @brief Send hardware trigger signal to ADC in debug mode. The trigger signal must last at least 1 ips clock period.
*
* @param base TSC peripheral base address.
* @param hwts Hardware trigger select signal, select which channel to start conversion. See "tsc_trigger_signal_t".
*             On ADC side, HWTS = 1 << x indicates the x logic channel is selected to start hardware ADC conversion.
* @param enable Switcher of the trigger signal. "true" means generate trigger signal, "false" means don't generate
*               trigger signal.
*/
void TSC_DebugTriggerSignalToADC(TSC_Type *base, tsc_trigger_signal_t hwts, bool enable);

/*!
* @brief Enable/Disable hardware generates an ADC COCO clear signal in debug mode.
*
* @param base TSC peripheral base address.
* @param enable Switcher of the function of hardware generating an ADC COCO clear signal.
*        "true" means prevent TSC from generate ADC COCO clear signal.
*        "false" means allow TSC hardware generates ADC COCO clear.
*/
static inline void TSC_DebugDisableHWClear(TSC_Type *base, bool enable)
{
    if (enable)
    {
        base->DEBUG_MODE |= TSC_DEBUG_MODE_ADC_COCO_CLEAR_DISABLE_MASK;
    }
    else
    {
        base->DEBUG_MODE &= ~TSC_DEBUG_MODE_ADC_COCO_CLEAR_DISABLE_MASK;
    }
}

/*!
* @brief Send clear ADC COCO signal to ADC in debug mode. The signal must hold a while.
*
* @param base TSC peripheral base address.
* @param enable Switcher of the clear signal."true" means generate clear signal, "false" means don't generate
*               clear signal.
*/
static inline void TSC_DebugClearSignalToADC(TSC_Type *base, bool enable)
{
    if (enable)
    {
        base->DEBUG_MODE |= TSC_DEBUG_MODE_ADC_COCO_CLEAR_MASK;
    }
    else
    {
        base->DEBUG_MODE &= ~TSC_DEBUG_MODE_ADC_COCO_CLEAR_MASK;
    }
}

/*!
* @brief Enable/Disable detection in debug mode.
*
* @param base TSC peripheral base address.
* @param detectionMode Set detect mode. See "tsc_detection_mode_t"
* @param enable Switcher of detect enable. "true" means enable detection, "false" means disable detection.
*/
void TSC_DebugEnableDetection(TSC_Type *base, tsc_detection_mode_t detectionMode, bool enable);

/*!
* @brief Set TSC port mode in debug mode.(pull down, pull up and 200k-pull up)
*
* @param base TSC peripheral base address.
* @param port TSC controller ports.
* @param mode TSC port mode.(pull down, pull up and 200k-pull up)
*/
void TSC_DebugSetPortMode(TSC_Type *base, tsc_port_source_t port, tsc_port_mode_t mode);
#endif /* _FSL_TSC_H_ */
