/*
 * Copyright (c) 2017. All rights reserved.
 *
 * Secure C functions for: sprintf, strcpy, memset, memmove
 */
#ifndef INCLUDE_MY_SECUREC_H_
#define INCLUDE_MY_SECUREC_H_

#ifdef SECURE_C
#include "securec.h"
#else
#include <stdio.h>
#include <string.h>
#endif

// **** NOTE ****: this header needs to be included as the first #include
//                 in a source file.

typedef void(*my_securec_print_error_hook)(const char *format,...);

/**
 * The wrapper for sprintf, to use the secure version of it if have
 *
 * Parameters:
 *    strDest - [In/Out] Destination buffer.
 *    destMax - [In] Size of the destination buffer.
 *    format - [In] Format-control string.
 *
 * Return: the number of stored characters, or negative value for errors
 */
inline
int my_sprintf(char* strDest, size_t destMax, const char* format, ...)
    __attribute__ ((format (printf, 3, 4)));

int my_sprintf(char* strDest, size_t destMax, const char* format, ...)
{
  int rc = 0;
  va_list args;
  va_start(args, format);

#ifdef SECURE_C
  rc = vsprintf_s(strDest, destMax, format, args);
  /* It returns the number of stored characters on success, or -1 on errors:
   * - strDest is NULL or destMax is 0: return -1 and not modified strDest
   * - other errors include incorrect destMax: return -1 and strDest[0] = ‘\0’ */
#else
  /* For now throw a compile error if secure C lib is not used */
#error "Error: Secure C library must be used!"
  /* Call the corresponding non-secure lib function */
  rc = vsprintf(strDest, format, args);
#endif
  va_end(args);

  return rc;
}

/**
 * The strcpy wrapper: Use the secure version strcpy_s if have, otherwise, use
 * the non-secure version.
 *
 * Parameters:
 *   strDest - [In/Out] Destination buffer.
 *   destMax - [In] Size of the destination buffer.
 *   strSrc - [In] Source buffer
 *   print_sec_err - The function to print detailed error message to error log.
 *                   Can be NULL: then not print any error message
 *
 * Return: NULL - If strDest is NULL or error happens inside
 *         not NULL - the strDest
 *
 * Note: caller should check if return value is NULL and properly handle the
 *       error (may also need to check if strDest is NULL)
 */
inline
char* my_strcpy(char* strDest, size_t destMax, const char* strSrc,
                my_securec_print_error_hook print_sec_err)
{
  char *retStr = strDest;

#ifdef SECURE_C
  int rc = strcpy_s(strDest, destMax, strSrc);
  if (rc != EOK) {
    if (print_sec_err != NULL) {
      print_sec_err("Security violation from my_strcpy() function argument! "
                    "errno=%d, strDest:%p, destMax:%zu, strSrc:%p\n",
                    rc, strDest, destMax, strSrc);
    }
    retStr = NULL; /* The caller should check the return value and the strDest */
  }
#else
  // For now throw a compile error if secure C lib is not used
#error "Error: Secure C library must be used!"

  // Call the corresponding non-secure lib function
  retStr = strcpy(strDest, strSrc);
#endif

  return retStr;
}

/**
 * The memset wrapper: Use the secure version memset_s if have, otherwise, use
 * the non-secure version.
 *
 * Parameters:
 *   dest - [In/Out] Destination buffer.
 *   destMax - [In] Size of the destination buffer.
 *   c - [In] Destination buffer characters to be specified.
 *   count - [In] Number of destination buffer characters to be specified.
 *   print_sec_err - The function to print detailed error message to error log.
 *                   Can be NULL: then not print any error message
 *
 * Return: NULL - If dest is NULL or error happens inside
 *         not NULL - the strDest
 *
 * Note: caller should check if return value is NULL and properly handle the
 *       error (may also need to check if strDest is NULL)
 */
inline
void* my_memset(void* dest, size_t destMax, int c, size_t count,
                my_securec_print_error_hook print_sec_err)
{
  void *retPtr = dest;
  if (0 == count) {
    return retPtr;
  }

#ifdef SECURE_C
  int rc = EOK;

  rc = memset_s(dest, destMax, c, count);

  if (rc != EOK) {
    if (print_sec_err != NULL) {
      print_sec_err("Security violation from my_memset() function argument! "
                    "errno=%d, dest:%p, destMax:%lu, c:%d, count:%lu\n",
                    rc, dest, destMax, c, count);
    }
    retPtr = NULL;
  }
#else
  // For now throw a compile error if secure C lib is not used
  #error "Error: Secure C library must be used!"

  // destMax parameter is ignoread for the insecure system memset()
  ::memset(dest, c, count);
#endif

  // Follow what the system memset() does -- returns a pointer to the memory area
  return retPtr;
}

/**
 * The memmove wrapper: Use the secure version memmove_s if have, otherwise, use
 * the non-secure version.
 *
 * Parameters:
 *     dest - [In/Out] Destination buffer.
 *     destMax - [In] Size of the destination buffer.
 *     src - [In] Source buffer
 *     count - [In] Number of destination buffer characters to be specified.
 *     print_sec_err - The function to print detailed error message to error log.
 *                     Can be NULL: then not print any error message
 *
 * Return: NULL - If dest is NULL or error happens inside
 *         not NULL - the strDest
 *
 * Note: caller should check if return value is NULL and properly handle the
 *       error (may also need to check if strDest is NULL)
 */
inline
void* my_memmove(void* dest, size_t destMax, const void* src, size_t count,
                 my_securec_print_error_hook print_sec_err)
{
  void *retPtr = dest;
  /*
    This check is needed since memmove_s will fail even if both
    destMax and count are 0, which is actually not a violation
    because it's a no-op.
   */
  if (0 == count) {
      return retPtr;
  }

#ifdef SECURE_C
  int rc = EOK;

  rc = memmove_s(dest, destMax, src, count);
  if (rc != EOK) {
    if (print_sec_err != NULL) {
      print_sec_err("Security violation from my_memmove() function argument! "
                    "errno=%d, dest:%p, destMax:%lu, src:%p, count:%lu\n",
                    rc, dest, destMax, src, count);
    }
    retPtr = NULL;
  }
#else
  // For now throw a compile error if secure C lib is not used
  #error "Error: Secure C library must be used!"

  // destMax parameter is ignoread for the insecure system memmove()
  ::memmove(dest, src, count);
#endif

  // Follow what the system memset() does -- returns a pointer to dest
  return retPtr;
}

#endif /* INCLUDE_MY_SECUREC_H_ */
