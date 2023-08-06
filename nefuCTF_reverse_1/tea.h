#pragma once

#define CBC 1
#define ECB 2

#define CIPHER TEA
#define CIPHER_MODE ECB

#ifdef CIPHER_MODE
#if CIPHER_MODE == CBC
#define CIPHER_NAME CBC_Mode
#define IV
#elif CIPHER_MODE == ECB
#define CIPHER_NAME ECB_Mode
#undef IV
#endif
#endif