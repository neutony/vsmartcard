/*
 * Copyright (C) 2010 Frank Morgner
 *
 * This file is part of ccid.
 *
 * ccid is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * ccid is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ccid.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _CCID_PACE_H
#define _CCID_PACE_H

#include "sm.h"
#include <linux/usb/ch9.h>
#include <opensc/opensc.h>
#include <openssl/pace.h>
#include <openssl/bn.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PACE_BITMAP_PACE  0x40
#define PACE_BITMAP_EID   0x20
#define PACE_BITMAP_ESIGN 0x10

//#define PACE_MRZ 0x01
//#define PACE_CAN 0x02
//#define PACE_PIN 0x03
//#define PACE_PUK 0x04

#define FID_EF_CARDACCESS 0x011C

#define MAX_EF_CARDACCESS 2048

struct pace_sm_ctx {
    int protocol;
    /* XXX SSC is up to 16 Bytes long, not only 16 Bits */
    BIGNUM *ssc;
    const BUF_MEM *key_mac;
    const BUF_MEM *key_enc;
    PACE_CTX *ctx;
};

struct pace_sm_ctx *
pace_sm_ctx_create(int protocol, const BUF_MEM *key_mac,
        const BUF_MEM *key_enc, PACE_CTX *ctx);
void
pace_sm_ctx_free(struct pace_sm_ctx *ctx);
void
pace_sm_ctx_clear_free(struct pace_sm_ctx *ctx);

int reset_ssc(struct sm_ctx *ctx);
int increment_ssc(struct sm_ctx *ctx);

int pace_sm_encrypt(sc_card_t *card, const struct sm_ctx *ctx,
        const u8 *data, size_t datalen, u8 **enc);
int pace_sm_decrypt(sc_card_t *card, const struct sm_ctx *ctx,
        const u8 *enc, size_t enclen, u8 **data);
int pace_sm_authenticate(sc_card_t *card, const struct sm_ctx *ctx,
        const u8 *data, size_t datalen, u8 **outdata);

int GetReadersPACECapabilities(sc_card_t *card, const __u8
        *in, __u8 **out, size_t *outlen);
int EstablishPACEChannel(sc_card_t *card, const __u8 *in,
        __u8 **out, size_t *outlen, struct sm_ctx *ctx);
int pace_test(sc_card_t *card);

int pace_transmit_apdu(struct sm_ctx *sctx, sc_card_t *card,
        sc_apdu_t *apdu);

#ifdef  __cplusplus
}
#endif
#endif
