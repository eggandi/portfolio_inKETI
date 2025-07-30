/*
 * ASN1 common BER definitions (internal) 
 * Copyright (C) 2011-2018 Fabrice Bellard
 */

void asn1_ber_put_tag(ASN1ByteBuffer *s, unsigned int tag, int constructed);
void asn1_ber_put_len(ASN1ByteBuffer *s, unsigned int len);
int asn1_ber_encode_int32(ASN1ByteBuffer *s, int val);
uint32_t asn1_ber_put_len_start(ASN1ByteBuffer *s);
void asn1_ber_put_len_end(ASN1ByteBuffer *s, uint32_t pos);
