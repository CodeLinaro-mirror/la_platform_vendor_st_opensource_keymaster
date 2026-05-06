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

#ifndef _WEAVER_PARSER_H_
#define _WEAVER_PARSER_H_

#include <weaver_common.h>

class WeaverParser {
public:
  /**
   * \brief virtual Function to Frame weaver applet request command for getSlots
   *
   * \param[out]    request - framed getslots command as vector
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual bool FrameGetSlotCmd(std::vector<uint8_t> &request) = 0;

  /**
   * \brief virtual Function to Frame weaver applet request command for open
   *
   * \param[out]    request - framed open command as vector
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual bool FrameOpenCmd(std::vector<uint8_t> &request) = 0;

  /**
   * \brief virtual Function to Frame weaver applet request command for read
   *
   * \param[in]     slotId  - input slotId to be used in read request.
   * \param[in]     key     - input key to be used in read request.
   * \param[out]    request - framed read command as vector
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual bool FrameReadCmd(uint32_t slotId, const std::vector<uint8_t> &key,
                            std::vector<uint8_t> &request) = 0;

  /**
   * \brief virtual Function to Frame weaver applet request command for write
   *
   * \param[in]     slotId  - input slotId to be used in write request.
   * \param[in]     key     - input key to be used in write request.
   * \param[in]     value   - input value to be used in write request.
   * \param[out]    request - framed write command as vector
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual bool FrameWriteCmd(uint32_t slotId, const std::vector<uint8_t> &key,
                             const std::vector<uint8_t> &value,
                             std::vector<uint8_t> &request) = 0;
                             
/**
 * \brief virtual Function to Frame weaver applet request command to get current throttling period
 *
 * \param[in]    slotId -  input slotId to query
 * \param[out]   request - framed getTimeout command as vector
 *
 * \retval This function return true in case of success
 *         In case of failure returns false.
 */                           
  virtual bool FrameGetTimeoutCmd(uint32_t slotId, std::vector<uint8_t> &request);
  
/**
 * \brief virtual Function to Parse getTimeout response
 *
 * \param[in]    slotId -  input slotId to query
 * \param[out]   timeoutMs  - parsed timeout Information read out from applet
 * response.
 *
 * \retval This function return true in case of success
 *         In case of failure returns false.
 */  
  virtual bool ParseGetTimeoutResponse(const std::vector<uint8_t> &response, uint64_t &timeoutMs);

  /**
   * \brief virtual Function to Parse getSlots response
   *
   * \param[in]     response  - response from applet.
   * \param[out]    slotInfo  - parsed slots Information read out from applet
   * response.
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual Status_Weaver ParseSlotInfo(std::vector<uint8_t> response,
                                      SlotInfo &slotInfo) = 0;

  /**
   * \brief virtual Function to Parse read response
   *
   * \param[in]     response  - response from applet.
   * \param[out]    readInfo  - parsed read Information read out from applet
   * response.
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual Status_Weaver ParseReadInfo(std::vector<uint8_t> response,
                                      ReadRespInfo &readInfo) = 0;

  /**
   * \brief virtual Function to check if response from applet is Success or not
   *
   * \param[in]     response  - response from applet.
   *
   * \retval This function return true if response code from applet is success
   *         and false in other cases.
   */
  virtual bool isSuccess(std::vector<uint8_t> response) = 0;

  /**
   * \brief virtual Function to get Weaver Applet ID
   *
   * \param[out]    aid  - applet id of the weaver applet.
   *
   * \retval This function return true in case of success
   *         In case of failure returns false.
   */
  virtual bool getAppletId(std::vector<uint8_t> &aid) = 0;
  
  /**
 * \brief Virtual function to frame GetMaxRemainingTime command
 *
 * This function constructs the request buffer (APDU/command) used to query
 * the maximum remaining throttling time from the Weaver applet.
 *
 * \param[out]    request  - Buffer where the framed command will be stored.
 *
 * \retval This function return Weaver_STATUS_OK (0) in case of success
 *         In case of failure returns other Status_Weaver.
 */
virtual Status_Weaver FrameGetMaxRemainingTimeCmd(std::vector<uint8_t> &request) = 0;

/**
 * \brief Virtual function to parse GetMaxRemainingTime response
 *
 * This function parses the response received from the Weaver applet and
 * extracts the remaining throttling timeout value.
 *
 * \param[in]     response   - Raw response buffer received from the applet.
 * \param[out]    timeoutMs  - Parsed remaining throttling time in milliseconds.
 *
 * \retval This function return Weaver_STATUS_OK (0) in case of success
 *         In case of failure returns other Status_Weaver.
 */
virtual Status_Weaver ParseMaxRemainingTimeResponse(
        const std::vector<uint8_t> &response,
        int64_t &timeoutMs) = 0;

  /**
   * \brief virtual destructor for weaver parser
   */
  virtual ~WeaverParser(){};
};
#endif /* _WEAVER_PARSER_H_ */
