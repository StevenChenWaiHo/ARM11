#ifndef AEMU_UNUSED_H
#define AEMU_UNUSED_H

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#endif
