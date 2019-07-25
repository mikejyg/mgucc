/*
 * WsaService.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_WSASERVICE_H_
#define MIKEJYG_WSASERVICE_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#include <stdexcept>

/**
 * for Initializing Winsock
 */
class WsaService {
protected:
	WSADATA wsaData;

	WsaService() {
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
		    throw std::runtime_error( "WSAStartup failed: " + std::to_string(iResult) );
		}
	}

public:

	static WsaService & getInstance() {
		static WsaService wsaService;
		return wsaService;
	}

};

#endif	// _WIN32

#endif /* MIKEJYG_WSASERVICE_H_ */
