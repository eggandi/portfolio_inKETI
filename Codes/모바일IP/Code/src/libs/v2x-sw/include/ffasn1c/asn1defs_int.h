/*
 * ASN1 definitions (internal) 
 * Copyright (C) 2011-2018 Fabrice Bellard
 */
#ifndef _ASN1_DEFS_INT_H
#define _ASN1_DEFS_INT_H

#include <asn1defs.h>

#if defined(_WIN32)

#include <limits.h>

/* some useful C99 defines which are not defined in MSVC CRT library */
#define snprintf _snprintf
#define vsnprintf _vsnprintf

#ifndef DBL_MAX 
#define DBL_MAX 1.7976931348623157e+308 /* mingw bug: not defined */
#endif

#define INFINITY (DBL_MAX+DBL_MAX)
#define NAN (INFINITY-INFINITY)

static inline int isnan(double x)
{
    return _isnan(x);
}

static inline int isinf(double x)
{
    return !_finite(x) && !_isnan(x);
}

#endif /* !_WIN32 */

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define force_inline inline __attribute__((always_inline))
#define no_inline __attribute__((noinline))

#define countof(x) (sizeof(x) / sizeof(x[0]))

#define xglue(x, y) x ## y
#define glue(x, y) xglue(x, y)
#define stringify(s)    tostring(s)
#define tostring(s)     #s

typedef struct ASN1ValueStack {
    struct ASN1ValueStack *prev; /* previous entry */
    const ASN1CType *type;
    void *data;
} ASN1ValueStack;

static inline int max_int(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

static inline int min_int(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

static inline int get_bit(const uint8_t *data, int index)
{
    return (data[index >> 3] >> (7 - (index & 7))) & 1;
}

static inline void put_bit(uint8_t *data, int index, int bit)
{
    data[index >> 3] |= bit << (7 - (index & 7));
}

static inline int clz32(unsigned int a)
{
    return __builtin_clz(a);
}

static inline uint32_t to_be32(const uint8_t *d)
{
    return ((uint32_t)d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];
}

static inline BOOL asn1_add_ov_uint(uint32_t a, uint32_t b)
{
    return (a + b) < a;
}

static inline BOOL check_malloc_size_overflow(size_t *psize,
                                              size_t size1, size_t size2)
{
    if (size1 <= UINT32_MAX && size2 <= UINT32_MAX) {
        /* 32 bit case or fast 64 bit case */
        uint64_t size = (uint64_t)size1 * (uint64_t)size2;
        if (unlikely(size > SIZE_MAX))
            return TRUE;
        *psize = size;
    } else if (size2 == 0) {
        *psize = 0;
    } else {
        if (size1 > (SIZE_MAX / size2))
            return TRUE;
        *psize = size1 * size2;
    }
    return FALSE;
}
    
/* same as asn1_malloc(size1 * size2), but checks overflow of the product. */
static inline void *asn1_malloc2(size_t size1, size_t size2)
{
    size_t size;
    if (check_malloc_size_overflow(&size, size1, size2))
        return NULL;
    return asn1_malloc(size);
}

static inline void *asn1_mallocz2(size_t size1, size_t size2)
{
    size_t size;
    if (check_malloc_size_overflow(&size, size1, size2))
        return NULL;
    return asn1_mallocz(size);
}

/* same as asn1_realloc(ptr, size1 * size2), but checks overflow of
   the product. */
static inline void *asn1_realloc2(void *ptr, size_t size1, size_t size2)
{
    size_t size;
    if (check_malloc_size_overflow(&size, size1, size2))
        return NULL;
    return asn1_realloc(ptr, size);
}

typedef struct ASN1ByteBuffer {
    uint8_t *buf;
    size_t len;
    size_t size;
    BOOL has_error; /* true if a memory allocation occured */
} ASN1ByteBuffer;

void asn1_byte_buffer_init(ASN1ByteBuffer *s);
asn1_exception int __asn1_byte_buffer_realloc(ASN1ByteBuffer *s, size_t size);

static inline asn1_exception int asn1_byte_buffer_expand(ASN1ByteBuffer *s, size_t added_len)
{
    size_t size = s->len + added_len;
    if (unlikely(size > s->size))
        return __asn1_byte_buffer_realloc(s, size);
    return 0;
}

static force_inline void asn1_put_byte(ASN1ByteBuffer *s, int b)
{
    if (unlikely(asn1_byte_buffer_expand(s, 1)))
        return;
    s->buf[s->len++] = b;
}

static force_inline void asn1_put_be16(ASN1ByteBuffer *s, unsigned int b)
{
    if (unlikely(asn1_byte_buffer_expand(s, 2)))
        return;
    s->buf[s->len++] = b >> 8;
    s->buf[s->len++] = b;
}

static force_inline void asn1_put_be32(ASN1ByteBuffer *s, unsigned int b)
{
    if (unlikely(asn1_byte_buffer_expand(s, 4)))
        return;
    s->buf[s->len++] = b >> 24;
    s->buf[s->len++] = b >> 16;
    s->buf[s->len++] = b >> 8;
    s->buf[s->len++] = b;
}

void asn1_put_bytes(ASN1ByteBuffer *s, const uint8_t *buf, size_t count);
void asn1_puts(ASN1ByteBuffer *s, const char *str);
void asn1_vprintf(ASN1ByteBuffer *s, const char *fmt, va_list ap);
void __attribute__((format(printf, 2, 3))) 
    asn1_printf(ASN1ByteBuffer *s, const char *fmt, ...);

asn1_exception int asn1_sequence_set_default_value(void *data, const ASN1SequenceField *f);
asn1_exception int asn1_sequence_set_default_fields(const ASN1SequenceField *f1,
                                                    int nb_fields, void *data,
                                                    const uint8_t *table_present);
BOOL asn1_sequence_is_default_present(const void *data, const ASN1SequenceField *f);
static inline BOOL asn1_sequence_is_field_present(const void *data, const ASN1SequenceField *f)
{
    BOOL option_bit;
    int flag, val;

    flag = ASN1_GET_SEQ_FLAG(f);
    if (flag == ASN1_SEQ_FLAG_OPTIONAL ||
        (flag == ASN1_SEQ_FLAG_NORMAL && ASN1_IS_SEQ_EXT(f))) {
        val = *(BOOL *)((const uint8_t *)data + f->u.option_offset);
        option_bit = (val != 0);
    } else if (flag == ASN1_SEQ_FLAG_DEFAULT) {
        option_bit = asn1_sequence_is_default_present(data, f);
    } else {
        option_bit = 1;
    }
    return option_bit;
}

int asn1_sequence_check_fields(const ASN1SequenceField *f1, int nb_fields, 
                               const uint8_t *table_present, 
                               char *msg, int msg_size);
#define ASN1_UTF8_MAX_LEN 4
int asn1_to_utf8(uint8_t *buf, unsigned int c);
int asn1_from_utf8(const uint8_t *p, size_t max_len, const uint8_t **pp);

#define ASN1_DOUBLE_DER_MAX_LEN (1 + 2 + 8)

int asn1_encode_real_der(uint8_t *buf, double d);
int asn1_decode_real_ber(const uint8_t *buf, int len, double *pd);
int asn1_cmp_real(double a1, double a2);

void asn1_pstrcpy(char *buf, size_t buf_size, const char *str);

const ASN1CType *asn1_get_constrained_type(const ASN1ValueStack *vs1,
                                           const ASN1CType *p);

/* Note: the integer must not be a large integer */
static inline BOOL asn1_is_uint32(const ASN1CType *p)
{
    int flags = *p++;
    return (!(flags & ASN1_CTYPE_HAS_EXT) && (flags & ASN1_CTYPE_HAS_LOW) &&
            (int)p[0] >= 0);
}

int asn1_find_enum_index(const ASN1CType *p, int nb_fields, int val, int flags);

extern const char *asn1_char_string_str[ASN1_CSTR_COUNT];

void asn1_error_init(ASN1Error *err);

/* type paths */

typedef struct {
    BOOL is_index;
    union {
        const char *name;
        int index;
    } u;
} ASN1TypePathElement;

#define ASN1_MAX_TYPE_ACCESSORS 32 /* must be a power of two */

typedef struct {
    int accessor_count;
    ASN1TypePathElement accessors[ASN1_MAX_TYPE_ACCESSORS];
} ASN1TypePath;
    
static inline void asn1_type_path_init(ASN1TypePath *s)
{
    s->accessor_count = 0;
}

static inline void asn1_type_path_push_name(ASN1TypePath *s, const char *name)
{
    ASN1TypePathElement *e;
    e = &s->accessors[s->accessor_count & (ASN1_MAX_TYPE_ACCESSORS - 1)];
    e->is_index = FALSE;
    e->u.name = name;
    s->accessor_count++;
}

static inline void asn1_type_path_push_index(ASN1TypePath *s, int index)
{
    ASN1TypePathElement *e;
    e = &s->accessors[s->accessor_count & (ASN1_MAX_TYPE_ACCESSORS - 1)];
    e->is_index = TRUE;
    e->u.index = index;
    s->accessor_count++;
}

static inline void asn1_type_path_pop(ASN1TypePath *s)
{
    s->accessor_count--;
}

void asn1_type_path_to_str(ASN1TypePath *s, char *buf, size_t buf_size);

#endif /* _ASN1_DEFS_INT_H */
