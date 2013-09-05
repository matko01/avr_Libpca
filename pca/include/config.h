#ifndef CONFIG_H_VPGOK90D
#define CONFIG_H_VPGOK90D

// ============================ SERIAL PORT =======================================

/// change to 0 and rebuild if you do not need stats information
#define SERIAL_COLLECT_STATS 1

/// change to 0 and rebuild if you do not with to include serial rx interrupt
/// some functionality will be missing
#define SERIAL_IMPLEMENT_RX_INT 1

/// change to 0 and rebuild if you do not with to include serial tx interrupt
/// some functionality will be missing
#define SERIAL_IMPLEMENT_TX_INT 0

/// 1 - STDIN data will be received by polling, 0 - interrupts
#define SERIAL_STDIN_POLL 0

/// 1 - STDOUT data will be sent by polling, 0 - interrupts
#define SERIAL_STDOUT_POLL 1

/// RX ring buffer size if SERIAL_IMPLEMENT_RX_INT == 1
#define SERIAL_RX_RING_SIZE 32

/// TX ring buffer size if SERIAL_IMPLEMENT_TX_INT == 1
#define SERIAL_TX_RING_SIZE 32

// ============================ TIMERS/BEEPER =====================================

/// change to 0 if to disable timer 0 delay and beeper interrupts and routines
#define TDELAY_IMPLEMENT_T0_INT 1

/// change to 0 if to disable timer 1 delay and beeper interrupts and routines
#define TDELAY_IMPLEMENT_T1_INT 0

/// change to 0 if to disable timer 2 delay and beeper interrupts and routines
#define TDELAY_IMPLEMENT_T2_INT 0

// ================================================================================


#endif /* end of include guard: CONFIG_H_VPGOK90D */

