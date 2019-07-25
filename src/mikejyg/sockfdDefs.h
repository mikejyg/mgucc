/*
 * sockfdDefs.h
 *
 *      Author: mikejyg
 *      
 * to accomodate the different types of sockfd between Windows and Linux.
 */

#ifndef MIKEJYG_SOCKFDDEFS_H_
#define MIKEJYG_SOCKFDDEFS_H_

#include <cstdint>

#ifdef _WIN32
typedef uint64_t SockFdType;
#else
typedef int SockFdType;
#define INVALID_SOCKET -1
#endif

#endif /* MIKEJYG_SOCKFDDEFS_H_ */
