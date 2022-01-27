/**
 * @file atomic_lock_free.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-25
 *
 * @copyright Copyright (c) 2022
 */

// lock-free: the standard does not guarantee lock-less atomicity for types of
// any size.

// 16-Byte CAS support： only some very old machines not have the 16-byte CAS
// support, such like ARM 6, old AMDs and Intel M Series。

// eg..__uint128_t, the standard does not define a 128-bit integer type. It's an
// extension that some compilers provide and others don't.