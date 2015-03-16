/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#ifndef __AIRT_ALGO_END_POINT_READ_H__
#define __AIRT_ALGO_END_POINT_READ_H__

#include <drain/EndPoint.h>

namespace drain{
	class EndPointRead : public EndPoint {
		protected:
			/**
			 * @brief Constructor
			 */
			EndPointRead();
			void init();
		public:
			static std11::shared_ptr<EndPointRead> create();
			/**
			 * @brief Destructor
			 */
			virtual ~EndPointRead() {};
			virtual void configurationChange();
			virtual bool process(std11::chrono::system_clock::time_point& _time,
			                     void* _input,
			                     size_t _inputNbChunk,
			                     void*& _output,
			                     size_t& _outputNbChunk);
			/**
			 * @brief Set buffer size in chunk number
			 * @param[in] _nbChunk Number of chunk in the buffer
			 */
			virtual void setBufferSize(size_t _nbChunk);
			/**
			 * @brief Set buffer size size of the buffer with the stored time in �s
			 * @param[in] _time Time in microsecond of the buffer
			 */
			virtual void setBufferSize(const std11::chrono::microseconds& _time);
			/**
			 * @brief get buffer size in chunk number
			 * @return Number of chunk that can be written in the buffer
			 */
			virtual size_t getBufferSize();
			/**
			 * @brief Set buffer size size of the buffer with the stored time in �s
			 * @return Time in microsecond that can be written in the buffer
			 */
			virtual std11::chrono::microseconds getBufferSizeMicrosecond();
			/**
			 * @brief Get buffer size filled in chunk number
			 * @return Number of chunk in the buffer (that might be read/write)
			 */
			virtual size_t getBufferFillSize();
			/**
			 * @brief Set buffer size size of the buffer with the stored time in �s
			 * @return Time in microsecond of the buffer (that might be read/write)
			 */
			virtual std11::chrono::microseconds getBufferFillSizeMicrosecond();
	};
};

#endif
