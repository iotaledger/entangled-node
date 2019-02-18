/***************************************************************************/ /**
                                                                               *     __
                                                                               *    / /___  ____ _____ ____  _____
                                                                               *   / / __ \/ __ `/ __ `/ _ \/ ___/
                                                                               *  / / /_/ / /_/ / /_/ /  __/ /
                                                                               * /_/\____/\__, /\__, /\___/_/
                                                                               *         /____//____/
                                                                               *
                                                                               * \file   logger.h
                                                                               *
                                                                               * \brief  Logging facility for C.
                                                                               * \author Markus Braun
                                                                               ******************************************************************************/
#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** Preprocessor string conversion helper */
#define LOGGER_STRINGIFY_(x) #x

/** Preprocessor string conversion helper */
#define LOGGER_STRINGIFY(x) LOGGER_STRINGIFY_(x)

/** Deprecation macro */
#if __GNUC__
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define LOGGER_DEPRECATED(__new)                                               \
  __attribute__((__deprecated__("Use " #__new " instead")))
#else /* __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 5 */
#define LOGGER_DEPRECATED(__new) __attribute__((__deprecated__))
#endif /* __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 5 */
#else  /* __GNUC__ */
#define LOGGER_DEPRECATED(__new)
#endif /* __GNUC__ */

/** Inline macro */
#define LOGGER_INLINE static inline

/** Unused macro */
#if __GNUC__
#define LOGGER_UNUSED __attribute__((__unused__))
#else /* __GNUC__ */
#define LOGGER_UNUSED
#endif /* __GNUC__ */

/** Format printf macro */
#if __GNUC__
#define LOGGER_FORMAT_PRINTF(__format_idx, __variadic_idx)                     \
  __attribute__((format(printf, __format_idx, __variadic_idx)))
#else /* __GNUC__ */
#define LOGGER_FORMAT_PRINTF(__format_idx, __variadic_idx)
#endif /* __GNUC__ */

/** Logger major version number */
#define LOGGER_VERSION_MAJOR 4

/** Logger minor version number */
#define LOGGER_VERSION_MINOR 0

/** Logger patch level */
#define LOGGER_VERSION_PATCH 0

/** Logger version number as integer */
#define LOGGER_VERSION                                                         \
  ((logger_version_t)(((LOGGER_VERSION_MAJOR << 16) & 0x00FF0000) |            \
                      ((LOGGER_VERSION_MINOR << 8) & 0x0000FF00) |             \
                      ((LOGGER_VERSION_PATCH)&0x000000FF)))

/** Logger version number as string */
#define LOGGER_VERSION_STRING                                                  \
  LOGGER_STRINGIFY(LOGGER_VERSION_MAJOR)                                       \
  "." LOGGER_STRINGIFY(LOGGER_VERSION_MINOR) "." LOGGER_STRINGIFY(             \
      LOGGER_VERSION_PATCH)

typedef uint32_t logger_version_t; /**< Logger version type. */

typedef uint8_t logger_bool_t; /**< Logger boolean type. */

/** Logger boolean true. */
#define logger_true ((logger_bool_t)1)

/** Logger boolean false. */
#define logger_false ((logger_bool_t)0)

/** Logger log level types */
typedef uint16_t logger_level_t;

/** Unknown level. */
#define LOGGER_UNKNOWN ((logger_level_t)(0))

/** Debug-level message. */
#define LOGGER_DEBUG ((logger_level_t)(1 << 0))

/** Informational message. */
#define LOGGER_INFO ((logger_level_t)(1 << 1))

/** Normal, but significant, condition. */
#define LOGGER_NOTICE ((logger_level_t)(1 << 2))

/** Warning conditions. */
#define LOGGER_WARNING ((logger_level_t)(1 << 3))

/** Error conditions. */
#define LOGGER_ERR ((logger_level_t)(1 << 4))

/** Critical conditions. */
#define LOGGER_CRIT ((logger_level_t)(1 << 5))

/** Action must be taken immediately. */
#define LOGGER_ALERT ((logger_level_t)(1 << 6))

/** System is unusable. */
#define LOGGER_EMERG ((logger_level_t)(1 << 7))

/** All possible levels. */
#define LOGGER_ALL ((logger_level_t)((1 << 8) - 1))

/** Number of valid levels. */
#define LOGGER_MAX (9)

/** Logger prefix type */
typedef uint16_t logger_prefix_t;

/** Empty prefix */
#define LOGGER_PFX_EMPTY ((logger_prefix_t)(0))

/** Prefix date string */
#define LOGGER_PFX_DATE ((logger_prefix_t)(1 << 0))

/** Prefix loger ID name */
#define LOGGER_PFX_NAME ((logger_prefix_t)(1 << 1))

/** Prefix logger level */
#define LOGGER_PFX_LEVEL ((logger_prefix_t)(1 << 2))

/** Prefix file name */
#define LOGGER_PFX_FILE ((logger_prefix_t)(1 << 3))

/** Prefix function name */
#define LOGGER_PFX_FUNCTION ((logger_prefix_t)(1 << 4))

/** Prefix function name */
#define LOGGER_PFX_LINE ((logger_prefix_t)(1 << 5))

/** Unset prefix */
#define LOGGER_PFX_UNSET ((logger_prefix_t)(1 << 6))

/** All prefixes */
#define LOGGER_PFX_ALL ((logger_prefix_t)(LOGGER_PFX_UNSET - 1))

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL) instead
 * instead */
static const logger_prefix_t LOGGER_PREFIX_SHORT LOGGER_DEPRECATED((
    LOGGER_PFX_NAME | LOGGER_PFX_LEVEL)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
 * LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE) instead instead */
static const logger_prefix_t LOGGER_PREFIX_FUNCTION LOGGER_DEPRECATED(
    (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FUNCTION |
     LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
                          LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
 * LOGGER_PFX_FILE | LOGGER_PFX_LINE) instead instead */
static const logger_prefix_t LOGGER_PREFIX_FILE
    LOGGER_DEPRECATED((LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FILE |
                       LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
                                            LOGGER_PFX_FILE | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
 * LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE) instead instead */
static const logger_prefix_t LOGGER_PREFIX_FULL LOGGER_DEPRECATED((
    LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION |
    LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FILE |
                         LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_EMPTY) instead */
static const logger_prefix_t LOGGER_PREFIX_EMPTY
    LOGGER_DEPRECATED((LOGGER_PFX_EMPTY)) = (LOGGER_PFX_EMPTY);

/** Legacy variable, please use (LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION |
 * LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_FILE_FUNCTION_LINE LOGGER_DEPRECATED(
    (LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION |
     LOGGER_PFX_LINE)) = (LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION |
                          LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_FILE | LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_FILE_LINE LOGGER_DEPRECATED(
    (LOGGER_PFX_FILE | LOGGER_PFX_LINE)) = (LOGGER_PFX_FILE | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE) instead
 */
static const logger_prefix_t LOGGER_PREFIX_FUNCTION_LINE LOGGER_DEPRECATED(
    (LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE)) = (LOGGER_PFX_FUNCTION |
                                                LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME) instead */
static const logger_prefix_t
    LOGGER_PREFIX_NAME LOGGER_DEPRECATED((LOGGER_PFX_NAME)) = (LOGGER_PFX_NAME);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_FILE |
 * LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_NAME_FILE_FUNCTION_LINE
    LOGGER_DEPRECATED((LOGGER_PFX_NAME | LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION |
                       LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_FILE |
                                            LOGGER_PFX_FUNCTION |
                                            LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_FILE |
 * LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_NAME_FILE_LINE LOGGER_DEPRECATED(
    (LOGGER_PFX_NAME | LOGGER_PFX_FILE |
     LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_FILE | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_FUNCTION |
 * LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_NAME_FUNCTION_LINE LOGGER_DEPRECATED(
    (LOGGER_PFX_NAME | LOGGER_PFX_FUNCTION |
     LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_FUNCTION |
                          LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL) instead */
static const logger_prefix_t LOGGER_PREFIX_NAME_LEVEL LOGGER_DEPRECATED((
    LOGGER_PFX_NAME | LOGGER_PFX_LEVEL)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
 * LOGGER_PFX_FILE | LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_NAME_LEVEL_FILE_FUNCTION_LINE
    LOGGER_DEPRECATED((LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FILE |
                       LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE)) =
        (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FILE |
         LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
 * LOGGER_PFX_FILE | LOGGER_PFX_LINE) instead */
static const logger_prefix_t LOGGER_PREFIX_NAME_LEVEL_FILE_LINE
    LOGGER_DEPRECATED((LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FILE |
                       LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
                                            LOGGER_PFX_FILE | LOGGER_PFX_LINE);

/** Legacy variable, please use (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
 * LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE) instead */
static const logger_prefix_t
    LOGGER_PREFIX_NAME_LEVEL_FUNCTION_LINE LOGGER_DEPRECATED(
        (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL | LOGGER_PFX_FUNCTION |
         LOGGER_PFX_LINE)) = (LOGGER_PFX_NAME | LOGGER_PFX_LEVEL |
                              LOGGER_PFX_FUNCTION | LOGGER_PFX_LINE);

/** Logger ID type. */
typedef int16_t logger_id_t;

/** Unknown logger ID. */
#define logger_id_unknown ((logger_id_t)-1)

/** Logger function return codes */
typedef enum logger_return_e {
  LOGGER_OK = 0,                     /**< Ok. */
  LOGGER_ERR_UNKNOWN = -1,           /**< Unspecified error. */
  LOGGER_ERR_OUTPUT_INVALID = -2,    /**< Given output stream is invalid. */
  LOGGER_ERR_OUTPUTS_FULL = -3,      /**< All available outputs are used. */
  LOGGER_ERR_OUTPUT_REGISTERED = -4, /**< Output already registered. */
  LOGGER_ERR_OUTPUT_NOT_FOUND = -5,  /**< Output not registered. */
  LOGGER_ERR_IDS_FULL = -6,          /**< All available ids are used. */
  LOGGER_ERR_ID_UNKNOWN = -7,        /**< Id is unknown. */
  LOGGER_ERR_LEVEL_UNKNOWN = -8,     /**< Level is unknown. */
  LOGGER_ERR_PREFIX_UNKNOWN = -9,    /**< Prefix is unknown. */
  LOGGER_ERR_OUT_OF_MEMORY = -10,    /**< Memory allocation error. */
  LOGGER_ERR_TYPE_INVALID = -11,     /**< Output type is invalid. */
  LOGGER_ERR_STREAM_INVALID = -12,   /**< File stream is invalid. */
  LOGGER_ERR_FUNCTION_INVALID = -13, /**< Function is invalid. */
  LOGGER_ERR_NAME_INVALID = -14,     /**< Name string is invalid. */
  LOGGER_ERR_FORMAT_INVALID = -15,   /**< Format string is invalid. */
  LOGGER_ERR_FILE_INVALID = -16,     /**< File string is invalid. */
  LOGGER_ERR_STRING_TOO_LONG = -17,  /**< Given string is too long. */
  LOGGER_ERR_STRING_INVALID = -18    /**< Given string is invalid. */
} logger_return_t;

/** Logger type for text style output. */
typedef unsigned int logger_text_attr_t;

/** Reset attributes. */
#define LOGGER_ATTR_RESET ((logger_text_attr_t)(1 << 0))

/** Bright attribute. */
#define LOGGER_ATTR_BRIGHT ((logger_text_attr_t)(1 << 1))

/** Dim attribute. */
#define LOGGER_ATTR_DIM ((logger_text_attr_t)(1 << 2))

/** Underline attribute. */
#define LOGGER_ATTR_UNDERLINE ((logger_text_attr_t)(1 << 3))

/** Blink attribute. */
#define LOGGER_ATTR_BLINK ((logger_text_attr_t)(1 << 4))

/** Reverse attribute. */
#define LOGGER_ATTR_REVERSE ((logger_text_attr_t)(1 << 5))

/** Hidden attribute. */
#define LOGGER_ATTR_HIDDEN ((logger_text_attr_t)(1 << 6))

/** Logger enum for text background color output. */
typedef enum logger_text_bg_e {
  LOGGER_BG_UNCHANGED = 0, /**< Unchanged background color. */
  LOGGER_BG_BLACK = 40,    /**< Black background color. */
  LOGGER_BG_RED = 41,      /**< Red background color. */
  LOGGER_BG_GREEN = 42,    /**< Green background color. */
  LOGGER_BG_YELLOW = 43,   /**< Yellow background color. */
  LOGGER_BG_BLUE = 44,     /**< Blue background color. */
  LOGGER_BG_MAGENTA = 45,  /**< Magenta background color. */
  LOGGER_BG_CYAN = 46,     /**< Cyan background color. */
  LOGGER_BG_WHITE = 47     /**< White background color. */
} logger_text_bg_t;

/** Logger enum for text foreground color output. */
typedef enum logger_text_fg_e {
  LOGGER_FG_UNCHANGED = 0, /**< Unchanged foreground color. */
  LOGGER_FG_BLACK = 30,    /**< Black foreground color. */
  LOGGER_FG_RED = 31,      /**< Red foreground color. */
  LOGGER_FG_GREEN = 32,    /**< Green foreground color. */
  LOGGER_FG_YELLOW = 33,   /**< Yellow foreground color. */
  LOGGER_FG_BLUE = 34,     /**< Blue foreground color. */
  LOGGER_FG_MAGENTA = 35,  /**< Magenta foreground color. */
  LOGGER_FG_CYAN = 36,     /**< Cyan foreground color. */
  LOGGER_FG_WHITE = 37     /**< White foreground color. */
} logger_text_fg_t;

/** Logger output function type */
typedef void (*logger_output_function_t)(const char *);

#ifdef LOGGER_ENABLE
logger_version_t logger_version(void);
logger_return_t logger_init(void);
logger_bool_t logger_is_initialized(void);
logger_return_t logger_enable(void);
logger_return_t logger_disable(void);
logger_bool_t logger_is_enabled(void);
logger_return_t logger_prefix_set(const logger_prefix_t prefix);
logger_prefix_t logger_prefix_get(void);
logger_return_t logger_output_register(FILE *stream);
logger_return_t logger_output_deregister(FILE *stream);
logger_bool_t logger_output_is_registered(FILE *stream);
logger_return_t logger_output_level_set(FILE *stream,
                                        const logger_level_t level);
logger_level_t logger_output_level_get(FILE *stream);
logger_return_t logger_output_level_mask_set(FILE *stream,
                                             const logger_level_t level);
logger_level_t logger_output_level_mask_get(FILE *stream);
logger_return_t logger_output_color_enable(FILE *stream);
logger_return_t logger_output_color_disable(FILE *stream);
logger_bool_t logger_output_color_is_enabled(FILE *stream);
logger_return_t logger_output_flush(void);
logger_return_t
logger_output_function_register(logger_output_function_t function);
logger_return_t
logger_output_function_deregister(logger_output_function_t function);
logger_bool_t
logger_output_function_is_registered(logger_output_function_t function);
logger_return_t
logger_output_function_level_set(logger_output_function_t function,
                                 const logger_level_t level);
logger_level_t
logger_output_function_level_get(logger_output_function_t function);
logger_return_t
logger_output_function_level_mask_set(logger_output_function_t function,
                                      const logger_level_t level);
logger_level_t
logger_output_function_level_mask_get(logger_output_function_t function);
logger_return_t
logger_output_function_color_enable(logger_output_function_t function);
logger_return_t
logger_output_function_color_disable(logger_output_function_t function);
logger_bool_t
logger_output_function_color_is_enabled(logger_output_function_t function);
logger_id_t logger_id_request(const char *name);
logger_return_t logger_id_release(const logger_id_t id);
logger_return_t logger_id_enable(const logger_id_t id);
logger_return_t logger_id_disable(const logger_id_t id);
logger_bool_t logger_id_is_enabled(const logger_id_t id);
logger_bool_t logger_id_generates_output(const logger_id_t id,
                                         const logger_level_t level);
logger_return_t logger_id_level_set(const logger_id_t id,
                                    const logger_level_t level);
logger_level_t logger_id_level_get(const logger_id_t id);
logger_return_t logger_id_level_mask_set(const logger_id_t id,
                                         const logger_level_t level);
logger_level_t logger_id_level_mask_get(const logger_id_t id);
logger_return_t logger_id_prefix_set(const logger_id_t id,
                                     const logger_prefix_t prefix);
logger_prefix_t logger_id_prefix_get(const logger_id_t id);
const char *logger_id_name_get(const logger_id_t id);
logger_return_t logger_id_output_register(const logger_id_t id, FILE *stream);
logger_return_t logger_id_output_deregister(const logger_id_t id, FILE *stream);
logger_bool_t logger_id_output_is_registered(const logger_id_t id,
                                             FILE *stream);
logger_return_t logger_id_output_level_set(const logger_id_t id, FILE *stream,
                                           const logger_level_t level);
logger_level_t logger_id_output_level_get(const logger_id_t id, FILE *stream);
logger_return_t logger_id_output_level_mask_set(const logger_id_t id,
                                                FILE *stream,
                                                const logger_level_t level);
logger_level_t logger_id_output_level_mask_get(const logger_id_t id,
                                               FILE *stream);
logger_return_t logger_id_output_color_enable(const logger_id_t id,
                                              FILE *stream);
logger_return_t logger_id_output_color_disable(const logger_id_t id,
                                               FILE *stream);
logger_bool_t logger_id_output_color_is_enabled(const logger_id_t id,
                                                FILE *stream);
logger_return_t
logger_id_output_function_register(const logger_id_t id,
                                   logger_output_function_t function);
logger_return_t
logger_id_output_function_deregister(const logger_id_t id,
                                     logger_output_function_t function);
logger_bool_t
logger_id_output_function_is_registered(const logger_id_t id,
                                        logger_output_function_t function);
logger_return_t
logger_id_output_function_level_set(const logger_id_t id,
                                    logger_output_function_t function,
                                    const logger_level_t level);
logger_level_t
logger_id_output_function_level_get(const logger_id_t id,
                                    logger_output_function_t function);
logger_return_t
logger_id_output_function_level_mask_set(const logger_id_t id,
                                         logger_output_function_t function,
                                         const logger_level_t level);
logger_level_t
logger_id_output_function_level_mask_get(const logger_id_t id,
                                         logger_output_function_t function);
logger_return_t
logger_id_output_function_color_enable(const logger_id_t id,
                                       logger_output_function_t function);
logger_return_t
logger_id_output_function_color_disable(const logger_id_t id,
                                        logger_output_function_t function);
logger_bool_t
logger_id_output_function_color_is_enabled(const logger_id_t id,
                                           logger_output_function_t function);
logger_return_t logger_id_color_console_set(const logger_id_t id,
                                            const logger_text_fg_t fg,
                                            const logger_text_bg_t bg,
                                            const logger_text_attr_t attr);
logger_return_t logger_id_color_string_set(const logger_id_t id,
                                           const char *begin, const char *end);
logger_return_t logger_id_color_reset(const logger_id_t id);
logger_return_t logger_color_prefix_enable(void);
logger_return_t logger_color_prefix_disable(void);
logger_bool_t logger_color_prefix_is_enabled(void);
logger_return_t logger_color_prefix_console_set(const logger_level_t level,
                                                const logger_text_fg_t fg,
                                                const logger_text_bg_t bg,
                                                const logger_text_attr_t attr);
logger_return_t logger_color_prefix_string_set(const logger_level_t level,
                                               const char *begin,
                                               const char *end);
logger_return_t logger_color_prefix_reset(void);
logger_return_t logger_color_message_enable(void);
logger_return_t logger_color_message_disable(void);
logger_bool_t logger_color_message_is_enabled(void);
const char *logger_level_name_get(const logger_level_t level);
logger_return_t logger_implementation(logger_id_t id, logger_level_t level,
                                      const char *file, const char *function,
                                      uint32_t line, const char *format, ...)
    LOGGER_FORMAT_PRINTF(6, 7);
logger_return_t logger_implementation_va(logger_id_t id, logger_level_t level,
                                         const char *file, const char *function,
                                         uint32_t line, const char *format,
                                         va_list argp);

/** Macro to call the real logger function logger() with the information about
 * the current position in code (file, function and line) */
#define logger(__id, __level, ...)                                             \
  logger_implementation(__id, __level, __FILE__, __FUNCTION__, __LINE__,       \
                        __VA_ARGS__)

/** Macro to call the real logger function logger_va() with the information
 * about the current position in code (file, function and line) */
#define logger_va(__id, __level, __format, __argp)                             \
  logger_implementation_va(__id, __level, __FILE__, __FUNCTION__, __LINE__,    \
                           __format, __argp)

/* helper functions */
uint16_t logger_level_to_index(const logger_level_t level);
logger_level_t logger_index_to_level(const uint16_t index);

/* legacy functions */
logger_return_t
logger_id_color_set(const logger_id_t id, const logger_text_fg_t fg,
                    const logger_text_bg_t bg, const logger_text_attr_t attr)
    LOGGER_DEPRECATED("logger_id_color_console_set()");
logger_return_t
logger_color_set(const logger_id_t id, const logger_text_fg_t fg,
                 const logger_text_bg_t bg, const logger_text_attr_t attr)
    LOGGER_DEPRECATED("logger_id_color_console_set()");
logger_return_t logger_color_reset(const logger_id_t id)
    LOGGER_DEPRECATED("logger_id_color_reset()");
logger_return_t logger_color_prefix_set(const logger_level_t level,
                                        const logger_text_fg_t fg,
                                        const logger_text_bg_t bg,
                                        const logger_text_attr_t attr)
    LOGGER_DEPRECATED("logger_id_color_console_set()");

#else /* LOGGER_ENABLE */

LOGGER_INLINE logger_version_t logger_disabled_version(void) {
  return (LOGGER_VERSION);
}

LOGGER_INLINE logger_return_t logger_disabled_ok(void) { return (LOGGER_OK); }

LOGGER_INLINE logger_return_t logger_disabled_err(void) {
  return (LOGGER_ERR_UNKNOWN);
}

LOGGER_INLINE logger_id_t logger_disabled_id(void) { return (0); }

LOGGER_INLINE logger_bool_t logger_disabled_true(void) { return (logger_true); }

LOGGER_INLINE logger_bool_t logger_disabled_false(void) {
  return (logger_false);
}

LOGGER_INLINE logger_level_t logger_disabled_unknown(void) {
  return (LOGGER_UNKNOWN);
}

LOGGER_INLINE logger_prefix_t logger_disabled_unset(void) {
  return (LOGGER_PFX_UNSET);
}

static const char *logger_empty_string = "";

LOGGER_INLINE const char *logger_disabled_string(void) {
  return (logger_empty_string);
}

LOGGER_INLINE uint16_t logger_disabled_zero(void) { return (0); }

#define logger_version() logger_disabled_version()
#define logger_init() logger_disabled_ok()
#define logger_is_initialized() logger_disabled_false()
#define logger_enable() logger_disabled_ok()
#define logger_disable() logger_disabled_ok()
#define logger_is_enabled(__id) logger_disabled_false()
#define logger_prefix_set(__prefix) logger_disabled_ok()
#define logger_prefix_get() logger_disabled_unset()
#define logger_output_register(__stream) logger_disabled_ok()
#define logger_output_deregister(__stream) logger_disabled_ok()
#define logger_output_is_registered(__stream) logger_disabled_false()
#define logger_output_level_set(__stream, __level) logger_disabled_ok()
#define logger_output_level_get(__stream) logger_disabled_unknown()
#define logger_output_level_mask_get(__stream) logger_disabled_unknown()
#define logger_output_level_mask_set(__stream, __level) logger_disabled_ok()
#define logger_output_color_enable(__stream) logger_disabled_ok()
#define logger_output_color_disable(__stream) logger_disabled_ok()
#define logger_output_color_is_enabled(__stream) logger_disabled_false()
#define logger_output_flush() logger_disabled_ok()
#define logger_output_function_register(__function) logger_disabled_ok()
#define logger_output_function_deregister(__function) logger_disabled_ok()
#define logger_output_function_is_registered(__function) logger_disabled_false()
#define logger_output_function_level_set(__function, __level)                  \
  logger_disabled_ok()
#define logger_output_function_level_get(__function) logger_disabled_unkown()
#define logger_output_function_level_mask_set(__function, __level)             \
  logger_disabled_ok()
#define logger_output_function_level_mask_get(__function)                      \
  logger_disabled_unkown()
#define logger_output_function_color_enable(__function) logger_disabled_ok()
#define logger_output_function_color_disable(__function) logger_disabled_ok()
#define logger_output_function_color_is_enabled(__function)                    \
  logger_disabled_false()
#define logger_id_request(__name) logger_disabled_id()
#define logger_id_release(__id) logger_disabled_ok()
#define logger_id_enable(__id) logger_disabled_ok()
#define logger_id_disable(__id) logger_disabled_ok()
#define logger_id_is_enabled(__id) logger_disabled_false()
#define logger_id_generates_output(__id, __level) logger_disabled_false()
#define logger_id_level_set(__id, __level) logger_disabled_ok()
#define logger_id_level_get(__id) logger_disabled_unknown()
#define logger_id_level_mask_set(__id, __level) logger_disabled_ok()
#define logger_id_level_mask_get(__id) logger_disabled_unknown()
#define logger_id_prefix_set(__id, __prefix) logger_disabled_ok()
#define logger_id_prefix_get(__id) logger_disabled_unset()
#define logger_id_name_get(__id) logger_disabled_string()
#define logger_id_output_register(__id, __stream) logger_disabled_ok()
#define logger_id_output_deregister(__id, __stream) logger_disabled_ok()
#define logger_id_output_is_registered(__id, __stream) logger_disabled_false()
#define logger_id_output_level_set(__id, __stream, __level) logger_disabled_ok()
#define logger_id_output_level_get(__id, __stream) logger_disabled_err()
#define logger_id_output_level_mask_set(__id, __stream, __level)               \
  logger_disabled_ok()
#define logger_id_output_level_mask_get(__id, __stream) logger_disabled_err()
#define logger_id_output_color_enable(__id, __stream) logger_disabled_ok()
#define logger_id_output_color_disable(__id, __stream) logger_disabled_ok()
#define logger_id_output_color_is_enabled(__id, __stream)                      \
  logger_disabled_false()
#define logger_id_output_function_register(__id, __function)                   \
  logger_disabled_ok()
#define logger_id_output_function_deregister(__id, __function)                 \
  logger_disabled_ok()
#define logger_id_output_function_is_registered(__id, __function)              \
  logger_disabled_false()
#define logger_id_output_function_level_set(__id, __function, __level)         \
  logger_disabled_ok()
#define logger_id_output_function_level_get(__id, __function)                  \
  logger_disabled_err()
#define logger_id_output_function_level_mask_set(__id, __function, __level)    \
  logger_disabled_ok()
#define logger_id_output_function_level_mask_get(__id, __function)             \
  logger_disabled_err()
#define logger_id_output_function_color_enable(__id, __function)               \
  logger_disabled_ok()
#define logger_id_output_function_color_disable(__id, __function)              \
  logger_disabled_ok()
#define logger_id_output_function_color_is_enabled(__id, __function)           \
  logger_disabled_false()
#define logger_id_color_console_set(__id, __fg, __bg, __attr)                  \
  logger_disabled_ok()
#define logger_id_color_string_set(__id, __begin, __end) logger_disabled_ok()
#define logger_id_color_reset(__id) logger_disabled_ok()
#define logger_color_prefix_enable() logger_disabled_ok()
#define logger_color_prefix_disable() logger_disabled_ok()
#define logger_color_prefix_is_enabled() logger_disabled_false()
#define logger_color_prefix_console_set(__level, __fg, __bg, __attr)           \
  logger_disabled_ok()
#define logger_color_prefix_string_set(__level, __begin, __end)                \
  logger_disabled_ok()
#define logger_color_prefix_reset() logger_disabled_ok()
#define logger_color_message_enable() logger_disabled_ok()
#define logger_color_message_disable() logger_disabled_ok()
#define logger_color_message_is_enabled() logger_disabled_false()
#define logger_level_name_get(__level) logger_disabled_string()
#define logger(__id, __level, ...) logger_disabled_ok()
#define logger_va(__id, __level, ...) logger_disabled_ok()

/* helper functions */
#define logger_level_to_index(__level) logger_disabled_zero()
#define logger_index_to_level(__index) logger_disabled_unkown()

/* legacy functions */
#define logger_id_color_set(__id, __fg, __bg, __attr) logger_disabled_ok()
#define logger_color_set(__id, __fg, __bg, __attr) logger_disabled_ok()
#define logger_color_reset(__id) logger_disabled_ok()
#define logger_id_color_set(__id, __fg, __bg, __attr) logger_disabled_ok()
#define logger_color_prefix_set(__level, __fg, __bg, __attr)                   \
  logger_disabled_ok()
#endif /* LOGGER_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* end of include guard: LOGGER_H */
