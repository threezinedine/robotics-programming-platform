#pragma once

#if defined(_MSC_VER)
/**
 * @brief Used for marking the object should be exposed to be binded into Python Module.
 */
#define RPP_PYTHYON_BINDING

/**
 * @brief Used for marking the object as deprecated. A warning will be shown when the object is used.
 */
#define RPP_DEPRECATED

/**
 * @brief Used for marking the singleton class (used in binding).
 */
#define RPP_SINGLETON
#elif defined(__GNUC__) || defined(__clang__)
/**
 * @brief Used for marking the object should be exposed to be binded into Python Module.
 */
#define RPP_PYTHYON_BINDING __attribute__((annotate("python_binding")))

/**
 * @brief Used for marking the object as deprecated. A warning will be shown when the object is used.
 */
#define RPP_DEPRECATED __attribute__((deprecated))

/**
 * @brief Used for marking the singleton class (used in binding).
 */
#define RPP_SINGLETON __attribute__((annotate("singleton")))
#endif