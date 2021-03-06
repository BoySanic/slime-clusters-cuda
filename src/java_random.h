#ifndef JAVA_RANDOM_H
#define JAVA_RANDOM_h

#include <cassert>
#include <stdexcept>

#include "cuda.h"
#include "random.h"

class JavaRandom : public Random {
public:
    CUDA_CALL JavaRandom() : JavaRandom(0UI64) {}

    CUDA_CALL JavaRandom(uint64_t seed) : Random(LCG(0x5DEECE66DUI64, 0xB, 1UI64 << 48), seed) {}

    CUDA_CALL static bool next_boolean(Random *random) {
        return next(random, 1) != 0;
    }

    CUDA_CALL static int32_t next_int(Random *random) {
        return next(random, 32);
    }

    CUDA_CALL static int32_t next_int(Random *random, int32_t range) {
        assert(range > 0 && "Range must be positive.");

        // When range is a power of 2:
        if((range & -range) == range)
            return (int32_t)((range * (uint64_t)next(random, 31)) >> 31);

        int32_t bits, value;
        do {
            bits = next(random, 31);
            value = bits % range;
        } while(bits - value + (range - 1) < 0);
        return value;
    }

    CUDA_CALL static uint64_t next_long(Random *random) {
        return ((uint64_t)next(random, 32) << 32) + next(random, 32);
    }

    CUDA_CALL static float next_float(Random *random) {
        return next(random, 24) / ((float)(1 << 24));
    }

    CUDA_CALL static double next_double(Random *random) {
        return (((uint64_t)next(random, 26) << 27) + next(random, 27)) / (double)(1UI64 << 53);
    }

    CUDA_CALL bool next_boolean() {
        return next_boolean(this);
    }

    CUDA_CALL int32_t next_int() {
        return next_int(this);
    }

    CUDA_CALL int32_t next_int(int32_t range) {
        return next_int(this, range);
    }

    CUDA_CALL uint64_t next_long() {
        return next_long(this);
    }

    CUDA_CALL float next_float() {
        return next_float(this);
    }

    CUDA_CALL double next_double() {
        return next_double(this);
    }

private:
    CUDA_CALL static int32_t next(Random *random, int32_t bits) {
        return (int32_t)(random->next_seed() >> (48 - bits));
    }
};

#endif // JAVA_RANDOM_H