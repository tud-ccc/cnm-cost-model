#ifndef SEL_PRED_H
#define SEL_PRED_H

#include "config.h"

// Shared by dpu.c and host/app.c so the device kernel and the host
// reference can never diverge on what "selected" means (they previously
// did: dpu.c's real filter condition didn't match this declared predicate).
static inline bool pred(const T x) { return (x % 2) == 0; }

#endif
