#pragma once

#if defined(_MSC_VER)
/**
 * @brief Used for marking the object should be exposed to be binded into Python Module.
 */
#define RPP_PYTHON_BINDING

/**
 * @brief Used for marking the object as deprecated. A warning will be shown when the object is used.
 */
#define RPP_DEPRECATED

/**
 * @brief Used for marking the singleton class (used in binding).
 */
#define RPP_SINGLETON

/**
 * @brief Used for marking the object should be hidden from Python binding.
 */
#define RPP_HIDE

/**
 * @brief Used for marking the class will be mapped to JSON object.
 */
#define RPP_JSON

/**
 * @brief map the attribute with the given name in JSON object.
 */
#define RPP_JSON_KEY(name)
#elif defined(__GNUC__) || defined(__clang__)
/**
 * @brief Used for marking the object should be exposed to be binded into Python Module.
 */
#define RPP_PYTHON_BINDING __attribute__((annotate("python")))

/**
 * @brief Used for marking the object as deprecated. A warning will be shown when the object is used.
 */
#define RPP_DEPRECATED __attribute__((deprecated))

/**
 * @brief Used for marking the singleton class (used in binding).
 */
#define RPP_SINGLETON __attribute__((annotate("singleton")))

/**
 * @brief Used for marking the object should be hidden from Python binding.
 */
#define RPP_HIDE __attribute__((annotate("hide")))

/**
 * @brief Used for marking the class will be mapped to JSON object.
 */
#define RPP_JSON __attribute__((annotate("json")))

/**
 * @brief map the attribute with the given name in JSON object.
 */
#define RPP_JSON_KEY(name) __attribute__((annotate("key:" name)))
#endif