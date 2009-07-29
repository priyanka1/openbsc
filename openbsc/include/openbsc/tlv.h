#ifndef _TLV_H
#define _TLV_H

#include <sys/types.h>
#include <string.h>

#include <openbsc/msgb.h>

#define LV_GROSS_LEN(x)		(x+1)
#define TLV_GROSS_LEN(x)	(x+2)
#define TLV16_GROSS_LEN(x)	((2*x)+2)
#define TL16V_GROSS_LEN(x)	(x+3)

/* TLV generation */

static inline u_int8_t *lv_put(u_int8_t *buf, u_int8_t len,
				const u_int8_t *val)
{
	*buf++ = len;
	memcpy(buf, val, len);
	return buf + len;
}

static inline u_int8_t *tlv_put(u_int8_t *buf, u_int8_t tag, u_int8_t len,
				const u_int8_t *val)
{
	*buf++ = tag;
	*buf++ = len;
	memcpy(buf, val, len);
	return buf + len;
}

static inline u_int8_t *tlv16_put(u_int8_t *buf, u_int8_t tag, u_int8_t len,
				const u_int16_t *val)
{
	*buf++ = tag;
	*buf++ = len;
	memcpy(buf, val, len*2);
	return buf + len*2;
}

static inline u_int8_t *tl16v_put(u_int8_t *buf, u_int8_t tag, u_int16_t len,
				const u_int8_t *val)
{
	*buf++ = tag;
	*buf++ = len >> 8;
	*buf++ = len & 0xff;
	memcpy(buf, val, len);
	return buf + len*2;
}

static inline u_int8_t *msgb_tlv16_put(struct msgb *msg, u_int8_t tag, u_int8_t len, const u_int16_t *val)
{
	u_int8_t *buf = msgb_put(msg, TLV16_GROSS_LEN(len));
	return tlv16_put(buf, tag, len, val);
}

static inline u_int8_t *msgb_tl16v_put(struct msgb *msg, u_int8_t tag, u_int16_t len,
					const u_int8_t *val)
{
	u_int8_t *buf = msgb_put(msg, TL16V_GROSS_LEN(len));
	return tl16v_put(buf, tag, len, val);
}

static inline u_int8_t *v_put(u_int8_t *buf, u_int8_t val)
{
	*buf++ = val;
	return buf;
}

static inline u_int8_t *tv_put(u_int8_t *buf, u_int8_t tag, 
				u_int8_t val)
{
	*buf++ = tag;
	*buf++ = val;
	return buf;
}

static inline u_int8_t *tv16_put(u_int8_t *buf, u_int8_t tag, 
				 u_int16_t val)
{
	*buf++ = tag;
	*buf++ = val >> 8;
	*buf++ = val & 0xff;
	return buf;
}

static inline u_int8_t *msgb_lv_put(struct msgb *msg, u_int8_t len, const u_int8_t *val)
{
	u_int8_t *buf = msgb_put(msg, LV_GROSS_LEN(len));
	return lv_put(buf, len, val);
}

static inline u_int8_t *msgb_tlv_put(struct msgb *msg, u_int8_t tag, u_int8_t len, const u_int8_t *val)
{
	u_int8_t *buf = msgb_put(msg, TLV_GROSS_LEN(len));
	return tlv_put(buf, tag, len, val);
}

static inline u_int8_t *msgb_tv_put(struct msgb *msg, u_int8_t tag, u_int8_t val)
{
	u_int8_t *buf = msgb_put(msg, 2);
	return tv_put(buf, tag, val);
}

static inline u_int8_t *msgb_v_put(struct msgb *msg, u_int8_t val)
{
	u_int8_t *buf = msgb_put(msg, 1);
	return v_put(buf, val);
}

static inline u_int8_t *msgb_tv16_put(struct msgb *msg, u_int8_t tag, u_int16_t val)
{
	u_int8_t *buf = msgb_put(msg, 3);
	return tv16_put(buf, tag, val);
}

static inline u_int8_t *msgb_tlv_push(struct msgb *msg, u_int8_t tag, u_int8_t len, const u_int8_t *val)
{
	u_int8_t *buf = msgb_push(msg, TLV_GROSS_LEN(len));
	return tlv_put(buf, tag, len, val);
}

static inline u_int8_t *msgb_tv_push(struct msgb *msg, u_int8_t tag, u_int8_t val)
{
	u_int8_t *buf = msgb_push(msg, 2);
	return tv_put(buf, tag, val);
}

static inline u_int8_t *msgb_tv16_push(struct msgb *msg, u_int8_t tag, u_int16_t val)
{
	u_int8_t *buf = msgb_push(msg, 3);
	return tv16_put(buf, tag, val);
}

/* TLV parsing */

struct tlv_p_entry {
	u_int16_t len;
	const u_int8_t *val;
};

enum tlv_type {
	TLV_TYPE_FIXED,
	TLV_TYPE_T,
	TLV_TYPE_TV,
	TLV_TYPE_TLV,
	TLV_TYPE_TL16V,
};

struct tlv_def {
	enum tlv_type type;
	u_int8_t fixed_len;
};

struct tlv_definition {
	struct tlv_def def[0xff];
};

struct tlv_parsed {
	struct tlv_p_entry lv[0xff];
};

int tlv_parse(struct tlv_parsed *dec, const struct tlv_definition *def,
	      const u_int8_t *buf, int buf_len, u_int8_t lv_tag, u_int8_t lv_tag2);

#define TLVP_PRESENT(x, y)	((x)->lv[y].val)
#define TLVP_LEN(x, y)		(x)->lv[y].len
#define TLVP_VAL(x, y)		(x)->lv[y].val

#endif /* _TLV_H */