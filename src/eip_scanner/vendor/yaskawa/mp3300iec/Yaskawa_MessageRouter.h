#ifndef EIPSCANNER_YASKAWA_MESSAGEROUTER_H
#define EIPSCANNER_YASKAWA_MESSAGEROUTER_H

#include <memory>
#include "eip_scanner/vendor/yaskawa/mp3300iec/Yaskawa_EPath.h"
#include "eip_scanner/cip/Services.h"
#include "eip_scanner/cip/MessageRouterResponse.h"
#include "eip_scanner/eip/CommonPacketItem.h"
#include "eip_scanner/SessionInfo.h"

namespace eip_scanner {
	/**
	* @class MessageRouter
	*
	* @brief Implements the explicit messaging with EIP adapter
	*/
	class Yaskawa_MessageRouter {
	public:
		using SPtr = std::shared_ptr<Yaskawa_MessageRouter>;

		/**
		 * @brief Default constructor
		 */
		Yaskawa_MessageRouter();

		/**
		 * @brief Default destructor
		 */
		virtual ~Yaskawa_MessageRouter();

		/**
		 * @brief Sends an explicit requests to the EIP adapter by calling a CIP service
		 * @param si the EIP session with the adapter
		 * @param service the service code (for standard codes see eip_scanner::cip::ServiceCodes)
		 * @param path the path to an element in Object Model that provides the called service
		 * @param data the encoded arguments of the service
		 * @param additionalPacketItems (needed only for eip_scanner::ConnectionManager)
		 * @return the received response from the EIP adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::Yaskawa_EPath& path, const std::vector<uint8_t>& data,
				const std::vector<eip::CommonPacketItem>& additionalPacketItems) const;

		/**
		 * @brief Sends an explicit requests to the EIP adapter by calling a CIP service
		 * @param si the EIP session with the adapter
		 * @param service the service code (for standard codes see eip_scanner::cip::ServiceCodes)
		 * @param path the path to an element in Object Model that provides the called service
		 * @param data the encoded arguments of the service
		 * @return the received response from the EIP adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::Yaskawa_EPath& path, const std::vector<uint8_t>& data) const;

		/**
		 * @brief Sends an explicit requests to the EIP adapter by calling a CIP service
		 * @param si the EIP session with the adapter
		 * @param service the service code (for standard codes see eip_scanner::cip::ServiceCodes)
		 * @param path the path to an element in Object Model that provides the called service
		 * @return the received response from the EIP adapter
		 * @throw std::runtime_error
		 * @throw std::system_error
		 */
		virtual cip::MessageRouterResponse sendRequest(SessionInfoIf::SPtr si, cip::CipUsint service,
				const cip::Yaskawa_EPath& path) const;

	};
}


#endif  // EIPSCANNER_YASKAWA_MESSAGEROUTER_H
