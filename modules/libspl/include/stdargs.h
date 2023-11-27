/*===---- stdarg.h - Variable argument handling ----------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 * This file has been modified by the author of this repository as a course 
 * project.
 */

#ifndef __STDARG_H
#define __STDARG_H

#ifndef _VA_LIST
typedef __builtin_va_list va_list;
#define _VA_LIST
#endif

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)

#endif /* __STDARG_H */
