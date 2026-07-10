/******************************************************************************
 *
 *  Copyright 2020 NXP
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
  /******************************************************************************
 **
 ** The original Work has been changed by THALES.
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 ** http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 **
 ** Copyright ©2023-2024 THALES. All rights Reserved.
 **
 *********************************************************************************/

#ifndef _WEAVER_IMPL_H_
#define _WEAVER_IMPL_H_

#include <mutex>
#include <weaver_interface.h>
#include <weaver_parser.h>
#include <weaver_transport.h>
#include "EseWakeLockManager.h"

class WeaverImpl : public WeaverInterface {
public:
  /**
   * \brief Function to initilize Weaver Interface
   *
   * \retval This function return Weaver_STATUS_OK (0) in case of success
   *         In case of failure returns other Status_Weaver.
   */
  Status_Weaver Init() override;

  /**
   * \brief Function to read slot information
   * \param[out]   slotInfo - slot information values read out
   *
   * \retval This function return Weaver_STATUS_OK (0) in case of success
   *         In case of failure returns other Status_Weaver errorcodes.
   */
  Status_Weaver GetSlots(SlotInfo &slotInfo) override;

  /**
   * \brief Function to read value of specific key & slotId
   * \param[in]    slotId -       input slotId which's information to be read
   * \param[in]    key -          input key which's information to be read
   * \param[out]   readRespInfo - read information values to be read out
   *
   * \retval This function return Weaver_STATUS_OK (0) in case of success
   *         In case of failure returns other Status_Weaver errorcodes.
   */
  Status_Weaver Read(uint32_t slotId, const std::vector<uint8_t> &key,
                     ReadRespInfo &readRespInfo) override;

  /**
   * \brief Function to write value to specific key & slotId
   * \param[in]    slotId -       input slotId where value to be write
   * \param[in]    key -          input key where value to be write
   * \param[in]   value -        input value which will be written
   *
   * \retval This function return Weaver_STATUS_OK (0) in case of success
   *         In case of failure returns other Status_Weaver.
   */
  Status_Weaver Write(uint32_t slotId, const std::vector<uint8_t> &key,
                      const std::vector<uint8_t> &value) override;
                      
 /**
  * \brief Function to warm up the eSE
  */
  Status_Weaver WarmUp() override;

/**
 * \brief Function to get current throttling period
 * \param[in]    slotId -     input slotId to query
 * \param[out]   timeout -      returned timeout value in milliseconds
 *
 * \retval This function return Weaver_STATUS_OK (0) in case of success
 *         In case of failure returns other Status_Weaver.
 */
 Status_Weaver GetTimeout(uint32_t slotId, uint64_t &timeout) override;  
 
 /**
 * \brief Retrieve maximum remaining throttling time from Weaver applet
 *
 * This function queries the Weaver applet to obtain the maximum remaining
 * throttling timeout. The returned value indicates how long the device
 * remains in a throttled state.
 *
 * \param[out]    remainingTime  - Remaining throttling time in milliseconds.
 *
 * \retval WEAVER_STATUS_OK           In case of successful retrieval.
 * \retval Other Status_Weaver        In case of communication or parsing failure.
 */
Status_Weaver GetMaxRemainingTime(int64_t &remainingTime);

/**
 * \brief Maintain eSE active state using wake lock for a specified duration
 *
 * This function ensures that the embedded Secure Element (eSE) remains active
 * during the throttling period by acquiring a wake lock for the specified
 * duration. If a wake lock is already held, the active duration may be extended.
 *
 * \param[in]    durationMs  - Duration in milliseconds to keep the eSE active.
 *
 * \retval This function does not return a value.
 */
void MaintainESEActive(int64_t durationMs);

  /**
   * \brief Function to de-initilize Weaver Interface
   *
   * \retval This function return Weaver_STATUS_OK (0) in case of success
   *         In case of failure returns other Status_Weaver.
   */
  Status_Weaver DeInit() override;

  /**
   * \brief static function to get the singleton instance of WeaverImpl class
   *
   * \retval instance of WeaverImpl.
   */
  static WeaverImpl *getInstance();

private:
  /* Transport interface to be use for communication */
  WeaverTransport *mTransport;
  /* Parser interface to frame weaver commands and parse response*/
  WeaverParser *mParser;
  /* Internal close api for transport close */
  bool close();
  /* Private constructor to make class singleton*/
  WeaverImpl() = default;
  /* Private destructor to make class singleton*/
  ~WeaverImpl() = default;
  /* Private copy constructor to make class singleton*/
  WeaverImpl(const WeaverImpl &) = delete;
  /* Private operator overload to make class singleton*/
  WeaverImpl &operator=(const WeaverImpl &) = delete;

  /* Private self instance for singleton purpose*/
  static WeaverImpl *s_instance;

  /* Private once flag (c++11) for singleton purpose.
   * once_flag should pass to multiple calls of
   * std::call_once allows those calls to coordinate with each other
   * such a way only one will actually run to completion.
   */
  static std::once_flag s_instanceFlag;

  /* Private function to create the instance of self class
   * Same will be used for std::call_once
   */
  static void createInstance();
  /**
   * \brief eSE wake lock manager instance
   *
   * This member manages the lifecycle of the wake lock used to keep the
   * embedded Secure Element (eSE) active during throttling periods.
   * It handles acquisition, extension, and release of the wake lock
   * based on remaining timeout.
   */
  EseWakeLockManager mEseWakeLockManager;
};

#endif /* _WEAVER_IMPL_H_ */
