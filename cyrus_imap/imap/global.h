/* global.h -- Header for global/shared variables & functions.
 * $Id: global.h,v 1.8 2006/11/30 17:11:17 murch Exp $
 * Copyright (c) 1998-2003 Carnegie Mellon University.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any other legal
 *    details, please contact  
 *      Office of Technology Transfer
 *      Carnegie Mellon University
 *      5000 Forbes Avenue
 *      Pittsburgh, PA  15213-3890
 *      (412) 268-4387, fax: (412) 268-7395
 *      tech-transfer@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef INCLUDED_GLOBAL_H
#define INCLUDED_GLOBAL_H

#include <sasl/sasl.h>
#include "libconfig.h"
#include "auth.h"
#include "mboxname.h"
#include "signals.h"

/* Flags for cyrus_init() */
enum {
    CYRUSINIT_NODB =	(1<<0)
};

/* Startup the configuration subsystem */
/* Note that cyrus_init is pretty much the wholesale startup function
 * for any libimap/libcyrus process, and should be called fairly early
 * (and needs an associated cyrus_done call) */
extern int cyrus_init(const char *alt_config, const char *ident,
		      unsigned flags);
extern void global_sasl_init(int client, int server,
			     const sasl_callback_t *callbacks);

/* Shutdown a cyrus process */
extern void cyrus_done();

/* sasl configuration */
extern int mysasl_config(void *context,
			 const char *plugin_name,
			 const char *option,
			 const char **result,
			 unsigned *len);
extern sasl_security_properties_t *mysasl_secprops(int flags);

/* user canonification */
extern char *canonify_userid(char *user, char *loginid, int *domain_from_ip);

extern int is_userid_anonymous(const char *user);

extern int mysasl_canon_user(sasl_conn_t *conn,
		             void *context,
		             const char *user, unsigned ulen,
		             unsigned flags,
		             const char *user_realm,
		             char *out_user,
		             unsigned out_max, unsigned *out_ulen);

extern int mysasl_proxy_policy(sasl_conn_t *conn,
			       void *context,
			       const char *requested_user, unsigned rlen,
			       const char *auth_identity, unsigned alen,
			       const char *def_realm __attribute__((unused)),
			       unsigned urlen __attribute__((unused)),
			       struct propctx *propctx __attribute__((unused)));

/* check if `authstate' is a valid member of class */
extern int global_authisa(struct auth_state *authstate, 
			  enum imapopt opt);

/* useful types */
struct protstream;
struct buf {
    char *s;
    int len;
    int alloc;
};

struct proxy_context {
    int use_acl;
    int proxy_servers;
    struct auth_state **authstate;
    int *userisadmin;
    int *userisproxyadmin;
};

/* imap parsing functions (imapparse.c) */
int getword(struct protstream *in, struct buf *buf);

/* IMAP_BIN_ASTRING is an IMAP_ASTRING that does not perform the
 * does-not-contain-a-NULL check (in the case of a literal) */
enum string_types { IMAP_ASTRING,
		    IMAP_BIN_ASTRING,
		    IMAP_NSTRING,
		    IMAP_QSTRING,
		    IMAP_STRING };

int getxstring(struct protstream *pin, struct protstream *pout,
	       struct buf *buf, int type);
#define getastring(pin, pout, buf) getxstring((pin), (pout), (buf), IMAP_ASTRING)
#define getbastring(pin, pout, buf) getxstring((pin), (pout), (buf), IMAP_BIN_ASTRING)
#define getnstring(pin, pout, buf) getxstring((pin), (pout), (buf), IMAP_NSTRING)
#define getqstring(pin, pout, buf) getxstring((pin), (pout), (buf), IMAP_QSTRING)
#define getstring(pin, pout, buf) getxstring((pin), (pout), (buf), IMAP_STRING)
void freebuf(struct buf *buf);

void eatline(struct protstream *pin, int c);

/* Misc utils */
extern void cyrus_ctime(time_t date, char *datebuf);
extern int shutdown_file(char *buf, int size);

/* Misc globals */
extern int config_implicitrights;
extern unsigned long config_metapartition_files;
extern struct cyrusdb_backend *config_mboxlist_db;
extern struct cyrusdb_backend *config_quota_db;
extern struct cyrusdb_backend *config_subscription_db;
extern struct cyrusdb_backend *config_annotation_db;
extern struct cyrusdb_backend *config_seenstate_db;
extern struct cyrusdb_backend *config_mboxkey_db;
extern struct cyrusdb_backend *config_duplicate_db;
extern struct cyrusdb_backend *config_tlscache_db;
extern struct cyrusdb_backend *config_ptscache_db;

#endif /* INCLUDED_GLOBAL_H */
