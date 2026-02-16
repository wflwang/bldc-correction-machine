//------------------------------------------------------------------------------
//
// Copyright (c) 2017 IAR Systems
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//------------------------------------------------------------------------------


#ifndef __CMSIS_IAR_H__
#define __CMSIS_IAR_H__

#ifndef __ICCARM__
  #error This file should only be compiled by ICCARM
#endif

#if (__VER__ < 8020000)
  #error This file should only be compiled by ICCARM 8.20 or later
#endif

#if defined(__CM_CMSIS_VERSION) && (__CM_CMSIS_VERSION >= 0x50001)
#pragma message("\nWarning: The header file 'cmsis_iar.h' is obsolete and should not be used.")
#pragma message("         A sutiable version is automatically included from the CMSIS-core package.")
#pragma message("         This file will be removed in a future release.")
#endif

#pragma system_include

#define __IAR_FT _Pragma("inline=forced") __intrinsic

#ifndef __ALIGNED
  #define __ALIGNED(x) __attribute__((aligned(x)))
#endif

/* Define compiler macros for CPU architecture, used in CMSIS 5.
 */
#if __ARM_ARCH_6M__ || __ARM_ARCH_7M__ || __ARM_ARCH_7EM__ || __ARM_ARCH_8M_BASE__ || __ARM_ARCH_8M_BASE__
/* Macros already defined */
#else
  #if defined(__ARM8M_MAINLINE__) || defined(__ARM8EM_MAINLINE__)
    #define __ARM_ARCH_8M_MAIN__ 1
  #elif defined(__ARM8M_BASELINE__)
    #define __ARM_ARCH_8M_BASE__ 1
  #elif defined(__ARM_ARCH_PROFILE) && __ARM_ARCH_PROFILE == 'M'
    #if __ARM_ARCH == 6
      #define __ARM_ARCH_6M__ 1
    #elif __ARM_ARCH == 7
      #if __ARM_FEATURE_DSP
        #define __ARM_ARCH_7EM__ 1
      #else
        #define __ARM_ARCH_7M__ 1
      #endif
    #endif /* __ARM_ARCH */
  #endif /* __ARM_ARCH_PROFILE == 'M' */
#endif


#if defined(__ARM_ARCH_6M__) && __ARM_ARCH_6M__==1
  #define __IAR_M0_FAMILY  1
#elif defined(__ARM_ARCH_8M_BASE__) && __ARM_ARCH_8M_BASE__==1
  #define __IAR_M0_FAMILY  1
#else
  #define __IAR_M0_FAMILY  0
#endif

#ifndef __ASM
  #define __ASM __asm
#endif

#ifndef __INLINE
  #define __INLINE inline
#endif

#ifndef   __NO_RETURN
  #define __NO_RETURN __attribute__((noreturn))
#endif

#ifndef   __PACKED
  #define __PACKED __attribute__((packed, aligned(1)))
#endif

#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT struct __attribute__((packed, aligned(1)))
#endif

#ifndef   __PACKED_UNION
  #define __PACKED_UNION union __attribute__((packed, aligned(1)))
#endif

#ifndef   __RESTRICT
  #define __RESTRICT restrict
#endif

#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE static inline
#endif

#ifndef __UNALIGNED_UINT16_READ
  #pragma language=save
  #pragma language=extended
  __IAR_FT uint16_t __iar_uint16_read(void const *ptr) {
    return *(__packed uint16_t*)(ptr);
  }
  #pragma language=restore
  #define __UNALIGNED_UINT16_READ(PTR) __iar_uint16_read(PTR)
#endif


#ifndef __UNALIGNED_UINT16_WRITE
  #pragma language=save
  #pragma language=extended
  __IAR_FT void __iar_uint16_write(void const *ptr, uint16_t val) {
    *(__packed uint16_t*)(ptr) = val;;
  }
  #pragma language=restore
  #define __UNALIGNED_UINT16_WRITE(PTR,VAL) __iar_uint16_write(PTR,VAL)
#endif

#ifndef __UNALIGNED_UINT32_READ
  #pragma language=save
  #pragma language=extended
  __IAR_FT uint32_t __iar_uint32_read(void const *ptr) {
    return *(__packed uint32_t*)(ptr);
  }
  #pragma language=restore
  #define __UNALIGNED_UINT32_READ(PTR) __iar_uint32_read(PTR)
#endif

#ifndef __UNALIGNED_UINT32_WRITE
  #pragma language=save
  #pragma language=extended
  __IAR_FT void __iar_uint32_write(void const *ptr, uint32_t val) {
    *(__packed uint32_t*)(ptr) = val;;
  }
  #pragma language=restore
  #define __UNALIGNED_UINT32_WRITE(PTR,VAL) __iar_uint32_write(PTR,VAL)
#endif

#ifndef __UNALIGNED_UINT32   /* deprecated */
  #pragma language=save
  #pragma language=extended
  __packed struct  __iar_u32 { uint32_t v; };
  #pragma language=restore
  #define __UNALIGNED_UINT32(PTR) (((struct __iar_u32 *)(PTR))->v)
#endif

#ifndef   __USED
  #define __USED __attribute__((used))
#endif

#ifndef   __WEAK
  #define __WEAK __attribute__((weak))
#endif


#if defined(__CLZ)
  #undef __CLZ
#endif
#if defined(__REVSH)
  #undef __REVSH
#endif
#if defined(__RBIT)
  #undef __RBIT
#endif
#if defined(__SSAT)
  #undef __SSAT
#endif
#if defined(__USAT)
  #undef __USAT
#endif

#include "iccarm_builtin.h"

#define __disable_fault_irq __iar_builtin_disable_fiq
#define __disable_irq       __iar_builtin_disable_interrupt
#define __enable_fault_irq  __iar_builtin_enable_fiq
#define __enable_irq        __iar_builtin_enable_interrupt
#define __arm_rsr 			    __iar_builtin_rsr
#define __arm_wsr 			    __iar_builtin_wsr


#define __get_APSR()                (__arm_rsr("APSR"))
#define __get_BASEPRI()             (__arm_rsr("BASEPRI"))
#define __get_CONTROL()             (__arm_rsr("CONTROL"))
#define __get_FAULTMASK()           (__arm_rsr("FAULTMASK"))

#if __FPU_PRESENT
  #define __get_FPSCR()             (__arm_rsr("FPSCR"))
#else
  #define __get_FPSCR()             ( 0 )
#endif

#define __get_IPSR()                (__arm_rsr("IPSR"))
#define __get_MSP()                 (__arm_rsr("MSP"))
#define __get_MSPLIM()              (__arm_rsr("MSPLIM"))
#define __get_PRIMASK()             (__arm_rsr("PRIMASK"))
#define __get_PSP()                 (__arm_rsr("PSP"))
#define __get_PSPLIM()              (__arm_rsr("PSPLIM"))
#define __get_xPSR()                (__arm_rsr("xPSR"))

#define __set_BASEPRI(VALUE)        (__arm_wsr("BASEPRI", (VALUE)))
#define __set_BASEPRI_MAX(VALUE)    (__arm_wsr("BASEPRI_MAX", (VALUE)))
#define __set_CONTROL(VALUE)        (__arm_wsr("CONTROL", (VALUE)))
#define __set_FAULTMASK(VALUE)      (__arm_wsr("FAULTMASK", (VALUE)))
#define __set_FPSCR(VALUE)          (__arm_wsr("FPSCR", (VALUE)))
#define __set_MSP(VALUE)            (__arm_wsr("MSP", (VALUE)))
#define __set_MSPLIM(VALUE)         (__arm_wsr("MSPLIM", (VALUE)))
#define __set_PRIMASK(VALUE)        (__arm_wsr("PRIMASK", (VALUE)))
#define __set_PSP(VALUE)            (__arm_wsr("PSP", (VALUE)))
#define __set_PSPLIM(VALUE)         (__arm_wsr("PSPLIM", (VALUE)))

#define __TZ_get_CONTROL_NS()       (__arm_rsr("CONTROL_NS"))
#define __TZ_set_CONTROL_NS(VALUE)  (__arm_wsr("CONTROL_NS", (VALUE)))
#define __TZ_get_PSP_NS()           (__arm_rsr("PSP_NS"))
#define __TZ_set_PSP_NS(VALUE)      (__arm_wsr("PSP_NS", (VALUE)))
#define __TZ_get_MSP_NS()           (__arm_rsr("MSP_NS"))
#define __TZ_set_MSP_NS(VALUE)      (__arm_wsr("MSP_NS", (VALUE)))
#define __TZ_get_SP_NS()            (__arm_rsr("SP_NS"))
#define __TZ_set_SP_NS(VALUE)       (__arm_wsr("SP_NS", (VALUE)))
#define __TZ_get_PRIMASK_NS()       (__arm_rsr("PRIMASK_NS"))
#define __TZ_set_PRIMASK_NS(VALUE)  (__arm_wsr("PRIMASK_NS", (VALUE)))
#define __TZ_get_BASEPRI_NS()       (__arm_rsr("BASEPRI_NS"))
#define __TZ_set_BASEPRI_NS(VALUE)  (__arm_wsr("BASEPRI_NS", (VALUE)))
#define __TZ_get_FAULTMASK_NS()     (__arm_rsr("FAULTMASK_NS"))
#define __TZ_set_FAULTMASK_NS(VALUE)(__arm_wsr("FAULTMASK_NS", (VALUE)))
#define __TZ_get_PSPLIM_NS()        (__arm_rsr("PSPLIM_NS"))
#define __TZ_set_PSPLIM_NS(VALUE)   (__arm_wsr("PSPLIM_NS", (VALUE)))
#define __TZ_get_MSPLIM_NS()        (__arm_rsr("MSPLIM_NS"))
#define __TZ_set_MSPLIM_NS(VALUE)   (__arm_wsr("MSPLIM_NS", (VALUE)))

#define __NOP    __iar_builtin_no_operation

#define __CLREX __iar_builtin_CLREX

//#if !__IAR_M0_FAMILY
  __IAR_FT uint8_t __CLZ(uint32_t val) {
	return __iar_builtin_CLZ(val);
  }
//#endif

#define __DMB   __iar_builtin_DMB
#define __DSB   __iar_builtin_DSB
#define __ISB   __iar_builtin_ISB

#define __LDREXB  __iar_builtin_LDREXB
#define __LDREXH  __iar_builtin_LDREXH
#define __LDREXW  __iar_builtin_LDREX

#define __RBIT    __iar_builtin_RBIT
#define __REV     __iar_builtin_REV
#define __REV16   __iar_builtin_REV16

__IAR_FT int32_t __REVSH(int32_t val) {
  return __iar_builtin_REVSH((int16_t)val);
}

#define __ROR     __iar_builtin_ROR
#define __RRX     __iar_builtin_RRX

#define __SEV     __iar_builtin_SEV

#if !__IAR_M0_FAMILY
  #define __SSAT    __iar_builtin_SSAT
#endif

#define __STREXB  __iar_builtin_STREXB
#define __STREXH  __iar_builtin_STREXH
#define __STREXW  __iar_builtin_STREX

#if !__IAR_M0_FAMILY
  #define __USAT    __iar_builtin_USAT
#endif

#define __WFE     __iar_builtin_WFE
#define __WFI     __iar_builtin_WFI

#if __ARM_MEDIA__
  #define __SADD8   __iar_builtin_SADD8
  #define __QADD8   __iar_builtin_QADD8
  #define __SHADD8  __iar_builtin_SHADD8
  #define __UADD8   __iar_builtin_UADD8
  #define __UQADD8  __iar_builtin_UQADD8
  #define __UHADD8  __iar_builtin_UHADD8
  #define __SSUB8   __iar_builtin_SSUB8
  #define __QSUB8   __iar_builtin_QSUB8
  #define __SHSUB8  __iar_builtin_SHSUB8
  #define __USUB8   __iar_builtin_USUB8
  #define __UQSUB8  __iar_builtin_UQSUB8
  #define __UHSUB8  __iar_builtin_UHSUB8
  #define __SADD16  __iar_builtin_SADD16
  #define __QADD16  __iar_builtin_QADD16
  #define __SHADD16 __iar_builtin_SHADD16
  #define __UADD16  __iar_builtin_UADD16
  #define __UQADD16 __iar_builtin_UQADD16
  #define __UHADD16 __iar_builtin_UHADD16
  #define __SSUB16  __iar_builtin_SSUB16
  #define __QSUB16  __iar_builtin_QSUB16
  #define __SHSUB16 __iar_builtin_SHSUB16
  #define __USUB16  __iar_builtin_USUB16
  #define __UQSUB16 __iar_builtin_UQSUB16
  #define __UHSUB16 __iar_builtin_UHSUB16
  #define __SASX    __iar_builtin_SASX
  #define __QASX    __iar_builtin_QASX
  #define __SHASX   __iar_builtin_SHASX
  #define __UASX    __iar_builtin_UASX
  #define __UQASX   __iar_builtin_UQASX
  #define __UHASX   __iar_builtin_UHASX
  #define __SSAX    __iar_builtin_SSAX
  #define __QSAX    __iar_builtin_QSAX
  #define __SHSAX   __iar_builtin_SHSAX
  #define __USAX    __iar_builtin_USAX
  #define __UQSAX   __iar_builtin_UQSAX
  #define __UHSAX   __iar_builtin_UHSAX
  #define __USAD8   __iar_builtin_USAD8
  #define __USADA8  __iar_builtin_USADA8
  #define __SSAT16  __iar_builtin_SSAT16
  #define __USAT16  __iar_builtin_USAT16
  #define __UXTB16  __iar_builtin_UXTB16
  #define __UXTAB16 __iar_builtin_UXTAB16
  #define __SXTB16  __iar_builtin_SXTB16
  #define __SXTAB16 __iar_builtin_SXTAB16
  #define __SMUAD   __iar_builtin_SMUAD
  #define __SMUADX  __iar_builtin_SMUADX
  #define __SMMLA   __iar_builtin_SMMLA
  #define __SMLAD   __iar_builtin_SMLAD
  #define __SMLADX  __iar_builtin_SMLADX
  #define __SMLALD  __iar_builtin_SMLALD
  #define __SMLALDX __iar_builtin_SMLALDX
  #define __SMUSD   __iar_builtin_SMUSD
  #define __SMUSDX  __iar_builtin_SMUSDX
  #define __SMLSD   __iar_builtin_SMLSD
  #define __SMLSDX  __iar_builtin_SMLSDX
  #define __SMLSLD  __iar_builtin_SMLSLD
  #define __SMLSLDX __iar_builtin_SMLSLDX
  #define __SEL     __iar_builtin_SEL
  #define __QADD    __iar_builtin_QADD
  #define __QSUB    __iar_builtin_QSUB
  #define __PKHBT   __iar_builtin_PKHBT
  #define __PKHTB   __iar_builtin_PKHTB
#endif

#define __BKPT(value)    __asm volatile ("BKPT     %0" : : "i"(value))

#if __IAR_M0_FAMILY

/*
  While there are definitions of these functions in arm_math.h
  these cannot be enabled.

  __IAR_FT uint8_t __CLZ(uint32_t val) {
    return __iar_builtin_CLZ(val);
  }


  __STATIC_INLINE int32_t __SSAT(int32_t val, uint32_t sat) {
    if ((sat >= 1U) && (sat <= 32U)) {
      const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
      const int32_t min = -1 - max ;
      if (val > max) {
        return max;
      } else if (val < min) {
        return min;
      }
    }
    return val;
  }

  __STATIC_INLINE int32_t __USAT(int32_t val, uint32_t sat) {
    if (sat <= 31U) {
      const int32_t max = (int32_t)((1U << sat) - 1U);
      const int32_t min = 0;
      if (val > max) {
        return max;
      } else if (val < min) {
        return min;
      }
    }
    return val;
  }

*/

#endif

#if (__CORTEX_M >= 0x03)   /* __CORTEX_M is defined in core_cm0.h, core_cm3.h and core_cm4.h. */

  __IAR_FT uint8_t __LDRBT(volatile uint8_t *addr) {
    uint32_t res;
    __ASM("LDRBT %0, [%1]" : "=r" (res) : "r" (addr) : "memory");
    return ((uint8_t)res);
  }

  __IAR_FT uint16_t __LDRHT(volatile uint16_t *addr) {
    uint32_t res;
    __ASM("LDRHT %0, [%1]" : "=r" (res) : "r" (addr) : "memory");
    return ((uint16_t)res);
  }

  __IAR_FT uint32_t __LDRT(volatile uint32_t *addr) {
    uint32_t res;
    __ASM("LDRT %0, [%1]" : "=r" (res) : "r" (addr) : "memory");
    return res;
  }

  __IAR_FT void __STRBT(uint8_t value, volatile uint8_t *addr) {
    __ASM("STRBT %1, [%0]" : : "r" (addr), "r" ((uint32_t)value) : "memory");
  }

  __IAR_FT void __STRHT(uint16_t value, volatile uint16_t *addr) {
    __ASM("STRHT %1, [%0]" : : "r" (addr), "r" ((uint32_t)value) : "memory");
  }

  __IAR_FT void __STRT(uint32_t value, volatile uint32_t *addr) {
    __ASM("STRT %1, [%0]" : : "r" (addr), "r" (value) : "memory");
  }

#endif /* (__CORTEX_M >= 0x03) */

#if ((defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1)) || \
     (defined (__ARM_ARCH_8M_BASE__ ) && (__ARM_ARCH_8M_BASE__ == 1))    )

  __IAR_FT uint8_t __LDAB(volatile uint8_t *ptr) {
    uint32_t res;
    __ASM volatile ("LDAB %0, [%1]" : "=r" (res) : "r" (*ptr) : "memory");
    return ((uint8_t)res);
  }

  __IAR_FT uint16_t __LDAH(volatile uint16_t *ptr) {
    uint32_t res;
    __ASM volatile ("LDAH %0, [%1]" : "=r" (res) : "r" (*ptr) : "memory");
    return ((uint16_t)res);
  }

  __IAR_FT uint32_t __LDA(volatile uint32_t *ptr) {
    uint32_t res;
    __ASM volatile ("LDA %0, [%1]" : "=r" (res) : "r" (*ptr) : "memory");
    return res;
  }

  __IAR_FT void __STLB(uint8_t value, volatile uint8_t *ptr) {
    __ASM volatile ("STLB %1, [%0]" :: "r" (*ptr), "r" (value) : "memory");
  }

  __IAR_FT void __STLH(uint16_t value, volatile uint16_t *ptr) {
    __ASM volatile ("STLH %1, [%0]" :: "r" (*ptr), "r" (value) : "memory");
  }

  __IAR_FT void __STL(uint32_t value, volatile uint32_t *ptr) {
    __ASM volatile ("STL %1, [%0]" :: "r" (*ptr), "r" (value) : "memory");
  }

  __IAR_FT uint8_t __LDAEXB(volatile uint8_t *ptr) {
    uint32_t res;
    __ASM volatile ("LDAEXB %0, [%1]" : "=r" (res) : "r" (*ptr) : "memory");
    return ((uint8_t)res);
  }

  __IAR_FT uint16_t __LDAEXH(volatile uint16_t *ptr) {
    uint32_t res;
    __ASM volatile ("LDAEXH %0, [%1]" : "=r" (res) : "r" (*ptr) : "memory");
    return ((uint16_t)res);
  }

  __IAR_FT uint32_t __LDAEX(volatile uint32_t *ptr) {
    uint32_t res;
    __ASM volatile ("LDAEX %0, [%1]" : "=r" (res) : "r" (*ptr) : "memory");
    return res;
  }

  __IAR_FT uint32_t __STLEXB(uint8_t value, volatile uint8_t *ptr) {
    uint32_t res;
    __ASM volatile ("STLEXB %0, %2, [%1]" : "=r" (res) : "r" (*ptr), "r" (value) : "memory");
    return res;
  }

  __IAR_FT uint32_t __STLEXH(uint16_t value, volatile uint16_t *ptr) {
    uint32_t res;
    __ASM volatile ("STLEXH %0, %2, [%1]" : "=r" (res) : "r" (*ptr), "r" (value) : "memory");
    return res;
  }

  __IAR_FT uint32_t __STLEX(uint32_t value, volatile uint32_t *ptr) {
    uint32_t res;
    __ASM volatile ("STLEX %0, %2, [%1]" : "=r" (res) : "r" (*ptr), "r" (value) : "memory");
    return res;
  }

#endif /* __ARM_ARCH_8M_MAIN__ or __ARM_ARCH_8M_BASE__ */

#undef __IAR_FT
#undef __IAR_M0_FAMILY

#pragma diag_default=Pe940
#pragma diag_default=Pe177

#endif /* __CMSIS_ICCARM_H__ */
