#ifndef SPLC_MSG_TYPE_H
#define SPLC_MSG_TYPE_H

#define SPLC_MSG_NONVERBOSE_OFFSET 0x4000
#define SPLC_MSG_WARNING_MASK 0x10000
#define SPLC_MSG_ERROR_MASK 0x20000

typedef enum splc_msg_type splc_msg_t;

enum splc_msg_type
{
    SPLM_DIAG, /* splc: diagnostic message */

    SPLM_NOTE = SPLC_MSG_NONVERBOSE_OFFSET,

    SPLM_Wuniv = SPLC_MSG_WARNING_MASK, /* universal warning */
    SPLM_Woverflow,
    SPLM_Wimplicit_int,
    SPLM_MACRO_WARN,
    
    SPLM_ERR_FATAL = SPLC_MSG_ERROR_MASK,
    SPLM_ERR_UNIV, /* splc: universal error */
    SPLM_ERR_SYN_A,
    SPLM_ERR_SYN_B,

    SPLM_ERR_SEM_1,
    SPLM_ERR_SEM_2,
    SPLM_ERR_SEM_3,
    SPLM_ERR_SEM_4,
    SPLM_ERR_SEM_5,
    SPLM_ERR_SEM_6,
    SPLM_ERR_SEM_7,
    SPLM_ERR_SEM_8,
    SPLM_ERR_SEM_9,
    SPLM_ERR_SEM_10,
    SPLM_ERR_SEM_11,
    SPLM_ERR_SEM_12,
    SPLM_ERR_SEM_13,
    SPLM_ERR_SEM_14,
    SPLM_ERR_SEM_15,

    SPLM_MACRO_ERROR,
};

#define SPLC_IS_MSG_NONVERBOSE(x) ((x) >= SPLC_MSG_NONVERBOSE_OFFSET)
#define SPLC_IS_MSG_WARNING(x) (((x) & SPLC_MSG_WARNING_MASK) == SPLC_MSG_WARNING_MASK)
#define SPLC_IS_MSG_ERROR(x) (((x) & SPLC_MSG_ERROR_MASK) == SPLC_MSG_ERROR_MASK)

const char *get_splc_msg_color_code(splc_msg_t type);

const char *splc_get_msg_type_prefix(splc_msg_t type);

const char *splc_get_msg_type_suffix(splc_msg_t type);

#endif /* SPLC_MSG_TYPE_H */