#ifndef SPLC_ERROR_TYPE_H
#define SPLC_ERROR_TYPE_H

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
    SPLM_ERR_A,
    SPLM_ERR_B,

    SPLM_SEM_ERR_1,
    SPLM_SEM_ERR_2,
    SPLM_SEM_ERR_3,
    SPLM_SEM_ERR_4,
    SPLM_SEM_ERR_5,
    SPLM_SEM_ERR_6,
    SPLM_SEM_ERR_7,
    SPLM_SEM_ERR_8,
    SPLM_SEM_ERR_9,
    SPLM_SEM_ERR_10,
    SPLM_SEM_ERR_11,
    SPLM_SEM_ERR_12,
    SPLM_SEM_ERR_13,
    SPLM_SEM_ERR_14,
    SPLM_SEM_ERR_15,

    SPLM_MACRO_ERROR,
};

#define SPLC_IS_MSG_NONVERBOSE(x) ((x) >= SPLC_MSG_NONVERBOSE_OFFSET)
#define SPLC_IS_MSG_WARNING(x) (((x) & SPLC_MSG_WARNING_MASK) == SPLC_MSG_WARNING_MASK)
#define SPLC_IS_MSG_ERROR(x) (((x) & SPLC_MSG_ERROR_MASK) == SPLC_MSG_ERROR_MASK)

#endif /* SPLC_ERROR_TYPE_H */